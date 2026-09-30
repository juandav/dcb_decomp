.EXTRA_PREREQS := $(abspath $(lastword $(MAKEFILE_LIST)))

-include local.mk

TOOLCHAIN ?= mipsel-linux-gnu-

BUILDDIR := build
ASM_DIR := asm
EXPECTEDDIR := expected
GENDIR := $(BUILDDIR)/generated

TARGET := disks/us/SLUS_013.28
ELF := $(BUILDDIR)/SLUS_013.28.elf
EXE := $(BUILDDIR)/SLUS_013.28
MAP := $(BUILDDIR)/SLUS_013.28.map

CPP := $(TOOLCHAIN)cpp
AS := $(TOOLCHAIN)as
LD := $(TOOLCHAIN)ld
OBJCOPY := $(TOOLCHAIN)objcopy

PYTHON := python3
SPLAT := $(PYTHON) -m splat split

GCC_VERSION ?= 2.95.2
CC1 ?= bin/gcc-$(GCC_VERSION)-psx/cc1
MASPSX := $(PYTHON) external/maspsx/maspsx.py
OBJDIFF ?= bin/objdiff-cli-linux-x86_64

INC := -Iinclude -Iexternal/psyq_headers/psyq_lib47/include

CPPFLAGS := $(INC) -undef -nostdinc \
	    -D__GNUC__=2 -Dmips -D__mips__ -D__mips -Dpsx -D__psx__ -D__psx \
	    -D_PSYQ -D__EXTENSIONS__ -D_MIPSEL -D_LANGUAGE_C -DLANGUAGE_C
CC1FLAGS := -quiet -O1 -G0 -mips1 -mcpu=3000 -mgas -msoft-float \
	    -fgnu-linker -Wall -Wno-unused
MASPSXFLAGS := --aspsx-version=2.86
# jump tables sit where the original files put them; see tools/fix_jtbl_align.py
ALIGN_FIX := $(PYTHON) tools/fix_jtbl_align.py
CC1_POST := cat

# The PsyQ libraries: one file per library object under src/main/psyq/, in
# the order of config/psyq_objects.txt. Each is compiled on its own and the
# outputs are assembled together as psyq.c.o, so that splat's alignment of
# the included rodata still counts from the start of the whole section.
# They were built with GCC 2.7.2 -O2, and their ASPSX moved the instruction
# before `j $31` into its delay slot (tools/aspsx_reorder.py). They use
# -mhard-float: with -msoft-float the FP registers are fixed, which lowers
# loop.c's threshold and keeps it from hoisting constants the originals hoist.
PSYQ_OBJECTS := $(shell awk '{print $$1}' config/psyq_objects.txt)
PSYQ_OBJ := $(PSYQ_OBJECTS:%=$(BUILDDIR)/src/main/psyq/%.c.s)
$(PSYQ_OBJ): GCC_VERSION := 2.7.2
# ...binary-patched into the libraries' cc1 (see tools/patch_cc1.py, from dw3)
PSYQ_CC1 := $(BUILDDIR)/tools/gcc-2.7.2-psx/cc1
$(PSYQ_OBJ): CC1 := $(PSYQ_CC1)
$(PSYQ_OBJ): $(PSYQ_CC1)
$(PSYQ_CC1): bin/gcc-2.7.2-psx/cc1 tools/patch_cc1.py
	$(PYTHON) tools/patch_cc1.py $< $@
$(PSYQ_OBJ): CC1FLAGS := -quiet -O2 -G0 -mips1 -mcpu=3000 -mgas -mhard-float \
	-fgnu-linker -fsigned-char -fno-builtin -fdollars-in-identifiers -Wall -Wno-unused
$(PSYQ_OBJ): ALIGN_FIX := $(PYTHON) tools/aspsx_reorder.py
$(PSYQ_OBJ): MASPSXFLAGS += --expand-div

# Some objects come from a GCC 2.8.1 without split addresses: it keeps the
# address of a global in a register and reaches its fields from there. It
# filled the delay slot of `j $31` itself; tools/unfill_epilogue.py undoes that
# so ASPSX's rule applies as for the rest.
PSYQ_GCC28 := $(shell awk '$$2 == "gcc2.8" {print $$1}' config/psyq_objects.txt)
PSYQ_GCC28_OBJ := $(PSYQ_GCC28:%=$(BUILDDIR)/src/main/psyq/%.c.s)
# That compiler never used `return` insns either (tools/sn_cc1.py).
SN_CC1 := $(BUILDDIR)/cc1-2.8.1-sn
$(PSYQ_GCC28_OBJ): CC1 := $(SN_CC1)
$(PSYQ_GCC28_OBJ): CC1FLAGS += -mno-split-addresses
# putchar() has _putchar() and _putchar_flash() inlined
$(BUILDDIR)/src/main/psyq/libc2_putchar.c.s: CC1FLAGS += -finline-functions
# StRingStatus's loop recomputes its ring address every time: that libcd
# object was built without strength reduction
$(BUILDDIR)/src/main/psyq/libcd_bios_1_2.c.s: CC1FLAGS += -fno-strength-reduce
$(PSYQ_GCC28_OBJ): CC1_POST := $(PYTHON) tools/unfill_epilogue.py
$(PSYQ_GCC28_OBJ): $(SN_CC1)
$(SN_CC1): bin/gcc-2.8.1-psx/cc1 tools/sn_cc1.py
	$(PYTHON) tools/sn_cc1.py $< $@

# Others come from GCC 2.7.2 run without the second CSE pass, as all of
# DW3's PsyQ: the first pass kept the address of a global in a register and
# the second one would put the constant address back
PSYQ_NOCSE := $(shell awk '$$2 == "nocse" {print $$1}' config/psyq_objects.txt)
PSYQ_NOCSE_OBJ := $(PSYQ_NOCSE:%=$(BUILDDIR)/src/main/psyq/%.c.s)
$(PSYQ_NOCSE_OBJ): CC1FLAGS += -fno-rerun-cse-after-loop
ASFLAGS := -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0 $(INC)
LDFLAGS := -nostdlib --no-check-sections -Map $(MAP) \
	   -T $(GENDIR)/main.ld \
	   -T config/undefined_syms.txt \
	   -T $(GENDIR)/undefined_syms_auto_main.txt \
	   -T $(GENDIR)/undefined_funcs_auto_main.txt

C_SRC := $(shell find src -name '*.c' -not -path 'src/main/psyq/*' 2> /dev/null) src/main/psyq.c
# Target objects for objdiff: splat's full disassembly of every C unit
TARGET_ASM := $(C_SRC:src/%.c=$(ASM_DIR)/%.s)

# Everything else splat wrote (header, data); not the function-by-function
# asm nor the full disassembly of the C files
ASM_SRC := $(filter-out $(TARGET_ASM),$(shell find $(ASM_DIR) -name '*.s' \
	   -not -path '*/nonmatchings/*' -not -path '*/matchings/*' 2> /dev/null))

# Code that was written in assembly, kept as assembly source: splat's hasm
# segments (src/<binary>/<name>.s; splat only writes one if it isn't there),
# and the PsyQ objects Sony assembled (src/main/psyq/<object>.s, used in
# place of <object>.c, see PSYQ_PARTS)
HASM_SRC := $(shell find src -name '*.s' -not -path 'src/main/psyq/*' 2> /dev/null)

C_OBJ := $(C_SRC:%.c=$(BUILDDIR)/%.c.o)
ASM_OBJ := $(ASM_SRC:%.s=$(BUILDDIR)/%.s.o)
HASM_OBJ := $(HASM_SRC:%.s=$(BUILDDIR)/%.s.o)
TARGET_OBJ := $(TARGET_ASM:%.s=$(BUILDDIR)/%.s.o)
OBJ := $(C_OBJ) $(ASM_OBJ) $(HASM_OBJ)

# Overlays: code the game loads from P.DRV at OVERLAY_LOAD_ADDR, the end of
# the executable's .bss. Each one has a splat config, config/<name>.yaml,
# its C files under src/<name>/ and its own ELF linked against the
# executable's symbols; `make compare` checks them with the executable.
OVERLAYS := endseg evoseg kawseg openseg saiseg subseg sugseg
OVERLAY_DRIVE := disks/us/P.DRV
OVERLAY_BINS := $(foreach o,$(OVERLAYS),$(BUILDDIR)/$(shell echo $(o) | tr a-z A-Z).BIN)

# the executable's named symbols, as a linker script for the overlays
$(GENDIR)/symbols_main.ld: config/symbols.txt
	@mkdir -p $(dir $@)
	sed -e 's|//.*||' $< > $@

define OVERLAY_RULES
$(1)_NAME := $(shell echo $(1) | tr a-z A-Z)

$(BUILDDIR)/disks/$$($(1)_NAME).BIN: $(OVERLAY_DRIVE) tools/extract_drv.py
	@mkdir -p $$(dir $$@)
	$(PYTHON) tools/extract_drv.py $$< $$($(1)_NAME) $$@

$(GENDIR)/$(1).ld: .EXTRA_PREREQS :=
$(GENDIR)/$(1).ld: config/$(1).yaml config/symbols.txt $(wildcard config/symbols_$(1).txt) $(BUILDDIR)/disks/$$($(1)_NAME).BIN
	$(SPLAT) $$< --disassemble-all --make-full-disasm-for-code
	@# a C file with no code (an overlay's zeroed data, <prefix>_bss.c) gets
	@# no full disassembly from splat; its data file is its whole target
	@for f in $(ASM_DIR)/$(1)/data/*.data.s; do \
		u=$$$$(basename $$$$f .data.s); \
		if [ -e src/$(1)/$$$$u.c ] && ! grep -qs '^glabel' $(ASM_DIR)/$(1)/$$$$u.s; then cp $$$$f $(ASM_DIR)/$(1)/$$$$u.s; fi; \
	done
	@touch $$@

$(BUILDDIR)/$$($(1)_NAME).elf: $(OBJ) $(GENDIR)/$(1).ld $(GENDIR)/symbols_main.ld config/undefined_syms.txt $(wildcard config/undefined_syms_$(1).txt)
	$(LD) -nostdlib --no-check-sections -Map $(BUILDDIR)/$$($(1)_NAME).map \
		-T $(GENDIR)/$(1).ld -T $(GENDIR)/symbols_main.ld -T config/undefined_syms.txt \
		$(addprefix -T ,$(wildcard config/undefined_syms_$(1).txt)) \
		-T $(GENDIR)/undefined_syms_auto_$(1).txt \
		-T $(GENDIR)/undefined_funcs_auto_$(1).txt -o $$@

$(BUILDDIR)/$$($(1)_NAME).BIN: $(BUILDDIR)/$$($(1)_NAME).elf
	$(OBJCOPY) -O binary $$< $$@
endef

$(foreach o,$(OVERLAYS),$(eval $(call OVERLAY_RULES,$(o))))

all: $(EXE) $(OVERLAY_BINS)

# Only rerun splat when its own inputs change, never for Makefile edits
$(GENDIR)/main.ld: .EXTRA_PREREQS :=
$(GENDIR)/main.ld: config/main.yaml config/symbols.txt
	# splat reads the INCLUDE_ASMs of the psyq segment from src/main/psyq.c;
	# without them it files every PsyQ function under asm/main/matchings
	grep -h '^INCLUDE_' src/main/psyq/*.c > src/main/psyq.c
	$(SPLAT) $< --disassemble-all --make-full-disasm-for-code; \
		r=$$?; rm -f src/main/psyq.c; exit $$r
	@touch $@

generate: $(GENDIR)/main.ld $(foreach o,$(OVERLAYS),$(GENDIR)/$(o).ld)

regenerate: reset
	$(MAKE) generate

compare: $(EXE) $(OVERLAY_BINS)
	@sha1sum -c config/SLUS_013.28.sha1 config/overlays.sha1

$(EXE): $(ELF)
	$(OBJCOPY) -O binary $< $@
	@truncate -s %2048 $@

$(ELF): $(OBJ) $(GENDIR)/main.ld config/undefined_syms.txt
	$(LD) $(LDFLAGS) -o $@

$(BUILDDIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -MMD -MP -MT $@ -MF $(@:.o=.d) $< -o $(@:.o=.i)
	$(CC1) $(CC1FLAGS) -o $(@:.o=.cc1.s) $(@:.o=.i)
	$(CC1_POST) < $(@:.o=.cc1.s) | $(MASPSX) $(MASPSXFLAGS) | $(ALIGN_FIX) $(patsubst src/%,%,$*) > $(@:.o=.s)
	$(AS) $(ASFLAGS) -o $@ $(@:.o=.s)
	@# gas aligns .data and .bss to 16 and GCC's jump tables align .rodata
	@# to 8; psylink packed the game's objects to 4, so a file's rodata can
	@# start 4 bytes past an 8-byte boundary, as the original's do
	@$(OBJCOPY) --set-section-alignment .rodata=4 --set-section-alignment .data=4 --set-section-alignment .bss=4 $@

# Local labels get the object's name so the outputs can be joined
$(BUILDDIR)/src/main/psyq/%.c.s: src/main/psyq/%.c
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -MMD -MP -MT $@ -MF $(@:.s=.d) $< -o $(@:.s=.i)
	$(CC1) $(CC1FLAGS) -o $(@:.s=.cc1.s) $(@:.s=.i)
	$(CC1_POST) < $(@:.s=.cc1.s) | $(MASPSX) $(MASPSXFLAGS) | $(ALIGN_FIX) \
		| sed -e 's/\$$L\(C\?[0-9]\)/$$L$*_\1/g' \
		      -e 's/\.L_\(NOT_DIV_BY_ZERO\|DIV_BY_POSITIVE_SIGN\)_/.L_\1_$*_/g' > $@

# A PsyQ object written in assembly is src/main/psyq/<object>.s, taken as it
# is in place of the compiled <object>.c, between the same .set lines as an
# INCLUDE_ASM
PSYQ_PARTS := $(foreach o,$(PSYQ_OBJECTS),$(or $(wildcard src/main/psyq/$(o).s),$(BUILDDIR)/src/main/psyq/$(o).c.s))

$(BUILDDIR)/src/main/psyq.c.o: $(PSYQ_PARTS) config/psyq_objects.txt
	for f in $(PSYQ_PARTS); do \
		case $$f in \
		src/*) printf '.section .text\n.set noat\n.set noreorder\n'; cat $$f; \
		       printf '\n.set reorder\n.set at\n.section .text\n' ;; \
		*) cat $$f ;; \
		esac; \
	done | awk '/^(gcc2_compiled\.|__gnu_compiled_c):$$/ && seen[$$0]++ {next} \
	     /^\.include "(include\/)?(labels|macro)\.inc"$$/ && seen["inc"]++ {next} \
	     /^[ \t]*\.file[ \t]/ {next} {print}' > $(@:.o=.s)
	$(AS) $(ASFLAGS) -o $@ $(@:.o=.s)

# gas aligns these sections to 16 bytes, psylink packed them to 4
$(BUILDDIR)/%.s.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<
	@$(OBJCOPY) --set-section-alignment .text=4 \
				--set-section-alignment .rodata=4 \
				--set-section-alignment .data=4 \
				--set-section-alignment .bss=4 $@

expected: $(TARGET_OBJ) $(C_OBJ)
	rm -rf $(EXPECTEDDIR)
	@mkdir -p $(EXPECTEDDIR)
	cp -r $(BUILDDIR)/$(ASM_DIR) $(EXPECTEDDIR)/$(ASM_DIR)

objdiff: expected
	$(PYTHON) tools/objdiff_generate.py

report: objdiff
	$(OBJDIFF) report generate -o $(BUILDDIR)/report.json

clean:
	rm -rf $(BUILDDIR)

reset: clean
	rm -rf $(ASM_DIR) $(EXPECTEDDIR)

-include $(C_OBJ:.o=.d) $(PSYQ_OBJ:.s=.d)

.PHONY: all generate regenerate compare expected objdiff report clean reset

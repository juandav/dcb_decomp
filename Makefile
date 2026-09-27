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
# game.c holds many original source files; see tools/fix_jtbl_align.py
ALIGN_FIX := $(PYTHON) tools/fix_jtbl_align.py
CC1_POST := cat

# The PsyQ libraries: one file per library object under src/main/psyq/, in
# the order of config/psyq_objects.txt. Each is compiled on its own and the
# outputs are assembled together as psyq.c.o, so that splat's alignment of
# the included rodata still counts from the start of the whole section.
# They were built with GCC 2.7.2 -O2, and their ASPSX moved the instruction
# before `j $31` into its delay slot (tools/aspsx_reorder.py)
PSYQ_OBJECTS := $(shell awk '{print $$1}' config/psyq_objects.txt)
PSYQ_OBJ := $(PSYQ_OBJECTS:%=$(BUILDDIR)/src/main/psyq/%.c.s)
$(PSYQ_OBJ): GCC_VERSION := 2.7.2
$(PSYQ_OBJ): CC1FLAGS := -quiet -O2 -G0 -mips1 -mcpu=3000 -mgas -msoft-float \
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
ASM_SRC := $(shell find $(ASM_DIR) -name '*.s' \
	   -not -path '*/nonmatchings/*' -not -path '*/matchings/*' \
	   -not -path '$(ASM_DIR)/main/game.s' \
	   -not -path '$(ASM_DIR)/main/psyq.s' 2> /dev/null)

# Target objects for objdiff: splat's full disassembly of every C unit
TARGET_ASM := $(C_SRC:src/%.c=$(ASM_DIR)/%.s)

C_OBJ := $(C_SRC:%.c=$(BUILDDIR)/%.c.o)
ASM_OBJ := $(ASM_SRC:%.s=$(BUILDDIR)/%.s.o)
TARGET_OBJ := $(TARGET_ASM:%.s=$(BUILDDIR)/%.s.o)
OBJ := $(C_OBJ) $(ASM_OBJ)

all: $(EXE)

# Only rerun splat when its own inputs change, never for Makefile edits
$(GENDIR)/main.ld: .EXTRA_PREREQS :=
$(GENDIR)/main.ld: config/main.yaml config/symbols.txt
	$(SPLAT) $< --disassemble-all --make-full-disasm-for-code
	@touch $@

generate: $(GENDIR)/main.ld

regenerate: reset
	$(MAKE) generate

compare: $(EXE)
	@sha1sum -c config/SLUS_013.28.sha1

$(EXE): $(ELF)
	$(OBJCOPY) -O binary $< $@
	@truncate -s %2048 $@

$(ELF): $(OBJ) $(GENDIR)/main.ld config/undefined_syms.txt
	$(LD) $(LDFLAGS) -o $@

$(BUILDDIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -MMD -MP -MT $@ -MF $(@:.o=.d) $< -o $(@:.o=.i)
	$(CC1) $(CC1FLAGS) -o $(@:.o=.cc1.s) $(@:.o=.i)
	$(CC1_POST) < $(@:.o=.cc1.s) | $(MASPSX) $(MASPSXFLAGS) | $(ALIGN_FIX) > $(@:.o=.s)
	$(AS) $(ASFLAGS) -o $@ $(@:.o=.s)

# Local labels get the object's name so the outputs can be joined
$(BUILDDIR)/src/main/psyq/%.c.s: src/main/psyq/%.c
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -MMD -MP -MT $@ -MF $(@:.s=.d) $< -o $(@:.s=.i)
	$(CC1) $(CC1FLAGS) -o $(@:.s=.cc1.s) $(@:.s=.i)
	$(CC1_POST) < $(@:.s=.cc1.s) | $(MASPSX) $(MASPSXFLAGS) | $(ALIGN_FIX) \
		| sed -e 's/\$$L\(C\?[0-9]\)/$$L$*_\1/g' > $@

$(BUILDDIR)/src/main/psyq.c.o: $(PSYQ_OBJ) config/psyq_objects.txt
	awk '/^(gcc2_compiled\.|__gnu_compiled_c):$$/ && seen[$$0]++ {next} \
	     /^\.include "include\/labels\.inc"$$/ && seen[$$0]++ {next} \
	     /^[ \t]*\.file[ \t]/ {next} {print}' $(PSYQ_OBJ) > $(@:.o=.s)
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

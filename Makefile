.EXTRA_PREREQS := $(abspath $(lastword $(MAKEFILE_LIST)))

-include local.mk

# `make` builds everything; without this the first rule below, a PsyQ
# object's, would be the default
.DEFAULT_GOAL := all

# The version of the game to build. Each one has its settings in
# mk/version/<version>.mk: the executable's name, the disc, the overlays and
# the source files. The C and the assembly see VERSION_US, VERSION_JP and
# VERSION_EU, the one being built as 1 and the others as 0.
VERSION ?= us
VERSIONS := us jp eu
ifeq ($(filter $(VERSION),$(VERSIONS)),)
$(error unsupported VERSION $(VERSION); supported: $(VERSIONS))
endif
VERSION_UPPER := $(shell echo $(VERSION) | tr a-z A-Z)
include mk/version/$(VERSION).mk
# the tools read it too (tools/version.py)
export VERSION

# splat configs, symbols and checksums
CONFIG_DIR := config/$(VERSION)

TOOLCHAIN ?= mipsel-linux-gnu-

BUILDDIR := build/$(VERSION)
ASM_DIR := asm/$(VERSION)
EXPECTEDDIR := expected/$(VERSION)
GENDIR := $(BUILDDIR)/generated
# the compilers the build patches, the same for every version
TOOLS_BUILDDIR := build/tools

ELF := $(BUILDDIR)/$(EXE_NAME).elf
EXE := $(BUILDDIR)/$(EXE_NAME)
MAP := $(BUILDDIR)/$(EXE_NAME).map

CPP := $(TOOLCHAIN)cpp
AS := $(TOOLCHAIN)as
LD := $(TOOLCHAIN)ld
OBJCOPY := $(TOOLCHAIN)objcopy

PYTHON := python3
SPLAT := $(PYTHON) -m splat split

# the prebuilt compilers and tools that tools/dl_deps.sh downloads; the
# Docker image keeps its own outside the repository and sets BIN_DIR
BIN_DIR ?= bin

GCC_VERSION ?= 2.95.2
CC1 ?= $(BIN_DIR)/gcc-$(GCC_VERSION)-psx/cc1
MASPSX := $(PYTHON) external/maspsx/maspsx.py
OBJDIFF ?= $(BIN_DIR)/objdiff-cli-linux-x86_64

INC := -Iinclude -Iexternal/psyq_headers/psyq_lib47/include

# -DVERSION_<VERSION>: include/version.h turns it into VERSION_US,
# VERSION_JP and VERSION_EU, each 0 or 1, for #if; -Wundef warns about an
# #if on a name that isn't defined, such as a misspelt version
CPPFLAGS := $(INC) -undef -nostdinc -Wundef \
	    -D__GNUC__=2 -Dmips -D__mips__ -D__mips -Dpsx -D__psx__ -D__psx \
	    -D_PSYQ -D__EXTENSIONS__ -D_MIPSEL -D_LANGUAGE_C -DLANGUAGE_C \
	    -DVERSION_$(VERSION_UPPER) -DASM_DIR='"$(ASM_DIR)"' \
	    -I$(BUILDDIR)
CC1FLAGS := -quiet -O1 -G0 -mips1 -mcpu=3000 -mgas -msoft-float \
	    -fgnu-linker -Wall -Wno-unused
# a version can add its own (MASPSX_EXTRA in mk/version/<version>.mk)
MASPSXFLAGS := --aspsx-version=2.86 $(MASPSX_EXTRA)
# jump tables sit where the original files put them; see tools/fix_jtbl_align.py
ALIGN_FIX := $(PYTHON) tools/fix_jtbl_align.py
CC1_POST := cat

# The PsyQ libraries: one file per library object under src/main/psyq/, in
# the order of config/<version>/psyq_objects.txt. Each is compiled on its own
# and the outputs are assembled together as psyq.c.o, so that splat's
# alignment of the included rodata still counts from the start of the whole
# section.
# They were built with GCC 2.7.2 -O2, and their ASPSX moved the instruction
# before `j $31` into its delay slot (tools/aspsx_reorder.py). They use
# -mhard-float: with -msoft-float the FP registers are fixed, which lowers
# loop.c's threshold and keeps it from hoisting constants the originals hoist.
# A version without that list (one still built from blobs) has none.
PSYQ_LIST := $(wildcard $(CONFIG_DIR)/psyq_objects.txt)
PSYQ_OBJECTS := $(if $(PSYQ_LIST),$(shell awk '{print $$1}' $(PSYQ_LIST)))
# the ones written in C, which splat reads for their INCLUDE_ASMs
PSYQ_C := $(wildcard $(PSYQ_OBJECTS:%=src/main/psyq/%.c))
PSYQ_OBJ := $(PSYQ_OBJECTS:%=$(BUILDDIR)/src/main/psyq/%.c.s)
$(PSYQ_OBJ): GCC_VERSION := 2.7.2
# ...binary-patched into the libraries' cc1 (see tools/patch_cc1.py, from dw3)
PSYQ_CC1 := $(TOOLS_BUILDDIR)/gcc-2.7.2-psx/cc1
$(PSYQ_OBJ): CC1 := $(PSYQ_CC1)
$(PSYQ_OBJ): $(PSYQ_CC1)
$(PSYQ_CC1): $(BIN_DIR)/gcc-2.7.2-psx/cc1 tools/patch_cc1.py
	$(PYTHON) tools/patch_cc1.py $< $@
$(PSYQ_OBJ): CC1FLAGS := -quiet -O2 -G0 -mips1 -mcpu=3000 -mgas -mhard-float \
	-fgnu-linker -fsigned-char -fno-builtin -fdollars-in-identifiers -Wall -Wno-unused
$(PSYQ_OBJ): ALIGN_FIX := $(PYTHON) tools/aspsx_reorder.py
$(PSYQ_OBJ): MASPSXFLAGS += --expand-div

# Some objects come from a GCC 2.8.1 without split addresses: it keeps the
# address of a global in a register and reaches its fields from there. It
# filled the delay slot of `j $31` itself; tools/unfill_epilogue.py undoes that
# so ASPSX's rule applies as for the rest.
PSYQ_GCC28 := $(if $(PSYQ_LIST),$(shell awk '$$2 == "gcc2.8" {print $$1}' $(PSYQ_LIST)))
PSYQ_GCC28_OBJ := $(PSYQ_GCC28:%=$(BUILDDIR)/src/main/psyq/%.c.s)
# That compiler never used `return` insns either (tools/sn_cc1.py).
SN_CC1 := $(TOOLS_BUILDDIR)/cc1-2.8.1-sn
$(PSYQ_GCC28_OBJ): CC1 := $(SN_CC1)
$(PSYQ_GCC28_OBJ): CC1FLAGS += -mno-split-addresses
# putchar() has _putchar() and _putchar_flash() inlined
$(BUILDDIR)/src/main/psyq/libc2_putchar.c.s: CC1FLAGS += -finline-functions
# StRingStatus's loop recomputes its ring address every time: that libcd
# object was built without strength reduction
$(BUILDDIR)/src/main/psyq/libcd_bios_1_2.c.s: CC1FLAGS += -fno-strength-reduce
$(PSYQ_GCC28_OBJ): CC1_POST := $(PYTHON) tools/unfill_epilogue.py
$(PSYQ_GCC28_OBJ): $(SN_CC1)
$(SN_CC1): $(BIN_DIR)/gcc-2.8.1-psx/cc1 tools/sn_cc1.py
	$(PYTHON) tools/sn_cc1.py $< $@

# Others come from GCC 2.7.2 run without the second CSE pass, as all of
# DW3's PsyQ: the first pass kept the address of a global in a register and
# the second one would put the constant address back
PSYQ_NOCSE := $(if $(PSYQ_LIST),$(shell awk '$$2 == "nocse" {print $$1}' $(PSYQ_LIST)))
PSYQ_NOCSE_OBJ := $(PSYQ_NOCSE:%=$(BUILDDIR)/src/main/psyq/%.c.s)
$(PSYQ_NOCSE_OBJ): CC1FLAGS += -fno-rerun-cse-after-loop
# the assembly sees every version as 0 or 1 too: .if VERSION_JP
ASFLAGS := -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0 $(INC) \
	   $(foreach v,$(VERSIONS),--defsym VERSION_$(shell echo $(v) | tr a-z A-Z)=$(if $(filter $(v),$(VERSION)),1,0))
# the hand-written symbols every binary links with (a version that is still
# blobs has none)
UNDEFINED_SYMS := $(wildcard $(CONFIG_DIR)/undefined_syms.txt)
LDFLAGS := -nostdlib --no-check-sections -Map $(MAP) \
	   -T $(GENDIR)/main.ld \
	   $(addprefix -T ,$(UNDEFINED_SYMS)) \
	   -T $(GENDIR)/undefined_syms_auto_main.txt \
	   -T $(GENDIR)/undefined_funcs_auto_main.txt

# The source files are the ones the version lists in mk/version/<version>.mk,
# <BINARY>_C_SRC and <BINARY>_HASM_SRC for the executable (MAIN) and each
# overlay: nothing else under src/ is built. The PsyQ objects are assembled
# into one psyq.c.o (see PSYQ_OBJECTS).
BINARIES := MAIN $(shell echo $(OVERLAYS) | tr a-z A-Z)
C_SRC := $(foreach b,$(BINARIES),$($(b)_C_SRC)) $(if $(PSYQ_OBJECTS),src/main/psyq.c)
# Target objects for objdiff: splat's full disassembly of every C unit
TARGET_ASM := $(C_SRC:src/%.c=$(ASM_DIR)/%.s)

# Everything else splat wrote (header, data); not the function-by-function
# asm nor the full disassembly of the C files
ASM_SRC := $(filter-out $(TARGET_ASM),$(shell find $(ASM_DIR) -name '*.s' \
	   -not -path '*/nonmatchings/*' -not -path '*/matchings/*' 2> /dev/null))

# The fonts the version draws text with (config/<version>/fonts.txt), which
# are the game's art and so not in the repository: `make generate` cuts each
# one out of the executable as a PNG sheet, assets/<version>/<name>.png, and
# the build turns the sheet into $(BUILDDIR)/assets/<name>.inc, the C
# initializer its source file includes as "assets/<name>.inc" (see
# tools/font.py). An edited sheet goes into the build as it is.
FONT_LIST := $(wildcard $(CONFIG_DIR)/fonts.txt)
FONTS := $(if $(FONT_LIST),$(shell awk '{ sub(/\#.*/, "") } NF { print $$1 }' $(FONT_LIST)))
ASSETS_DIR := assets/$(VERSION)
FONT_PNG := $(FONTS:%=$(ASSETS_DIR)/%.png)
FONT_INC := $(FONTS:%=$(BUILDDIR)/assets/%.inc)

# Code that was written in assembly, kept as assembly source: splat's hasm
# segments (src/<binary>/<name>.s; splat only writes one if it isn't there),
# and the PsyQ objects Sony assembled (src/main/psyq/<object>.s, used in
# place of <object>.c, see PSYQ_PARTS)
HASM_SRC := $(foreach b,$(BINARIES),$($(b)_HASM_SRC))

C_OBJ := $(C_SRC:%.c=$(BUILDDIR)/%.c.o)
ASM_OBJ := $(ASM_SRC:%.s=$(BUILDDIR)/%.s.o)
HASM_OBJ := $(HASM_SRC:%.s=$(BUILDDIR)/%.s.o)
TARGET_OBJ := $(TARGET_ASM:%.s=$(BUILDDIR)/%.s.o)
OBJ := $(C_OBJ) $(ASM_OBJ) $(HASM_OBJ)
# splat's asm segments keep their jump tables in their rodata's file, so the
# labels those point to are global there (see jlabel in include/macro.inc)
$(ASM_OBJ): ASFLAGS += --defsym GLOBAL_JLABELS=1

# Overlays: code the game loads from P.DRV at OVERLAY_LOAD_ADDR, the end of
# the executable's .bss. Each one has a splat config,
# config/<version>/<name>.yaml, its C files under src/<name>/ and its own ELF
# linked against the executable's symbols; `make compare` checks them with
# the executable. The version's OVERLAYS are in mk/version/<version>.mk.
OVERLAY_DRIVE := $(DISK_DIR)/P.DRV
OVERLAY_BINS := $(foreach o,$(OVERLAYS),$(BUILDDIR)/$(shell echo $(o) | tr a-z A-Z).BIN)

# the executable's named symbols, as a linker script for the overlays; an
# overlay built for another executable than the version's (eu's NISSEG) links
# with that one's names instead, config/<version>/symbols_<overlay>_exe.txt
$(GENDIR)/symbols_main.ld: $(CONFIG_DIR)/symbols.txt
	@mkdir -p $(dir $@)
	sed -e 's|//.*||' $< > $@
$(GENDIR)/symbols_%_exe.ld: $(CONFIG_DIR)/symbols_%_exe.txt
	@mkdir -p $(dir $@)
	sed -e 's|//.*||' $< > $@

define OVERLAY_RULES
$(1)_NAME := $(shell echo $(1) | tr a-z A-Z)

$(BUILDDIR)/disks/$$($(1)_NAME).BIN: $(OVERLAY_DRIVE) tools/extract_drv.py
	@mkdir -p $$(dir $$@)
	$(PYTHON) tools/extract_drv.py $$< $$($(1)_NAME) $$@

$(GENDIR)/$(1).ld: .EXTRA_PREREQS :=
$(GENDIR)/$(1).ld: $(CONFIG_DIR)/$(1).yaml $(CONFIG_DIR)/symbols.txt $(wildcard $(CONFIG_DIR)/symbols_$(1).txt $(CONFIG_DIR)/symbols_$(1)_exe.txt) $(BUILDDIR)/disks/$$($(1)_NAME).BIN
	$(SPLAT) $$< --disassemble-all --make-full-disasm-for-code
	@# a C file with no code (an overlay's data-only files, such as
	@# <prefix>_bss.c) gets no full disassembly from splat; its target is
	@# splat's files of its rodata and data
	@for c in $(filter src/$(1)/%,$(C_SRC)); do \
		u=$$$${c#src/$(1)/}; u=$$$${u%.c}; t=$(ASM_DIR)/$(1)/$$$$u.s; \
		if ! grep -qs '^glabel' $$$$t; then \
			mkdir -p $$$$(dirname $$$$t); \
			{ echo '.include "macro.inc"'; cat $(ASM_DIR)/$(1)/data/$$$$u.rodata.s $(ASM_DIR)/$(1)/data/$$$$u.data.s 2>/dev/null \
				| grep -v '^\.include "macro.inc"'; } > $$$$t; \
		fi; \
	done
	@touch $$@

$(1)_EXE_SYMS := $(if $(wildcard $(CONFIG_DIR)/symbols_$(1)_exe.txt),$(GENDIR)/symbols_$(1)_exe.ld,$(GENDIR)/symbols_main.ld $(UNDEFINED_SYMS))

$(BUILDDIR)/$$($(1)_NAME).elf: $(OBJ) $(GENDIR)/$(1).ld $$($(1)_EXE_SYMS) $(wildcard $(CONFIG_DIR)/undefined_syms_$(1).txt)
	$(LD) -nostdlib --no-check-sections -Map $(BUILDDIR)/$$($(1)_NAME).map \
		-T $(GENDIR)/$(1).ld $$(addprefix -T ,$$($(1)_EXE_SYMS)) \
		$(addprefix -T ,$(wildcard $(CONFIG_DIR)/undefined_syms_$(1).txt)) \
		-T $(GENDIR)/undefined_syms_auto_$(1).txt \
		-T $(GENDIR)/undefined_funcs_auto_$(1).txt -o $$@

$(BUILDDIR)/$$($(1)_NAME).BIN: $(BUILDDIR)/$$($(1)_NAME).elf
	$(OBJCOPY) -O binary $$< $$@
endef

$(foreach o,$(OVERLAYS),$(eval $(call OVERLAY_RULES,$(o))))

all: $(EXE) $(OVERLAY_BINS)

# Only rerun splat when its own inputs change, never for Makefile edits
$(GENDIR)/main.ld: .EXTRA_PREREQS :=
$(GENDIR)/main.ld: $(CONFIG_DIR)/main.yaml $(CONFIG_DIR)/symbols.txt $(wildcard $(CONFIG_DIR)/symbols_overlay_calls.txt)
	# splat reads the INCLUDE_ASMs of the psyq segment from src/main/psyq.c;
	# without them it files every PsyQ function under $(ASM_DIR)/main/matchings
	$(if $(PSYQ_C),grep -h '^INCLUDE_' $(PSYQ_C) > src/main/psyq.c)
	$(SPLAT) $< --disassemble-all --make-full-disasm-for-code; \
		r=$$?; rm -f src/main/psyq.c; exit $$r
	@touch $@

generate: $(GENDIR)/main.ld $(foreach o,$(OVERLAYS),$(GENDIR)/$(o).ld) $(FONT_PNG)

# only extract a sheet again when the executable or the list change, so that
# an edited sheet stays
$(ASSETS_DIR)/%.png: .EXTRA_PREREQS :=
$(ASSETS_DIR)/%.png: $(DISK_DIR)/$(EXE_NAME) $(FONT_LIST)
	@mkdir -p $(dir $@)
	$(PYTHON) tools/font.py extract $< $(FONT_LIST) $* $@

$(BUILDDIR)/assets/%.inc: $(ASSETS_DIR)/%.png $(FONT_LIST) tools/font.py
	@mkdir -p $(dir $@)
	$(PYTHON) tools/font.py build $(FONT_LIST) $* $< $@

# the C files include them (their .d files then say which one does)
$(C_OBJ): | $(FONT_INC)

regenerate: reset
	$(MAKE) generate

compare: $(EXE) $(OVERLAY_BINS)
	@sha1sum -c $(CONFIG_DIR)/$(EXE_NAME).sha1 $(CONFIG_DIR)/overlays.sha1

$(EXE): $(ELF)
	$(OBJCOPY) -O binary $< $@
	@truncate -s %2048 $@

$(ELF): $(OBJ) $(GENDIR)/main.ld $(UNDEFINED_SYMS)
	$(LD) $(LDFLAGS) -o $@

$(BUILDDIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -MMD -MP -MT $@ -MF $(@:.o=.d) $< -o $(@:.o=.i)
	$(if $(TEXT_ENCODING),$(PYTHON) tools/sjis_escape.py $(@:.o=.i) $(@:.o=.i))
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

$(BUILDDIR)/src/main/psyq.c.o: $(PSYQ_PARTS) $(CONFIG_DIR)/psyq_objects.txt
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

# the targets: splat's full disassembly of each C file, and the code still in
# asm segments (with their data, which objdiff_generate.py links to them)
expected: $(TARGET_OBJ) $(C_OBJ) $(ASM_OBJ)
	rm -rf $(EXPECTEDDIR)
	@mkdir -p $(EXPECTEDDIR)
	cp -r $(BUILDDIR)/$(ASM_DIR) $(EXPECTEDDIR)/asm

objdiff: expected
	$(PYTHON) tools/objdiff_generate.py

report: objdiff
	$(OBJDIFF) report generate -o $(BUILDDIR)/report.json

clean:
	rm -rf $(BUILDDIR) $(TOOLS_BUILDDIR)

reset: clean
	rm -rf $(ASM_DIR) $(EXPECTEDDIR) $(ASSETS_DIR)

-include $(C_OBJ:.o=.d) $(PSYQ_OBJ:.s=.d)

.PHONY: all generate regenerate compare expected objdiff report clean reset

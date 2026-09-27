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

INC := -Iinclude

CPPFLAGS := $(INC) -undef -nostdinc \
	    -D__GNUC__=2 -Dmips -D__mips__ -D__mips -Dpsx -D__psx__ -D__psx \
	    -D_PSYQ -D__EXTENSIONS__ -D_MIPSEL -D_LANGUAGE_C -DLANGUAGE_C
CC1FLAGS := -quiet -O1 -G0 -mips1 -mcpu=3000 -mgas -msoft-float \
	    -fgnu-linker -Wall -Wno-unused
MASPSXFLAGS := --aspsx-version=2.86
# game.c holds many original source files; see tools/fix_jtbl_align.py
ALIGN_FIX := $(PYTHON) tools/fix_jtbl_align.py
# The PsyQ libraries were built with GCC 2.7.2 -O2, and their ASPSX moved the
# instruction before `j $31` into its delay slot (tools/aspsx_reorder.py)
$(BUILDDIR)/src/main/psyq.c.o: GCC_VERSION := 2.7.2
$(BUILDDIR)/src/main/psyq.c.o: CC1FLAGS := -quiet -O2 -G0 -mips1 -mcpu=3000 -mgas -msoft-float \
	-fgnu-linker -fsigned-char -fno-builtin -fdollars-in-identifiers -Wall -Wno-unused
$(BUILDDIR)/src/main/psyq.c.o: ALIGN_FIX := $(PYTHON) tools/aspsx_reorder.py
ASFLAGS := -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0 $(INC)
LDFLAGS := -nostdlib --no-check-sections -Map $(MAP) \
	   -T $(GENDIR)/main.ld \
	   -T config/undefined_syms.txt \
	   -T $(GENDIR)/undefined_syms_auto_main.txt \
	   -T $(GENDIR)/undefined_funcs_auto_main.txt

C_SRC := $(shell find src -name '*.c' 2> /dev/null)
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
	$(MASPSX) $(MASPSXFLAGS) < $(@:.o=.cc1.s) | $(ALIGN_FIX) > $(@:.o=.s)
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

-include $(C_OBJ:.o=.d)

.PHONY: all generate regenerate compare expected objdiff report clean reset

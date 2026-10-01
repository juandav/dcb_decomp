# Digimon Digital Card Battle, Europe (SLES-03900)
#
# Plain values: tools/version.py reads EXE_NAME and DISK_DIR from here too.

# the executable, as it is on the disc
EXE_NAME := SLES_039.00

# where the dumped disc is: the executable and P.DRV, which holds the overlays
DISK_DIR := disks/eu

# the overlays the build splits out of P.DRV, links and checks: each one has
# its splat config, config/<version>/<name>.yaml
OVERLAYS := endseg intseg kawseg nisseg saiseg subseg sugseg vssver openseg evoseg

# The game's code was built with GCC 2.8.1 -O1, as jp's: with us's C, 2.8.1
# gives the European tmd_sort, stage, memcard, text and card_db 108 of
# their 117 functions, 2.95.2 only 55
GCC_VERSION := 2.8.1

# The source files this version builds (see mk/version/us.mk), the same C as
# us's. The rest of each binary is splat's assembly, one asm segment per us
# module (config/eu/*.yaml, tools/split_version.py), except VSSVER: not
# code but a Visual SourceSafe file, it stays one blob, splat's databin.
MAIN_C_SRC := \
	$(addprefix src/main/system/, memcard.c)
EVOSEG_C_SRC := \
	src/evoseg/effect/evo_effect.c \
	$(addprefix src/evoseg/fusion/, evo_card_list.c)
KAWSEG_C_SRC := \
	$(addprefix src/kawseg/duel/, kaw_effect.c)

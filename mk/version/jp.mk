# Digimon Digital Card Battle, Japan (SLPS-02506)
#
# Plain values: tools/version.py reads EXE_NAME and DISK_DIR from here too.

# the executable, as it is on the disc
EXE_NAME := SLPS_025.06

# where the dumped disc is: the executable and P.DRV, which holds the overlays
DISK_DIR := disks/jp

# the overlays the build splits out of P.DRV, links and checks: each one has
# its splat config, config/<version>/<name>.yaml
OVERLAYS := endseg intseg kawseg nisseg saiseg subseg sugseg

# The game's code was built with GCC 2.8.1 -O1, the same flags as us's
# 2.95.2 (2.8.0 gives the same code): it copies a register that already
# holds a constant where 2.95.2 loads the constant again
GCC_VERSION := 2.8.1
# and assembled with an ASPSX that expands div and rem with their checks for
# a zero divisor and for overflow, as PsyQ's libraries are
MASPSX_EXTRA := --expand-div

# The source files this version builds (see mk/version/us.mk): the modules
# that build from the same C as us's, by subsystem. The rest of every binary
# is still splat's assembly (config/jp/*.yaml), one asm segment per us module.
MAIN_C_SRC := \
	src/main/duel/card_zones.c \
	$(addprefix src/main/gfx/, prim3d.c prim_pair.c prim_util.c tmd_sort.c \
		transform.c vram_upload.c) \
	$(addprefix src/main/model/, anim_control.c model_anim.c model_load.c) \
	src/main/script/script.c \
	$(addprefix src/main/system/, cd_file.c heap.c sound.c sound_play.c \
		task.c) \
	src/main/ui/str_util.c
SUGSEG_C_SRC := \
	$(addprefix src/sugseg/battle/, sug_battle.c sug_hud.c sug_sprite.c) \
	$(addprefix src/sugseg/effect/, sug_history.c sug_tex_anim.c)
KAWSEG_C_SRC := \
	$(addprefix src/kawseg/cpu/, kaw_card_queries.c kaw_cpu.c \
		kaw_cpu_attack.c kaw_cpu_digivolve.c kaw_cpu_placement.c)

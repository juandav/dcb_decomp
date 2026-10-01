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

# The source files this version builds (see mk/version/us.mk): the modules
# that build from the same C as us's, by subsystem. The rest of every binary
# is still splat's assembly (config/jp/*.yaml), one asm segment per us module.
MAIN_C_SRC := \
	$(addprefix src/main/gfx/, prim3d.c prim_util.c transform.c \
		vram_upload.c) \
	src/main/system/task.c

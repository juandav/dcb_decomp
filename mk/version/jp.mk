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
	src/main/card/player_rank.c \
	$(addprefix src/main/duel/, card_zones.c cpu_decision.c) \
	$(addprefix src/main/gfx/, display.c fade.c prim.c prim3d.c prim_pair.c \
		prim_util.c screen_copy.c tmd_sort.c transform.c vram_upload.c) \
	$(addprefix src/main/model/, anim_control.c camera.c model_anim.c \
		model_load.c scene3d.c stage.c wire_grid.c) \
	src/main/script/script.c \
	$(addprefix src/main/system/, angle.c archive.c cd_file.c game_exit.c \
		heap.c loader.c opening_movie.c save_checksum.c sound.c \
		sound_play.c task.c) \
	src/main/ui/str_util.c
ENDSEG_C_SRC := src/endseg/title/open_movie.c
SUGSEG_C_SRC := \
	$(addprefix src/sugseg/battle/, sug_battle.c sug_camera.c sug_hud.c \
		sug_sprite.c) \
	$(addprefix src/sugseg/effect/, sug_fade_rect.c sug_gradient.c \
		sug_effect_script.c sug_history.c sug_light_motion.c \
		sug_model_effect_jp.c sug_scroll_texture.c sug_sphere.c \
		sug_sprite_effect.c sug_tex_anim.c sug_trail.c) \
	$(addprefix src/sugseg/model/, effect_object.c effect_prims.c \
		streak_particles.c)
KAWSEG_C_SRC := \
	$(addprefix src/kawseg/cpu/, kaw_battle_sim.c kaw_card_queries.c \
		kaw_cpu.c kaw_cpu_attack.c kaw_cpu_digivolve.c kaw_cpu_placement.c) \
	src/kawseg/ui/kaw_result_jp.c

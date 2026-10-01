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
	src/main/card/player_rank.c \
	$(addprefix src/main/duel/, battle_hud.c duel_launch.c duel_session.c \
		duel_util.c hud_panels.c) \
	$(addprefix src/main/system/, angle.c archive.c decompress.c \
		game_flow.c memcard.c opening_movie.c save_checksum.c sort.c \
		sound_play.c vblank.c) \
	src/main/ui/menu.c
EVOSEG_C_SRC := \
	src/evoseg/effect/evo_effect.c \
	$(addprefix src/evoseg/fusion/, evo_banners.c evo_card_list.c \
		evo_fusion_result.c evo_screen_flash.c evo_type_choice.c)
KAWSEG_C_SRC := \
	src/kawseg/duel/kaw_effect.c \
	$(addprefix src/kawseg/ui/, kaw_exp.c kaw_prize.c)
OPENSEG_C_SRC := src/openseg/friend/open_friend.c
SAISEG_C_SRC := src/saiseg/hacking/sai_hacking.c
SUGSEG_C_SRC := \
	src/sugseg/battle/sug_camera.c \
	$(addprefix src/sugseg/effect/, sug_fade_rect.c sug_light_motion.c \
		sug_model_effect.c sug_scroll_texture.c sug_sprite_effect.c)

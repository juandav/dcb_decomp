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
# 2.95.2: it copies a register that already holds a constant where 2.95.2
# loads the constant again. PsyQ 4.4's own CC1PSX.EXE (GCC 2.8.1 SN32 build
# 4.0.0010) gives the same code as bin's 2.8.1 on every object; 2.8.0
# (PsyQ 4.3's) doesn't
GCC_VERSION := 2.8.1
# and assembled with an ASPSX that expands div and rem with their checks for
# a zero divisor and for overflow, as PsyQ's libraries are
MASPSX_EXTRA := --expand-div
# Its text is Shift JIS: the C writes it as UTF-8, and tools/sjis_escape.py
# re-encodes the string literals before cc1
TEXT_ENCODING := cp932

# The source files this version builds (see mk/version/us.mk): the modules
# that build from the same C as us's, by subsystem. The rest of every binary
# is still splat's assembly (config/jp/*.yaml), one asm segment per us module.
MAIN_C_SRC := \
	src/main/main.c \
	$(addprefix src/main/card/, card_db_jp.c card_render_jp.c card_sprite_jp.c \
		card_vram.c partner_level_jp.c player_data_jp.c player_rank.c) \
	$(addprefix src/main/duel/, battle_log_jp.c card_zones.c card_motion_jp.c \
		cpu_decision.c duel_jp.c duel_launch_jp.c duel_session_jp.c \
		duel_setup_jp.c duel_util_jp.c hud_panels_jp.c tutorial_jp.c) \
	$(addprefix src/main/gfx/, display.c fade.c prim.c prim3d.c prim_desc.c \
		prim_pair.c prim_util.c screen_copy.c tmd_sort.c transform.c \
		vram_upload.c) \
	$(addprefix src/main/model/, anim_control.c camera.c model_anim.c \
		model_load.c scene3d.c stage.c wire_grid.c) \
	src/main/script/script.c \
	$(addprefix src/main/system/, angle.c archive.c boot.c cd_file.c \
		frame_callback.c game_exit.c heap.c loader.c memcard.c \
		memcard_screen.c opening_movie.c pad.c render_loop.c \
		save_checksum.c sound.c sound_play.c task.c vblank.c) \
	$(addprefix src/main/ui/, str_util.c text_jp.c window_jp.c)
ENDSEG_C_SRC := \
	$(addprefix src/endseg/, end_bss_jp.c endseg_jp.c) \
	src/endseg/title/open_movie.c
INTSEG_C_SRC := $(addprefix src/intseg/, int_bss.c intseg.c)
SUGSEG_C_SRC := \
	$(addprefix src/sugseg/battle/, sug_battle.c sug_camera.c sug_hud.c \
		sug_sprite.c) \
	$(addprefix src/sugseg/effect/, sug_fade_rect.c sug_gradient.c \
		sug_effect_script.c sug_history.c sug_light_motion.c \
		sug_model_effect_jp.c sug_scroll_texture.c sug_sphere.c \
		sug_sprite_effect.c sug_tex_anim.c sug_trail.c) \
	$(addprefix src/sugseg/model/, effect_object.c effect_prims.c \
		streak_particles.c) \
	src/sugseg/sug_bss_jp.c
KAWSEG_C_SRC := \
	$(addprefix src/kawseg/cpu/, kaw_battle_sim.c kaw_card_queries.c \
		kaw_cpu.c kaw_cpu_attack.c kaw_cpu_digivolve.c kaw_cpu_placement.c) \
	src/kawseg/duel/kaw_hand_jp.c \
	src/kawseg/kaw_bss_jp.c \
	$(addprefix src/kawseg/ui/, kaw_exp_jp.c kaw_hud_jp.c \
		kaw_match_intro_jp.c kaw_prize_jp.c kaw_result_jp.c)
SUBSEG_C_SRC := \
	src/subseg/deck/sub_name_entry_jp.c \
	src/subseg/shop/sub_shop.c \
	src/subseg/sub_bss_jp.c
SAISEG_C_SRC := \
	$(addprefix src/saiseg/area/, sai_area_jp.c sai_opponent_select_jp.c \
		sai_slot_machine.c) \
	src/saiseg/map/sai_world_map_jp.c \
	src/saiseg/player/sai_player_data_jp.c \
	$(addprefix src/saiseg/, sai_bss_jp.c sai_data_jp.c) \
	$(addprefix src/saiseg/ui/, sai_labels_jp.c sai_panel_jp.c sai_text_jp.c)
NISSEG_C_SRC := \
	src/nisseg/deck/nis_auto_deck.c \
	src/nisseg/deck/nis_deck_editor.c \
	src/nisseg/deck/nis_deck_scene.c \
	src/nisseg/deck/nis_deck_windows.c \
	src/nisseg/nis_bss.c \
	src/nisseg/title/nis_title.c \
	src/nisseg/trade/nis_trade.c \
	src/nisseg/viewer/nis_model_viewer.c \
	src/nisseg/vs/nis_vs_deck_select.c \
	src/nisseg/vs/nis_vs_mode.c

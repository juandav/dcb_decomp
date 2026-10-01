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
	src/main/main.c \
	$(addprefix src/main/card/, card_db.c card_render.c partner_level.c \
		player_data.c player_rank.c) \
	$(addprefix src/main/duel/, battle_hud.c card_motion.c card_zones.c \
		cpu_decision.c duel.c duel_launch.c duel_session.c duel_setup.c \
		duel_util.c hud_panels.c) \
	$(addprefix src/main/gfx/, display.c fade.c prim.c prim3d.c prim_pair.c \
		prim_util.c screen_copy.c scroll_bg.c tmd_sort.c transform.c \
		vram_upload.c) \
	$(addprefix src/main/model/, anim_control.c camera.c effect_object.c \
		effect_prims.c model_anim.c model_load.c scene3d.c stage.c \
		wire_grid.c) \
	src/main/script/script.c \
	$(addprefix src/main/system/, angle.c archive.c boot.c cd_file.c \
		decompress.c frame_callback.c game_exit.c game_flow.c heap.c \
		loader.c memcard.c opening_movie.c pad.c render_loop.c \
		save_checksum.c sort.c sound.c sound_play.c task.c vblank.c) \
	$(addprefix src/main/ui/, dialog.c hacking_shell.c menu.c str_util.c \
		text.c window.c)
ENDSEG_C_SRC := src/endseg/endseg.c
EVOSEG_C_SRC := \
	$(addprefix src/evoseg/cutscene/, evo_cutscene.c evo_shatter.c) \
	src/evoseg/effect/evo_effect.c \
	src/evoseg/evo_bss.c \
	$(addprefix src/evoseg/fusion/, evo_banners.c evo_card_list.c \
		evo_data.c evo_fusion.c evo_fusion_result.c evo_fusion_script.c \
		evo_lists.c evo_partner_status.c evo_rewards.c evo_screen_flash.c \
		evo_text.c evo_trays.c evo_type_choice.c)
INTSEG_C_SRC := $(addprefix src/intseg/, int_bss.c intseg.c)
# eu's INTSEG is a leftover of a Japanese build with debug code, linked
# against that build's executable (config/eu/intseg.yaml): its C builds as
# jp's, with jp's headers and Shift JIS text (mk/version/jp.mk), and with
# JP_DEBUG_BUILD for the debug code (include/version.h)
build/eu/src/intseg/%.c.o: CPPFLAGS += -UVERSION_EU -DVERSION_JP -DJP_DEBUG_BUILD=1
build/eu/src/intseg/%.c.o: TEXT_ENCODING := cp932
KAWSEG_C_SRC := \
	$(addprefix src/kawseg/cpu/, kaw_battle_sim.c kaw_card_queries.c \
		kaw_cpu.c kaw_cpu_attack.c kaw_cpu_digivolve.c kaw_cpu_placement.c) \
	$(addprefix src/kawseg/duel/, kaw_bonus.c kaw_effect.c kaw_hand.c \
		kaw_tutorial.c) \
	src/kawseg/kaw_bss.c \
	$(addprefix src/kawseg/ui/, kaw_duel_menu.c kaw_exp.c kaw_hud.c \
		kaw_match_intro.c kaw_prize.c kaw_result.c)
NISSEG_C_SRC := \
	src/nisseg/debug/nis_debug_menu.c \
	$(addprefix src/nisseg/deck/, nis_auto_deck.c nis_deck_editor.c \
		nis_deck_scene.c nis_deck_windows.c) \
	src/nisseg/nis_bss.c \
	src/nisseg/title/nis_title.c \
	src/nisseg/trade/nis_trade.c \
	src/nisseg/viewer/nis_model_viewer.c \
	src/nisseg/vs/nis_vs_deck_select.c \
	src/nisseg/vs/nis_vs_mode.c
# eu's NISSEG is that Japanese debug build too, linked against the same
# executable (config/eu/symbols_nisseg_exe.txt): its C builds as jp's, with
# JP_DEBUG_BUILD, and is assembled with jp's ASPSX, which expands div and
# rem with their checks (mk/version/jp.mk)
build/eu/src/nisseg/%.c.o: CPPFLAGS += -UVERSION_EU -DVERSION_JP -DJP_DEBUG_BUILD=1
build/eu/src/nisseg/%.c.o: TEXT_ENCODING := cp932
build/eu/src/nisseg/%.c.o: MASPSXFLAGS += --expand-div
OPENSEG_C_SRC := \
	$(addprefix src/openseg/friend/, open_friend.c open_trade.c) \
	$(addprefix src/openseg/memcard/, open_memcard.c open_save.c) \
	src/openseg/open_bss.c \
	$(addprefix src/openseg/registration/, open_name_entry.c \
		open_registration.c open_starter.c) \
	$(addprefix src/openseg/title/, open_movie.c open_title.c)
SAISEG_C_SRC := \
	$(addprefix src/saiseg/area/, sai_area.c sai_opponent_info.c \
		sai_opponent_select.c sai_splash.c) \
	$(addprefix src/saiseg/hacking/, sai_hacking.c sai_word_input.c) \
	src/saiseg/map/sai_world_map.c \
	$(addprefix src/saiseg/player/, sai_digi_parts.c sai_partner_get.c \
		sai_player_data.c sai_reward.c) \
	$(addprefix src/saiseg/, sai_bss.c sai_data.c) \
	$(addprefix src/saiseg/script/, sai_area_script.c sai_flags.c) \
	$(addprefix src/saiseg/ui/, sai_choice.c sai_labels.c sai_panel.c \
		sai_sprite.c sai_text.c)
SUBSEG_C_SRC := \
	$(addprefix src/subseg/deck/, sub_auto_deck.c sub_base_deck.c \
		sub_deck_editor.c sub_deck_screens.c sub_name_entry.c sub_sort.c) \
	src/subseg/partner/sub_partner.c \
	src/subseg/sub_bss.c
SUGSEG_C_SRC := \
	$(addprefix src/sugseg/battle/, sug_battle.c sug_camera.c sug_hud.c \
		sug_sprite.c) \
	$(addprefix src/sugseg/effect/, sug_effect_script.c sug_fade_rect.c \
		sug_gradient.c sug_history.c sug_light_motion.c sug_model_effect.c \
		sug_screen_copy.c sug_scroll_texture.c sug_sphere.c \
		sug_sprite_effect.c sug_stage_fade.c sug_tex_anim.c sug_trail.c) \
	src/sugseg/sug_bss.c

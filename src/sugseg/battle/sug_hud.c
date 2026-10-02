#include "common.h"
#include "game.h"
#include "dcb/sug_hud.h"
#include "dcb/player_data.h"
#include "dcb/prim.h"
#include "dcb/sugseg.h"
#include "dcb/anim_control.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define setRECT(r, _x, _y, _w, _h) (r)->x = (_x), (r)->y = (_y), (r)->w = (_w), (r)->h = (_h)

typedef struct {
    float pos;
    s16 target[2];
    s16 trail[6];
    s16 flags;
    s16 brightness;
    Rect16 uv;
} HudSlide;

extern u16 SUG_HUD_TPAGE;

void SUG_fillShorts(s16 *dst, s32 count, s16 value) {
    s32 i;

    for (i = 0; i < count; i++) {
        dst[i] = value;
    }
}

void SUG_drawHudSpriteTrail(s32 x, s32 y, Rect16 *uv, u16 tpage, s32 clut, s32 otz, u8 brightness, s8 blend, s16 *trail, s32 count) {
    float level;
    s32 i;

    drawTexturedSprite(x, y, uv, tpage, clut, otz, brightness, blend);
    level = brightness;
    level -= 4.0f;
    otz++;
    for (i = 0; i < count; i++, level -= 4.0f, otz++) {
        if ((s32)level >= 9) {
            drawTexturedSprite(trail[i], y, uv, tpage, clut, otz, (s32)level, 3);
        }
    }
    for (i = count - 1; i >= 0; i--) {
        if (i == 0) {
            trail[i] = x;
        } else {
            trail[i] = trail[i - 1];
        }
    }
}

void SUG_initHudSlide(HudSlide *obj, s16 pos, s16 target0, s16 target1, s8 flags) {
    obj->pos = pos;
    obj->target[0] = target0;
    obj->target[1] = target1;
    obj->flags = flags;
    if (obj->flags & 2) {
        SUG_fillShorts(obj->trail, 6, pos);
    }
    obj->brightness = 0x80;
}

/* old-style definition: the callers pass an int, the byte is read here */
s32 SUG_tickHudSlides(obj, count, state, speed)
    HudSlide *obj;
    s32 count;
    s32 state;
    s8 speed;
{
    s32 i;
    s32 settled;

    i = 0;
    settled = 0;
    for (; i < count; i++, obj++) {
        switch (state) {
        case 0:
        case 1:
            obj->pos = (obj->target[state] - obj->pos) / (8 >> state) + obj->pos;
            if (ABS((s16)obj->pos - obj->target[state]) < 2) {
                settled++;
            }
            break;
        case 2:
            if (obj->flags & 1) {
                if (obj->brightness <= 0) {
                    obj->pos = (340.0f - obj->pos) * 0.125f + obj->pos;
                    if (ABS((s16)obj->pos - obj->target[state]) < 2) {
                        state = 3;
                    }
                } else {
                    obj->brightness--;
                }
            } else {
                obj->brightness -= speed;
                if (obj->brightness <= 0) {
                    state = 3;
                }
            }
            break;
        }
    }
    if (state < 2 && settled == count) {
        state++;
    }
    return state;
}

void SUG_drawNumber(s32 x, s32 y, s32 value, u8 brightness) {
    s32 digits[4];
    Rect16 uv;
    s32 any;
    s32 clut;
    s32 count;
    s32 i;
    s32 j;

    count = 1;
    digits[0] = value / 1000;
    digits[1] = value % 1000 / 100;
    digits[2] = value % 100 / 10;
    digits[3] = value % 10;
    uv.x = 0x78;
    uv.y = 0xA8;
    uv.w = 0x28;
    uv.h = 0x18;
    clut = 0x1469;
    if (x != 0x1F) {
        drawTexturedSprite(x, y, &uv, SUG_HUD_TPAGE, 0x1468, 1, brightness, 1);
        x += 16;
    }
    uv.w = 0x18;
    uv.y = 0xC0;
    for (i = 0; i < 4; i++) {
        j = 0;
        any = 0;
        do {
            any |= digits[j];
            j++;
        } while (j <= i);
        if (any != 0 || i == 3) {
            uv.x = digits[i] * 24;
            /* jp's digits are 24 pixels apart */
#if VERSION_JP
            drawTexturedSprite(x + count * 24, y, &uv, SUG_HUD_TPAGE, clut, 1, brightness, 1);
#elif VERSION_US || VERSION_EU
            drawTexturedSprite(x + 4 + count * 21, y, &uv, SUG_HUD_TPAGE, clut, 1, brightness, 1);
#endif
            count++;
        }
    }
}

void SUG_runDamagePopupTask(s32 side) {
    Rect16 uv0;
    Rect16 uv1;
    u8 brightness[3];
    s32 i;

    i = 0;
    memset(brightness, 0, 3);
    side ^= 1;
    uv0.x = 0xA0;
    uv0.y = 0x90;
    uv0.w = 0x60;
    uv0.h = 0x10;
    uv1.x = 0xA0;
    uv1.y = 0xB0;
    uv1.w = 0x3C;
    uv1.h = 0x10;
    for (; i < 150; i++) {
        waitFrames(FRAME_INTERVAL);
        drawTexturedSprite(15, 0xAE, &uv0, SUG_HUD_TPAGE, 0x1568, 1, brightness[0], 1);
        SUG_drawNumber(0x1F, 0xC1, SUG_BATTLE->players[side].damage, brightness[1]);
        drawTexturedSprite(0x3F, 0xD9, &uv1, SUG_HUD_TPAGE, 0x1569, 1, brightness[2], 1);
        if (i < 20) {
            brightness[0] = i * 8;
        } else if (i < 40) {
            brightness[1] = (i - 20) * 8;
        } else if (i < 60) {
            brightness[2] = (i - 40) * 8;
        } else if (i < 71) {
            /* hold */
        } else if (i < 90) {
            brightness[0] = (89 - i) * 8;
        } else if (i < 110) {
            brightness[1] = (109 - i) * 8;
        } else if (i < 130) {
            brightness[2] = (129 - i) * 8;
        }
    }
}

void SUG_animateHpCounter(s32 side) {
    Rect16 uv;
    s32 step;
    s32 count;
    s32 minFrame;
    s32 b;
    ModelData *model;

    count = 40;
    if (side >= 0) {
        spawnTask(0, -1, 0, 0x400, SUG_runDamagePopupTask, side);
        minFrame = 4;
    } else {
        side = ~side;
        minFrame = 0;
    }
    uv.x = SUG_BATTLE->players[side].element * 40;
    uv.y = 0xA8;
    uv.w = 0x28;
    uv.h = 0x18;
    step = SUG_TARGET_HP[side];
    if (step < 0) {
        step = 0;
    }
    step = (SUG_BATTLE->players[side].hp - step) / 40;
    if (step == 0 && SUG_TARGET_HP[side] != SUG_BATTLE->players[side].hp) {
        if (SUG_BATTLE->players[side].hp < SUG_TARGET_HP[side]) {
            step = -1;
        } else {
            step = 1;
        }
        count = SUG_TARGET_HP[side] - SUG_BATTLE->players[side].hp < 0 ? -(SUG_TARGET_HP[side] - SUG_BATTLE->players[side].hp)
                                                                         : SUG_TARGET_HP[side] - SUG_BATTLE->players[side].hp;
    }
    do {
        waitFrames(FRAME_INTERVAL);
        model = SCENE_3D->models[side];
        if (model->animKeyTimer >= 0 && model->animClip >= minFrame) {
            if ((SUG_BATTLE->players[side].hp -= step) < 0) {
                SUG_BATTLE->players[side].hp = 0;
                break;
            }
            count--;
        }
        SUG_drawNumber(0xA8, 0x10, SUG_BATTLE->players[side].hp, 0x80);
        drawTexturedSprite(0x78, 0x10, &uv, SUG_HUD_TPAGE, getClut(SUG_BATTLE->players[side].element * 16 + 0x290, 0x50), 1, 0x80, 1);
    } while (count > 0);
    if (SUG_TARGET_HP[side] > 0) {
        SUG_BATTLE->players[side].hp = SUG_TARGET_HP[side];
    } else {
        SUG_BATTLE->players[side].hp = 0;
    }
    for (b = 0x80; b >= 0; b -= 4) {
        waitFrames(FRAME_INTERVAL);
        SUG_drawNumber(0xA8, 0x10, SUG_BATTLE->players[side].hp, b);
        drawTexturedSprite(0x78, 0x10, &uv, SUG_HUD_TPAGE, getClut(SUG_BATTLE->players[side].element * 16 + 0x290, 0x50), 1, b, 1);
    }
}

void SUG_showAttackLabel(s32 side) {
    HudSlide obj;
    HudSlide obj2;
    Rect16 uv2;
    s32 state;
    s32 alt;
    s32 both;

    state = 0;
    alt = SUG_BATTLE->players[side].crash;
    both = (SUG_BATTLE->flags.bits.flag1 != 0) | (alt != 0);
    SUG_initHudSlide(&obj, 0x140, both * 32 + 0x4E, both * 32 + 0x50, 2);
    obj.uv.x = 0;
    obj.uv.y = 0x88;
    obj.uv.w = 0xA0;
    obj.uv.h = 0x10;
    obj.brightness = 0x5A;
    SUG_initHudSlide(&obj2, -0x40, 0x32, 0x30, 2);
    obj2.uv.x = alt * 64;
    obj2.uv.y = 0x58;
    obj2.uv.w = 0x40;
    obj2.uv.h = 0x20;
    uv2.x = SUG_BATTLE->players[side].attack * 24 + 0xA0;
    uv2.y = 0xA0;
    uv2.w = 0x18;
    uv2.h = 0x10;
    do {
        waitFrames(FRAME_INTERVAL);
        state = SUG_tickHudSlides(&obj, SUG_BATTLE->flags.bits.counter + 1, state, 2);
        SUG_drawHudSpriteTrail((s16)obj.pos, 0xA2, &obj.uv, SUG_HUD_TPAGE, getClut(0x280, 0x53), 1, 0x80, 1, obj.trail, 1);
        if (both) {
            SUG_drawHudSpriteTrail((s16)obj2.pos, 0x92, &obj2.uv, SUG_HUD_TPAGE, getClut(alt * 16 + 0x2A0, 0x54), 1, 0x80, 0, obj2.trail, 6);
        }
        drawTexturedSprite((s16)obj.pos + 6, 0xA2, &uv2, SUG_HUD_TPAGE, getClut(SUG_BATTLE->players[side].attack * 16 + 0x280, 0x52), 0, 0x80, 1);
    } while (state != 2);
}

void SUG_showAttackBanner(s32 side) {
    HudSlide banners[5];
    Rect16 unused; /* unused, but it is in the original stack frame */
    Rect16 uv0;
    Rect16 uv1;
    s32 alt;
    s32 flipped;
    s32 c;
    s32 b;
    s32 a;
    s32 count;
    s32 state;
    s32 both;

    flipped = 0;
    state = 0;
    count = 2;
    a = -1;
    b = -1;
    c = -1;
    if (side >= 0) {
        SUG_initHudSlide(&banners[0], -0x100, 0x2B, 0x20, 2);
        banners[0].uv.x = 0;
        banners[0].uv.y = 0;
        banners[0].uv.w = 0x100;
        banners[0].uv.h = 0x28;
    } else {
        SUG_initHudSlide(&banners[0], -0x80, -0x80, 0x60, 2);
        banners[0].uv.x = 0x80;
        banners[0].uv.y = 0x58;
        banners[0].uv.w = 0x80;
        banners[0].uv.h = 0x20;
        /* -side - 1 rather than ~side: jp's match depends on it, for its
           register allocation */
        side = -side - 1;
        flipped = 1;
    }
    uv0.x = SUG_BATTLE->players[side].element * 40;
    uv0.y = 0xA8;
    uv0.w = 0x28;
    uv0.h = 0x18;
    uv1.x = SUG_BATTLE->players[side].attack * 24 + 0xA0;
    uv1.y = 0xA0;
    uv1.w = 0x18;
    uv1.h = 0x10;
    alt = SUG_BATTLE->players[side].crash;
    both = (SUG_BATTLE->flags.bits.flag1 != 0) | (alt != 0);
    if (!SUG_BATTLE->flags.bits.counter) {
        SUG_initHudSlide(&banners[1], 0x140, both * 32 + 0x4E, both * 32 + 0x50, 2);
        if (both) {
            a = count++;
            SUG_initHudSlide(&banners[a], -0x40, 0x32, 0x30, 2);
        }
    } else {
        if (both) {
            a = count++;
            SUG_initHudSlide(&banners[a], 0x30, -0xFA, -0xFA, 2);
        }
        SUG_initHudSlide(&banners[1], both * 32 + 0x50, -0xC8, -0xC8, 2);
        b = count++;
        SUG_initHudSlide(&banners[b], 0x140, 0x4E, 0x50, 2);
    }
    if (SUG_BATTLE->players[side].unk8_6) {
        c = count++;
        SUG_initHudSlide(&banners[c], banners[1].pos + 160.0f, banners[1].target[0] + 0xA0, banners[1].target[1] + 0xA0, 2);
        setRECT(&banners[c].uv, 0xE8, 0xA0, 0x18, 0x20);
    }
    banners[1].uv.x = 0;
    banners[1].uv.y = 0x88;
    banners[1].uv.w = 0xA0;
    banners[1].uv.h = 0x10;
    if (a >= 0) {
        setRECT(&banners[a].uv, alt * 64, 0x58, 0x40, 0x20);
    }
    if (b >= 0) {
        setRECT(&banners[b].uv, 0, 0x98, 0x9F, 0x10);
    }
    banners[1].brightness = 0x5A;
    do {
        waitFrames(FRAME_INTERVAL);
        state = SUG_tickHudSlides(&banners[0], count, state, side < 0 ? 1 : 2);
        if (!flipped) {
            SUG_drawHudSpriteTrail((s16)banners[0].pos, 0xB4, &banners[0].uv, SUG_HUD_TPAGE, 0x1428, 1, banners[0].brightness, 0, banners[0].trail, 6);
        } else {
            SUG_drawHudSpriteTrail((s16)banners[0].pos, 0xC2, &banners[0].uv, SUG_HUD_TPAGE, 0x146A, 1, banners[0].brightness, 0, banners[0].trail, 6);
        }
        SUG_drawHudSpriteTrail((s16)banners[1].pos, 0xA2, &banners[1].uv, SUG_HUD_TPAGE, 0x14E8, 1, banners[1].brightness, 1, banners[1].trail, 1);
        if (a >= 0) {
            SUG_drawHudSpriteTrail((s16)banners[a].pos, 0x92, &banners[a].uv, SUG_HUD_TPAGE, getClut(alt * 16 + 0x2A0, 0x54), 1, banners[a].brightness, 0, banners[a].trail, 6);
        }
        if (b >= 0) {
            SUG_drawHudSpriteTrail((s16)banners[b].pos, 0xA2, &banners[b].uv, SUG_HUD_TPAGE, 0x14E9, 1, banners[b].brightness, 0, banners[b].trail, 6);
        } else {
            drawTexturedSprite((s16)banners[1].pos + 6, 0xA2, &uv1, SUG_HUD_TPAGE, getClut(SUG_BATTLE->players[side].attack * 16 + 0x280, 0x52), 0, banners[1].brightness, 1);
        }
        if (c >= 0) {
            SUG_drawHudSpriteTrail((s16)banners[c].pos, 0x98, &banners[c].uv, SUG_HUD_TPAGE, 0x14EA, 1, banners[c].brightness, 0, banners[c].trail, 6);
        }
        drawTexturedSprite(0x10, 0x10, &uv0, SUG_HUD_TPAGE, getClut(SUG_BATTLE->players[side].element * 16 + 0x290, 0x50), 1, banners[0].brightness, 1);
    } while (state != 3);
    SUG_BATTLE->flags.bits.flag1 = 0;
    SUG_BATTLE->flags.bits.counter = 0;
}

void SUG_showHpBanner(s32 side) {
    HudSlide bar;
    HudSlide icon;
#if VERSION_JP
    HudSlide label;
#endif
    HudSlide num;
#if VERSION_US || VERSION_EU
    s32 unused[8]; /* unused, but it is in the original stack frame */
#endif
    s32 state;

    state = 0;
    SUG_initHudSlide(&bar, -0x100, 0x3C, 0x32, 2);
    bar.uv.x = 0;
    bar.uv.y = 0xD8;
    bar.uv.w = 0x100;
    bar.uv.h = 0x28;
    SUG_initHudSlide(&icon, -0x18, 0xC6, 0xBC, 2);
    icon.uv.x = SUG_BATTLE->players[side].element * 40;
    icon.uv.y = 0xA8;
    icon.uv.w = 0x28;
    icon.uv.h = 0x18;
    /* jp also slides in a label */
#if VERSION_JP
    SUG_initHudSlide(&label, 0x140, 0, 0xA, 2);
    label.uv.x = 0;
    label.uv.y = 0x78;
    label.uv.w = 0xA0;
    label.uv.h = 0x10;
#endif
    SUG_initHudSlide(&num, -0x18, 0x10, 0x10, 2);
    waitFrames(0x28);
    do {
        waitFrames(FRAME_INTERVAL);
#if VERSION_JP
        state = SUG_tickHudSlides(&bar, 4, state, 2);
#elif VERSION_US || VERSION_EU
        state = SUG_tickHudSlides(&bar, 3, state, 2);
#endif
        SUG_drawHudSpriteTrail((s16)bar.pos, 0xB4, &bar.uv, SUG_HUD_TPAGE, 0x1528, 1, bar.brightness, 0, bar.trail, 6);
        SUG_drawHudSpriteTrail(10, (s16)icon.pos, &icon.uv, SUG_HUD_TPAGE,
                      getClut(0x290 + SUG_BATTLE->players[side].element * 16, 0x50), 1, icon.brightness, 0,
                      icon.trail, 6);
#if VERSION_JP
        SUG_drawHudSpriteTrail((s16)label.pos, 0xA2, &label.uv, SUG_HUD_TPAGE, 0x1529, 1, label.brightness, 0, label.trail, 6);
#endif
        SUG_drawNumber(0xA8, (s16)num.pos, SUG_BATTLE->players[side].hp, num.brightness);
        if (((ModelData *)SCENE_3D->models[side])->animKeyTimer < 0) {
            /* jp's models keep their animations loaded */
#if VERSION_JP
            startModelAnimation(side, 0, -2, 0);
#elif VERSION_US || VERSION_EU
            playModelAnimation(side, 0);
#endif
        }
    } while (state != 3);
    waitFrames(FRAME_INTERVAL);
}

void SUG_showWinnerBanner(s32 side) {
    HudSlide banner;
    HudSlide icon;
    s32 state;

    state = 0;
    SUG_initHudSlide(&banner, -0x100, 0x3C, 0x32, 3);
    banner.uv.x = 0;
    banner.uv.y = 0x28;
    banner.uv.w = 0x100;
    banner.uv.h = 0x28;
    SUG_initHudSlide(&icon, -0x18, 0xC6, 0xBC, 3);
    icon.uv.x = SUG_BATTLE->players[side].element * 40;
    icon.uv.y = 0xA8;
    icon.uv.w = 0x28;
    icon.uv.h = 0x18;
    do {
        waitFrames(FRAME_INTERVAL);
        state = SUG_tickHudSlides(&banner, 2, state, 2);
        SUG_drawHudSpriteTrail((s16)banner.pos, 0xB4, &banner.uv, SUG_HUD_TPAGE, 0x14EB, 1, 0x80, 0, banner.trail, 6);
        SUG_drawHudSpriteTrail(10, (s16)icon.pos, &icon.uv, SUG_HUD_TPAGE, getClut(0x290 + SUG_BATTLE->players[side].element * 16, 0x50), 1,
                      0x80, 0, icon.trail, 6);
    } while (state != 3);
}

void SUG_showEatUpHpBanner(void) {
    HudSlide obj;
    Rect16 unused; /* unused, but it is in the original stack frame */
    s32 state;

    state = 0;
    SUG_initHudSlide(&obj, -0x60, 0x7A, 0x70, 3);
    obj.uv.x = 0xA0;
    obj.uv.y = 0x78;
    obj.uv.w = 0x60;
    obj.uv.h = 0x18;
    obj.brightness = 0x20;
    do {
        waitFrames(FRAME_INTERVAL);
        state = SUG_tickHudSlides(&obj, 1, state, 2);
        SUG_drawHudSpriteTrail((s16)obj.pos, 0xB4, &obj.uv, SUG_HUD_TPAGE, 0x146B, 1, 0x80, 0, obj.trail, 6);
    } while (state != 3);
}

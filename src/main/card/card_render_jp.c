#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/card_render.h"
#include "dcb/card_db.h"
#include "dcb/battle_hud.h"
#include "dcb/heap.h"
#include "dcb/text.h"

/* jp's card_render (card_render.c is us's and eu's): the duel's HUD panels,
   textured quads it moves around in 3D (us has flat ones, in KAWSEG), the
   duel's background and the HUD's small sprites, then the card tables' data
   (the shop's booster packs, the cross effects' names) */

/* a HUD panel's rect in the texture page */
typedef struct {
    /* 0x0 */ u8 u;
    /* 0x1 */ u8 v;
    /* 0x2 */ u8 w;
    /* 0x3 */ u8 h;
    /* 0x4 */ u16 tpage;
    /* 0x6 */ u16 clut;
    /* 0x8 */ u8 flipped; /* bit 0: turned over (rotated 180 degrees on x) */
    /* 0x9 */ u8 unk9;
} HudPanelInit;

#define MARK(i) (DUEL->marks[i])
/* libgpu's */
#define setPolyFT4(p) (setlen(p, 9), setcode(p, 0x2C))
#define setUV4(p, _u0, _v0, _u1, _v1, _u2, _v2, _u3, _v3)                                         \
    ((p)->u0 = (_u0), (p)->v0 = (_v0), (p)->u1 = (_u1), (p)->v1 = (_v1), (p)->u2 = (_u2), (p)->v2 = (_v2), \
     (p)->u3 = (_u3), (p)->v3 = (_v3))
#define setXY4(p, _x0, _y0, _x1, _y1, _x2, _y2, _x3, _y3)                                         \
    ((p)->x0 = (_x0), (p)->y0 = (_y0), (p)->x1 = (_x1), (p)->y1 = (_y1), (p)->x2 = (_x2), (p)->y2 = (_y2), \
     (p)->x3 = (_x3), (p)->y3 = (_y3))

HudPanelInit HUD_PANEL_INITS[23] = {
    { 0x98, 0x00, 0x60, 0x50, 0x1D, 0x6837, 0, 0 }, { 0x00, 0xC0, 0x50, 0x38, 0x1C, 0x6837, 0, 0 },
    { 0xE8, 0x00, 0x18, 0x48, 0x1C, 0x6837, 0, 0 }, { 0x00, 0x00, 0xE8, 0x40, 0x1C, 0x6837, 0, 0 },
    { 0xB0, 0xB0, 0x50, 0x48, 0x1C, 0x6837, 0, 0 }, { 0x00, 0x00, 0x98, 0x60, 0x1D, 0x6837, 0, 0 },
    { 0x00, 0xA0, 0xE1, 0x10, 0x1C, 0x6837, 0, 0 }, { 0x00, 0xB0, 0xAF, 0x10, 0x1C, 0x6837, 0, 0 },
    { 0x00, 0x00, 0xD8, 0x50, 0x1B, 0x702F, 0, 0 }, { 0x00, 0x50, 0xB0, 0x60, 0x1B, 0x70AF, 0, 0 },
    { 0x00, 0xB0, 0x80, 0x10, 0x1B, 0x70EF, 0, 0 }, { 0x98, 0x50, 0x60, 0x50, 0x1D, 0x6837, 0, 0 },
    { 0x50, 0xC0, 0x50, 0x38, 0x1C, 0x6837, 0, 0 }, { 0xE8, 0x48, 0x18, 0x48, 0x1C, 0x6837, 0, 0 },
    { 0x00, 0x40, 0xE8, 0x40, 0x1C, 0x6837, 0, 0 }, { 0xB0, 0xB0, 0x50, 0x48, 0x1C, 0x6837, 1, 0 },
    { 0x00, 0x60, 0x98, 0x60, 0x1D, 0x6837, 0, 0 }, { 0x00, 0xA0, 0xE1, 0x10, 0x1C, 0x6837, 0, 0 },
    { 0x00, 0xB0, 0xAF, 0x10, 0x1C, 0x6837, 0, 0 }, { 0x00, 0x00, 0xD8, 0x50, 0x1B, 0x702F, 0, 0 },
    { 0x00, 0x50, 0xB0, 0x60, 0x1B, 0x70AF, 0, 0 }, { 0x00, 0xB0, 0x80, 0x10, 0x1B, 0x70EF, 0, 0 },
    { 0x00, 0x80, 0xE0, 0x20, 0x1C, 0x6837, 0, 0 },
};
/* the frame the duel's animated background is at */
s32 DUEL_BG_FRAME = 0;

/* the duel's 23 HUD panels and their polys, in the session block */
void initHudPanels(void) {
    s32 i;

    ((SessionData *)SESSION_DATA)->prims = allocTaskHeapBlock(0x820);
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[11] = (s32)&((SessionData *)SESSION_DATA)->prims[i * 0x410];
    }
    ((SessionData *)SESSION_DATA)->panels = allocTaskHeapBlock(23 * sizeof(Panel));
    for (i = 0; i < 23; i++) {
        HUD_PANEL(i)->code = 0x2C;
        HUD_PANEL(i)->rgb[0] = 0x80;
        HUD_PANEL(i)->rgb[1] = 0x80;
        HUD_PANEL(i)->rgb[2] = 0x80;
        HUD_PANEL(i)->uv[0][0] = HUD_PANEL_INITS[i].u;
        HUD_PANEL(i)->uv[0][1] = HUD_PANEL_INITS[i].v;
        HUD_PANEL(i)->uv[1][0] = HUD_PANEL_INITS[i].u + (HUD_PANEL_INITS[i].w - 1);
        HUD_PANEL(i)->uv[1][1] = HUD_PANEL_INITS[i].v;
        HUD_PANEL(i)->uv[2][0] = HUD_PANEL_INITS[i].u;
        HUD_PANEL(i)->uv[2][1] = HUD_PANEL_INITS[i].v + (HUD_PANEL_INITS[i].h - 1);
        HUD_PANEL(i)->uv[3][0] = HUD_PANEL_INITS[i].u + (HUD_PANEL_INITS[i].w - 1);
        HUD_PANEL(i)->uv[3][1] = HUD_PANEL_INITS[i].v + (HUD_PANEL_INITS[i].h - 1);
        HUD_PANEL(i)->tpage = HUD_PANEL_INITS[i].tpage;
        HUD_PANEL(i)->clut = HUD_PANEL_INITS[i].clut;
        HUD_PANEL(i)->w = HUD_PANEL_INITS[i].w - 1;
        HUD_PANEL(i)->h = HUD_PANEL_INITS[i].h - 1;
        HUD_PANEL(i)->pos.vx = 0;
        HUD_PANEL(i)->pos.vy = 0;
        HUD_PANEL(i)->pos.vz = 0;
        HUD_PANEL(i)->rot.vx = (HUD_PANEL_INITS[i].flipped & 1) << 11;
        HUD_PANEL(i)->rot.vy = 0;
        HUD_PANEL(i)->rot.vz = 0;
        HUD_PANEL(i)->flags = 0;
        HUD_PANEL(i)->state = 0;
        HUD_PANEL(i)->sx = 0x140;
        HUD_PANEL(i)->sy = 0x100;
    }
}

void freeHudPanels(void) {
    freeHeapBlock(((SessionData *)SESSION_DATA)->prims);
    freeHeapBlock(((SessionData *)SESSION_DATA)->panels);
}

/* draws PANEL (HUD panel INDEX) at depth Z, and keeps where its first corner
   landed on screen */
void renderHudPanel(Panel *panel, s32 index, s32 z) {
    MATRIX matrix;
    SVECTOR v[4];
    s32 sxy[4];
    s32 depthCue;
    s32 flag;
    u32 *pk; /* its POLY_FT4, word by word */
    u16 tpage;
    u16 clut;

    buildRotTransMatrix(&panel->pos, &panel->rot, &matrix);
    CompMatrix((MATRIX *)SCENE_3D->viewMatrix, &matrix, &matrix);
    SetRotMatrix((s32)&matrix);
    SetTransMatrix(&matrix);
    v[0].vx = 0;
    v[0].vy = 0;
    v[0].vz = 0;
    v[1].vx = panel->w;
    v[1].vy = 0;
    v[1].vz = 0;
    v[2].vx = 0;
    v[2].vy = panel->h;
    v[2].vz = 0;
    v[3].vx = panel->w;
    v[3].vy = panel->h;
    v[3].vz = 0;
    RotAverage4(&v[0], &v[1], &v[2], &v[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
    tpage = panel->tpage;
    clut = panel->clut;
    panel->sx = sxy[0];
    panel->sy = sxy[0] >> 16;
    pk = (u32 *)CURRENT_FRAME_BUFFER->primSlots[11];
    pk += index * (sizeof(POLY_FT4) / 4);
    pk[0] = 0x09000000;
    pk[1] = *(u32 *)panel->rgb;
    pk[2] = sxy[0];
    pk[3] = (clut << 16) | (panel->uv[0][1] << 8) | panel->uv[0][0];
    pk[4] = sxy[1];
    pk[5] = (tpage << 16) | (panel->uv[1][1] << 8) | panel->uv[1][0];
    pk[6] = sxy[2];
    pk[7] = (panel->uv[2][1] << 8) | panel->uv[2][0];
    pk[8] = sxy[3];
    pk[9] = (panel->uv[3][1] << 8) | panel->uv[3][0];
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], pk);
}

/* the duel's background: an animated 160x240 image, its right half
   mirrored, behind everything */
void renderDuelBackground(s32 brightness) {
    POLY_FT4 *poly;

    DUEL_BG_FRAME++;
    poly = (POLY_FT4 *)CURRENT_FRAME_BUFFER->primSlots[11] + 23;
    setPolyFT4(poly);
    setRGB0(poly, brightness, brightness, brightness);
    setUV4(poly, 0, 0, 0xA0, 0, 0, 0xF0, 0xA0, 0xF0);
    setXY4(poly, 0, 0, 0xA0, 0, 0, 0xF0, 0xA0, 0xF0);
    poly->tpage = 0x1E;
    poly->clut = getClut(0x380, 0x1F8 + DUEL_BG_FRAME / 4 % 6);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], poly);
    poly++;
    setPolyFT4(poly);
    setRGB0(poly, brightness, brightness, brightness);
    setUV4(poly, 0, 0, 0xA0, 0, 0, 0xF0, 0xA0, 0xF0);
    setXY4(poly, 0x13F, 0, 0x9F, 0, 0x13F, 0xF0, 0x9F, 0xF0);
    poly->tpage = 0x1E;
    poly->clut = getClut(0x380, 0x1F8 + DUEL_BG_FRAME / 4 % 6);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], poly);
}

/* an animated placeholder (four 32-pixel frames) where the card's art
   isn't loaded yet */
void drawCardArtPlaceholder(s32 x, s32 y, s32 z) {
    POLY_FT4 *poly;
    s32 u;

    u = ((PlayerProfile *)PLAYER_PROFILES)->playTime / 4 % 4 * 32;
    poly = (POLY_FT4 *)CURRENT_FRAME_BUFFER->primSlots[11] + 25;
    setPolyFT4(poly);
    setRGB0(poly, 0x80, 0x80, 0x80);
    setUV4(poly, u + 0x40, 0, u + 0x60, 0, u + 0x40, 0x40, u + 0x60, 0x40);
    setXY4(poly, x, y, x + 0x40, y, x, y + 0x40, x + 0x40, y + 0x40);
    poly->tpage = 0x1A;
    poly->clut = 0x7E71;
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], poly);
}

/* slides HUD mark MARK out when what it shows changes to KIND (-1: nothing),
   then back in up to X; 0 while it shows something */
s32 slideHudMark(s32 x, s32 y, s32 kind, s32 mark) {
    if (MARK(mark).owner != kind) {
        MARK(mark).shown = 1;
    }
    if (MARK(mark).shown != 0) {
        MARK(mark).x -= 8;
        if (MARK(mark).x < -0x50) {
            MARK(mark).x = -0x50;
            MARK(mark).owner = kind;
            if (kind == -1) {
                return -1;
            }
            MARK(mark).shown = 0;
        }
    } else if (MARK(mark).shown == 0) {
        MARK(mark).x += 8;
        if (MARK(mark).x > x) {
            MARK(mark).x = x;
        }
        MARK(mark).y = y;
    }
    if (MARK(mark).owner == -1) {
        return -1;
    }
    return 0;
}

/* the marks at the screen's side: the step's (mark 0), the player's
   (mark 1), the turn's (mark 2) and, from mark 3 down, what the buttons do
   (drawButtonMarks) */
void drawStepMark(s32 x, s32 y, s32 kind, s32 z) {
    if (slideHudMark(x, y, kind, 0) == 0 && isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = MARK(0).x;
        CUR_SPRT->sp.y0 = MARK(0).y;
        CUR_SPRT->sp.u0 = 0xA0;
        CUR_SPRT->sp.v0 = (MARK(0).owner * 16 + 0x100) % 256;
        CUR_SPRT->sp.clut = 0x7D3B;
        CUR_SPRT->sp.w = 0x50;
        CUR_SPRT->sp.h = 0x10;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

void drawHudMark(s32 x, s32 y, s32 kind, s32 mark, s32 z) {
    if (slideHudMark(x, y, kind, mark) == 0 && isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = MARK(mark).x;
        CUR_SPRT->sp.y0 = MARK(mark).y;
        CUR_SPRT->sp.u0 = 0xA0;
        CUR_SPRT->sp.v0 = (MARK(mark).owner * 14 + 0x130) % 256;
        CUR_SPRT->sp.clut = 0x7D3B;
        CUR_SPRT->sp.w = 0x50;
        CUR_SPRT->sp.h = 0xE;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

/* the buttons' marks for the duel's state DUEL->unk44A, one under the
   other from Y; the state becomes STATE */
void drawButtonMarks(s32 x, s32 y, s32 state, s32 z) {
    s32 dy;

    dy = 0;
    switch (DUEL->unk44A) {
    case 0:
    case 1:
    case 2:
    case 4:
    case 7:
    case 8:
    case 9:
        drawHudMark(x, y + dy, 1, dy / 14 + 3, z);
        dy += 14;
        break;
    }
    switch (DUEL->unk44A) {
    case 1:
    case 2:
    case 3:
    case 6:
    case 7:
    case 8:
        drawHudMark(x, y + dy, 2, dy / 14 + 3, z);
        dy += 14;
        break;
    }
    switch (DUEL->unk44A) {
    case 0:
        drawHudMark(x, y + dy, 0, dy / 14 + 3, z);
        dy += 14;
        break;
    case 1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 8:
        drawHudMark(x, y + dy, 3, dy / 14 + 3, z);
        dy += 14;
        break;
    }
    switch (DUEL->unk44A) {
    case 0:
    case 1:
    case 2:
    case 3:
        drawHudMark(x, y + dy, 4, dy / 14 + 3, z);
        dy += 14;
        break;
    }
    for (; dy < 0x38; dy += 14) {
        MARK(dy / 14 + 3).owner = -1;
    }
    if (DUEL->unk44A != state) {
        DUEL->unk44A = state;
    }
}

void drawTurnMark(s32 x, s32 y, s32 kind, s32 z) {
    if (slideHudMark(x, y, kind, 2) == 0 && isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = MARK(2).x + 0x18;
        CUR_SPRT->sp.y0 = MARK(2).y;
        CUR_SPRT->sp.u0 = 0xA0;
        CUR_SPRT->sp.v0 = (MARK(2).owner * 14 + 0x176) % 256;
        CUR_SPRT->sp.clut = 0x7D3B;
        CUR_SPRT->sp.w = 0x38;
        CUR_SPRT->sp.h = 0xE;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

void drawPlayerMark(s32 x, s32 y, s32 kind, s32 z) {
    if (slideHudMark(x, y, kind, 1) == 0 && isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = MARK(1).x;
        CUR_SPRT->sp.y0 = MARK(1).y;
        CUR_SPRT->sp.u0 = 0xD8;
        CUR_SPRT->sp.v0 = (MARK(1).owner * 14 + 0x176) % 256;
        CUR_SPRT->sp.clut = 0x7D7B;
        CUR_SPRT->sp.w = 0x18;
        CUR_SPRT->sp.h = 0xE;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

/* SIDE 2: the turn's light, blinking at full BRIGHTNESS; else a 32x16
   badge of the side */
void drawTurnSideBadge(s32 x, s32 y, s32 side, s32 brightness, s32 z) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        if (side == 2) {
            CUR_SPRT->sp.u0 = 0xD0;
            CUR_SPRT->sp.v0 = 0x78;
            if (brightness == 0x80) {
                CUR_SPRT->sp.clut = getClut(0x370, 0x1F0 + ((PlayerProfile *)PLAYER_PROFILES)->playTime / 4 % 16);
            } else {
                CUR_SPRT->sp.clut = 0x7C37;
            }
            CUR_SPRT->sp.w = 12;
            CUR_SPRT->sp.h = 12;
        } else {
            CUR_SPRT->sp.u0 = 0xE0;
            CUR_SPRT->sp.v0 = (side * 16 + 0x128) % 256;
            CUR_SPRT->sp.clut = getClut(0x2F0, side + 0x1C5);
            CUR_SPRT->sp.w = 0x20;
            CUR_SPRT->sp.h = 0x10;
        }
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = brightness;
        CUR_SPRT->sp.g0 = brightness;
        CUR_SPRT->sp.b0 = brightness;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1B);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

/* a win's star, SIZE 0 (16x16) or 1 (24x24); -1: none */
void drawWinMarker(s32 x, s32 y, s32 size, s32 z) {
    if (size != -1 && isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = 0xE0;
        CUR_SPRT->sp.v0 = (size * 16 + 0x100) % 256;
        CUR_SPRT->sp.clut = 0x712F;
        CUR_SPRT->sp.w = size * 8 + 0x10;
        CUR_SPRT->sp.h = size * 8 + 0x10;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1B);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

/* the six booster packs of the card shop (SUBSEG) */
BoosterPack BOOSTER_PACKS[6] = {
    { 20, 3, "赤", { "１", "２", "３", "４" } },
    { 20, 3, "青", { "１", "２", "３", "４" } },
    { 20, 3, "緑", { "１", "２", "３", "４" } },
    { 20, 3, "黒", { "１", "２", "３", "４" } },
    { 20, 3, "黄", { "１", "２", "３", "４" } },
    { 10, 3, "基本", { "１", "２", "３", "４" } },
};
/* the cross effects, as shown on a card */
u8 *CROSS_EFFECT_SHORT_NAMES[16] = {
    "~ﾅｼ",
    "~ｾﾝｾｲ",
    "\001\010~ｦ0ﾆ",
    "\001\011~ｦ0ﾆ",
    "\001\n~ｦ0ﾆ",
    "\001\010ｶｳﾝﾀｰ",
    "\001\011ｶｳﾝﾀｰ",
    "\001\nｶｳﾝﾀｰ",
    "~ｼﾞﾊﾞｸ",
    "~ｽｲﾄﾙ",
    "~ﾎﾞｳｶﾞｲ",
    "~ﾀｲ\001\001*3",
    "~ﾀｲ\001\002*3",
    "~ﾀｲ\001\003*3",
    "~ﾀｲ\001\004*3",
    "~ﾀｲ\001\005*3",
};
/* the cross effects, as shown in the battle panel */
u8 *CROSS_EFFECT_NAMES[16] = {
    "なし",
    "~先制",
    "b0を０に",
    "b1を０に",
    "b2を０に",
    "b0カウンター",
    "b1カウンター",
    "b2カウンター",
    "自爆",
    "すいとる",
    "ぼうがい",
    "対a0×３",
    "対a1×３",
    "対a2×３",
    "対a3×３",
    "対a4×３",
};
/* jp's has four more, all 0 */
u8 CROSS_EFFECT_ICONS[20] = { 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 3, 1, 1, 1, 1, 1 };

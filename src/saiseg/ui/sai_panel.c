#include "common.h"
#include "game.h"
#include "dcb/sai_panel.h"
#include "dcb/card_render.h"
#include "dcb/saiseg.h"
#include "dcb/sai_labels.h"
#include "dcb/sai_sprite.h"
#include "dcb/sai_world_map.h"

void SAI_initPanelCover(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        SetPolyF4(&SAI_AREA.coverPolys[i]);
        SetSemiTrans(&SAI_AREA.coverPolys[i], 1);
        SetDrawTPage(&SAI_AREA.coverTpages[i], 0, 0, 0x40);
        SAI_AREA.coverPolys[i].b0 = 0xFF;
        SAI_AREA.coverPolys[i].g0 = 0xFF;
        SAI_AREA.coverPolys[i].r0 = 0xFF;
    }
    SAI_AREA.coverAlpha = 0xFF;
    SAI_AREA.pos.vz = 0;
    SAI_AREA.pos.vy = 0;
    SAI_AREA.pos.vx = 0;
    SAI_AREA.rot.vz = 0;
    SAI_AREA.rot.vy = 0;
    SAI_AREA.rot.vx = 0;
    SAI_AREA.corners[0].vx = SAI_AREA.corners[2].vx = -0x87;
    SAI_AREA.corners[1].vx = SAI_AREA.corners[3].vx = 0x8A;
    SAI_AREA.corners[0].vy = SAI_AREA.corners[1].vy = -0x4D;
    SAI_AREA.corners[2].vy = SAI_AREA.corners[3].vy = 0x48;
    for (i = 0; i < 4; i++) {
        SAI_AREA.corners[i].vz = 0;
    }
}

void SAI_drawPanelCover(void) {
    MATRIX matrix;
    SVECTOR corners[4];
    s16 xs[2] = { -0x8E, 0xB2 }; /* unused, but it is in the original stack frame */
    s16 ys[2] = { -0x4D, 0x48 }; /* unused, but it is in the original stack frame */
    s32 sxy[4];
    s32 depthCue;
    s32 flag;

    buildRotTransMatrix(&SAI_AREA.pos, &SAI_AREA.rot, &matrix);
    CompMatrix((MATRIX *)SCENE_3D->viewMatrix, &matrix, &matrix);
    SetRotMatrix((s32)&matrix);
    SetTransMatrix(&matrix);
    corners[0] = SAI_AREA.corners[0];
    corners[1] = SAI_AREA.corners[1];
    corners[2] = SAI_AREA.corners[2];
    corners[3] = SAI_AREA.corners[3];
    RotAverage4(&corners[0], &corners[1], &corners[2], &corners[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].r0 = SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].g0 = SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].b0 = SAI_AREA.coverAlpha;
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].x0 = sxy[0];
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].y0 = sxy[0] >> 16;
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].x1 = sxy[1];
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].y1 = sxy[1] >> 16;
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].x2 = sxy[2];
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].y2 = sxy[2] >> 16;
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].x3 = sxy[3];
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].y3 = sxy[3] >> 16;
    addPrim(&CURRENT_FRAME_BUFFER->ot[36], &SAI_AREA.coverPolys[FRAME_BUFFER_INDEX]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[36], &SAI_AREA.coverTpages[FRAME_BUFFER_INDEX]);
}

/* not called by any code */
void SAI_createPanelSprite(void) {
    Rect16 rect;

    rect.x = 0x180;
    rect.y = 0x100;
    rect.w = 0xFF;
    rect.h = 0x80;
    if (SAI_AREA.unk11E != 0) {
        SAI_SPRITES[0] = SAI_createSprite(4);
        SAI_AREA.unk116 = 1;
        waitFrames(20);
        SAI_toggleMessageWindow(1);
    } else {
        if (SAI_AREA.unk116 == 0) {
            SAI_SPRITES[0] = SAI_createSprite(3);
        } else {
            SAI_setSpriteImage8Bit(SAI_SPRITES[0], &rect, 0);
            SAI_SPRITES[0]->quads[0].clut = SAI_SPRITES[0]->quads[1].clut = getClut(0x280, 0x1F8);
        }
        SAI_AREA.unk116 = 1;
    }
}

s32 SAI_uncoverPanel(void) {
    s32 wasZero = SAI_AREA.coverAlpha == 0;
    s16 value;

    value = SAI_AREA.coverAlpha;
    value -= 8;
    if (value < 0) {
        value = 0;
    }
    SAI_PANEL_COVER_ALPHA = value;
    return wasZero;
}

s32 SAI_coverPanel(void) {
    s32 done;
    s16 level;

    SAI_AREA.unk115 = 1;
    done = SAI_AREA.coverAlpha == 0xFF;
    level = SAI_AREA.coverAlpha;
    level += 8;
    if (level > 0xFF) {
        level = 0xFF;
    }
    SAI_PANEL_COVER_ALPHA = level;
    return done;
}

void SAI_createPanelFrame(void) {
    s16 xs[5] = { -0xA2, -0x7A, -2, 0x76, 0x9E };
    s16 ys[4] = { -0x60, -0x38, 0x38, 0x60 };
    s32 i;
    s32 j;
    s32 palette;
    s32 k;

    if ((s8)SESSION->location < 0) {
        palette = 0;
    } else {
        palette = (s8)SESSION->location;
    }
    for (j = 10, i = 1; j < 20; j++, i++) {
        SAI_SPRITES[i] = SAI_createSprite(j);
        k = palette + 0xEB;
        SAI_SPRITES[i]->quads[0].clut = getClut(0x210, k);
        SAI_SPRITES[i]->quads[1].clut = getClut(0x210, palette + 0xEB);
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            k = i * 2 + j;
            SAI_SPRITES[k + 1]->corners[0].vx = SAI_SPRITES[k + 1]->corners[2].vx = xs[j + 1];
            SAI_SPRITES[k + 1]->corners[1].vx = SAI_SPRITES[k + 1]->corners[3].vx = xs[j + 2];
            SAI_SPRITES[k + 1]->corners[0].vy = SAI_SPRITES[k + 1]->corners[1].vy = ys[i * 2];
            SAI_SPRITES[k + 1]->corners[2].vy = SAI_SPRITES[k + 1]->corners[3].vy = ys[i * 2 + 1];
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            k = i * 3 + j;
            SAI_SPRITES[k + 5]->corners[0].vx = SAI_SPRITES[k + 5]->corners[2].vx = xs[i * 3];
            SAI_SPRITES[k + 5]->corners[1].vx = SAI_SPRITES[k + 5]->corners[3].vx = xs[i * 3 + 1];
            SAI_SPRITES[k + 5]->corners[0].vy = SAI_SPRITES[k + 5]->corners[1].vy = ys[j];
            SAI_SPRITES[k + 5]->corners[2].vy = SAI_SPRITES[k + 5]->corners[3].vy = ys[j + 1];
        }
    }
}

void SAI_createPanelFrameShadow(void) {
    s16 xs[5] = { -0x9D, -0x75, 3, 0x7B, 0x9F };
    s16 ys[4] = { -0x63, -0x3B, 0x3D, 0x65 };
    s32 i;
    s32 j;
    s32 k;

    for (j = 0x30, i = 11; j < 0x3A; j++, i++) {
        SAI_SPRITES[i] = SAI_createSprite(j);
        SAI_SPRITES[i]->quads[0].clut = getClut(0x200, 0xE2);
        SAI_SPRITES[i]->quads[1].clut = getClut(0x200, 0xE2);
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            k = i * 2 + j + 11;
            SAI_SPRITES[k]->corners[0].vx = SAI_SPRITES[k]->corners[2].vx = xs[j + 1];
            SAI_SPRITES[k]->corners[1].vx = SAI_SPRITES[k]->corners[3].vx = xs[j + 2];
            SAI_SPRITES[k]->corners[0].vy = SAI_SPRITES[k]->corners[1].vy = ys[i * 2];
            SAI_SPRITES[k]->corners[2].vy = SAI_SPRITES[k]->corners[3].vy = ys[i * 2 + 1];
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            k = i * 3 + j + 15;
            SAI_SPRITES[k]->corners[0].vx = SAI_SPRITES[k]->corners[2].vx = xs[i * 3];
            SAI_SPRITES[k]->corners[1].vx = SAI_SPRITES[k]->corners[3].vx = xs[i * 3 + 1];
            SAI_SPRITES[k]->corners[0].vy = SAI_SPRITES[k]->corners[1].vy = ys[j];
            SAI_SPRITES[k]->corners[2].vy = SAI_SPRITES[k]->corners[3].vy = ys[j + 1];
        }
    }
}

const char D_801DE490[] = "";

void SAI_drawPanelFrame(void) {
    s32 i;
    s32 j;

    for (i = 10, j = 1; i < 20; i++, j++) {
        SAI_drawSprite(SAI_SPRITES[j]);
    }
}

void SAI_drawPanelFrameShadow(void) {
    s32 i;
    s32 j;

    for (i = 0x30, j = 11; i < 0x3A; i++, j++) {
        SAI_drawSprite(SAI_SPRITES[j]);
    }
}

void SAI_freePanelFrame(void) {
    s32 i;
    s32 j;

    for (i = 10, j = 1; i < 20; i++, j++) {
        SAI_freeSprite(SAI_SPRITES[j]);
    }
}

void SAI_freePanelFrameShadow(void) {
    s32 i;
    s32 j;

    for (i = 0x30, j = 11; i < 0x3A; i++, j++) {
        SAI_freeSprite(SAI_SPRITES[j]);
    }
}

s32 SAI_spinPanel(void) {
    s32 done = 0;
    s32 i;
    s32 y;

    if (SAI_AREA.opening == 1) {
        if (SAI_AREA.angle == 0x400 && SAI_AREA.openTimer == 35) {
            done = 1;
            SAI_AREA.opening = 0;
        }
        SAI_AREA.openTimer++;
        if (SAI_AREA.openTimer > 35) {
            SAI_AREA.openTimer = 35;
        }
        SAI_WORLD_MAP.iconSlide = SAI_AREA.openTimer - 20;
        if (SAI_WORLD_MAP.iconSlide < 0) {
            SAI_WORLD_MAP.iconSlide = 0;
        } else if (SAI_WORLD_MAP.iconSlide > 15) {
            SAI_WORLD_MAP.iconSlide = 15;
        }
    } else {
        SAI_AREA.openTimer--;
        if (SAI_AREA.openTimer < 0) {
            SAI_AREA.openTimer = 0;
            SAI_AREA.opening = 1;
            done = 1;
        }
        SAI_WORLD_MAP.iconSlide = SAI_AREA.openTimer - 20;
        if (SAI_WORLD_MAP.iconSlide < 0) {
            SAI_WORLD_MAP.iconSlide = 0;
        } else if (SAI_WORLD_MAP.iconSlide > 15) {
            SAI_WORLD_MAP.iconSlide = 15;
        }
    }
    SAI_AREA.angle = (SAI_AREA.openTimer << 10) / 35;
    if (SAI_AREA.angle > 0x400) {
        SAI_AREA.angle = 0x400;
    }
    for (i = 1; i < 11; i++) {
        SAI_SPRITES[i]->rot.vz = -(SAI_AREA.openTimer << 12) / 35;
    }
    for (i = 11; i < 21; i++) {
        SAI_SPRITES[i]->rot.vz = (SAI_AREA.openTimer << 12) / 35;
    }
    SAI_AREA.rot.vz = (SAI_AREA.openTimer << 12) / 35;
    SAI_AREA.pos.vz = rcos(SAI_AREA.angle) * 2 / 3;
    y = -(SAI_WORLD_MAP.iconSlide * 14) / 15;
    for (i = 1; i < 21; i++) {
        SAI_SPRITES[i]->pos.vy = y;
        SAI_SPRITES[i]->pos.vz = SAI_AREA.pos.vz;
    }
    SAI_SPRITES[0]->pos.vy = y;
    SAI_SPRITES[21]->pos.vy = (SAI_WORLD_MAP.iconSlide * -92 + (15 - SAI_WORLD_MAP.iconSlide) * -188) / 15;
    SAI_SPRITES[22]->pos.vy = SAI_SPRITES[21]->pos.vy + 1;
    return done;
}

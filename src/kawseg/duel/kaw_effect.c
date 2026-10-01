#include "common.h"
#include "game.h"
#include "dcb/kaw_effect.h"
#include "dcb/card_zones.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/script.h"
#include "dcb/battle_hud.h"
#include "dcb/transform.h"
#include "dcb/effect_object.h"
#include "dcb/effect_prims.h"
#include "dcb/decompress.h"
#include "dcb/scroll_bg.h"
#include "dcb/sound_play.h"
#include "dcb/vblank.h"
#include "dcb/kawseg.h"
#include "dcb/kaw_hud.h"

typedef struct {
     s16 kind;
     s16 active;
     u8 *obj;
} EffectTableEntry;

typedef struct {
     void *data;
     Script *script;
     s32 *regs;
     EffectTableEntry entries[16];
     s32 count;
} EffectTable;

/* the effect script's registers from byte 0x248 (register 0x92) on hold an
 * effect object's parameters: what KAW_getEffectParams reads out of the object
 * and KAW_setEffectParams writes back */
typedef struct {
    /* 0x00 */ s32 px;
    /* 0x04 */ s32 py;
    /* 0x08 */ s32 pz;
    /* 0x0C */ s32 px2;
    /* 0x10 */ s32 py2;
    /* 0x14 */ s32 pz2;
    /* 0x18 */ s32 moveSpeed;
    /* 0x1C */ s32 moveAccel;
    /* 0x20 */ s32 rx;
    /* 0x24 */ s32 ry;
    /* 0x28 */ s32 rz;
    /* 0x2C */ s32 drx;
    /* 0x30 */ s32 dry;
    /* 0x34 */ s32 drz;
    /* 0x38 */ s32 ddrx;
    /* 0x3C */ s32 ddry;
    /* 0x40 */ s32 ddrz;
    /* 0x44 */ s32 sx;
    /* 0x48 */ s32 sy;
    /* 0x4C */ s32 sz;
    /* 0x50 */ s32 sxT;
    /* 0x54 */ s32 syT;
    /* 0x58 */ s32 szT;
    /* 0x5C */ s32 dsx;
    /* 0x60 */ s32 dsy;
    /* 0x64 */ s32 dsz;
    /* 0x68 */ s32 hitRadius;
    /* 0x6C */ s32 period;
    /* 0x70 */ s32 fadeMode;
    /* 0x74 */ s32 speed;
    /* 0x78 */ s32 mode;
    /* 0x7C */ s32 wavePhase;
    /* 0x80 */ s32 waveAmplitude;
    /* 0x84 */ s32 waveFreq;
    /* 0x88 */ s32 parent; /* -2: none, -1: KAW_EFFECT_ROOT, else an entry of the table */
} EffectParams;

extern s8 KAW_EFFECT_PLAYER;
extern s8 KAW_EFFECT_CARD;
extern s8 KAW_EFFECT_TARGET_CARD;
extern void (*KAW_EFFECT_FREE_FUNCS[])(u8 *);
extern EffectObject KAW_EFFECT_ROOT;
extern u8 CLEAR_BG_ON_DRAW;

/* SUGSEG's colour quad drawer: in KAWSEG this address is inside KAW_chooseSupportCard */
void SUG_createFadeRect(Rect16 *rect, u8 *rgb, u8 *rgb2, u8 blend, s32 step, u8 mode);

void KAW_runEffectScriptTask(void *data, s32 task);
void KAW_initEffectFromParams(EffectTemplate *template, u8 *fx, EffectTable *table);
void KAW_runEffectScript(EffectTable *table);
void KAW_getEffectParams(EffectObject *o, u8 *fx, s32 current);
void KAW_setEffectParams(EffectObject *o, u8 *fx);
void KAW_getEffectWorldPos(void *xform, u8 *fx);
void KAW_createEffectEntry(s32 index, s32 kind, s32 params, EffectTable *table);

/* per effect kind: what updates it each frame, creates it and frees it */
void (*KAW_EFFECT_TICK_FUNCS[4])(u8 *) = {
    /* SUGSEG's colour quad renderer, SUG_tickFadeRect: in KAWSEG this address
       is inside KAW_chooseSupportCard, and nothing here relocates it */
#if VERSION_US
    (void (*)(u8 *))0x801E651C,
#elif VERSION_EU
    (void (*)(u8 *))0x801E8C60,
#else
#error "kawseg/duel/kaw_effect: version not checked"
#endif
    (void (*)(u8 *))renderRingEffect,
    (void (*)(u8 *))updateEffectObject,
    (void (*)(u8 *))renderStreakParticles,
};

void KAW_loadEffectArchive(void) {
    spawnTask(0, -1, 0, 0x800, loadFileTagged, "B:\\CBTL_EFF.ARC", getCurrentTaskId(), 0x38E);
    KAW_DUEL->effectArchive = waitFrames(0x7FFFFFFF);
}

void KAW_freeEffectArchive(void) {
    freeHeapBlock((void *)KAW_DUEL->effectArchive);
}

s32 KAW_playEffect(s32 entry, s32 player) {
    KAW_playEffectScript(entry, player, player, 0, 0);
}

void KAW_playCardEffect(s32 entry, s32 player, s32 mode) {
    KAW_playEffectScript(entry, player, player, mode, mode);
}

s32 KAW_playEffectOnOpponent(s32 entry, s32 player) {
    KAW_playEffectScript(entry, player, player ^ 1, 0, 0);
}

void KAW_playEffectScript(s32 entry, s32 player1, s32 player2, s32 mode1, s32 mode2) {
    s32 data;

    KAW_EFFECT_PLAYER = player1;
    switch (mode1) {
    case 0:
        KAW_EFFECT_CARD = getActiveDigimonCard(player1);
        break;
    case 1:
        KAW_EFFECT_CARD = getPlayedCard(player1);
        break;
    }
    switch (mode2) {
    case 0:
        KAW_EFFECT_TARGET_CARD = getActiveDigimonCard(player2);
        break;
    case 1:
        KAW_EFFECT_TARGET_CARD = getPlayedCard(player2);
        break;
    }
    data = decompressArchiveEntry(KAW_DUEL->effectArchive, entry);
    spawnTask(0, 0x1F, 0, 0x800, KAW_runEffectScriptTask, data, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    freeHeapBlock((void *)data);
}

#define EFFECT_PARAMS(regs) ((EffectParams *)((u8 *)(regs) + 0x248))

s32 KAW_tickEffectScript(EffectTable *table) {
    s32 i;

    PushMatrix();
    tickEffectMotion((s32)&KAW_EFFECT_ROOT, 0);
    PopMatrix();
    table->regs[0] = 1;
    KAW_runEffectScript(table);
    for (i = 0; i < 16; i++) {
        if (table->entries[i].active != 0 && KAW_EFFECT_TICK_FUNCS[table->entries[i].kind] != NULL) {
            KAW_EFFECT_TICK_FUNCS[table->entries[i].kind](table->entries[i].obj);
            if (table->entries[i].kind > 0) {
                table->regs[i + 0x52] = ((EffectObject *)table->entries[i].obj)->state;
                table->regs[i + 0x72] = ((EffectObject *)table->entries[i].obj)->flag;
            }
        }
    }
    return table->regs[0];
}

void KAW_freeEffectEntries(EffectTable *table) {
    s32 i;

    waitFrames(FRAME_INTERVAL);
    for (i = 0; i < 16; i++) {
        if (table->entries[i].kind != -1) {
            KAW_EFFECT_FREE_FUNCS[table->entries[i].kind](table->entries[i].obj);
        }
    }
}

void KAW_getCardPosition(s32 index, u8 *fx) {
    CardAnim *anim;
    s32 base;

    if (index >= 0) {
        base = (s32)CARD_ANIMS;
        anim = (CardAnim *)(index * 36 + base);
        EFFECT_PARAMS(fx)->px = anim->spr->pos.vx;
        EFFECT_PARAMS(fx)->py = anim->spr->pos.vy;
        EFFECT_PARAMS(fx)->pz = anim->spr->pos.vz;
    }
}

void KAW_setCardSpriteColor(s32 index, u8 *fx) {
    u8 rgb[3];

    if (index >= 0) {
        rgb[0] = *(s32 *)(fx + 0x94);
        rgb[1] = *(s32 *)(fx + 0x98);
        rgb[2] = *(s32 *)(fx + 0x9C);
        KAW_fadeCardSprite(CARD_SPR(index), rgb);
    }
}

void KAW_runEffectScript(EffectTable *table) {
    s32 *vars;
    s32 result;
    s32 index;

    if (table->count != 0) {
        table->count--;
        return;
    }
    vars = table->regs;
    do {
        result = runScriptToNextEvent(table->script, vars);
        if (result == 1) {
            switch (table->script->eventOp) {
            case 10:
                switch (table->script->eventArg) {
                case 0:
                    KAW_getEffectParams(&KAW_EFFECT_ROOT, (u8 *)vars, 0);
                    break;
                case 1:
                    KAW_setEffectParams(&KAW_EFFECT_ROOT, (u8 *)vars);
                    restartEffectMotion((u8 *)&KAW_EFFECT_ROOT);
                    break;
                case 2:
                    SCREEN_COPY_MODE = 1;
                    break;
                case 3:
                    SCREEN_COPY_MODE = 0;
                    break;
                case 4:
                    CLEAR_BG_ON_DRAW = 0;
                    break;
                case 5:
                    CLEAR_BG_ON_DRAW = 1;
                    break;
                case 6:
                    KAW_getCardPosition(KAW_EFFECT_CARD, (u8 *)vars);
                    break;
                case 7:
                    KAW_getCardPosition(KAW_EFFECT_TARGET_CARD, (u8 *)vars);
                    break;
                case 8:
                    KAW_setCardSpriteColor(KAW_EFFECT_CARD, (u8 *)vars);
                    break;
                case 9:
                    KAW_setCardSpriteColor(KAW_EFFECT_TARGET_CARD, (u8 *)vars);
                    break;
                case 10:
                    index = KAW_EFFECT_CARD;
                    if (index >= 0) {
                        KAW_hideCardLabel(SPRITE(index));
                    }
                    break;
                case 11:
                    index = KAW_EFFECT_TARGET_CARD;
                    if (index >= 0) {
                        KAW_hideCardLabel(SPRITE(index));
                    }
                    break;
                }
                break;
            case 11:
                switch (table->script->eventArg) {
                case 0:
                    playSoundEffect((s16)table->script->params[0]);
                    break;
                case 1:
                    KAW_getEffectParams((EffectObject *)table->entries[(s16)table->script->params[0]].obj, (u8 *)vars, 0);
                    break;
                case 2:
                    KAW_initEffectFromParams((EffectTemplate *)table->entries[(s16)table->script->params[0]].obj, (u8 *)vars, table);
                    initEffectObject(table->entries[(s16)table->script->params[0]].obj);
                    break;
                case 3:
                    stopSoundVoice(table->script->params[0]);
                    break;
                case 4:
                    if (table->entries[(s16)table->script->params[0]].obj != NULL) {
                        table->entries[(s16)table->script->params[0]].active = 1;
                    }
                    break;
                case 5:
                    table->entries[(s16)table->script->params[0]].active = 0;
                    break;
                case 6:
                    table->count = (s16)table->script->params[0] - 1;
                    return;
                case 7:
                    KAW_getEffectWorldPos(table->entries[(s16)table->script->params[0]].obj, (u8 *)vars);
                    break;
                case 8:
                    KAW_getEffectParams((EffectObject *)table->entries[(s16)table->script->params[0]].obj, (u8 *)vars, 1);
                    break;
                case 9:
                    KAW_getCardPosition(getActiveDigimonCard(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 10:
                    KAW_getCardPosition(getPlayedCard(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 11:
                    KAW_getCardPosition(peekOnlineDeckTop(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 12:
                    KAW_getCardPosition(peekOfflineDeckTop(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 13:
                    KAW_setCardSpriteColor(getActiveDigimonCard(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 14:
                    KAW_setCardSpriteColor(getPlayedCard(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 15:
                    KAW_setCardSpriteColor(peekOnlineDeckTop(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 16:
                    KAW_setCardSpriteColor(peekOfflineDeckTop(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 17:
                    index = getActiveDigimonCard(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]);
                    if (index >= 0) {
                        KAW_hideCardLabel(SPRITE(index));
                    }
                    break;
                case 18:
                    index = getPlayedCard(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]);
                    if (index >= 0) {
                        KAW_hideCardLabel(SPRITE(index));
                    }
                    break;
                case 19:
                    index = KAW_EFFECT_CARD;
                    if (index >= 0) {
                        KAW_showCardLabel(SPRITE(index), (s16)table->script->params[0]);
                    }
                    break;
                case 20:
                    index = KAW_EFFECT_TARGET_CARD;
                    if (index >= 0) {
                        KAW_showCardLabel(SPRITE(index), (s16)table->script->params[0]);
                    }
                    break;
                }
                break;
            case 12:
                switch (table->script->eventArg) {
                case 0:
                    KAW_createEffectEntry((s16)table->script->params[0], (s16)table->script->params[1], (s32)vars, table);
                    break;
                case 1:
                    vars[1] = rsin((s16)table->script->params[1]) * (s16)table->script->params[0] / 4096;
                    break;
                case 2:
                    vars[1] = rcos((s16)table->script->params[1]) * (s16)table->script->params[0] / 4096;
                    break;
                case 3:
                    playSoundEffectOnVoice((s16)table->script->params[0], (s16)table->script->params[1]);
                    break;
                case 4:
                    index = getActiveDigimonCard(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]);
                    if (index >= 0) {
                        KAW_showCardLabel(SPRITE(index), (s16)table->script->params[1]);
                    }
                    break;
                case 5:
                    index = getPlayedCard(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]);
                    if (index >= 0) {
                        KAW_showCardLabel(SPRITE(index), (s16)table->script->params[1]);
                    }
                    break;
                }
                break;
            }
        }
        clearScriptBusy(table->script);
    } while (result != 0);
}

void KAW_getEffectParams(EffectObject *o, u8 *fx, s32 current) {
    if (current == 0) {
        EFFECT_PARAMS(fx)->px = o->px;
        EFFECT_PARAMS(fx)->py = o->py;
        EFFECT_PARAMS(fx)->pz = o->pz;
        EFFECT_PARAMS(fx)->rx = (s16)o->rx0;
        EFFECT_PARAMS(fx)->ry = (s16)o->ry0;
        EFFECT_PARAMS(fx)->rz = (s16)o->rz0;
        EFFECT_PARAMS(fx)->sx = o->sx0;
        EFFECT_PARAMS(fx)->sy = o->sy0;
        EFFECT_PARAMS(fx)->sz = o->sz0;
    } else {
        EFFECT_PARAMS(fx)->px = o->posX;
        EFFECT_PARAMS(fx)->py = o->posY;
        EFFECT_PARAMS(fx)->pz = o->posZ;
        EFFECT_PARAMS(fx)->rx = o->rotX;
        EFFECT_PARAMS(fx)->ry = o->rotY;
        EFFECT_PARAMS(fx)->rz = o->rotZ;
        EFFECT_PARAMS(fx)->sx = o->sx;
        EFFECT_PARAMS(fx)->sy = o->sy;
        EFFECT_PARAMS(fx)->sz = o->sz;
    }
    EFFECT_PARAMS(fx)->px2 = o->px2;
    EFFECT_PARAMS(fx)->py2 = o->py2;
    EFFECT_PARAMS(fx)->pz2 = o->pz2;
    EFFECT_PARAMS(fx)->moveSpeed = o->moveSpeed;
    EFFECT_PARAMS(fx)->moveAccel = o->moveAccel;
    EFFECT_PARAMS(fx)->drx = o->drx;
    EFFECT_PARAMS(fx)->dry = o->dry;
    EFFECT_PARAMS(fx)->drz = o->drz;
    EFFECT_PARAMS(fx)->ddrx = o->ddrx;
    EFFECT_PARAMS(fx)->ddry = o->ddry;
    EFFECT_PARAMS(fx)->ddrz = o->ddrz;
    EFFECT_PARAMS(fx)->sxT = o->sxT;
    EFFECT_PARAMS(fx)->syT = o->syT;
    EFFECT_PARAMS(fx)->szT = o->szT;
    EFFECT_PARAMS(fx)->dsx = o->dsx;
    EFFECT_PARAMS(fx)->dsy = o->dsy;
    EFFECT_PARAMS(fx)->dsz = o->dsz;
    EFFECT_PARAMS(fx)->hitRadius = o->hitRadius;
    EFFECT_PARAMS(fx)->period = o->period;
    EFFECT_PARAMS(fx)->fadeMode = o->fadeMode;
    EFFECT_PARAMS(fx)->speed = o->speed;
    EFFECT_PARAMS(fx)->wavePhase = o->wavePhase;
    EFFECT_PARAMS(fx)->waveAmplitude = o->waveAmplitude;
    EFFECT_PARAMS(fx)->waveFreq = o->waveFreq;
    EFFECT_PARAMS(fx)->mode = o->mode;
}

void KAW_setEffectParams(EffectObject *o, u8 *fx) {
    o->px = EFFECT_PARAMS(fx)->px;
    o->py = EFFECT_PARAMS(fx)->py;
    o->pz = EFFECT_PARAMS(fx)->pz;
    o->px2 = EFFECT_PARAMS(fx)->px2;
    o->py2 = EFFECT_PARAMS(fx)->py2;
    o->pz2 = EFFECT_PARAMS(fx)->pz2;
    o->moveSpeed = EFFECT_PARAMS(fx)->moveSpeed;
    o->moveAccel = EFFECT_PARAMS(fx)->moveAccel;
    o->rx0 = EFFECT_PARAMS(fx)->rx;
    o->ry0 = EFFECT_PARAMS(fx)->ry;
    o->rz0 = EFFECT_PARAMS(fx)->rz;
    o->drx = EFFECT_PARAMS(fx)->drx;
    o->dry = EFFECT_PARAMS(fx)->dry;
    o->drz = EFFECT_PARAMS(fx)->drz;
    o->ddrx = EFFECT_PARAMS(fx)->ddrx;
    o->ddry = EFFECT_PARAMS(fx)->ddry;
    o->ddrz = EFFECT_PARAMS(fx)->ddrz;
    o->sx0 = EFFECT_PARAMS(fx)->sx;
    o->sy0 = EFFECT_PARAMS(fx)->sy;
    o->sz0 = EFFECT_PARAMS(fx)->sz;
    o->sxT = EFFECT_PARAMS(fx)->sxT;
    o->syT = EFFECT_PARAMS(fx)->syT;
    o->szT = EFFECT_PARAMS(fx)->szT;
    o->dsx = EFFECT_PARAMS(fx)->dsx;
    o->dsy = EFFECT_PARAMS(fx)->dsy;
    o->dsz = EFFECT_PARAMS(fx)->dsz;
    o->hitRadius = EFFECT_PARAMS(fx)->hitRadius;
    o->period = EFFECT_PARAMS(fx)->period;
    o->fadeMode = EFFECT_PARAMS(fx)->fadeMode;
    o->speed = EFFECT_PARAMS(fx)->speed;
    o->wavePhase = EFFECT_PARAMS(fx)->wavePhase;
    o->waveAmplitude = EFFECT_PARAMS(fx)->waveAmplitude;
    o->waveFreq = EFFECT_PARAMS(fx)->waveFreq;
    o->mode = EFFECT_PARAMS(fx)->mode;
}

void KAW_getEffectWorldPos(void *xform, u8 *fx) {
    SVECTOR pos;

    getTransformWorldPos(xform, &pos);
    EFFECT_PARAMS(fx)->px = pos.vx;
    EFFECT_PARAMS(fx)->py = pos.vy;
    EFFECT_PARAMS(fx)->pz = pos.vz;
}

void KAW_initEffectFromParams(EffectTemplate *template, u8 *fx, EffectTable *table) {
    KAW_setEffectParams((EffectObject *)template, fx);
    if (EFFECT_PARAMS(fx)->parent == -2) {
        template->data[0x26] = 0;
    } else if (EFFECT_PARAMS(fx)->parent == -1) {
        template->data[0x26] = (s32)&KAW_EFFECT_ROOT;
    } else {
        template->data[0x26] = (s32)table->entries[EFFECT_PARAMS(fx)->parent].obj;
    }
}

void KAW_createFadeRectFromParams(u8 *fx) {
    Rect16 rect;
    u8 rgb[3];
    u8 rgb2[3];

    rect.x = *(s32 *)(fx + 0x128);
    rect.y = *(s32 *)(fx + 0x12C);
    rect.w = *(s32 *)(fx + 0x130);
    rect.h = *(s32 *)(fx + 0x134);
    rgb[0] = *(s32 *)(fx + 0x94);
    rgb[1] = *(s32 *)(fx + 0x98);
    rgb[2] = *(s32 *)(fx + 0x9C);
    rgb2[0] = *(s32 *)(fx + 0xA0);
    rgb2[1] = *(s32 *)(fx + 0xA4);
    rgb2[2] = *(s32 *)(fx + 0xA8);
    SUG_createFadeRect(&rect, rgb, rgb2, *(s32 *)(fx + 0x6C), *(s16 *)(fx + 0x70), *(s32 *)(fx + 0x110));
}

void KAW_createRingFromParams(u8 *fx, EffectTable *table) {
    Bytes4 inner;
    Bytes4 mid;
    Bytes4 outer;
    EffectTemplate template;

    KAW_initEffectFromParams(&template, fx, table);
    inner.b[0] = *(s32 *)(fx + 0x94);
    inner.b[1] = *(s32 *)(fx + 0x98);
    inner.b[2] = *(s32 *)(fx + 0x9C);
    mid.b[0] = *(s32 *)(fx + 0xA0);
    mid.b[1] = *(s32 *)(fx + 0xA4);
    mid.b[2] = *(s32 *)(fx + 0xA8);
    outer.b[0] = *(s32 *)(fx + 0xAC);
    outer.b[1] = *(s32 *)(fx + 0xB0);
    outer.b[2] = *(s32 *)(fx + 0xB4);
    createRingEffect(*(s32 *)(fx + 0xDC), &inner, &mid, &outer, &template, *(s32 *)(fx + 0x54), *(s32 *)(fx + 0x68),
                     *(s32 *)(fx + 0x6C), *(s32 *)(fx + 0x118), *(s32 *)(fx + 0xF4), *(s32 *)(fx + 0xF8), *(s32 *)(fx + 0x104),
                     *(s32 *)(fx + 0xFC), *(s32 *)(fx + 0x100), NULL, 0, 0, 0, *(s32 *)(fx + 0x114), *(s32 *)(fx + 0x120),
                     *(s32 *)(fx + 0x124), 0);
}

void KAW_createEffectObjectFromParams(u8 *fx, EffectTable *table) {
    EffectTemplate template;

    KAW_initEffectFromParams(&template, fx, table);
    cloneEffectObject(&template);
}

void KAW_createStreaksFromParams(u8 *fx, EffectTable *table) {
    EffectTemplate template;
    u8 startColor[3];
    u8 endColor[3];

    startColor[0] = *(s32 *)(fx + 0x94);
    startColor[1] = *(s32 *)(fx + 0x98);
    startColor[2] = *(s32 *)(fx + 0x9C);
    endColor[0] = *(s32 *)(fx + 0xA0);
    endColor[1] = *(s32 *)(fx + 0xA4);
    endColor[2] = *(s32 *)(fx + 0xA8);
    KAW_initEffectFromParams(&template, fx, table);
    createStreakParticles(startColor, endColor, &template, *(s32 *)(fx + 0x2DC), *(s32 *)(fx + 0x2D8), *(s32 *)(fx + 0x2E0),
                          *(s32 *)(fx + 0x2F4), *(s32 *)(fx + 0x2E4), *(s32 *)(fx + 0x2E8), *(s32 *)(fx + 0x2EC), *(s32 *)(fx + 0x2F0),
                          *(s32 *)(fx + 0x13C), *(s32 *)(fx + 0x140), *(s32 *)(fx + 0x138), *(s32 *)(fx + 0x144), *(s16 *)(fx + 0x68),
                          *(s32 *)(fx + 0x114), *(s32 *)(fx + 0x124));
}

u8 *(*KAW_EFFECT_CREATE_FUNCS[4])(s32, EffectTable *) = {
    (u8 *(*)(s32, EffectTable *))KAW_createFadeRectFromParams,
    (u8 *(*)(s32, EffectTable *))KAW_createRingFromParams,
    (u8 *(*)(s32, EffectTable *))KAW_createEffectObjectFromParams,
    (u8 *(*)(s32, EffectTable *))KAW_createStreaksFromParams,
};

void KAW_freeFadeRect(void *ptr) {
    freeHeapBlock(ptr);
}

void (*KAW_EFFECT_FREE_FUNCS[4])(u8 *) = {
    (void (*)(u8 *))KAW_freeFadeRect,
    (void (*)(u8 *))freeRingEffect,
    (void (*)(u8 *))freeEffectObject,
    (void (*)(u8 *))freeStreakParticles,
};

void KAW_createEffectEntry(s32 index, s32 kind, s32 params, EffectTable *table) {
    if (KAW_EFFECT_CREATE_FUNCS[kind] != NULL) {
        table->entries[index].kind = kind;
        table->entries[index].active = 0;
        table->entries[index].obj = KAW_EFFECT_CREATE_FUNCS[kind](params, table);
        if ((++table->count & 0xF) == 0) {
            waitFrames(FRAME_INTERVAL);
        }
    }
}

EffectTable *KAW_createEffectScript(void *data) {
    EffectObject fx;
    EffectTable *table;
    s32 i;

    table = allocTaskHeapBlock(sizeof(EffectTable));
    table->data = data;
    table->script = createScriptContext(data);
    table->regs = allocScriptRegisters(0xBE);
    for (i = 0; i < 16; i++) {
        table->entries[i].kind = -1;
        table->entries[i].active = 0;
        table->entries[i].obj = NULL;
    }
    table->count = 0;
    fx.px = 0;
    fx.py = 0;
    fx.pz = 0;
    fx.px2 = 0;
    fx.py2 = 0;
    fx.pz2 = 0;
    fx.rx0 = 0;
    fx.ry0 = 0;
    fx.rz0 = 0;
    fx.drx = 0;
    fx.dry = 0;
    fx.drz = 0;
    fx.ddrx = 0;
    fx.ddry = 0;
    fx.ddrz = 0;
    fx.sx0 = 0x1000;
    fx.sy0 = 0x1000;
    fx.sz0 = 0x1000;
    fx.sxT = 0x1000;
    fx.syT = 0x1000;
    fx.szT = 0x1000;
    fx.dsx = 0;
    fx.dsy = 0;
    fx.dsz = 0;
    fx.fadeMode = 0;
    fx.speed = 0;
    fx.hitRadius = 0x80;
    *(GsCOORDINATE2 **)((u8 *)&fx + 0x98) = (GsCOORDINATE2 *)SCENE_3D->viewMatrix;
    fx.mode = 0;
    KAW_EFFECT_ROOT = fx;
    initEffectObject(&KAW_EFFECT_ROOT);
    KAW_runEffectScript(table);
    return table;
}

void KAW_runEffectScriptTask(void *data, s32 task) {
    EffectTable *table;

    table = KAW_createEffectScript(data);
    do {
        waitFrames(FRAME_INTERVAL);
    } while (KAW_tickEffectScript(table));
    KAW_freeEffectEntries(table);
    freeScriptContext(table->script, table->regs);
    freeHeapBlock(table);
    resumeTask(task);
}

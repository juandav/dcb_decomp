#include "common.h"
#include "game.h"
#include "dcb/sug_effect_script.h"
#include "dcb/heap.h"
#include "dcb/effect_object.h"
#include "dcb/effect_prims.h"
#include "dcb/archive.h"
#include "dcb/loader.h"
#include "dcb/task.h"
#include "dcb/vram_upload.h"
#include "dcb/transform.h"
#include "dcb/script.h"
#include "dcb/vblank.h"
#include "dcb/anim_control.h"
#include "dcb/player_data.h"
#include "dcb/scroll_bg.h"
#include "dcb/sound_play.h"
#include "dcb/angle.h"
#include "dcb/sugseg.h"
#include "dcb/sug_sphere.h"
#include "dcb/sug_model_effect.h"
#include "dcb/sug_stage_fade.h"
#include "dcb/sug_sprite_effect.h"
#include "dcb/sug_trail.h"
#include "dcb/sug_fade_rect.h"
#include "dcb/sug_scroll_texture.h"
#include "dcb/sug_light_motion.h"
#include "dcb/sug_screen_copy.h"
#include "dcb/sug_sprite.h"
#include "dcb/sug_battle.h"

typedef struct {
    u8 unk0[0x20];
    s32 livePos[3];
    u8 unk2C[4];
    s16 liveRot[3];
    u8 unk36[2];
    s32 liveScale[3];
#if VERSION_JP
    u8 unk44[0x58]; /* jp's EffectObject: the fields below are 4 bytes further */
#elif VERSION_US || VERSION_EU
    u8 unk44[0x54];
#endif
    void *parent;
    u8 unk9C[0x10];
    s32 scale[3];
    u8 unkB8[4];
    s32 scale2[3];
    u8 unkC8[4];
    s16 scaleStep[3];
    u8 unkD2[2];
    s16 pos[3];
    u8 unkDA[2];
    s16 pos2[3];
    u8 unkE2[2];
    s16 rot[3];
    u8 unkEA[2];
    s16 rot2[3];
    u8 unkF2[2];
    s16 rotAccel[3];
    u8 unkFA[0x26];
    s16 moveSpeed;
    s16 moveAccel;
    s16 period;
    s16 waveFreq;
    s16 wavePhase;
    s16 waveAmplitude;
    s16 hitRadius;
    s16 mode;
    s16 speed;
    u8 unk132[5];
    u8 fadeMode;
    u8 unk138[2];
    s8 target;
    s8 source;
} EffectInit;

typedef struct {
    s32 v[4];
} Entry16;

/* the kinds of effect an effect script creates: the index into
   SUG_EFFECT_TICK_FUNCS, SUG_EFFECT_CREATE_FUNCS and SUG_EFFECT_FREE_FUNCS.
   From EFFECT_SPHERE on they all start with an EffectObject */
enum {
    EFFECT_FADE_RECT = 0,
    EFFECT_SCROLL_TEXTURE = 1,
    EFFECT_LIGHT_MOTION = 3,
    EFFECT_SPHERE = 10,
    EFFECT_TRAIL = 12,
    EFFECT_RING = 13,
    EFFECT_OBJECT = 14,
    EFFECT_MODEL = 15,
    EFFECT_SPRITE = 16,
    EFFECT_STREAKS = 17,
};

/* one effect a script created: its kind and the effect itself */
typedef struct {
    s16 id;
    s16 active;
    s32 value;
#if VERSION_JP
    s32 unk8; /* jp: only SUG_initEffectSlots touches it, clearing it */
#endif
} EffectSlot;

typedef struct {
    EffectSlot slots[150];
    s32 modelSlots[3];
    u8 xform[0x4C];
    s32 pak;
    s32 count;
} EffectSlots;

typedef struct {
    void *script;
    void *context;
    s32 *regs;
    EffectSlots *slots;
    s32 waitFrames;
    s32 waitReleased; /* the first wait holds until regs[0] is -1 */
} EffectScript;

typedef struct Xform {
    u8 unk0[0x48];
    struct Xform *parent;
} Xform;

/* BoneAnim with its nine channels as three groups of x, y, z */
typedef struct {
    AnimChan ch[3][3];
} BoneChannels;

/* per effect slot kind: the functions that update, create and free it */
typedef void (*SlotUpdate)(u8 *value);

typedef s32 (*SlotCreate)(s32 arg, EffectSlots *slots);
typedef void (*SlotFree)(s32 value);

extern RootEffect SUG_EFFECT_ROOT;
extern u8 CLEAR_BG_ON_DRAW;

u16 GetClut(s32 x, s32 y);
void GsGetLw(GsCOORDINATE2 *coord, MATRIX *out);
void SUG_runEffectScript(EffectScript *runner);
void SUG_freeEffectEntries(EffectSlots *slots);
void SUG_initEffectFromParams(EffectTemplate *template, EffectParams *cmd, EffectSlots *ctx);
void SUG_getEffectParams(EffectInit *fx, EffectParams *cmd, s32 live);

/* takes a third argument, but this caller does not set it */
void SUG_setEffectParams();

void SUG_getEffectWorldPos(void *xform, EffectParams *params);
void SUG_createEffectEntry(s32 index, s32 kind, s32 arg, EffectSlots *slots);

SlotUpdate SUG_EFFECT_TICK_FUNCS[18] = {
    (SlotUpdate)SUG_tickFadeRect,
    (SlotUpdate)SUG_scrollTexture,
    NULL,
    (SlotUpdate)SUG_tickLightMotion,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    (SlotUpdate)SUG_tickSphereEffect,
    NULL,
    (SlotUpdate)SUG_tickTrailEffect,
    (SlotUpdate)renderRingEffect,
    (SlotUpdate)updateEffectObject,
    (SlotUpdate)SUG_tickModelEffect,
    (SlotUpdate)SUG_tickSpriteEffect,
    (SlotUpdate)renderStreakParticles,
};

/* jp's scripts can't read or set an effect's depth or detach it */
#if VERSION_US || VERSION_EU
/* copy the ordering table depth an effect is drawn at to or from the otz
   register */
void SUG_getEffectOtz(EffectSlot *slot, EffectParams *params) {
    switch (slot->id) {
    case EFFECT_SPHERE:
        params->otz.w = ((SphereEffect *)slot->value)->otz;
        break;
    case EFFECT_TRAIL:
        params->otz.w = ((TrailEffect *)slot->value)->otz;
        break;
    case EFFECT_RING:
        params->otz.w = ((RingEffect *)slot->value)->fixedOtz;
        break;
    case EFFECT_SPRITE:
        params->otz.w = ((SpriteEffect *)slot->value)->sprite.otz;
        break;
    case EFFECT_STREAKS:
        params->otz.w = ((StreakParticles *)slot->value)->fixedOtz;
        break;
    }
}

void SUG_setEffectOtz(EffectSlot *slot, EffectParams *params) {
    switch (slot->id) {
    case EFFECT_SPHERE:
        ((SphereEffect *)slot->value)->otz = params->otz.w;
        break;
    case EFFECT_TRAIL:
        ((TrailEffect *)slot->value)->otz = params->otz.w;
        break;
    case EFFECT_RING:
        ((RingEffect *)slot->value)->fixedOtz = params->otz.w;
        break;
    case EFFECT_SPRITE:
        ((SpriteEffect *)slot->value)->sprite.otz = params->otz.b;
        break;
    case EFFECT_STREAKS:
        ((StreakParticles *)slot->value)->fixedOtz = params->otz.h;
        break;
    }
}

void SUG_detachEffectToWorld(EffectSlots *slots, s32 id, EffectParams *cmd) {
    SVECTOR v;
    SVECTOR unused;
    VECTOR out;
    MATRIX m;
    Xform *chain[18];
    Xform saved;
    s32 flag;
    Model *model;
    GsCOORDINATE2 *coord;
    Xform *xform;
    s32 i;
    s32 n;
    Xform **p;

    memset(&unused, 0, sizeof(unused));
    model = SCENE_3D->models[slots->modelSlots[cmd->source]];
    coord = model->coord;
    SUG_getEffectParams((EffectInit *)slots->slots[id].value, cmd, 1);
    PushMatrix();
    if (cmd->source >= 0) {
        SCENE_3D->root.flg = model->root.flg = 0;
        SCENE_3D->root.coord = GsIDMATRIX;
        model->root.coord = GsIDMATRIX;
        for (i = 0; i < model->nobj; i++, coord++) {
            coord->flg = 0;
        }
        GsGetLw(((Model *)SCENE_3D->models[slots->modelSlots[cmd->source]])->obj[cmd->target].coord2, &m);
        SetRotMatrix((s32)&m);
        SetTransMatrix(&m);
    } else {
        xform = (Xform *)slots->slots[id].value;
        saved = *(Xform *)slots->xform;
        initTransform(slots->xform, 0, 0, 0, 0, 0, 0, 0);
        /* the chain up to the slots' own transform; the match depends on
           the form of the loop: each version's compiler needs its own */
#if VERSION_US
        n = 0;
        p = chain;
        chain[0] = xform;
        if (xform != (Xform *)slots->xform) {
            do {
                xform = xform->parent;
                n++;
                if (n >= 16) {
                    break;
                }
                *(Xform **)((s32)p + (n << 2)) = xform;
            } while (xform != (Xform *)slots->xform);
        }
#elif VERSION_EU
        for (n = 0; n < 16; n++) {
            chain[n] = xform;
            if (xform == (Xform *)slots->xform) {
                break;
            }
            xform = xform->parent;
        }
#endif
        for (i = n; i > 0; i--) {
            updateTransformMatrix(chain[i], 0);
        }
        *(Xform *)slots->xform = saved;
    }
    v.vx = cmd->pos[0];
    v.vy = cmd->pos[1];
    v.vz = cmd->pos[2];
    RotTrans((u16 *)&v, &out, &flag);
    cmd->pos[0] = out.vx;
    cmd->pos[1] = out.vy;
    cmd->pos[2] = out.vz;
    v.vx = cmd->pos2[0];
    v.vy = cmd->pos2[1];
    v.vz = cmd->pos2[2];
    RotTrans((u16 *)&v, &out, &flag);
    cmd->pos2[0] = out.vx;
    cmd->pos2[1] = out.vy;
    cmd->pos2[2] = out.vz;
    PopMatrix();
    cmd->target = -1;
    SUG_initEffectFromParams((EffectTemplate *)slots->slots[id].value, cmd, slots);
    initEffectObject((void *)slots->slots[id].value);
}
#endif

void SUG_uploadEffectTim(EffectParams *params, s32 dy, Chunk *pak) {
    char path[32];
    u32 *tim;
    s32 loaded = 0;

    tim = findPakChunk(pak, 5, params->texAnimId);
    if (tim == NULL) {
#if VERSION_JP
        /* jp pads the file numbers with zeros */
        sprintf(path, "E:\\ANM\\%03d_%d.TIM", params->texAnimId / 10, params->texAnimId % 10);
#elif VERSION_US || VERSION_EU
        sprintf(path, "E:\\ANM\\%d_%d.TIM", params->texAnimId / 10, params->texAnimId % 10);
#endif
        tim = (u32 *)loadFile(path, getCurrentTaskId());
        loaded = 1;
        if (tim == NULL) {
            return;
        }
    }
#if VERSION_JP
    /* jp moves only the texture down to the side's VRAM rows */
    uploadTim(tim, params->texX, params->texY + dy, params->clutX, params->clutY);
#elif VERSION_US || VERSION_EU
    uploadTim(tim, params->texX, params->texY + dy, params->clutX, params->clutY + dy);
#endif
    DrawSync(0);
    if (loaded) {
        freeHeapBlock(tim);
    }
}

void SUG_uploadTimFile(s32 id, Chunk *pak) {
    char path[32];
    u32 *tims;
    s32 loaded = 0;

    tims = findPakChunk(pak, 5, id);
    if (tims == NULL) {
        sprintf(path, "E:\\TIM\\%04d.TIM", id);
        tims = (u32 *)loadFile(path, getCurrentTaskId());
        loaded = 1;
        if (tims == NULL) {
            return;
        }
    }
    uploadTimList(tims);
    if (loaded) {
        freeHeapBlock(tims);
    }
}

s32 SUG_loadEffectPak(s32 id) {
    char path[32];
    s32 file;

#if VERSION_JP
    /* jp pads the file numbers with zeros */
    sprintf(path, "E:\\%03d.PAK", id);
#elif VERSION_US || VERSION_EU
    sprintf(path, "E:\\%d.PAK", id);
#endif
    file = loadFileTagged((s32 *)path, getCurrentTaskId(), 0x12C);
    if (file != 0) {
        return file;
    }
    return 0;
}

void SUG_initEffectRoot(void) {
    RootEffect fx;

    fx.pos[0] = 100;
    fx.pos[1] = -150;
    fx.pos[2] = 9000;
    fx.pos2[0] = 100;
    fx.pos2[1] = -150;
    fx.pos2[2] = 9000;
    fx.rot[0] = 160;
    fx.rot[1] = 5800;
    fx.rot[2] = 0;
    fx.rot2[0] = 0;
    fx.rot2[1] = 0;
    fx.rot2[2] = 0;
    fx.rotAccel[0] = 0;
    fx.rotAccel[1] = 0;
    fx.rotAccel[2] = 0;
    fx.scale[0] = 0x1000;
    fx.scale[1] = 0x1000;
    fx.scale[2] = 0x1000;
    fx.scale2[0] = 0x1000;
    fx.scale2[1] = 0x1000;
    fx.scale2[2] = 0x1000;
    fx.scaleStep[0] = 0;
    fx.scaleStep[1] = 0;
    fx.scaleStep[2] = 0;
    fx.fadeMode = 0;
    fx.speed = 0;
    fx.hitRadius = 0x80;
    fx.parent = SCENE_3D->viewMatrix;
    fx.mode = 0;
    SUG_EFFECT_ROOT = fx;
    initEffectObject(&SUG_EFFECT_ROOT);
}

void SUG_initEffectSlots(EffectScript *obj) {
    s32 i;

    obj->slots = allocTaskHeapBlock(sizeof(EffectSlots));
    for (i = 0; i < 150; i++) {
        obj->slots->slots[i].id = -1;
        obj->slots->slots[i].active = 0;
        obj->slots->slots[i].value = 0;
#if VERSION_JP
        obj->slots->slots[i].unk8 = 0;
#endif
    }
    obj->slots->count = obj->waitFrames = 0;
    obj->waitReleased = 0;
    obj->slots->pak = 0;
}

s32 SUG_tickEffectScript(EffectScript *runner) {
    EffectSlots *slots;
    void (*fn)(u8 *);
    s32 i;

    slots = runner->slots;
    PushMatrix();
    tickEffectMotion((s32)&SUG_EFFECT_ROOT, 0);
    updateTransformMatrix(slots->xform, 0);
    PopMatrix();
    runner->regs[0] = 1;
    SUG_runEffectScript(runner);
    for (i = 0; i < 150; i++) {
        if (slots->slots[i].active != 0) {
            fn = SUG_EFFECT_TICK_FUNCS[slots->slots[i].id];
            if (fn != NULL) {
                fn((u8 *)slots->slots[i].value);
                /* let the script see how each 3D effect's motion is going */
                if (slots->slots[i].id >= EFFECT_SPHERE) {
                    runner->regs[i + 107] = ((EffectObject *)slots->slots[i].value)->state;
                    runner->regs[i + 257] = ((EffectObject *)slots->slots[i].value)->flag;
                }
            }
        }
    }
    return runner->regs[0];
}

void SUG_freeEffectSlots(void *obj) {
    SUG_freeEffectEntries(obj);
    freeHeapBlock(obj);
}

/* the running script, its n-th operand and its registers as EffectParams */
#define SCRIPT ((Script *)runner->context)

#define PARAM(n) ((s16)SCRIPT->params[n])

#define PARAMS ((EffectParams *)regs)

void SUG_runEffectScript(EffectScript *runner) {
    EffectSlots *slots;
    s32 *regs;
    s32 result;
    s32 anim;
    s32 tpage;
    s32 vramY;
    u8 *prev;
    u8 *next;
    BoneChannels *src;
    BoneChannels *dst;
    s32 i;

    slots = runner->slots;
    if (runner->waitFrames != 0) {
        runner->waitFrames--;
        return;
    }
    regs = runner->regs;
    do {
        result = runScriptToNextEvent(runner->context, regs);
        if (result == 1) {
            switch (SCRIPT->eventOp) {
            case 10:
                switch (SCRIPT->eventArg) {
                case 0: /* read the root's motion into the registers */
                    SUG_getEffectParams((EffectInit *)&SUG_EFFECT_ROOT, PARAMS, 0);
                    break;
                case 1: /* move the root as the registers say */
                    SUG_setEffectParams(&SUG_EFFECT_ROOT, regs);
                    restartEffectMotion((u8 *)&SUG_EFFECT_ROOT);
                    break;
                /* jp turns the main screen copy on and off, the others
                   start and stop SUGSEG's own */
#if VERSION_JP
                case 2:
                    SCREEN_COPY_MODE = 1;
                    break;
                case 3:
                    SCREEN_COPY_MODE = 0;
                    break;
#elif VERSION_US || VERSION_EU
                case 2:
                    SCREEN_COPY_EFFECT.abr = SUG_SCREEN_FX_PHASE = 0;
                    SCREEN_COPY_EFFECT.mode = 1;
                    break;
                case 3:
                    SUG_SCREEN_FX_PHASE = SCREEN_COPY_EFFECT.abr = SCREEN_COPY_EFFECT.x = SCREEN_COPY_EFFECT.y = SCREEN_COPY_EFFECT.mode = 0;
                    SCREEN_COPY_EFFECT.r = 0xA8;
                    SCREEN_COPY_EFFECT.g = 0xA8;
                    SCREEN_COPY_EFFECT.b = 0xA8;
                    break;
#endif
                case 4:
                    CAMERA->buffers[0].draw.isbg = 0;
                    break;
                case 5:
                    CAMERA->buffers[0].draw.isbg = 1;
                    break;
                case 6:
                    SUG_uploadEffectTim(PARAMS, slots->modelSlots[0] << 8, (Chunk *)slots->pak);
                    break;
                case 7:
                    pauseModelAnimation(slots->modelSlots[PARAMS->source]);
                    break;
                case 8:
                    resumeModelAnimation(slots->modelSlots[PARAMS->source]);
                    break;
                /* jp's scripts have no commands past here */
#if VERSION_US || VERSION_EU
                case 9: /* turn to face from pos to pos2 */
                    PARAMS->rot[1] = -computeVectorAngle(PARAMS->pos[0] - PARAMS->pos2[0], PARAMS->pos2[2] - PARAMS->pos[2]);
                    break;
                case 10:
                    SUG_startScreenCopyEffect(PARAMS, 0);
                    break;
#endif
                }
                break;
            case 11:
                switch (SCRIPT->eventArg) {
                case 0:
                    if (PARAM(0) >= 0) {
                        SUG_uploadTimFile(PARAM(0), (Chunk *)slots->pak);
                    }
                    break;
                /* jp's models have their animations loaded already */
#if VERSION_JP
                case 1:
                    applyAnimationFirstFrame(slots->modelSlots[PARAMS->source], PARAM(0));
                    break;
#elif VERSION_US || VERSION_EU
                case 1:
                    setModelAnimationPose(slots->modelSlots[PARAMS->source], PARAM(0));
                    break;
#endif
                case 2:
                    /* a model whose HP ran out plays animation 5 instead of 4 */
                    anim = PARAM(0);
                    if (anim == 4 && SUG_TARGET_HP[slots->modelSlots[PARAMS->source]] <= 0) {
                        anim = 5;
                    }
#if VERSION_JP
                    startModelAnimation(slots->modelSlots[PARAMS->source], anim, -2, 0);
#elif VERSION_US || VERSION_EU
                    playModelAnimation(slots->modelSlots[PARAMS->source], anim);
#endif
                    break;
                case 3:
                    playSoundEffect(PARAM(0));
                    break;
                case 5:
#if VERSION_US || VERSION_EU
                    SUG_getEffectOtz(&slots->slots[PARAM(0)], PARAMS);
#endif
                    SUG_getEffectParams((EffectInit *)slots->slots[PARAM(0)].value, PARAMS, 0);
                    break;
                case 6:
                    SUG_initEffectFromParams((EffectTemplate *)slots->slots[PARAM(0)].value, PARAMS, slots);
                    initEffectObject((void *)slots->slots[PARAM(0)].value);
#if VERSION_US || VERSION_EU
                    SUG_setEffectOtz(&slots->slots[PARAM(0)], PARAMS);
#endif
                    break;
                case 7:
                    stopSoundVoice(SCRIPT->params[0]);
                    break;
                case 8:
                    if (slots->slots[PARAM(0)].value != 0) {
                        slots->slots[PARAM(0)].active = 1;
                    }
                    break;
                case 9:
                    slots->slots[PARAM(0)].active = 0;
                    break;
                case 10: /* wait PARAM(0) frames */
                    if (runner->waitReleased == 0) {
                        if (runner->regs[0] != -1) {
                            break;
                        }
                        runner->waitReleased = 1;
                    }
                    runner->waitFrames = PARAM(0) - 1;
                    return;
                case 13: /* read where an effect is in the world */
                    SUG_getEffectWorldPos((void *)slots->slots[PARAM(0)].value, PARAMS);
                    break;
                case 14:
                    switch (PARAM(0)) {
                    case 2:
                        CAMERA->targetModel = -1;
                        SCENE_3D->modelState[0] = SCENE_3D->modelState[1] = 1;
                        break;
                    case 0:
                        if (CAMERA->targetModel > 0
                            && *(s16 *)((u8 *)SCENE_3D->models[CAMERA->targetModel] + 6) > 1000) {
                            prev = SCENE_3D->models[CAMERA->targetModel];
                            CAMERA->targetModel = slots->modelSlots[PARAMS->source];
                            next = SCENE_3D->models[CAMERA->targetModel];
                            src = (BoneChannels *)(prev + 0xD80) + *(s16 *)(prev + 4);
                            dst = (BoneChannels *)(next + 0xD80) + *(s16 *)(next + 4);
                            for (i = 0; i < 3; i++) {
                                dst->ch[0][i].value = src->ch[0][i].value;
                                dst->ch[1][i].value = src->ch[1][i].value;
                                dst->ch[2][i].value = src->ch[2][i].value;
                                dst->ch[0][i].accel0 = dst->ch[1][i].accel0 = dst->ch[2][i].accel0 =
                                    dst->ch[0][i].accel1 = dst->ch[1][i].accel1 = dst->ch[2][i].accel1 =
                                    dst->ch[0][i].velocity = dst->ch[1][i].velocity = dst->ch[2][i].velocity = 0;
                            }
                            *(s16 *)((u8 *)dst + 0x42) += *(s16 *)(prev + 0xC);
                            *(s16 *)((u8 *)dst + 0x52) += *(s16 *)(prev + 0x10);
                        } else {
                            CAMERA->targetModel = slots->modelSlots[PARAMS->source];
                        }
                        SCENE_3D->modelState[CAMERA->targetModel] = 1;
                        SCENE_3D->modelState[(s16)(CAMERA->targetModel ^ 1)] = -1;
                        CAMERA->targetFacedModel = 0;
                        break;
                    case 1:
                        CAMERA->targetModel = ((ModelEffect *)slots->slots[PARAMS->source].value)->modelSlot;
                        CAMERA->targetFacedModel = slots->modelSlots[0];
                        break;
                    }
                    /* jp has no break here: it goes on to case 15 */
#if VERSION_US || VERSION_EU
                    break;
#endif
                case 15:
#if VERSION_US || VERSION_EU
                    SUG_getEffectOtz(&slots->slots[PARAM(0)], PARAMS);
#endif
                    SUG_getEffectParams((EffectInit *)slots->slots[PARAM(0)].value, PARAMS, 1);
                    break;
                case 16:
                    slots->pak = SUG_loadEffectPak(PARAM(0));
                    break;
#if VERSION_US || VERSION_EU
                case 17:
                    SUG_detachEffectToWorld(slots, PARAM(0), PARAMS);
                    break;
                case 18:
                    SUG_startScreenCopyEffect(PARAMS, PARAMS->abr);
                    if ((SCREEN_COPY_EFFECT.mode = SCRIPT->params[0]) >= 2) {
                        SCREEN_COPY_EFFECT.abr = PARAMS->abr;
                    }
                    break;
                case 19:
                    SUG_shadeModelClut(slots->modelSlots[PARAMS->source], PARAM(0));
                    break;
#endif
                }
                break;
            case 12:
                switch (SCRIPT->eventArg) {
                case 0: /* create effect kind PARAM(1) in slot PARAM(0) */
                    SUG_createEffectEntry(PARAM(0), PARAM(1), (s32)regs, slots);
                    break;
#if VERSION_US || VERSION_EU
                case 5:
                    ((EffectInit *)slots->slots[PARAM(0)].value)->mode = SCRIPT->params[1];
                    break;
                case 4:
                    regs[1] = computeVectorAngle(PARAM(0), PARAM(1));
                    break;
#endif
                case 1:
                    regs[1] = rsin(PARAM(1)) * PARAM(0) / 4096;
                    break;
                case 2:
                    regs[1] = rcos(PARAM(1)) * PARAM(0) / 4096;
                    break;
                case 3:
                    playSoundEffectOnVoice(PARAM(0), PARAM(1));
                    break;
                }
                break;
            case 13:
                if (SCRIPT->eventArg == 0) {
                    tpage = PARAM(1);
                    vramY = ((tpage & 0x10) << 4) + (PARAM(2) << 7);
                    SUG_loadSprite(PARAM(0), (tpage & 0xF) << 6, vramY + (slots->modelSlots[0] << 8),
                                  slots->pak);
                }
                break;
            }
        }
        clearScriptBusy(runner->context);
    } while (result != 0);
}

#undef SCRIPT

#undef PARAM

#undef PARAMS

void SUG_getEffectParams(EffectInit *fx, EffectParams *cmd, s32 live) {
    if (live == 0) {
        cmd->pos[0] = fx->pos[0];
        cmd->pos[1] = fx->pos[1];
        cmd->pos[2] = fx->pos[2];
        cmd->rot[0] = fx->rot[0];
        cmd->rot[1] = fx->rot[1];
        cmd->rot[2] = fx->rot[2];
        cmd->scale[0] = fx->scale[0];
        cmd->scale[1] = fx->scale[1];
        cmd->scale[2] = fx->scale[2];
    } else {
        cmd->pos[0] = fx->livePos[0];
        cmd->pos[1] = fx->livePos[1];
        cmd->pos[2] = fx->livePos[2];
        cmd->rot[0] = fx->liveRot[0];
        cmd->rot[1] = fx->liveRot[1];
        cmd->rot[2] = fx->liveRot[2];
        cmd->scale[0] = fx->liveScale[0];
        cmd->scale[1] = fx->liveScale[1];
        cmd->scale[2] = fx->liveScale[2];
    }
    cmd->pos2[0] = fx->pos2[0];
    cmd->pos2[1] = fx->pos2[1];
    cmd->pos2[2] = fx->pos2[2];
    cmd->moveSpeed = fx->moveSpeed;
    cmd->moveAccel = fx->moveAccel;
    cmd->rot2[0] = fx->rot2[0];
    cmd->rot2[1] = fx->rot2[1];
    cmd->rot2[2] = fx->rot2[2];
    cmd->rotAccel[0] = fx->rotAccel[0];
    cmd->rotAccel[1] = fx->rotAccel[1];
    cmd->rotAccel[2] = fx->rotAccel[2];
    cmd->scale2[0] = fx->scale2[0];
    cmd->scale2[1] = fx->scale2[1];
    cmd->scale2[2] = fx->scale2[2];
    cmd->scaleStep[0] = fx->scaleStep[0];
    cmd->scaleStep[1] = fx->scaleStep[1];
    cmd->scaleStep[2] = fx->scaleStep[2];
    cmd->hitRadius = fx->hitRadius;
    cmd->period = fx->period;
    cmd->fadeMode = fx->fadeMode;
    cmd->speed = fx->speed;
    cmd->wavePhase = fx->wavePhase;
    cmd->waveAmplitude = fx->waveAmplitude;
    cmd->waveFreq = fx->waveFreq;
    cmd->mode = fx->mode;
    /* jp's effects don't keep what they hang from */
#if VERSION_US || VERSION_EU
    cmd->source = fx->source;
    cmd->target = fx->target;
#endif
}

void SUG_setEffectParams(EffectInit *fx, EffectParams *cmd, void *ctx) {
    fx->pos[0] = cmd->pos[0];
    fx->pos[1] = cmd->pos[1];
    fx->pos[2] = cmd->pos[2];
    fx->pos2[0] = cmd->pos2[0];
    fx->pos2[1] = cmd->pos2[1];
    fx->pos2[2] = cmd->pos2[2];
    fx->moveSpeed = cmd->moveSpeed;
    fx->moveAccel = cmd->moveAccel;
    fx->rot[0] = cmd->rot[0];
    fx->rot[1] = cmd->rot[1];
    fx->rot[2] = cmd->rot[2];
    fx->rot2[0] = cmd->rot2[0];
    fx->rot2[1] = cmd->rot2[1];
    fx->rot2[2] = cmd->rot2[2];
    fx->rotAccel[0] = cmd->rotAccel[0];
    fx->rotAccel[1] = cmd->rotAccel[1];
    fx->rotAccel[2] = cmd->rotAccel[2];
    fx->scale[0] = cmd->scale[0];
    fx->scale[1] = cmd->scale[1];
    fx->scale[2] = cmd->scale[2];
    fx->scale2[0] = cmd->scale2[0];
    fx->scale2[1] = cmd->scale2[1];
    fx->scale2[2] = cmd->scale2[2];
    fx->scaleStep[0] = cmd->scaleStep[0];
    fx->scaleStep[1] = cmd->scaleStep[1];
    fx->scaleStep[2] = cmd->scaleStep[2];
    fx->hitRadius = cmd->hitRadius;
    fx->period = cmd->period;
    fx->fadeMode = cmd->fadeMode;
    fx->speed = cmd->speed;
    fx->wavePhase = cmd->wavePhase;
    fx->waveAmplitude = cmd->waveAmplitude;
    fx->waveFreq = cmd->waveFreq;
    fx->mode = cmd->mode;
}

void SUG_getEffectWorldPos(void *xform, EffectParams *params) {
    SVECTOR pos;

    getTransformWorldPos(xform, &pos);
    params->pos[0] = pos.vx;
    params->pos[1] = pos.vy;
    params->pos[2] = pos.vz;
}

void SUG_initEffectFromParams(EffectTemplate *template, EffectParams *cmd, EffectSlots *ctx) {
    SUG_setEffectParams((EffectInit *)template, cmd, ctx);
    /* jp's effects don't keep what they hang from, and can't hang from a
       model effect's bone */
#if VERSION_US || VERSION_EU
    ((EffectInit *)template)->source = cmd->source;
    ((EffectInit *)template)->target = cmd->target;
#endif
    if (cmd->target < 0) {
        ((EffectInit *)template)->parent = ctx->xform;
    } else if (cmd->source < 0) {
#if VERSION_JP
        ((EffectInit *)template)->parent = (void *)ctx->slots[cmd->target].value;
#elif VERSION_US || VERSION_EU
        if (cmd->source == -1) {
            ((EffectInit *)template)->parent = (void *)ctx->slots[cmd->target].value;
        } else {
            ((EffectInit *)template)->parent = ((ModelEffect *)ctx->slots[-cmd->source - 2].value)->model->boneMatrices[cmd->target];
        }
#endif
    } else {
        ((EffectInit *)template)->parent = ((ModelData *)SCENE_3D->models[ctx->modelSlots[cmd->source]])->boneMatrices[cmd->target];
    }
}

void SUG_createFadeRectFromParams(EffectParams *params) {
    Rect16 rect;
    Color from;
    Color to;

    rect.x = params->rectX;
    rect.y = params->rectY;
    rect.w = params->rectW;
    rect.h = params->rectH;
    from.r = params->r0;
    from.g = params->g0;
    from.b = params->b0;
    to.r = params->r1;
    to.g = params->g1;
    to.b = params->b1;
    SUG_createFadeRect(&rect, &from, &to, params->abr, params->rate, params->variant);
}

void SUG_createScrollTextureFromParams(EffectParams *params, EffectSlots *ctx) {
    Rect16 rect;

    /* the texture sits in the VRAM rows of the first model */
    rect.x = params->texX;
    rect.y = params->texY + ctx->modelSlots[0] * 256;
    rect.w = params->texW;
    rect.h = params->texH;
    SUG_createScrollTexture(&rect, params->texDepth, params->variant, params->rate);
}

/* the VRAM row of an effect's CLUT: jp leaves it in the upper rows, the
   others move it down to the side's rows with the texture */
#if VERSION_JP
#define EFFECT_CLUT_Y(cmd, ctx) ((cmd)->clutY)
#elif VERSION_US || VERSION_EU
#define EFFECT_CLUT_Y(cmd, ctx) (((ctx)->modelSlots[0] << 8) + (cmd)->clutY)
#endif

/* (u16)(v) << n, written as the two shifts GCC 2.8.1 keeps: the match
   depends on this form */
#define U16_SHL(v, n) ((u32)((v) << 16) >> (16 - (n)))

void SUG_createSphereFromParams(EffectParams *cmd, EffectSlots *ctx) {
    u8 color[3];
    Rect16 uv;
    EffectTemplate template;
    s32 tpage;
    s32 x;
    s32 y;
    s16 u;
    s32 v;

    SUG_initEffectFromParams(&template, cmd, ctx);
    color[0] = cmd->r0;
    color[1] = cmd->g0;
    color[2] = cmd->b0;
    x = cmd->texX;
    u = x & 0x3F;
    y = (ctx->modelSlots[0] << 8) + cmd->texY;
    v = y & 0xFF;
    switch (cmd->texDepth) {
    case 0:
        u = U16_SHL(u, 2);
        break;
    case 1:
        u = U16_SHL(u, 1);
        break;
    }
    uv.x = u;
    uv.y = v;
    uv.w = cmd->texW;
    uv.h = cmd->texH;
    tpage = GetTPage(cmd->texDepth, cmd->abr, cmd->texX, (ctx->modelSlots[0] << 8) + cmd->texY);
    SUG_createSphereEffect(cmd->brightness, color, cmd->pulse, cmd->pulseMode, &template, cmd->segments, cmd->slices, cmd->radius, cmd->semiTrans, cmd->abr,
                  cmd->primKind, cmd->count, cmd->texAnimId, &uv, tpage, GetClut(cmd->clutX, EFFECT_CLUT_Y(cmd, ctx)),
                  cmd->cull, cmd->otz.w, ctx->pak);
}

void SUG_createTrailFromParams(EffectParams *cmd, EffectSlots *ctx) {
    Bytes4 c0;
    Bytes4 c1;
    Bytes4 c2;
    Bytes4 c3;
    Rect16 uv;
    EffectTemplate template;
    s32 tpage;
    s32 x;
    s32 y;
    s16 u;
    s32 v;

    SUG_initEffectFromParams(&template, cmd, ctx);
    c0.b[0] = cmd->r0;
    c0.b[1] = cmd->g0;
    c0.b[2] = cmd->b0;
    c1.b[0] = cmd->r1;
    c1.b[1] = cmd->g1;
    c1.b[2] = cmd->b1;
    c2.b[0] = cmd->r2;
    c2.b[1] = cmd->g2;
    c2.b[2] = cmd->b2;
    c3.b[0] = cmd->r3;
    c3.b[1] = cmd->g3;
    c3.b[2] = cmd->b3;
    x = cmd->texX;
    u = x & 0x3F;
    /* jp's trails keep their texture in the rows the script names */
#if VERSION_JP
    v = cmd->texY & 0xFF;
#elif VERSION_US || VERSION_EU
    y = (ctx->modelSlots[0] << 8) + cmd->texY;
    v = y & 0xFF;
#endif
    switch (cmd->texDepth) {
    case 0:
        u = U16_SHL(u, 2);
        break;
    case 1:
        u = U16_SHL(u, 1);
        break;
    }
    uv.x = u;
    uv.y = v;
    uv.w = cmd->texW;
    uv.h = cmd->texH;
#if VERSION_JP
    tpage = GetTPage(cmd->texDepth, cmd->abr, cmd->texX, cmd->texY);
    SUG_createTrailEffect(cmd->brightness, &c0, &c1, &c2, &c3, &template, cmd->edgeX0, cmd->edgeX1, cmd->count, cmd->rows, cmd->variant,
                  cmd->pulseMode, cmd->semiTrans, cmd->abr, cmd->primKind, cmd->texAnimId, &uv, tpage,
                  GetClut(cmd->clutX, cmd->clutY), cmd->otz.w);
#elif VERSION_US || VERSION_EU
    tpage = GetTPage(cmd->texDepth, cmd->abr, cmd->texX, (ctx->modelSlots[0] << 8) + cmd->texY);
    SUG_createTrailEffect(cmd->brightness, &c0, &c1, &c2, &c3, &template, cmd->edgeX0, cmd->edgeX1, cmd->count, cmd->rows, cmd->variant,
                  cmd->pulseMode, cmd->semiTrans, cmd->abr, cmd->primKind, cmd->texAnimId, &uv, tpage,
                  GetClut(cmd->clutX, EFFECT_CLUT_Y(cmd, ctx)), cmd->otz.w, ctx->pak);
#endif
}

void SUG_createRingFromParams(EffectParams *cmd, EffectSlots *ctx) {
    Bytes4 inner;
    Bytes4 mid;
    Bytes4 outer;
    Rect16 uv;
    EffectTemplate template;
    s32 tpage;
    s32 x;
    s32 y;
    s16 u;
    s32 v;

    SUG_initEffectFromParams(&template, cmd, ctx);
    inner.b[0] = cmd->r0;
    inner.b[1] = cmd->g0;
    inner.b[2] = cmd->b0;
    mid.b[0] = cmd->r1;
    mid.b[1] = cmd->g1;
    mid.b[2] = cmd->b1;
    outer.b[0] = cmd->r2;
    outer.b[1] = cmd->g2;
    outer.b[2] = cmd->b2;
    x = cmd->texX;
    u = x & 0x3F;
    y = (ctx->modelSlots[0] << 8) + cmd->texY;
    v = y & 0xFF;
    switch (cmd->texDepth) {
    case 0:
        u = U16_SHL(u, 2);
        break;
    case 1:
        u = U16_SHL(u, 1);
        break;
    }
    uv.x = u;
    uv.y = v;
    uv.w = cmd->texW;
    uv.h = cmd->texH;
    tpage = GetTPage(cmd->texDepth, cmd->abr, cmd->texX, (ctx->modelSlots[0] << 8) + cmd->texY);
    createRingEffect(cmd->brightness, &inner, &mid, &outer, &template, cmd->count, cmd->semiTrans, cmd->abr, cmd->primKind, cmd->innerRadius,
                     cmd->outerRadius, cmd->midPercent, cmd->innerZ, cmd->outerZ, (Bytes8 *)&uv, tpage,
                     GetClut(cmd->clutX, EFFECT_CLUT_Y(cmd, ctx)), cmd->texAnimId, cmd->flags, cmd->cull, cmd->otz.w,
                     ctx->pak);
}

EffectTemplate *SUG_createEffectObjectFromParams(EffectParams *cmd, EffectSlots *ctx) {
    EffectTemplate template;

    SUG_initEffectFromParams(&template, cmd, ctx);
    return cloneEffectObject(&template);
}

void SUG_createModelEffectFromParams(EffectParams *params, EffectSlots *ctx) {
    EffectTemplate buf;
    EffectTemplate *template;
    Entry16 *entry;

    template = &buf;
    SUG_initEffectFromParams(template, params, ctx);
    entry = (Entry16 *)params->vramEntries;
    if (entry != NULL) {
        entry += ctx->modelSlots[0];
    }
#if VERSION_JP
    /* jp's model effects animate all their bones and take one CLUT bank */
    SUG_createModelEffect(params->brightness, template, params->id, params->anim, params->modelTexAnimId, (s32)entry,
                  params->flags, ctx->pak);
#elif VERSION_US || VERSION_EU
    SUG_createModelEffect(params->brightness, template, params->id, params->anim, params->modelTexAnimId, (s32)entry,
                  params->flags, params->allBones, ctx->pak, ctx->modelSlots[2]);
#endif
}

void SUG_createSpriteEffectFromParams(EffectParams *params, EffectSlots *ctx) {
    EffectTemplate buf;
    EffectTemplate *template;
    SVECTOR unused; /* unused, but it is in the original stack frame */

    template = NULL;
    if (params->vramEntries != 0) {
        template = &buf;
        SUG_initEffectFromParams(template, params, ctx);
    }
#if VERSION_JP
    /* jp's sprite effects can't be flipped or placed at their origin */
    SUG_createSpriteEffect(params->brightness, template, params->id, params->otz.w, ctx->pak);
#elif VERSION_US || VERSION_EU
    SUG_createSpriteEffect(params->brightness, template, params->id, params->flipX, params->flipY,
                  params->unk724, params->useOrigin, params->otz.w, ctx->pak);
#endif
}

void SUG_createStreaksFromParams(EffectParams *cmd, void *ctx) {
    EffectTemplate template;
    u8 from[3];
    u8 to[3];

    from[0] = cmd->r0;
    from[1] = cmd->g0;
    from[2] = cmd->b0;
    to[0] = cmd->r1;
    to[1] = cmd->g1;
    to[2] = cmd->b1;
    SUG_initEffectFromParams(&template, cmd, ctx);
#if VERSION_JP
    /* jp's streaks keep one length */
    createStreakParticles(from, to, &template, cmd->spreadX, cmd->spreadY, cmd->length, cmd->frames, cmd->speedRange,
                          cmd->reverse, cmd->streakCount, cmd->zOffset, cmd->spin, cmd->pattern, cmd->kind, cmd->semiTrans, cmd->flags, cmd->otz.w);
#elif VERSION_US || VERSION_EU
    createStreakParticles(from, to, &template, cmd->spreadX, cmd->spreadY, cmd->length, cmd->endLength, cmd->frames, cmd->speedRange,
                          cmd->reverse, cmd->streakCount, cmd->zOffset, cmd->spin, cmd->pattern, cmd->kind, cmd->semiTrans, cmd->flags, cmd->otz.w);
#endif
}

void SUG_createLightMotionFromParams(EffectParams *params) {
    VECTOR a;
    VECTOR b;
    VECTOR c;
    VECTOR d;

    a.vx = params->pos[0];
    a.vy = params->pos[1];
    a.vz = params->pos[2];
    b.vx = params->pos2[0];
    b.vy = params->pos2[1];
    b.vz = params->pos2[2];
    c.vx = params->scale[0];
    c.vy = params->scale[1];
    c.vz = params->scale[2];
    d.vx = params->scale2[0];
    d.vy = params->scale2[1];
    d.vz = params->scale2[2];
    SUG_createLightMotion(&a, &b, &c, &d, params->rot[0], params->rot[1], params->rot[2]);
}

SlotCreate SUG_EFFECT_CREATE_FUNCS[18] = {
    (SlotCreate)SUG_createFadeRectFromParams,
    (SlotCreate)SUG_createScrollTextureFromParams,
    NULL,
    (SlotCreate)SUG_createLightMotionFromParams,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    (SlotCreate)SUG_createSphereFromParams,
    NULL,
    (SlotCreate)SUG_createTrailFromParams,
    (SlotCreate)SUG_createRingFromParams,
    (SlotCreate)SUG_createEffectObjectFromParams,
    (SlotCreate)SUG_createModelEffectFromParams,
    (SlotCreate)SUG_createSpriteEffectFromParams,
    (SlotCreate)SUG_createStreaksFromParams,
};

void SUG_freeFadeRect(void *ptr) {
    freeHeapBlock(ptr);
}

SlotFree SUG_EFFECT_FREE_FUNCS[18] = {
    (SlotFree)SUG_freeFadeRect,
    (SlotFree)SUG_freeScrollTexture,
    NULL,
    (SlotFree)SUG_freeLightMotion,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    (SlotFree)SUG_freeSphereEffect,
    NULL,
    (SlotFree)SUG_freeTrailEffect,
    (SlotFree)freeRingEffect,
    (SlotFree)freeEffectObject,
    (SlotFree)SUG_freeModelEffect,
    (SlotFree)SUG_freeSpriteEffect,
    (SlotFree)freeStreakParticles,
};

void SUG_createEffectEntry(s32 index, s32 kind, s32 arg, EffectSlots *slots) {
    if (SUG_EFFECT_CREATE_FUNCS[kind] != NULL) {
        slots->slots[index].id = kind;
        slots->slots[index].active = 0;
        slots->slots[index].value = SUG_EFFECT_CREATE_FUNCS[kind](arg, slots);
        slots->count++;
        if ((slots->count & 0xF) == 0) {
            waitFrames(FRAME_INTERVAL);
        }
    }
}

void SUG_freeEffectEntries(EffectSlots *slots) {
    void (*fn)(s32);
    s32 i;

    for (i = 2; i < 23; i++) {
        if (SCENE_3D->modelState[i] > 0) {
            SCENE_3D->modelState[i] = -1;
        }
    }
    waitFrames(FRAME_INTERVAL);
    for (i = 0; i < 150; i++) {
        if (slots->slots[i].id != -1) {
            fn = SUG_EFFECT_FREE_FUNCS[slots->slots[i].id];
            if (fn != NULL) {
                fn(slots->slots[i].value);
            }
        }
    }
}

EffectScript *SUG_createEffectScript(void *script, s32 side, s32 a2, s32 *state) {
    EffectScript *runner;

    runner = allocTaskHeapBlock(sizeof(EffectScript));
    runner->script = script;
    runner->context = createScriptContext(script);
#if VERSION_JP
    runner->regs = allocScriptRegisters(0x1C6); /* jp's scripts have fewer registers */
#elif VERSION_US || VERSION_EU
    runner->regs = allocScriptRegisters(0x1CC);
#endif
    SUG_initEffectSlots(runner);
    runner->slots->modelSlots[0] = side;
    runner->slots->modelSlots[1] = side ^ 1;
    runner->slots->modelSlots[2] = a2;
    switch (side) {
    case 0:
    case 1:
        initTransform(runner->slots->xform, (s32)SCENE_3D->viewMatrix, 0, 0, ((ModelData *)SCENE_3D->models[side])->x, 0,
                      ((ModelData *)SCENE_3D->models[side])->rotY, 0);
        break;
    default:
        runner->slots->modelSlots[0] = runner->slots->modelSlots[1] = 0;
        initTransform(runner->slots->xform, (s32)SCENE_3D->viewMatrix, 0, 0, 0, 0, 0, 0);
        break;
    }
    SUG_runEffectScript(runner);
    return runner;
}

void SUG_freeEffectScript(EffectScript *runner) {
    SUG_freeEffectSlots(runner->slots);
    freeScriptContext(runner->context, runner->regs);
    if ((void *)runner->slots->pak != NULL) {
        freeHeapBlock((void *)runner->slots->pak);
    }
    freeHeapBlock(runner);
}

s32 SUG_pauseEffectScript(EffectScript *runner, s32 *state) {
    if (*state != 3) {
        *state = 1;
        truncatePakTextures((Chunk *)runner->slots->pak);
        do {
            waitFrames(FRAME_INTERVAL);
        } while (*state == 1);
    }
}

void SUG_runEffectScriptTask(void *script, s32 slot, s32 a2, s32 *state) {
    EffectScript *obj;
    s32 prev;
    s32 running;

    /* the match depends on the extra block, for the register allocation */
    do {
        prev = *state;
        obj = SUG_createEffectScript(script, slot, a2, state);
        running = *obj->regs;
        do {
            waitFrames(FRAME_INTERVAL);
            if (*state == 3) {
                break;
            }
            if (running < 0 && *state < 0 && SUG_pauseEffectScript(obj, state) == 3) {
                break;
            }
            running = SUG_tickEffectScript(obj);
            /* jp's scripts have no screen copy effect */
#if VERSION_US || VERSION_EU
            SUG_tickScreenCopyEffect();
#endif
        } while (running != 0);
        if (prev == -2) {
            pauseModelAnimation(slot);
        }
    } while (0);
    *state = 0;
    SUG_freeEffectScript(obj);
}

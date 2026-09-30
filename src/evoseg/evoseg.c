#include "common.h"
#include "game.h"
#include "dcb/scene3d.h"
#include "dcb/model_anim.h"
#include "dcb/wire_grid.h"
#include "dcb/frame_callback.h"
#include "dcb/heap.h"
#include "dcb/model_load.h"
#include "dcb/anim_control.h"
#include "dcb/task.h"
#include "dcb/stage.h"
#include "dcb/player_data.h"
#include "dcb/card_db.h"
#include "dcb/window.h"
#include "dcb/prim_util.h"
#include "dcb/text.h"
#include "dcb/fade.h"
#include "dcb/decompress.h"
#include "dcb/effect_object.h"
#include "dcb/battle_hud.h"
#include "dcb/transform.h"
#include "dcb/loader.h"
#include "dcb/script.h"
#include "dcb/vram_upload.h"
#include "dcb/card_zones.h"
#include "dcb/dialog.h"
#include "dcb/memcard.h"
#include "dcb/partner_level.h"
#include "dcb/sound_play.h"
#include "dcb/vblank.h"
#include "dcb/prim.h"
#include "dcb/sound.h"
#include "dcb/scroll_bg.h"
#include "dcb/menu.h"
#include "dcb/sort.h"
#include "dcb/game_flow.h"
#include "gte.h"

void func_801E0AFC(GsDOBJ4 *obj, s32 mode);
void GsGetLs(GsCOORDINATE2 *coord, MATRIX *m);
void GsSetLsMatrix(MATRIX *m);
void GsSetLightMatrix(MATRIX *m);
void GsSortObject4(GsDOBJ4 *obj, GsOT *ot, s32 shift, u32 *scratch);

extern s32 D_801F5518;

typedef struct {
    s32 unk0;
    s32 unk4;
    u32 size;
    s32 unkC;
    u8 code[1];
} EvoMsd;

typedef struct {
    EvoMsd *data;
    Script *script;
    s32 *vars;
} EvoProgram;
Script *func_801E8650(EvoMsd *data);

extern Menu D_801F0030;
extern Menu D_801F005C;
void func_801EE2DC(s32 index, s32 arg);
void func_801EE330(s32 index, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_801EB2A0(void);
extern UiWindow D_801F51B8;

typedef struct {
    u8 olen;
    u8 ilen;
    u8 flag;
    u8 mode;
    u8 u0, v0;
    u16 cba;
    u8 u1, v1;
    u16 tsb;
    u8 u2, v2;
    u16 pad;
    u8 u3, v3;
    u16 pad2;
    u16 idx[8];
} TmdPacketFT4;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, pad1;
    s16 x1, y1;
    u8 r2, g2, b2, pad2;
    s16 x2, y2;
} PolyG3;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, pad1;
    s16 x1, y1;
    u8 r2, g2, b2, pad2;
    s16 x2, y2;
    u8 r3, g3, b3, pad3;
    s16 x3, y3;
} PolyG4;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
} PolyF3;

typedef struct {
    u8 olen;
    u8 ilen;
    u8 flag;
    u8 mode;
    u8 u0, v0;
    u16 cba;
    u8 u1, v1;
    u16 tsb;
    u8 u2, v2;
    u16 pad;
    u16 idx[8];
} TmdPacketFT3;
typedef struct {
    s16 r;
    s16 g;
    s16 b;
} EvoColor;
extern s8 *D_801F0540;
extern SVECTOR *D_801F0538;
extern EvoColor D_801F0530;
extern MATRIX D_801DBEA0;
long func_80062C44(void);
void func_80062C34(long base);
typedef struct {
    s16 first;
    s16 last;
} EvoRange;
typedef struct {
    s16 id;
    s8 type;
    u8 name[0x15];
    u8 unk18;
    u8 level;
    u8 attr;
    u8 pad1B[0xE5 - 0x1B];
    u8 modelId;
} EvoCardInfo;
extern EvoRange D_801EFF00[];
extern EvoCardInfo *D_801F4980[];
typedef struct {
    Rect16 rect;
    u16 src[0x100];
    u16 dst[0x100];
    s16 brighten;
    s16 level;
} EvoClut;

void func_801E0618(s32 arg);
void func_801E2E30();
void func_801E00F4();
typedef struct {
    s16 x;
    s16 y;
} EvoPose;
typedef struct {
    u8 olen;
    u8 ilen;
    u8 flag;
    u8 mode;
    u8 u0;
    u8 v0;
    u16 cba;
    u8 u1;
    u8 v1;
    u16 tsb;
} TmdPrim;
typedef struct {
    SVECTOR *vertTop;
    u32 nvert;
    SVECTOR *normTop;
    u32 nnormal;
    u8 *primTop;
    u32 nprim;
    s32 scale;
    TmdPrim prims[1];
} TmdObject;
typedef struct {
    u8 pad0[8];
    TmdObject *tmd;
    u8 padC[4];
} EvoPart;
typedef struct {
    u8 pad0[4];
    s16 partCount;
    u8 pad6[0xB80 - 0x6];
    EvoPart parts[320];
    EvoPose *pose;
    u8 pad1F84[0x22B0 - 0x1F84];
    MATRIX matrices[2];
} EvoModel;
void func_801DF7EC(s16 slot);
void func_801E08D4(EvoPart *part, s32 enable);
extern s16 D_80079584;
void func_801DFC18(s32 parentTask);
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
    u8 r;
    u8 g;
    u8 b;
    u8 pad;
} FlatLight;
s32 GsSetFlatLight(s32 id, FlatLight *light);
extern u8 *D_801F4E34;
extern u8 *D_801F4E40[3];
typedef struct {
    u8 pad0[0xA0];
    s32 *unkA0;
    s32 unkA4;
    s32 unkA8;
    s16 unkAC;
    s16 unkAE;
    s16 unkB0;
    s16 candidates[2];
    s8 unkB6;
    s8 unkB7;
    s8 unkB8;
    s8 unkB9;
    s8 unkBA;
    s8 unkBB;
    s8 unkBC;
    s8 unkBD;
    s8 unkBE;
    s8 unkBF;
    s8 unkC0;
    s8 unkC1;
    s8 unkC2;
    s8 unkC3;
    s8 unkC4;
    s8 unkC5;
    s8 unkC6;
    s8 unkC7;
    s8 unkC8;
    s8 unkC9;
    s8 unkCA;
    u8 unkCB;
    s8 busy[3];
} EvoMenu;
typedef struct {
    POLY_FT4 polys[2][3];
    u8 padF0[0x120 - 0xF0];
    s32 unk120;
    s16 unk124;
    s16 unk126;
    u8 pad128[0x12C - 0x128];
} EvoScene;
extern EvoMenu D_801F5478;
extern EvoScene D_801F5548[2];
extern u8 D_801F0056;
extern UiWindow D_801F5008;
extern UiWindow D_801F5050;
extern Rect16 D_801F0168;
extern Rect16 D_801F0184;
typedef struct {
    u32 tag;
    u32 code[1];
} DrTPage;
typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
    s16 x3, y3;
} PolyF4;
typedef struct {
    DrTPage tpage[2];
    PolyF4 poly[2];
    s16 unk40;
    s8 unk42;
} EvoFade;
extern EvoFade D_801F53C8;
void func_801E7F64(void);
void func_801E81C4(EvoProgram *program);
extern EvoProgram *D_801F5250;
typedef struct {
    u8 text[0x3C];
    s16 pos;
    s8 active;
    s8 len;
} EvoText;
extern UiWindow D_801F5128;
extern Rect16 D_801F01D8;
extern UiWindow D_801F43D0;
extern Rect16 D_801F0038;
void func_801ECC6C(void);
extern u8 D_801F553F;
extern s8 D_801F59A4;
extern s8 D_801F59A5;
extern s8 D_801F59A6;
extern u8 *D_801F59A8;
void func_801EF7DC();
typedef struct {
    u8 pad0[0x20];
    VECTOR curRot;
    SVECTOR curScale;
    s32 curPos[3];
    u8 pad44[0x98 - 0x44];
    void *unk98;
    u8 pad9C[0xAC - 0x9C];
    VECTOR pos;
    VECTOR vel;
    SVECTOR accel;
    SVECTOR rot;
    SVECTOR rotVel;
    SVECTOR scale;
    SVECTOR scaleVel;
    SVECTOR scaleAccel;
    u8 padFC[0x118 - 0xFC];
    s32 unk118;
    s32 unk11C;
    s16 unk120;
    s16 unk122;
    s16 unk124;
    s16 unk126;
    s16 unk128;
    s16 unk12A;
    s16 unk12C;
    s16 unk12E;
    s16 unk130;
    u8 pad132[0x137 - 0x132];
    u8 unk137;
    u8 unk138;
    u8 unk139;
    u8 pad13A[2];
} EvoFx;
typedef struct {
    s16 kind;
    s16 active;
    EvoFx *handle;
} EvoEntry;
typedef struct {
    EvoMsd *data;
    Script *script;
    s32 *vars;
    EvoEntry entries[16];
    s32 counter;
    void *buffer;
} EvoLoader;
extern EvoFx D_801F5868;
extern void (*D_801F0280[])(EvoFx *);
extern void (*D_801F02A8[])(EvoFx *);
void func_801EE69C(EvoLoader *loader);
typedef struct {
    u8 pad[0x260];
    s32 x;
    s32 y;
    s32 z;
} EvoObject;
typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 pad;
} Color;
typedef struct {
    u8 pad0[0x98];
    s32 r;
    s32 g;
    s32 b;
} EvoLight;
void func_801F893C(void *sprite, Color *color);

typedef struct {
    UiWindow win;
    s8 unk44;
    s8 z;
    u8 pad46[2];
} EvoWindow;
extern EvoWindow D_801F4E58[];

typedef struct {
    Rect16 rect;
    s32 brightness;
    s32 style;
    s32 flags;
    s32 label;
    u8 labelPalette;
} EvoWindowDef;
extern EvoWindowDef D_801F00C0[];

extern EvoWindowDef D_801F006C[];
extern EvoWindowDef D_801F0114[];
extern u8 D_801F5358;
extern s8 D_801F53B3;

extern SVECTOR *D_801F2BBC;
extern s8 D_801F05F8;
extern s8 D_801F05F4;
void func_801E285C(void);

typedef struct {
    char *name;
    s8 learnLevels[6];
    u8 unkA[2];
} EvoAbilityInfo;
extern EvoAbilityInfo D_801EF8D4[];
typedef struct {
    u8 pad0[0xA5];
    s8 choice;
} EvoDialog;
extern EvoDialog D_801F57A8;
extern u8 D_801F57A0;
extern u8 D_801F57A1;
extern s16 D_801F5860[4];
extern s32 D_801F4E50;
extern UiWindow D_801F5410;
void func_801EB670(void);

extern EvoFx *(*D_801F0294[])(s32 *, EvoLoader *);

/* libgs's GsSortObject4J function table (_GsFCALL) */
#define GsDivMODE_NDIV 0
#define GsDivMODE_DIV 1
#define GsLMODE_NORMAL 0
#define GsLMODE_LOFF 2
typedef u32 *(*GsSortFunc)();
typedef struct {
    GsSortFunc f3[2][3];
    GsSortFunc nf3[2];
    GsSortFunc g3[2][3];
    GsSortFunc ng3[2];
    GsSortFunc tf3[2][3];
    GsSortFunc ntf3[2];
    GsSortFunc tg3[2][3];
    GsSortFunc ntg3[2];
    GsSortFunc f4[2][3];
    GsSortFunc nf4[2];
    GsSortFunc g4[2][3];
    GsSortFunc ng4[2];
    GsSortFunc tf4[2][3];
    GsSortFunc ntf4[2];
    GsSortFunc tg4[2][3];
    GsSortFunc ntg4[2];
    GsSortFunc f3g[3];
    GsSortFunc g3g[3];
    GsSortFunc f4g[3];
    GsSortFunc g4g[3];
} _GsFCALL;
extern _GsFCALL D_801DBFB0;
u32 *GsTMDfastF3L(), *GsTMDfastTF3L(), *GsTMDfastTF3NL(), *GsTMDfastTNF3();
u32 *GsTMDfastG3L(), *GsTMDfastTG3L(), *GsTMDfastTG3NL(), *GsTMDfastTNG3();
u32 *GsTMDfastF4L(), *GsTMDfastTF4L(), *GsTMDfastTF4NL(), *GsTMDfastTNF4();
u32 *GsTMDfastG4L(), *GsTMDfastTG4L(), *GsTMDfastTG4NL(), *GsTMDfastTNG4();
u32 *GsTMDdivTF4L(), *GsTMDdivTF4NL(), *GsTMDdivTF3NL(), *GsTMDdivTG4NL();
u32 *GsTMDdivTG3NL(), *GsTMDdivTNG4(), *GsTMDdivTNG3(), *GsTMDdivTNF4();
u32 *GsTMDdivTNF3(), *GsTMDfastF4NL(), *GsTMDfastNF4();

typedef struct {
    u8 pad0[0x260];
    s32 args[34];
} EvoFxParams;

extern s16 D_801F2BFC[3];
extern s8 D_801F540A;
void func_801DFD14(s32 parentTask);
extern EvoText D_801F5258[4];
s32 func_801E943C(u8 *src);
typedef struct {
    u8 card;
    u8 ability;
} EvoAbilityReward;
extern u8 D_801EFED4[6];
extern EvoAbilityReward D_801EFF1C[][5];
extern UiWindow D_801F5200;
extern Rect16 D_801F022C;

extern u8 D_801F5360;

typedef struct {
    u8 pad0[0x20];
    s16 joint;
    s16 timer;
} EvoSpark;
void func_801E5244(EvoSpark *spark);
s32 func_801E5024(s32 min, s32 max);

typedef struct {
    s16 timer;
    u16 count;
    SVECTOR *offsets;
    SVECTOR *verts;
    s8 *prims;
    s16 primCount;
    s16 unk12;
} EvoShard;
extern EvoShard *D_801F05FC;

extern Rect16 D_801F00B8;

typedef struct {
    u8 pad0[0xE];
    s16 unkE;
    u8 pad10[0x33 - 0x10];
    s8 side;
} EvoChoice;
extern EvoChoice D_801F5380;

extern u8 D_801F4E4C;
void func_801E7E8C(s32 mode);
void func_801EA2D4(void);

extern u8 D_800795A8;
void func_801EF108(EvoFx *fx, s32 *vars, EvoLoader *loader);
void func_801F8928(void *sprite);
void func_801F8910(void *sprite, s32 arg);

extern SVECTOR *D_801F2BB8;

SVECTOR *ApplyMatrixSV(MATRIX *m, SVECTOR *v0, SVECTOR *v1);

extern s8 D_801EFEDC[][6];
extern u8 D_801DF03C[20][4];
void func_801ED5F4(u8 type, u8 level, s16 *out, s16 excludeA, s16 excludeB);

void func_801E335C(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void func_801E3694(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void func_801E39CC(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void func_801E3D6C(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void func_801E410C(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void func_801E4444(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void func_801E55FC(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void func_801E5934(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void func_801E5C1C(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void func_801E5F54(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void func_801E9390(EvoText *slot);
void func_801EA110(void);
void func_801EA414(void);
void func_801EA530(void);
void func_801EA790(void);
void func_801EA820(void);
void func_801EB0F4(void);
void func_801EB1D0(void);
void func_801EB234(void);
void func_801ECBE8(void);
void func_801ECC24(void);
void func_801EED18(EvoFx *fx, EvoFxParams *params, s32 current);
void func_801EEF24(EvoFx *fx, EvoFxParams *params);
void func_801EF0BC(void *xform, EvoObject *obj);
void func_801EF5C8(s32 index, s32 kind, s32 *vars, EvoLoader *loader);

EvoLoader *func_801EF65C(EvoMsd *data);

s32 StoreImage(Rect16 *rect, void *p);
extern u16 D_801F02C8[16];
extern s8 D_801F02C0;
extern u8 D_801F02E8;
void func_801DFB00(void);

const char D_801DDF38[] = "\t";

void func_801DF9D0(void);

void func_801DFEC0(s32 unused);
void func_801E8E88();

typedef struct {
    s32 count;
    s16 *queue;
    u8 pad8[0x26 - 0x8];
    s8 total;
    s8 next;
    s8 model;
} EvoShatter;
extern EvoShatter D_801F0600;
extern s16 D_801F0550[40];
extern s16 D_801F05A0[42];
extern s16 D_801EF874[40];
extern u16 D_801F05F6;

s32 func_801E47E4(s32 part, s32 arg);
void func_801E2FC8(s32 index);

void func_801E2C0C(s32 model);

extern u32 D_801F2BB0;
extern u32 D_801EF85C;
extern EvoSpark D_801F02F0[16];
void func_801E51E4(EvoSpark *spark);

extern char *D_801F0000[];
extern s32 (*D_801F0088[])(s8 *, s8 *);
extern EvoCardInfo *D_801F44C0[];
void func_801E65A8(void);

extern void (*D_801F0248[])();
extern UiWindow D_801F4380;
void func_801E7178();
void func_801E9B94(UiWindow *w);
void func_801E6FCC(UiWindow *w);
void func_801E780C(EvoScene *scene);

s32 func_801E97E4(s32 x, s32 y, EvoText *t, s32 z);

extern CursorHighlight D_801F4470;
extern CursorHighlight D_801F4420;

void GsMulCoord3(MATRIX *m1, MATRIX *m2);

typedef struct {
    DrTPage tpage[2];
    PolyF4 poly[2];
    Bytes4 from;
    Bytes4 to;
    s16 speed;
    u8 state;
    u8 mode;
} EvoFadeRect;
void initPolyF4Pair();

typedef struct {
    EvoFx fx;
    Model *model;
    u8 pad140[0x160 - 0x140];
    EvoClut clut;
    s16 prevLevel;
    s8 slot;
    u8 unk56F;
    s8 unk570;
    s8 active;
    u8 pad572[2];
    s32 unk574;
} EvoModelFx;
void func_801ED8B0(EvoClut *clut, u16 flags);

void func_801E9C88(void);
void func_801EA598(void);
void func_801EABB0(void);
void func_801EB440(void);
void func_801EBA08(void);
void func_801EA9AC(void);
void func_801EAA5C(void);
void func_801EBE08(void);
void func_801EA864(s32 index);
void func_801EA934(s32 index);
void func_801EBD64(void);
void func_801EBD04(void);
void func_801E89F0(void);
void func_801DFF78(void);
void func_801E623C(void);
void func_801E8C74(void);
void func_801E62D0(void);
void func_801EE248(void);
void func_801E6398(void);
void func_801E87A8(void);
void func_801E80E4(void);
EvoProgram *func_801E8708(s32 index);
s32 *func_801E86B4(s32 count);
s32 func_801E8614(EvoProgram *program);
void func_801E8BD8(void);
void func_801E8DC0(void);
void func_801E7B8C(void);
void func_801E7CB4(void);
void func_801E7C3C(void);
void func_801E7BE4(void);
void func_801E7D2C(s32 active);
void func_801E8E1C(void);

extern s16 D_801F5458;
extern Rect16 D_801F0064;
extern Rect16 D_801F0210;
void func_801E8CD8(s32 id, s32 slot);
s16 func_801ECCA0(void);
void func_801EAAE8(s16 cardId);

extern Bytes4 D_801EFF58[];
extern Bytes4 D_801EFF60;
extern Bytes4 D_801EFF64;
extern char D_801DF15C[];
extern char D_801DF168[];
extern char D_801DF16C[];

extern char D_801DF13C[];
extern char D_801DF548[];

void func_801DF658(s8 evolved) {
    if (evolved == 0) {
        func_801E0618(1);
        createWireGrid(3000, 3000, 11, 11, 1, 0);
    } else {
        initScene3D(1);
        addFrameCallback((s32)renderWireGrid);
    }
    GRID_VISIBLE = 0;
    func_80014A00(0x19);
    func_800149B8(0x19, 0x1F, 0, 0x800, &runSceneCameraTask, 1);
    func_80014A00(0x1B);
    func_800149B8(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
}

void func_801DF724(s8 evolved) {
    func_80014A00(0x1B);
    func_80014A00(0x19);
    removeFrameCallback((s32)renderWireGrid);
    removeFrameCallback((s32)func_801E2E30);
    if (evolved == 0) {
        removeFrameCallback((s32)func_801E00F4);
    } else {
        removeFrameCallback((s32)renderSceneModels);
    }
    func_80014C08(1);
    unloadAllModels();
    GRID_VISIBLE = 0;
    freeHeapBlocksByTag(0x7F);
    if (evolved == 0) {
        freeHeapBlocksByTag(0x1F5);
        freeHeapBlocksByTag(0x41);
    } else {
        freeHeapBlocksByTag(0x84);
    }
    freeHeapBlocksByTag(0x1F4);
    freeHeapBlocksByTag(0x40);
}

void func_801DF7EC(s16 slot) {
    applyAnimationFirstFrame(slot, 0);
    startModelAnimation(slot, 0, -2, 0);
}

void func_801DF830(void) {
    Graphics *camera;
    EvoModel *model;
    EvoPart *part;
    s32 i;

    camera = (Graphics *)&GRAPHICS;
    camera->snapCamera = 1;
    camera->unk90 = 3000;
    camera->unk92 = -((EvoModel *)SCENE_3D->models[0])->pose->y * 3;
    camera->targetModel = 0;
    SCENE_3D->modelState[0] = 1;
    SCENE_3D->modelState[1] = -1;
    GRID_VISIBLE = 1;
    func_801DF7EC(0);
    model = SCENE_3D->models[1];
    for (i = 0, part = model->parts; i < model->partCount; i++, part++) {
        func_801E08D4(part, 1);
    }
}

void func_801DF914(s32 id) {
    loadDigimonModelPak(0, id, 0, 0);
    func_80014C08(2);
    func_80014C08(20);
    setModelAnimationPose(0, 0);
    D_80079584 = 0;
    func_80014C08(1);
    SCENE_3D->modelState[0] = 1;
    playModelAnimation(0, 0);
    GRID_VISIBLE = 1;
    func_800149B8(0, -1, 0, 0x400, func_801DFC18, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(10);
}

void func_801DF9D0(void) {
    Rect16 rect = { 320, 240, 32, 1 };
    s8 i;

    StoreImage(&rect, D_801F02C8);
    for (i = 0; i < 16; i++) {
        if (D_801F02C8[i] != 0) {
            D_801F02C8[i] |= 0x8000;
        } else {
            D_801F02C8[i] = 0;
        }
    }
    LoadImage((s16 *)&rect, (s32)D_801F02C8);
    for (i = 0; i < 16; i++) {
        if (D_801F02C8[i] != 0) {
            D_801F02C8[i] = 0xFFFF;
        }
    }
    rect.y = 241;
    LoadImage((s16 *)&rect, (s32)D_801F02C8);
    D_801F02C0 = 2;
    D_801F02E8 = 0;
    addFrameCallback((s32)func_801DFB00);
}

const u8 D_801DDF44[20][3] = {
    { 0x01, 0x04, 0xEC },
    { 0x04, 0x23, 0xEC },
    { 0x02, 0x25, 0xED },
    { 0x06, 0x27, 0xED },
    { 0x8E, 0x28, 0xEE },
    { 0x2A, 0x2B, 0xEF },
    { 0x4C, 0x07, 0xF0 },
    { 0x4D, 0x4B, 0xF1 },
    { 0x4B, 0x8F, 0xF2 },
    { 0x51, 0x03, 0xF3 },
    { 0x70, 0x4A, 0xF4 },
    { 0x4E, 0x6F, 0xF5 },
    { 0x6E, 0x6F, 0xF6 },
    { 0x6D, 0x90, 0xF7 },
    { 0x93, 0x73, 0xF8 },
    { 0x96, 0x74, 0xF9 },
    { 0x91, 0x26, 0xFA },
    { 0x0C, 0x75, 0xFB },
    { 0x53, 0x0D, 0xFC },
    { 0x52, 0x97, 0xFD },
};

void func_801DFB00(void) {
    Rect16 uv;

    if (D_801F02C0 == 1) {
        D_801F02E8 += 8;
        if (D_801F02E8 > 0x80) {
            D_801F02E8 = 0x80;
        }
    } else if (D_801F02C0 == 2) {
        if (D_801F02E8 >= 8) {
            D_801F02E8 -= 8;
        } else {
            D_801F02E8 = 0;
        }
    }
    uv.x = 0;
    uv.y = 0x80;
    uv.w = 0xFF;
    uv.h = 0x27;
    drawTexturedSprite(0x28, 0xB4, &uv, 0x25, 0x3C14, 0, D_801F02E8, 0);
    drawTexturedSprite(0x28, 0xB4, &uv, 0x45, 0x3C54, 0, D_801F02E8, 0);
}

void func_801DFC18(s32 parentTask) {
    s32 frames;

    func_801DF9D0();
    setScreenFadeParams(1, 1, 8);
    playSoundEffect(0x8D);
    func_80014C08(120);
    D_801F02C0 = 1;
    func_80014C08(180);
    frames = 0;
    do {
        func_80014C08(FRAME_INTERVAL);
        frames++;
        if ((PAD_STATES[0]->pressed & 0x40) || frames > 180) {
            D_801F02C0 = 2;
            D_801F05F4 = 6;
        }
    } while (D_801F05F4 != 6);
    D_801F540A = 1;
    func_80014C08(10);
    func_80014A48(parentTask);
}

void func_801DFD14(s32 parentTask) {
    do {
        func_80014C08(FRAME_INTERVAL);
        switch (D_801F05F4) {
        case 1:
            func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 1, 8, 0);
            func_80014C08(60);
            D_801F05F4 = 2;
            SCENE_3D->modelState[0] = -1;
            break;
        case 2:
            playSoundEffect(0x8D);
            func_801E2C0C(1);
            SCENE_3D->modelState[1] = 1;
            applyAnimationFirstFrame(1, 0);
            startModelAnimation(1, 0, -2, 0);
            D_80079584 = 1;
            func_80014C08(5);
            setScreenFadeParams(1, 1, 8);
            D_801F05F4 = 3;
            break;
        case 4:
            func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 1, 8, 0);
            func_80014C08(60);
            D_801F05F4 = 5;
            break;
        }
    } while (D_801F05F4 != 5);
    func_80014C08(10);
    func_80014A48(parentTask);
}

void func_801DFEC0(s32 unused) {
    s32 models[2];

    models[0] = loadDigimonModelPak(0, D_801F2BFC[0], 1, 1);
    models[1] = loadDigimonModelPak(1, D_801F2BFC[1], 1, 1);
    func_801DF830();
    D_80079584 = 0;
    func_80014C08(20);
    D_801F540A = 0;
    playSoundEffect(0x8D);
    func_800149B8(0, -1, 0, 0x400, func_801DFD14, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(10);
}

void func_801DFF78(void) {
    loadSoundEffectBank(0);
    playMusic(0, 0x88, 100);
    func_801DF658(0);
    func_801DFEC0(0x73);
    func_801DF724(0);
    func_80014C08(2);
    playMusic(0, 0x89, 100);
    func_80014C08(2);
    func_801DF658(1);
    func_801DF914(D_801F2BFC[2]);
    func_801DF724(1);
    func_80014C08(10);
    do {
        func_80014C08(1);
    } while (PAD_STATES[0]->pressed & 0x40);
    removeFrameCallback((s32)func_801DFB00);
    loadSoundEffectBank(1);
    changeScrollingBackground(((PlayerProfile *)PLAYER_PROFILES)->unk56, 0x380, 0, 0x380, 0x80);
    func_800149B8(0, -1, 0, 0x400, func_801E8E88, -1, getCurrentTaskId(), 0, 0);
}

void func_801E00A4(MATRIX *m) {
    gte_SetRotMatrix(m);
    gte_SetTransMatrix(m);
}

void func_801E00F4(FrameBuffer *buffer, s32 bufferIndex) {
    MATRIX localScreen;
    SVECTOR position;
    MATRIX unused;
    MATRIX lightMatrix;
    MATRIX colorMatrix;
    s32 flag;
    Model *model;
    GsDOBJ4 *obj;
    s32 i;
    s32 j;
    s32 *scratch;

    if (SCENE_3D_ENABLED != 0) {
        func_80062C34((long)buffer->scenePackets);
        colorMatrix = SCENE_LIGHT_COLORS;
        for (i = 0; i < 24; i++) {
            if (SCENE_3D->modelState[i] <= 0) {
                continue;
            }
            model = SCENE_3D->models[i];
            lightMatrix = SCENE_LIGHT_MATRIX;
            gte_SetColorMatrix(&colorMatrix);
            PushMatrix();
            position.vx = model->pos.vx;
            position.vy = model->pos.vy;
            position.vz = model->pos.vz;
            RotTrans((u16 *)&position, localScreen.t, &flag);
            RotMatrix(&SCENE_WORLD_ROTATION, &SCENE_3D->root.coord);
            SCENE_3D->root.flg = 0;
            SCENE_3D->root.coord.t[0] = localScreen.t[0];
            SCENE_3D->root.coord.t[1] = localScreen.t[1];
            SCENE_3D->root.coord.t[2] = localScreen.t[2];
            RotMatrixYXZ(&model->rot, &model->root.coord);
            model->root.flg = 0;
            ScaleMatrix(&model->root.coord, &model->scale);
            if (D_801F05F8 == 0) {
                func_801E2C0C(0);
                D_801F05F8 = 1;
            }
            scratch = (s32 *)0x1F800000;
            scratch[12] = model->tpageOffset;
            scratch[13] = model->clutOffset;
            obj = model->obj;
            for (j = 0; j < model->nobj; j++, obj++) {
                obj->coord2->flg = 0;
                if (obj->id != -1 && obj->tmd != NULL && obj->attribute == 0) {
                    GsGetLws(obj->coord2, &model->lw[j], &localScreen);
                    GsGetLs(obj->coord2, &localScreen);
                    GsSetLsMatrix(&localScreen);
                    GsSetLightMatrix(&localScreen);
                    if (D_801EF874[j] != 16) {
                        func_801E0AFC(obj, D_801EF874[j]);
                    } else {
                        GsSortObject4(obj, &SCENE_3D->ot[bufferIndex], 2, (u32 *)0x1F800000);
                    }
                }
            }
            PopMatrix();
        }
    }
}

void func_801E0488(s32 projection) {
    func_8005C484(160, 120);
    func_8005C4A4(projection);
    func_80062484(projection);
    SCENE_3D->view.vpx = 0;
    SCENE_3D->view.vpy = 0;
    SCENE_3D->view.vpz = 0;
    SCENE_3D->view.vrx = 0;
    SCENE_3D->view.vry = 0;
    SCENE_3D->view.vrz = 0;
    SCENE_3D->view.rz = 0;
    SCENE_3D->view.super = NULL;
    GsSetRefView2(&SCENE_3D->view);
}

void func_801E04F8(void) {
    ((FlatLight *)SCENE_3D->unkE4)[0].vx = 0;
    ((FlatLight *)SCENE_3D->unkE4)[0].vy = -100;
    ((FlatLight *)SCENE_3D->unkE4)[0].vz = 100;
    ((FlatLight *)SCENE_3D->unkE4)[0].r = 0xFF;
    ((FlatLight *)SCENE_3D->unkE4)[0].g = 0xFF;
    ((FlatLight *)SCENE_3D->unkE4)[0].b = 0xFF;
    GsSetFlatLight(0, &((FlatLight *)SCENE_3D->unkE4)[0]);
    ((FlatLight *)SCENE_3D->unkE4)[1].vx = 0;
    ((FlatLight *)SCENE_3D->unkE4)[1].vy = 100;
    ((FlatLight *)SCENE_3D->unkE4)[1].vz = 100;
    ((FlatLight *)SCENE_3D->unkE4)[1].r = 0x80;
    ((FlatLight *)SCENE_3D->unkE4)[1].g = 0x80;
    ((FlatLight *)SCENE_3D->unkE4)[1].b = 0x80;
    GsSetFlatLight(1, &((FlatLight *)SCENE_3D->unkE4)[1]);
    ((FlatLight *)SCENE_3D->unkE4)[2].vx = 0;
    ((FlatLight *)SCENE_3D->unkE4)[2].vy = 0;
    ((FlatLight *)SCENE_3D->unkE4)[2].vz = 0;
    ((FlatLight *)SCENE_3D->unkE4)[2].r = 0;
    ((FlatLight *)SCENE_3D->unkE4)[2].g = 0;
    ((FlatLight *)SCENE_3D->unkE4)[2].b = 0;
    GsSetFlatLight(2, &((FlatLight *)SCENE_3D->unkE4)[2]);
    GsSetAmbient(0x40, 0x40, 0x40);
    func_8005C464(0x30, 0x30, 0x40);
    GsSetLightMode(0);
}

void func_801E0618(s32 allocBuffers) {
    s32 i;

    func_801E285C();
    initModelScene();
    func_8006A804();
    for (i = 0; i < 2; i++) {
        if (allocBuffers) {
            DB(i).scenePackets = allocHeapBlock(0x1F400, 0x7F);
        }
        SCENE_3D->ot[i].length = 12;
        SCENE_3D->ot[i].org = DB(i).ot;
        SCENE_3D->ot[i].offset = 0;
        SCENE_3D->ot[i].point = 0;
        SCENE_3D->ot[i].tag = SCENE_3D->ot[i].org + 0xFFF;
    }
    D_801F05FC = allocHeapBlock(0x4B0, 0x7F);
    D_801F2BBC = allocHeapBlock(0x3E80, 0x7F);
    D_801F05F8 = 0;
    D_801F05F4 = 0;
    GsInit3D();
    func_801E0488(0x1C0);
    func_801E04F8();
    func_8006A814();
    {
        MATRIX lightMatrices[2] = {
            { { { 0, 0x1800, -0x1800 }, { 0, 0, 0 }, { 0, 0, 0 } }, { 0, 0, 0 } },
            { { { 0, 0x1000, -0x5DC }, { 0, -0x1000, -0x7D0 }, { 0, 0, 0 } }, { 0, 0, 0 } },
        };
        MATRIX colorMatrices[2] = {
            { { { 0x800, 0, 0 }, { 0x800, 0, 0 }, { 0x800, 0, 0 } }, { 0, 0, 0 } },
            { { { 0x1000, 0x5DC, 0 }, { 0xFF5, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 } }, { 0, 0, 0 } },
        };

        SCENE_LIGHT_MATRIX = lightMatrices[1];
        SCENE_LIGHT_COLORS = colorMatrices[1];
    }
    if (allocBuffers) {
        mountDriveTask((s32)"M:", getCurrentTaskId());
        addFrameCallback((s32)func_801E00F4);
    }
}

void func_801E08D4(EvoPart *part, s32 enable) {
    TmdObject *tmd;
    s32 count;
    TmdPrim *prim;
    u8 *next;
    s32 i;

    tmd = part->tmd;
    count = tmd->nprim;
    prim = tmd->prims;
    if (enable == 0) {
        return;
    }
    for (i = 0; i < count; i++) {
        if ((prim->mode & 0x3D) == 0x2C) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x20;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x3D) == 0x3C) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x24;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x35) == 0x34) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x1C;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x35) == 0x24) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x18;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x3D) == 0x2D) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x20;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x3D) == 0x28) {
            next += 0x14;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x35) == 0x25) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x1C;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x3D) == 0x38) {
            next += 0x18;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x35) == 0x30) {
            next += 0x14;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x35) == 0x20) {
            next += 0x10;
            prim = (TmdPrim *)next;
        }
    }
}

/* libgs TMD primitive layouts */
typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 n0, v0;
    u16 v1, v2;
} TMD_P_F3;
typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 n0, v0;
    u16 n1, v1;
    u16 n2, v2;
} TMD_P_G3;
typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 n0, v0;
    u16 v1, v2;
    u16 v3, p;
} TMD_P_F4;
typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 n0, v0;
    u16 n1, v1;
    u16 n2, v2;
    u16 n3, v3;
} TMD_P_G4;
typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p;
    u16 n0, v0;
    u16 v1, v2;
} TMD_P_TF3;
typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p;
    u16 n0, v0;
    u16 n1, v1;
    u16 n2, v2;
} TMD_P_TG3;
typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 r0, g0, b0, p1;
    u16 v0, v1;
    u16 v2, p2;
} TMD_P_TNF3;
typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 tu3, tv3;
    u16 p1;
    u16 n0, v0;
    u16 v1, v2;
    u16 v3, p2;
} TMD_P_TF4;
typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 tu3, tv3;
    u16 p1;
    u16 n0, v0;
    u16 n1, v1;
    u16 n2, v2;
    u16 n3, v3;
} TMD_P_TG4;
typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 tu3, tv3;
    u16 p1;
    u8 r0, g0, b0, p2;
    u16 v0, v1;
    u16 v2, v3;
} TMD_P_TNF4;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
    s16 x3, y3;
    u32 pad;
} LINE_F4;

/* the GPU packet being built: one of these, or a shattered wireframe */
typedef union {
    PolyF3 f3;
    PolyF4 f4;
    PolyG3 g3;
    PolyG4 g4;
    POLY_FT3 ft3;
    POLY_FT4 ft4;
    POLY_GT3 gt3;
    POLY_GT4 gt4;
    LINE_F4 line;
} EvoPacket;

extern MATRIX D_801DBE40;
extern s16 D_801EF860;
extern s16 D_801EF862;
extern s8 D_801EF864[16];
void MulMatrix0(MATRIX *m0, MATRIX *m1, MATRIX *m2);
void SetLightMatrix(MATRIX *m);
s32 RotNclip3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, s32 *sxy0, s32 *sxy1, s32 *sxy2, s32 *p, s32 *otz, s32 *flag);
s32 RotNclip4(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, s32 *sxy0, s32 *sxy1, s32 *sxy2, s32 *sxy3, s32 *p,
              s32 *otz, s32 *flag);
void NormalColorCol3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, Color *in, Color *out0, Color *out1, Color *out2);
void func_8005FBE4(SVECTOR *n, Color *in, Color *out);

/* Wireframe of a quad that is shattering: a closed 4-point polyline v0 v1 v3 v2 */
#define SHATTER_QUAD(pk, shade, otz)                        \
    {                                                       \
        LINE_F2 *line;                                      \
        setlen(&(pk)->line, 6);                             \
        setcode(&(pk)->line, 0x4C);                         \
        (pk)->line.pad = 0x55555555;                        \
        (pk)->line.r0 = (pk)->line.g0 = (pk)->line.b0 = shade; \
        otz >>= 2;                                          \
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &(pk)->line); \
        line = (LINE_F2 *)(&(pk)->line + 1);                \
        setlen(line, 3);                                    \
        setcode(line, 0x40);                                \
        line->r0 = line->g0 = line->b0 = shade;             \
        line->x0 = (pk)->line.x3;                           \
        line->y0 = (pk)->line.y3;                           \
        line->x1 = (pk)->line.x0;                           \
        line->y1 = (pk)->line.y0;                           \
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], line);      \
        pk = (EvoPacket *)(line + 1);                       \
    }

/* Wireframe of a triangle that is shattering: a closed polyline v0 v1 v2 v0 */
#define SHATTER_TRI(pk, shade, otz)                           \
    {                                                         \
        (pk)->line.x3 = (pk)->line.x0;                        \
        (pk)->line.y3 = (pk)->line.y0;                        \
        setlen(&(pk)->line, 6);                               \
        setcode(&(pk)->line, 0x4C);                           \
        (pk)->line.pad = 0x55555555;                          \
        (pk)->line.r0 = (pk)->line.g0 = (pk)->line.b0 = shade; \
        otz >>= 2;                                            \
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &(pk)->line); \
        pk = (EvoPacket *)(&(pk)->line + 1);                  \
    }

void func_801E0AFC(GsDOBJ4 *obj, s32 mode) {
    Color rgb;
    MATRIX m;
    s32 p;
    s32 otz;
    s32 flag;
    s32 i;
    s32 nprim;
    SVECTOR *norm;
    SVECTOR *vert;
    TmdPrim *prim;
    EvoPacket *pk;
    TmdObject *tmd;
    GsCOORDINATE2 *coord;
    s8 shade;
    union {
        TMD_P_F3 *f3;
        TMD_P_G3 *g3;
        TMD_P_F4 *f4;
        TMD_P_G4 *g4;
        TMD_P_TF3 *tf3;
        TMD_P_TG3 *tg3;
        TMD_P_TNF3 *tnf3;
        TMD_P_TF4 *tf4;
        TMD_P_TG4 *tg4;
        TMD_P_TNF4 *tnf4;
    } op;
    s32 code;

    shade = D_801EF860 + rand() % (D_801EF862 - D_801EF860);
    tmd = (TmdObject *)obj->tmd;
    vert = tmd->vertTop;
    norm = tmd->normTop;
    nprim = tmd->nprim;
    prim = tmd->prims;
    pk = (EvoPacket *)func_80062C44();
    rgb.r = rgb.g = rgb.b = 0x80;
    coord = obj->coord2;
    if (coord->flg == 0) {
        coord->flg = 1;
        MulMatrix0(&coord->coord, &coord->super->workm, &coord->workm);
    }
    MulMatrix0(&D_801DBE40, &coord->workm, &m);
    SetLightMatrix(&m);
    CompMatrix(&D_801DBEA0, &coord->workm, &m);
    func_801E00A4(&m);
    for (i = 0; i < nprim; i++) {
        if ((code = prim->mode & 0x3D) == 0x2C) {
            op.tf4 = (TMD_P_TF4 *)prim;
            if (D_801EF864[i & 0xF] < mode) {
                if (RotNclip4(&vert[op.tf4->v0], &vert[op.tf4->v1], &vert[op.tf4->v2], &vert[op.tf4->v3],
                              (s32 *)&pk->ft4.x0, (s32 *)&pk->ft4.x1, (s32 *)&pk->ft4.x2, (s32 *)&pk->ft4.x3, &p, &otz,
                              &flag) > 0) {
                    func_8005FBE4(&norm[op.tf4->n0], &rgb, (Color *)&pk->ft4.r0);
                    pk->ft4.u0 = op.tf4->tu0;
                    pk->ft4.v0 = op.tf4->tv0;
                    pk->ft4.u1 = op.tf4->tu1;
                    pk->ft4.v1 = op.tf4->tv1;
                    pk->ft4.u2 = op.tf4->tu2;
                    pk->ft4.v2 = op.tf4->tv2;
                    pk->ft4.u3 = op.tf4->tu3;
                    pk->ft4.v3 = op.tf4->tv3;
                    pk->ft4.clut = op.tf4->clut;
                    pk->ft4.tpage = op.tf4->tpage;
                    setlen(&pk->ft4, 9);
                    setcode(&pk->ft4, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->ft4);
                    pk = (EvoPacket *)(&pk->ft4 + 1);
                }
                prim = (TmdPrim *)(op.tf4 + 1);
            } else {
                if (RotNclip4(&vert[op.tf4->v0], &vert[op.tf4->v1], &vert[op.tf4->v2], &vert[op.tf4->v3],
                              (s32 *)&pk->line.x0, (s32 *)&pk->line.x1, (s32 *)&pk->line.x3, (s32 *)&pk->line.x2, &p,
                              &otz, &flag) > 0) {
                    SHATTER_QUAD(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tf4 + 1);
            }
        } else if ((code = prim->mode & 0x3D) == 0x3C) {
            op.tg4 = (TMD_P_TG4 *)prim;
            if (D_801EF864[i & 0xF] < mode) {
                if (RotNclip4(&vert[op.tg4->v0], &vert[op.tg4->v1], &vert[op.tg4->v2], &vert[op.tg4->v3],
                              (s32 *)&pk->gt4.x0, (s32 *)&pk->gt4.x1, (s32 *)&pk->gt4.x2, (s32 *)&pk->gt4.x3, &p, &otz,
                              &flag) > 0) {
                    NormalColorCol3(&norm[op.tg4->n0], &norm[op.tg4->n1], &norm[op.tg4->n2], &rgb, (Color *)&pk->gt4.r0,
                                    (Color *)&pk->gt4.r1, (Color *)&pk->gt4.r2);
                    func_8005FBE4(&norm[op.tg4->n3], &rgb, (Color *)&pk->gt4.r3);
                    pk->gt4.u0 = op.tg4->tu0;
                    pk->gt4.v0 = op.tg4->tv0;
                    pk->gt4.u1 = op.tg4->tu1;
                    pk->gt4.v1 = op.tg4->tv1;
                    pk->gt4.u2 = op.tg4->tu2;
                    pk->gt4.v2 = op.tg4->tv2;
                    pk->gt4.u3 = op.tg4->tu3;
                    pk->gt4.v3 = op.tg4->tv3;
                    pk->gt4.clut = op.tg4->clut;
                    pk->gt4.tpage = op.tg4->tpage;
                    setlen(&pk->gt4, 12);
                    setcode(&pk->gt4, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->gt4);
                    pk = (EvoPacket *)(&pk->gt4 + 1);
                }
                prim = (TmdPrim *)(op.tg4 + 1);
            } else {
                if (RotNclip4(&vert[op.tg4->v0], &vert[op.tg4->v1], &vert[op.tg4->v2], &vert[op.tg4->v3],
                              (s32 *)&pk->line.x0, (s32 *)&pk->line.x1, (s32 *)&pk->line.x3, (s32 *)&pk->line.x2, &p,
                              &otz, &flag) > 0) {
                    SHATTER_QUAD(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tg4 + 1);
            }
        } else if ((code = prim->mode & 0x35) == 0x34) {
            op.tg3 = (TMD_P_TG3 *)prim;
            if (D_801EF864[i & 0xF] < mode) {
                if (RotNclip3(&vert[op.tg3->v0], &vert[op.tg3->v1], &vert[op.tg3->v2], (s32 *)&pk->gt3.x0,
                              (s32 *)&pk->gt3.x1, (s32 *)&pk->gt3.x2, &p, &otz, &flag) > 0) {
                    NormalColorCol3(&norm[op.tg3->n0], &norm[op.tg3->n1], &norm[op.tg3->n2], &rgb, (Color *)&pk->gt3.r0,
                                    (Color *)&pk->gt3.r1, (Color *)&pk->gt3.r2);
                    pk->gt3.u0 = op.tg3->tu0;
                    pk->gt3.v0 = op.tg3->tv0;
                    pk->gt3.u1 = op.tg3->tu1;
                    pk->gt3.v1 = op.tg3->tv1;
                    pk->gt3.u2 = op.tg3->tu2;
                    pk->gt3.v2 = op.tg3->tv2;
                    pk->gt3.clut = op.tg3->clut;
                    pk->gt3.tpage = op.tg3->tpage;
                    setlen(&pk->gt3, 9);
                    setcode(&pk->gt3, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->gt3);
                    pk = (EvoPacket *)(&pk->gt3 + 1);
                }
                prim = (TmdPrim *)(op.tg3 + 1);
            } else {
                if (RotNclip3(&vert[op.tg3->v0], &vert[op.tg3->v1], &vert[op.tg3->v2], (s32 *)&pk->line.x0,
                              (s32 *)&pk->line.x1, (s32 *)&pk->line.x2, &p, &otz, &flag) > 0) {
                    SHATTER_TRI(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tg3 + 1);
            }
        } else if ((code = prim->mode & 0x35) == 0x24) {
            op.tf3 = (TMD_P_TF3 *)prim;
            if (D_801EF864[i & 0xF] < mode) {
                if (RotNclip3(&vert[op.tf3->v0], &vert[op.tf3->v1], &vert[op.tf3->v2], (s32 *)&pk->ft3.x0,
                              (s32 *)&pk->ft3.x1, (s32 *)&pk->ft3.x2, &p, &otz, &flag) > 0) {
                    func_8005FBE4(&norm[op.tf3->n0], &rgb, (Color *)&pk->ft3.r0);
                    pk->ft3.u0 = op.tf3->tu0;
                    pk->ft3.v0 = op.tf3->tv0;
                    pk->ft3.u1 = op.tf3->tu1;
                    pk->ft3.v1 = op.tf3->tv1;
                    pk->ft3.u2 = op.tf3->tu2;
                    pk->ft3.v2 = op.tf3->tv2;
                    pk->ft3.clut = op.tf3->clut;
                    pk->ft3.tpage = op.tf3->tpage;
                    setlen(&pk->ft3, 7);
                    setcode(&pk->ft3, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->ft3);
                    pk = (EvoPacket *)(&pk->ft3 + 1);
                }
                prim = (TmdPrim *)(op.tf3 + 1);
            } else {
                if (RotNclip3(&vert[op.tf3->v0], &vert[op.tf3->v1], &vert[op.tf3->v2], (s32 *)&pk->line.x0,
                              (s32 *)&pk->line.x1, (s32 *)&pk->line.x2, &p, &otz, &flag) > 0) {
                    SHATTER_TRI(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tf3 + 1);
            }
        } else if ((prim->mode & 0x3D) == 0x2D) {
            op.tnf4 = (TMD_P_TNF4 *)prim;
            if (D_801EF864[i & 0xF] < mode) {
                if (RotNclip4(&vert[op.tnf4->v0], &vert[op.tnf4->v1], &vert[op.tnf4->v2], &vert[op.tnf4->v3],
                              (s32 *)&pk->ft4.x0, (s32 *)&pk->ft4.x1, (s32 *)&pk->ft4.x2, (s32 *)&pk->ft4.x3, &p, &otz,
                              &flag) > 0) {
                    pk->ft4.u0 = op.tnf4->tu0;
                    pk->ft4.v0 = op.tnf4->tv0;
                    pk->ft4.u1 = op.tnf4->tu1;
                    pk->ft4.v1 = op.tnf4->tv1;
                    pk->ft4.u2 = op.tnf4->tu2;
                    pk->ft4.v2 = op.tnf4->tv2;
                    pk->ft4.u3 = op.tnf4->tu3;
                    pk->ft4.v3 = op.tnf4->tv3;
                    pk->ft4.clut = op.tnf4->clut;
                    pk->ft4.tpage = op.tnf4->tpage;
                    setlen(&pk->ft4, 9);
                    setcode(&pk->ft4, 0x2C);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->ft4);
                    pk = (EvoPacket *)(&pk->ft4 + 1);
                }
                prim = (TmdPrim *)(op.tnf4 + 1);
            } else {
                if (RotNclip4(&vert[op.tnf4->v0], &vert[op.tnf4->v1], &vert[op.tnf4->v2], &vert[op.tnf4->v3],
                              (s32 *)&pk->line.x0, (s32 *)&pk->line.x1, (s32 *)&pk->line.x3, (s32 *)&pk->line.x2, &p,
                              &otz, &flag) > 0) {
                    SHATTER_QUAD(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tnf4 + 1);
            }
        } else if ((code = prim->mode & 0x3D) == 0x28) {
            op.f4 = (TMD_P_F4 *)prim;
            if (D_801EF864[i & 0xF] < mode) {
                if (RotNclip4(&vert[op.f4->v0], &vert[op.f4->v1], &vert[op.f4->v2], &vert[op.f4->v3], (s32 *)&pk->f4.x0,
                              (s32 *)&pk->f4.x1, (s32 *)&pk->f4.x2, (s32 *)&pk->f4.x3, &p, &otz, &flag) > 0) {
                    func_8005FBE4(&norm[op.f4->n0], &rgb, (Color *)&pk->f4.r0);
                    setlen(&pk->f4, 5);
                    setcode(&pk->f4, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->f4);
                    pk = (EvoPacket *)(&pk->f4 + 1);
                }
                prim = (TmdPrim *)(op.f4 + 1);
            } else {
                if (RotNclip4(&vert[op.f4->v0], &vert[op.f4->v1], &vert[op.f4->v2], &vert[op.f4->v3],
                              (s32 *)&pk->line.x0, (s32 *)&pk->line.x1, (s32 *)&pk->line.x3, (s32 *)&pk->line.x2, &p,
                              &otz, &flag) > 0) {
                    SHATTER_QUAD(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.f4 + 1);
            }
        } else if ((prim->mode & 0x35) == 0x25) {
            op.tnf3 = (TMD_P_TNF3 *)prim;
            if (D_801EF864[i & 0xF] < mode) {
                if (RotNclip3(&vert[op.tnf3->v0], &vert[op.tnf3->v1], &vert[op.tnf3->v2], (s32 *)&pk->ft3.x0,
                              (s32 *)&pk->ft3.x1, (s32 *)&pk->ft3.x2, &p, &otz, &flag) > 0) {
                    pk->ft3.u0 = op.tnf3->tu0;
                    pk->ft3.v0 = op.tnf3->tv0;
                    pk->ft3.u1 = op.tnf3->tu1;
                    pk->ft3.v1 = op.tnf3->tv1;
                    pk->ft3.u2 = op.tnf3->tu2;
                    pk->ft3.v2 = op.tnf3->tv2;
                    pk->ft3.clut = op.tnf3->clut;
                    pk->ft3.tpage = op.tnf3->tpage;
                    setlen(&pk->ft3, 7);
                    setcode(&pk->ft3, 0x24);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->ft3);
                    pk = (EvoPacket *)(&pk->ft3 + 1);
                }
                prim = (TmdPrim *)(op.tnf3 + 1);
            } else {
                if (RotNclip3(&vert[op.tnf3->v0], &vert[op.tnf3->v1], &vert[op.tnf3->v2], (s32 *)&pk->line.x0,
                              (s32 *)&pk->line.x1, (s32 *)&pk->line.x2, &p, &otz, &flag) > 0) {
                    SHATTER_TRI(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tnf3 + 1);
            }
        } else if ((code = prim->mode & 0x3D) == 0x38) {
            op.g4 = (TMD_P_G4 *)prim;
            if (D_801EF864[i & 0xF] < mode) {
                if (RotNclip4(&vert[op.g4->v0], &vert[op.g4->v1], &vert[op.g4->v2], &vert[op.g4->v3], (s32 *)&pk->g4.x0,
                              (s32 *)&pk->g4.x1, (s32 *)&pk->g4.x2, (s32 *)&pk->g4.x3, &p, &otz, &flag) > 0) {
                    NormalColorCol3(&norm[op.g4->n0], &norm[op.g4->n1], &norm[op.g4->n2], &rgb, (Color *)&pk->g4.r0,
                                    (Color *)&pk->g4.r1, (Color *)&pk->g4.r2);
                    func_8005FBE4(&norm[op.g4->n3], &rgb, (Color *)&pk->g4.r3);
                    setlen(&pk->g4, 8);
                    setcode(&pk->g4, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->g4);
                    pk = (EvoPacket *)(&pk->g4 + 1);
                }
                prim = (TmdPrim *)(op.g4 + 1);
            } else {
                if (RotNclip4(&vert[op.g4->v0], &vert[op.g4->v1], &vert[op.g4->v2], &vert[op.g4->v3],
                              (s32 *)&pk->line.x0, (s32 *)&pk->line.x1, (s32 *)&pk->line.x3, (s32 *)&pk->line.x2, &p,
                              &otz, &flag) > 0) {
                    SHATTER_QUAD(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.g4 + 1);
            }
        } else if ((code = prim->mode & 0x35) == 0x30) {
            op.g3 = (TMD_P_G3 *)prim;
            if (D_801EF864[i & 0xF] < mode) {
                if (RotNclip3(&vert[op.g3->v0], &vert[op.g3->v1], &vert[op.g3->v2], (s32 *)&pk->g3.x0,
                              (s32 *)&pk->g3.x1, (s32 *)&pk->g3.x2, &p, &otz, &flag) > 0) {
                    func_8005FBE4(&norm[op.g3->n0], &rgb, (Color *)&pk->g3.r0);
                    setlen(&pk->g3, 6);
                    setcode(&pk->g3, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->g3);
                    pk = (EvoPacket *)(&pk->g3 + 1);
                }
                prim = (TmdPrim *)(op.g3 + 1);
            } else {
                if (RotNclip3(&vert[op.g3->v0], &vert[op.g3->v1], &vert[op.g3->v2], (s32 *)&pk->line.x0,
                              (s32 *)&pk->line.x1, (s32 *)&pk->line.x2, &p, &otz, &flag) > 0) {
                    SHATTER_TRI(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.g3 + 1);
            }
        } else if ((code = prim->mode & 0x35) == 0x20) {
            op.f3 = (TMD_P_F3 *)prim;
            if (D_801EF864[i & 0xF] < mode) {
                if (RotNclip3(&vert[op.f3->v0], &vert[op.f3->v1], &vert[op.f3->v2], (s32 *)&pk->f3.x0,
                              (s32 *)&pk->f3.x1, (s32 *)&pk->f3.x2, &p, &otz, &flag) > 0) {
                    func_8005FBE4(&norm[op.f3->n0], &rgb, (Color *)&pk->f3.r0);
                    setlen(&pk->f3, 4);
                    setcode(&pk->f3, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->f3);
                    pk = (EvoPacket *)(&pk->f3 + 1);
                }
                prim = (TmdPrim *)(op.f3 + 1);
            } else {
                if (RotNclip3(&vert[op.f3->v0], &vert[op.f3->v1], &vert[op.f3->v2], (s32 *)&pk->line.x0,
                              (s32 *)&pk->line.x1, (s32 *)&pk->line.x2, &p, &otz, &flag) > 0) {
                    SHATTER_TRI(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.f3 + 1);
            }
        }
    }
    func_80062C34((long)pk);
}

void func_801E285C(void) {
    D_801DBFB0.f3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastF3L;
    D_801DBFB0.tf3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTF3L;
    D_801DBFB0.tf3[GsDivMODE_NDIV][GsLMODE_LOFF] = GsTMDfastTF3NL;
    D_801DBFB0.ntf3[GsDivMODE_NDIV] = GsTMDfastTNF3;
    D_801DBFB0.g3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastG3L;
    D_801DBFB0.tg3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTG3L;
    D_801DBFB0.tg3[GsDivMODE_NDIV][GsLMODE_LOFF] = GsTMDfastTG3NL;
    D_801DBFB0.ntg3[GsDivMODE_NDIV] = GsTMDfastTNG3;
    D_801DBFB0.f4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastF4L;
    D_801DBFB0.tf4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTF4L;
    D_801DBFB0.tf4[GsDivMODE_NDIV][GsLMODE_LOFF] = GsTMDfastTF4NL;
    D_801DBFB0.ntf4[GsDivMODE_NDIV] = GsTMDfastTNF4;
    D_801DBFB0.g4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastG4L;
    D_801DBFB0.tg4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTG4L;
    D_801DBFB0.tg4[GsDivMODE_NDIV][GsLMODE_LOFF] = GsTMDfastTG4NL;
    D_801DBFB0.ntg4[GsDivMODE_NDIV] = GsTMDfastTNG4;
    D_801DBFB0.tf4[GsDivMODE_DIV][GsLMODE_NORMAL] = GsTMDdivTF4L;
    D_801DBFB0.tf4[GsDivMODE_DIV][GsLMODE_LOFF] = GsTMDdivTF4NL;
    D_801DBFB0.tf3[GsDivMODE_DIV][GsLMODE_LOFF] = GsTMDdivTF3NL;
    D_801DBFB0.tg4[GsDivMODE_DIV][GsLMODE_LOFF] = GsTMDdivTG4NL;
    D_801DBFB0.tg3[GsDivMODE_DIV][GsLMODE_LOFF] = GsTMDdivTG3NL;
    D_801DBFB0.ntg4[GsDivMODE_DIV] = GsTMDdivTNG4;
    D_801DBFB0.ntg3[GsDivMODE_DIV] = GsTMDdivTNG3;
    D_801DBFB0.ntf4[GsDivMODE_DIV] = GsTMDdivTNF4;
    D_801DBFB0.ntf3[GsDivMODE_DIV] = GsTMDdivTNF3;
    D_801DBFB0.f4[GsDivMODE_NDIV][GsLMODE_LOFF] = GsTMDfastF4NL;
    D_801DBFB0.nf4[GsDivMODE_NDIV] = GsTMDfastNF4;
}

void func_801E29AC(GsCOORDINATE2 *coord, MATRIX *m) {
    GsCOORDINATE2 *chain[100];
    GsCOORDINATE2 **p;

    chain[0] = coord;
    p = &chain[1];
    while (coord->super != NULL) {
        coord = coord->super;
        *p++ = coord;
    }
    p--;
    *m = (*p)->coord;
    while (chain < p) {
        p--;
        GsMulCoord3(m, &(*p)->coord);
    }
}

void func_801E2AA8(s32 part, MATRIX *m) {
    GsCOORDINATE2 root;
    GsCOORDINATE2 node;
    VECTOR trans;

    trans = *(VECTOR *)((Model *)SCENE_3D->models[0])->bonepos[0];
    root = ((Model *)SCENE_3D->models[0])->coord[0];
    node = ((Model *)SCENE_3D->models[0])->coord[part];
    RotMatrix(&((Model *)SCENE_3D->models[0])->rots[0], &root.coord);
    ScaleMatrix(&root.coord, (VECTOR *)((Model *)SCENE_3D->models[0])->unk2000);
    TransMatrix(&root.coord, &trans);
    func_801E29AC(&node, m);
}

void func_801E2C0C(s32 model) {
    s32 i;
    s32 j;
    s16 tmp;
    u8 unused[0x50]; /* stack space the original reserves but never touches */

    D_801F0600.total = -1;
    D_801F0600.next = 0;
    D_801F0600.count = 0;
    D_801F0600.model = model;
    D_801F2BB8 = D_801F2BBC;
    for (i = 0; i < 60; i++) {
        D_801F05FC[i].timer = -1;
    }
    for (i = 0; i < 40; i++) {
        D_801EF874[i] = 0x10;
    }
    D_801F0600.total = ((EvoModel *)SCENE_3D->models[D_801F0600.model])->partCount;
    D_801F0600.queue = D_801F0550;
    for (i = 0; i < D_801F0600.total; i++) {
        D_801F05A0[i] = 1;
    }
    for (i = 0; i < D_801F0600.total; i++) {
        D_801F0550[i] = i;
    }
    for (i = 0; i < D_801F0600.total; i++) {
        j = rand() % D_801F0600.total;
        tmp = D_801F0550[i];
        D_801F0550[i] = D_801F0550[j];
        D_801F0550[j] = tmp;
    }
    if (D_801F05F8 == 0) {
        addFrameCallback((s32)func_801E2E30);
        D_801F05F8 = 1;
    }
    D_801F05F6 = 120;
}

void func_801E2E30(void) {
    s32 part;
    s32 i;

    if (D_801F0600.next < D_801F0600.total) {
        if (D_801F05F6 != 0) {
            D_801F05F6--;
        } else {
            D_801F05A0[D_801F0600.next]--;
        }
        if (D_801F05A0[D_801F0600.next] < 0) {
            part = *D_801F0600.queue++;
            D_801F0600.next++;
            func_801E47E4(part, part);
            D_801EF874[part] = 0;
            playSoundEffect(0x58);
        }
    }
    if (D_801F0600.next > D_801F0600.total - 1) {
        if (D_801F05F4 == 0) {
            D_801F05F4 = 1;
        } else if (D_801F05F4 == 3) {
            D_801F05F4 = 4;
        }
    }
    for (i = 0; i < D_801F0600.count; i++) {
        func_801E2FC8(i);
    }
}

void func_801E2FC8(s32 index) {
    EvoShard *shard;
    SVECTOR *offset;
    s32 count;

    shard = &D_801F05FC[index];
    offset = shard->offsets;
    count = shard->primCount;
    D_801F0540 = shard->prims;
    D_801F0538 = shard->verts;
    if (shard->timer < 120) {
        if (shard->timer >= 0) {
            shard->timer++;
            D_801F0530.r = (61 - shard->timer) * 74 / 60 + 54;
            D_801F0530.g = D_801F0530.r;
            D_801F0530.b = D_801F0530.r;
            while (count-- > 0) {
                if (D_801F0540[3] == 0x34 || D_801F0540[3] == 0x36) {
                    func_801E335C(offset, 0, 120, shard->timer);
                    offset++;
                    D_801F0540 += 0x1C;
                } else if (D_801F0540[3] == 0x24) {
                    func_801E3694(offset, 0, 120, shard->timer);
                    offset++;
                    D_801F0540 += 0x18;
                } else if (D_801F0540[3] == 0x3C || D_801F0540[3] == 0x3E) {
                    func_801E39CC(offset, 0, 120, shard->timer);
                    offset++;
                    D_801F0540 += 0x24;
                } else if (D_801F0540[3] == 0x2C) {
                    func_801E3D6C(offset, 0, 120, shard->timer);
                    offset++;
                    D_801F0540 += 0x20;
                } else if (D_801F0540[3] == 0x25) {
                    func_801E410C(offset, 0, 120, shard->timer);
                    offset++;
                    D_801F0540 += 0x1C;
                } else if (D_801F0540[3] == 0x2D || D_801F0540[3] == 0x2F) {
                    func_801E4444(offset, 0, 120, shard->timer);
                    offset++;
                    D_801F0540 += 0x20;
                } else if (D_801F0540[3] == 0x28) {
                    func_801E55FC(offset, 0, 120, shard->timer);
                    offset++;
                    D_801F0540 += 0x14;
                } else if (D_801F0540[3] == 0x30) {
                    func_801E5934(offset, 0, 120, shard->timer);
                    offset++;
                    D_801F0540 += 0x14;
                } else if (D_801F0540[3] == 0x38) {
                    func_801E5C1C(offset, 0, 120, shard->timer);
                    offset++;
                    D_801F0540 += 0x18;
                } else if (D_801F0540[3] == 0x20) {
                    func_801E5F54(offset, 0, 120, shard->timer);
                    offset++;
                    D_801F0540 += 0x10;
                }
            }
        }
    }
}

void func_801E335C(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT3 *prim;
    POLY_FT3 *poly;

    prim = (TmdPacketFT3 *)D_801F0540;
    poly = (POLY_FT3 *)func_80062C44();
    func_80067724(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &D_801F0538[prim->idx[1]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[3]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[5]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 7);
    setcode(poly, 0x24);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void func_801E3694(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT3 *prim;
    POLY_FT3 *poly;

    prim = (TmdPacketFT3 *)D_801F0540;
    poly = (POLY_FT3 *)func_80062C44();
    func_80067724(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &D_801F0538[prim->idx[1]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[2]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[3]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 7);
    setcode(poly, 0x24);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void func_801E39CC(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT4 *prim;
    POLY_FT4 *poly;

    prim = (TmdPacketFT4 *)D_801F0540;
    poly = (POLY_FT4 *)func_80062C44();
    func_800677A4(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    poly->u3 = prim->u3;
    poly->v3 = prim->v3;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &D_801F0538[prim->idx[1]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[3]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[5]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[7]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void func_801E3D6C(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT4 *prim;
    POLY_FT4 *poly;

    prim = (TmdPacketFT4 *)D_801F0540;
    poly = (POLY_FT4 *)func_80062C44();
    func_800677A4(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    poly->u3 = prim->u3;
    poly->v3 = prim->v3;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &D_801F0538[prim->idx[1]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[2]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[3]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[4]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void func_801E410C(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT3 *prim;
    POLY_FT3 *poly;

    prim = (TmdPacketFT3 *)D_801F0540;
    poly = (POLY_FT3 *)func_80062C44();
    func_80067724(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &D_801F0538[prim->idx[2]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[3]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[4]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 7);
    setcode(poly, 0x24);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void func_801E4444(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT4 *prim;
    POLY_FT4 *poly;

    prim = (TmdPacketFT4 *)D_801F0540;
    poly = (POLY_FT4 *)func_80062C44();
    func_800677A4(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    poly->u3 = prim->u3;
    poly->v3 = prim->v3;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &D_801F0538[prim->idx[2]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[3]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[4]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[5]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

s32 func_801E47E4(s32 part, s32 arg) {
    SVECTOR out;
    MATRIX rot;
    MATRIX world;
    SVECTOR n0;
    SVECTOR n1;
    SVECTOR n2;
    SVECTOR n3;
    SVECTOR center;
    MATRIX unused;
    TmdObject *obj;
    SVECTOR *vert;
    SVECTOR *verts;
    SVECTOR *a;
    SVECTOR *b;
    SVECTOR *c;
    SVECTOR *d;
    u8 *prim;
    EvoShard *shard;
    s32 slot;
    u32 i;
    u32 k;

    obj = ((EvoModel *)SCENE_3D->models[D_801F0600.model])->parts[part].tmd;
    if (obj == NULL) {
        return -1;
    }
    for (slot = 0; slot < 30 && D_801F05FC[slot].timer >= 0; slot++) {
    }
    if (slot == 30) {
        return -1;
    }
    PushMatrix();
    rot = ((EvoModel *)SCENE_3D->models[D_801F0600.model])->matrices[part];
    verts = D_801F2BB8;
    vert = obj->vertTop;
    world = ((EvoModel *)SCENE_3D->models[D_801F0600.model])->matrices[part];
    for (i = 0; i < obj->nvert; i++) {
        ApplyMatrixSV(&world, vert++, &out);
        D_801F2BB8->vx = out.vx + world.t[0];
        D_801F2BB8->vy = out.vy + world.t[1];
        D_801F2BB8->vz = out.vz + world.t[2];
        D_801F2BB8++;
    }
    shard = &D_801F05FC[slot];
    shard->timer = 0;
    shard->count = obj->nprim;
    shard->offsets = D_801F2BB8;
    shard->unk12 = arg;
    shard->verts = verts;
    shard->prims = (s8 *)obj->primTop;
    shard->primCount = obj->nprim;
    prim = obj->primTop;
    for (k = 0; k < obj->nprim; k++) {
        switch ((s8)(prim[3] - 0x20)) {
        case 0x14:
        case 0x16:
            a = &obj->normTop[*(u16 *)(prim + 0x10)];
            b = &obj->normTop[*(u16 *)(prim + 0x14)];
            c = &obj->normTop[*(u16 *)(prim + 0x18)];
            ApplyMatrixSV(&rot, a, &n0);
            ApplyMatrixSV(&rot, b, &n1);
            ApplyMatrixSV(&rot, c, &n2);
            center.vx = (n0.vx + n1.vx + n2.vx) / 3;
            center.vy = (n0.vy + n1.vy + n2.vy) / 3;
            center.vz = (n0.vz + n1.vz + n2.vz) / 3;
            D_801F2BB8->vx = center.vx;
            D_801F2BB8->vy = center.vy;
            D_801F2BB8->vz = center.vz;
            D_801F2BB8++;
            prim += 0x1C;
            break;
        case 0x1C:
        case 0x1E:
            a = &obj->normTop[*(u16 *)(prim + 0x14)];
            b = &obj->normTop[*(u16 *)(prim + 0x18)];
            c = &obj->normTop[*(u16 *)(prim + 0x1C)];
            d = &obj->normTop[*(u16 *)(prim + 0x20)];
            ApplyMatrixSV(&rot, a, &n0);
            ApplyMatrixSV(&rot, b, &n1);
            ApplyMatrixSV(&rot, c, &n2);
            ApplyMatrixSV(&rot, d, &n3);
            center.vx = (n0.vx + n1.vx + n2.vx) / 3;
            center.vy = (n0.vy + n1.vy + n2.vy) / 3;
            center.vz = (n0.vz + n1.vz + n2.vz) / 3;
            D_801F2BB8->vx = center.vx;
            D_801F2BB8->vy = center.vy;
            D_801F2BB8->vz = center.vz;
            D_801F2BB8++;
            prim += 0x24;
            break;
        case 0x0C:
            ApplyMatrixSV(&rot, &obj->normTop[*(u16 *)(prim + 0x14)], &n0);
            center = n0;
            D_801F2BB8->vx = center.vx;
            D_801F2BB8->vy = center.vy;
            D_801F2BB8->vz = center.vz;
            D_801F2BB8++;
            prim += 0x20;
            break;
        case 0x04:
            ApplyMatrixSV(&rot, &obj->normTop[*(u16 *)(prim + 0x10)], &n0);
            center = n0;
            D_801F2BB8->vx = center.vx;
            D_801F2BB8->vy = center.vy;
            D_801F2BB8->vz = center.vz;
            D_801F2BB8++;
            prim += 0x18;
            break;
        case 0x0D:
        case 0x0F:
            ApplyMatrixSV(&rot, &obj->normTop[1], &n0);
            center = n0;
            D_801F2BB8->vx = center.vx;
            D_801F2BB8->vy = center.vy;
            D_801F2BB8->vz = center.vz;
            D_801F2BB8++;
            prim += 0x20;
            break;
        case 0x05:
            ApplyMatrixSV(&rot, &obj->normTop[1], &n0);
            center = n0;
            D_801F2BB8->vx = center.vx;
            D_801F2BB8->vy = center.vy;
            D_801F2BB8->vz = center.vz;
            D_801F2BB8++;
            prim += 0x1C;
            break;
        case 0x08:
        case 0x10:
            ApplyMatrixSV(&rot, &obj->normTop[*(u16 *)(prim + 0x8)], &n0);
            center = n0;
            D_801F2BB8->vx = center.vx;
            D_801F2BB8->vy = center.vy;
            D_801F2BB8->vz = center.vz;
            D_801F2BB8++;
            prim += 0x14;
            break;
        case 0x18:
            ApplyMatrixSV(&rot, &obj->normTop[*(u16 *)(prim + 0x8)], &n0);
            center = n0;
            D_801F2BB8->vx = center.vx;
            D_801F2BB8->vy = center.vy;
            D_801F2BB8->vz = center.vz;
            D_801F2BB8++;
            prim += 0x18;
            break;
        case 0x00:
            ApplyMatrixSV(&rot, &obj->normTop[*(u16 *)(prim + 0x8)], &n0);
            center = n0;
            D_801F2BB8->vx = center.vx;
            D_801F2BB8->vy = center.vy;
            D_801F2BB8->vz = center.vz;
            D_801F2BB8++;
            prim += 0x10;
            break;
        }
    }
    PopMatrix();
    D_801F0600.count = slot + 1;
    return slot;
}

s32 func_801E5024(s32 min, s32 max) {
    s32 tmp;

    if (max == min) {
        return min;
    }
    if (max < min) {
        tmp = min;
        min = max;
        max = tmp;
    }
    D_801F2BB0 = D_801F2BB0 * 0x41C650AD + 0x3039;
    D_801EF85C = D_801EF85C * 0x41C650AD + 0x3039;
    return min + ((D_801F2BB0 >> 16) | (D_801EF85C << 16)) % (max - min + 1);
}

void func_801E50BC(void) {
    EvoSpark *spark;
    s32 i;

    for (i = 0; i < 16; i++) {
        spark = &D_801F02F0[i];
        spark->joint = -1;
    }
}

s32 func_801E50F8(s16 joint, s16 timer) {
    EvoSpark *spark;
    EvoSpark *slot;
    s32 i;

    for (i = 0; i < 16; i++) {
        slot = &D_801F02F0[i];
        if (slot->joint == -1) {
            break;
        }
    }
    if (i == 16) {
        return -1;
    }
    spark = &D_801F02F0[i];
    spark->joint = joint;
    spark->timer = timer;
    func_800149B8(0, -1, 0, 0x1000, func_801E51E4, spark, getCurrentTaskId(), 0, 0);
    return i;
}

void func_801E51E4(EvoSpark *spark) {
    do {
        func_80014C08(1);
        func_801E5244(spark);
    } while (--spark->timer >= 0);
    func_80014C08(10);
    spark->joint = -1;
    func_80014A90();
}

void func_801E5244(EvoSpark *spark) {
    MATRIX mat;
    SVECTOR from;
    SVECTOR to;
    SVECTOR tip;
    s32 depthCue;
    s32 flag;
    s32 dist;
    s32 len;
    PolyF3 *poly;
    s32 shade;

    PushMatrix();
    mat = ((EvoModel *)SCENE_3D->models[0])->matrices[spark->joint];
    from.vx = mat.t[0];
    from.vy = mat.t[1];
    from.vz = mat.t[2];
    mat = ((EvoModel *)SCENE_3D->models[0])->matrices[1];
    to.vx = from.vx - mat.t[0];
    to.vy = from.vy - mat.t[1];
    to.vz = from.vz - mat.t[2];
    from.vx = mat.t[0];
    from.vy = mat.t[1];
    from.vz = mat.t[2];
    dist = to.vx * to.vx + to.vy * to.vy + to.vz * to.vz;
    len = func_801E5024(400, 500);
    len *= len;
    if (dist == 0) {
        dist = 1;
    }
    to.vx = from.vx + to.vx * len / dist;
    to.vy = from.vy + to.vy * len / dist;
    to.vz = from.vz + to.vz * len / dist;
    tip.vx = to.vx + func_801E5024(-80, 80);
    tip.vy = to.vy + func_801E5024(-80, 80);
    tip.vz = to.vz + func_801E5024(-80, 80);
    poly = (PolyF3 *)func_80062C44();
    func_80067704(poly);
    SetSemiTrans(poly, 1);
    shade = rand() % 128 + 10;
    poly->r0 = shade;
    poly->g0 = shade;
    poly->b0 = shade;
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    if ((u32)((RotTransPers3((s32)&tip, (s32)&from, (s32)&to, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag) >> 2) - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        AddPrim((s32 *)CURRENT_FRAME_BUFFER->ot, (s32)poly);
        func_80062C34((long)(poly + 1));
    }
    PopMatrix();
}

void func_801E55FC(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    u16 *prim;
    PolyF4 *poly;

    prim = (u16 *)D_801F0540;
    poly = (PolyF4 *)func_80062C44();
    setlen(poly, 5);
    setcode(poly, 0x28);
    SetSemiTrans(poly, 1);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &D_801F0538[prim[5]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[6]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[7]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[8]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 5);
    setcode(poly, 0x28);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void func_801E5934(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    u16 *prim;
    PolyG3 *poly;

    prim = (u16 *)D_801F0540;
    poly = (PolyG3 *)func_80062C44();
    setlen(poly, 6);
    setcode(poly, 0x30);
    SetSemiTrans(poly, 1);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &D_801F0538[prim[5]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[7]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[9]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 6);
    setcode(poly, 0x30);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void func_801E5C1C(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    u16 *prim;
    PolyG4 *poly;

    prim = (u16 *)D_801F0540;
    poly = (PolyG4 *)func_80062C44();
    setlen(poly, 8);
    setcode(poly, 0x38);
    SetSemiTrans(poly, 1);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &D_801F0538[prim[5]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[7]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[9]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[11]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 8);
    setcode(poly, 0x38);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void func_801E5F54(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    u16 *prim;
    PolyF3 *poly;

    prim = (u16 *)D_801F0540;
    poly = (PolyF3 *)func_80062C44();
    setlen(poly, 4);
    setcode(poly, 0x20);
    SetSemiTrans(poly, 1);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &D_801F0538[prim[5]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[6]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[7]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 4);
    setcode(poly, 0x20);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DE084);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF03C);

void func_801E623C(void) {
    char path[24];
    u32 *pack;

    sprintf(path, "C:\\OBJECT\\unit.TIS", (s8)((SessionData *)D_8006E054)->unk100C->unk1A4);
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    pack = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
}

const char D_801DF0A0[] = "Number of Cards that can be Fused.";
const char D_801DF0C4[] = "Level *e5";
const char D_801DF0D0[] = "Level *e4";
const char D_801DF0DC[] = "Level *e3";
const char D_801DF0E8[] = "*a6 Option";
const char D_801DF0F4[] = "*a5 Option";
const char D_801DF100[] = "*a4 Rare";
const char D_801DF10C[] = "*a3 Darkness";
const char D_801DF11C[] = "*a2 Nature";
const char D_801DF128[] = "*a1 Ice";
const char D_801DF130[] = "*a0 Fire";

void func_801E62D0(void) {
    Graphics *camera;

    initScene3D(1);
    func_800149B8(0x19, -1, 0, 0x800, &runSceneCameraTask, 0);
    func_80014A00(0x1B);
    func_800149B8(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
    func_80014C08(2);
    camera = (Graphics *)&GRAPHICS;
    camera->rotX = 0;
    camera->rotY = 0;
    camera->rotZ = 0;
    camera->posX = 0;
    camera->posY = 0;
    camera->posZ = 0;
    camera->unk8E = 0;
    camera->unk90 = 20;
    camera->unk92 = 0;
    camera->unk94 = 0;
    camera->targetModel = -1;
    camera->snapCamera = 1;
    func_80014C08(2);
}

void func_801E6398(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 0x12D; i++) {
        D_801F4E34[i] = getOwnedCardCount(0, i);
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 0x12D; j++) {
            D_801F4E40[i][j] = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].inUse != 0) {
            for (j = 0; j < 30; j++) {
                D_801F4E40[i][getCardId(((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].cards[j].type,
                                        ((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].cards[j].index)]++;
            }
        }
    }
    for (i = 0; i < 0x12D; i++) {
        for (j = 1; j < 3; j++) {
            if (D_801F4E40[0][i] < D_801F4E40[j][i]) {
                D_801F4E40[0][i] = D_801F4E40[j][i];
            }
        }
    }
    for (i = 0; i < 0x12D; i++) {
        D_801F4E34[i] -= D_801F4E40[0][i];
    }
}

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF13C);

void func_801E65A8(void) {
    s32 i;
    s32 j;

    for (i = 0, j = 0; j < 0xBF; j++, i++) {
        D_801F44C0[i] = (EvoCardInfo *)(DIGIMON_CARDS + j * 0x13C);
        D_801F4980[i] = D_801F44C0[i];
    }
    for (j = 0; j < 0x66; j++, i++) {
        D_801F44C0[i] = (EvoCardInfo *)(OPTION_CARDS + j * 0xE2);
        D_801F4980[i] = D_801F44C0[i];
    }
    for (j = 0; j < 8; j++, i++) {
        D_801F44C0[i] = (EvoCardInfo *)(DIGIVOLVE_CARDS + j * 0x70);
        D_801F4980[i] = D_801F44C0[i];
    }
    for (j = 0; j < 3; j++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->partners[j].cardId != 0) {
            D_801F44C0[((PlayerProfile *)PLAYER_PROFILES)->partners[j].cardId] =
                (EvoCardInfo *)&((PlayerProfile *)PLAYER_PROFILES)->partners[j].card[0];
        }
    }
}

s32 func_801E6718(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 0) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 0) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 func_801E67F4(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 1) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 1) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 func_801E68D0(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 2) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 2) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 func_801E69B0(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 3) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 3) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 func_801E6A90(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 4) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 4) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 func_801E6B70(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka = (*a)->type == 1;
    s32 kb = (*b)->type == 1;

    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 func_801E6C04(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka = (*a)->type == 2;
    s32 kb = (*b)->type == 2;

    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 func_801E6C98(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr & 0xF;
    }
    if (ka == 0) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 0) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 func_801E6D74(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr & 0xF;
    }
    if (ka == 2) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 2) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 func_801E6E54(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr & 0xF;
    }
    if (ka == 3) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 3) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 func_801E6F34(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka = D_801F4E34[(*a)->id];
    s32 kb = D_801F4E34[(*b)->id];

    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

void func_801E6FCC(UiWindow *w) {
    s32 x = w->originX;
    s32 z = w->z;
    s32 i;
    u8 unused[0x48]; /* stack space the original reserves but never touches */

    for (i = 0; i < D_801F005C.nrows; i++) {
        if (i < w->view.y / D_801F005C.rowH) {
            continue;
        }
        if ((w->view.y + w->rect.h) / D_801F005C.rowH < i) {
            break;
        }
        drawText(x, w->originY + i * D_801F005C.rowH + 1, (s32)D_801F0000[i], 7, z);
    }
    updateMenuCursor(&D_801F005C);
    if (D_801F005C.active != 0 && (PAD_STATES[0]->pressed & 0x40)) {
        playMenuSound(1);
        D_801F0030.row = 0;
        centerMenuOnCursor(&D_801F0030);
        if (D_801F0088[D_801F005C.row] != NULL) {
            sortArray((s8 *)D_801F44C0, 0x12D, 4, D_801F0088[D_801F005C.row]);
        } else {
            func_801E65A8();
        }
    }
}

void func_801E7178(UiWindow *w) {
    char text[72];
    u8 *color;
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 type;
    s32 palette;

    x = w->originX;
    z = w->z;
    x -= 10;
    if (D_801F5380.unkE == 0) {
        w->brightness = 0x80;
    } else {
        w->brightness = 0x40;
    }
    for (i = 0; i < D_801F0030.nrows; i++) {
        if (i < w->view.y / D_801F0030.rowH) {
            continue;
        }
        if ((w->view.y + w->rect.h) / D_801F0030.rowH < i) {
            break;
        }
        y = w->originY + i * D_801F0030.rowH + 1;
        type = D_801F44C0[i]->type;
        color = D_801EFF58[0].b;
        palette = 7;
        if (D_801F44C0[i]->unk18 == 0) {
            palette = 3;
        }
        if (type == 0 && (D_801F44C0[i]->attr & 0xF) > D_801F5358) {
            palette = 3;
        }
        if (((PlayerProfile *)PLAYER_PROFILES)->cardCollection[D_801F44C0[i]->id] & 0x40) {
            if (D_801F4E34[D_801F44C0[i]->id] == 0) {
                color = D_801EFF60.b;
            }
            drawTextColored(x + 0x3C, y, D_801F44C0[i]->name, color, palette, z);
            switch (type) {
            case 0:
                drawIconColored(x + 0x20, y, 0, D_801F44C0[i]->attr >> 4, color, z);
                if (palette == 3) {
                    drawIconColored(x + 0x2C, y, 0, (D_801F44C0[i]->attr & 0xF) + 0x10, D_801EFF64.b, z);
                } else {
                    drawIconColored(x + 0x2C, y, 0, (D_801F44C0[i]->attr & 0xF) + 0x10, color, z);
                }
                break;
            case 1:
                drawIconColored(x + 0x20, y, 0, 5, color, z);
                break;
            case 2:
                drawIconColored(x + 0x20, y, 0, 6, color, z);
                break;
            }
        } else {
            palette = 9;
            drawTextColored(x + 0x20, y, "??", color, palette, z);
            drawTextColored(x + 0x3C, y, "------------------", color, palette, z);
        }
        sprintf(text, D_801DF15C, D_801F44C0[i]->id);
        drawTextColored(x + 0xA, y, text, color, palette, z);
        sprintf(text, D_801DF168, D_801F4E34[D_801F44C0[i]->id]);
        drawTextColored(x + 0xB5, y, text, color, palette, z);
        drawTinyTextColored(x + 0xBD, y + 6, D_801DF16C, palette, color, z);
    }
    updateMenuCursor(&D_801F0030);
}

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF15C);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF168);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF16C);

void func_801E75B0(EvoWindow *w) {
    s32 x = w->win.originX;
    s32 y = w->win.originY;
    s32 z = w->win.z;
    u8 unused[0x48]; /* stack space the original reserves but never touches */

    if (w->unk44 == 0) {
        drawTextColored(x + 0x4B, y, "Card Fusion", D_801EFF58[D_801F53B3].b, 6, z);
    } else {
        drawTextColored(x + 0x41, y, "Partner Fusion", D_801EFF58[(s8)(D_801F53B3 ^ 1)].b, 6, z);
    }
}

void func_801E765C(EvoWindow *w) {
    s32 x = w->win.originX;
    s32 y = w->win.originY;
    s32 z = w->win.z;
    u8 unused[0x48]; /* stack space the original reserves but never touches */

    if (w->unk44 == 0) {
        x += 2;
        drawTextColored(x, y, "*w1Create a New Card", D_801EFF58[D_801F5380.side].b, 8, z);
        drawTextColored(x, y + 0xC, "*w1by Fusing 2 Cards.", D_801EFF58[D_801F5380.side].b, 8, z);
        drawTextColored(x, y + 0x18, "*w1Partner Cards can't be used.", D_801EFF58[D_801F5380.side].b, 8, z);
    } else {
        x += 2;
        drawTextColored(x, y, "*w1Increase Experience Points by", D_801EFF58[(s8)(D_801F5380.side ^ 1)].b, 8, z);
        drawTextColored(x, y + 0xC, "*w1Fusing a Card to a Partner Card.", D_801EFF58[(s8)(D_801F5380.side ^ 1)].b, 8, z);
        drawTextColored(x, y + 0x18, "*w1Also,2 Partner Cards can't be Fused.", D_801EFF58[(s8)(D_801F5380.side ^ 1)].b, 8, z);
    }
}

void func_801E780C(EvoScene *scene) {
    char text[40];
    Rect16 uv;
    POLY_FT4 *poly;
    s32 player;

    poly = scene->polys[FRAME_BUFFER_INDEX];
    player = 2;
    if (scene == &D_801F5548[0]) {
        player = 1;
    }
    /* the index is added before the field offset in the original */
    if (((EvoMenu *)((u8 *)&D_801F5478 + player))->busy[0] == 0) {
        bzero((Scene3D *)text, 0x21);
        sprintf(text, "TRAY%d", player);
        drawLargeText(scene->unk124 + 16, 0x5C, (s32)text, 7, 0x1D);
        uv.x = 0;
        uv.y = 0x74;
        uv.w = 0x50;
        uv.h = 0x74;
        drawTexturedSprite(scene->unk124, scene->unk126, &uv, 0x18, 0x7BDF, 0x1E, 0x80, 0);
        return;
    }
    setlen(poly, 9);
    setcode(poly, 0x2C);
    poly->clut = 0x7BD8;
    poly->tpage = 0x18;
    poly->r0 = 0x80;
    poly->g0 = 0x80;
    poly->b0 = 0x80;
    poly->x0 = scene->unk124;
    poly->y0 = scene->unk126;
    poly->x1 = scene->unk124 + 0x50;
    poly->y1 = scene->unk126;
    poly->x2 = scene->unk124;
    poly->y2 = scene->unk126 + 0x74;
    poly->x3 = scene->unk124 + 0x50;
    poly->y3 = scene->unk126 + 0x74;
    poly->u0 = 0;
    poly->v0 = 0;
    poly->u1 = 0x50;
    poly->v1 = 0;
    poly->u2 = 0;
    poly->v2 = 0x74;
    poly->u3 = 0x50;
    poly->v3 = 0x74;
    addPrim(&CURRENT_FRAME_BUFFER->ot[29], poly);
    poly++;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    if (player == 1 && D_801F5478.unkC6 == 0) {
        poly->clut = 0x7A98;
        poly->u0 = 0;
        poly->v0 = 0x40;
        poly->u1 = 0x3F;
        poly->v1 = 0x40;
        poly->u2 = 0;
        poly->v2 = 0x7F;
        poly->u3 = 0x3F;
        poly->v3 = 0x7F;
    } else {
        poly->clut = getClut(0x180, player + 0x1E7);
        poly->u0 = (player - 1) * 64;
        poly->v0 = 0;
        poly->u1 = (player - 1) * 64 + 0x3F;
        poly->v1 = 0;
        poly->u2 = (player - 1) * 64;
        poly->v2 = 0x3F;
        poly->u3 = (player - 1) * 64 + 0x3F;
        poly->v3 = 0x3F;
    }
    poly->tpage = 0x99;
    poly->r0 = 0x80;
    poly->g0 = 0x80;
    poly->b0 = 0x80;
    poly->x0 = scene->unk124 + 8;
    poly->y0 = scene->unk126 + 0x14;
    poly->x1 = scene->unk124 + 0x48;
    poly->y1 = scene->unk126 + 0x14;
    poly->x2 = scene->unk124 + 8;
    poly->y2 = scene->unk126 + 0x54;
    poly->x3 = scene->unk124 + 0x48;
    poly->y3 = scene->unk126 + 0x54;
    addPrim(&CURRENT_FRAME_BUFFER->ot[28], poly);
}

const char D_801DF258[] = "MESSAGE";
const char D_801DF260[] = "";

void func_801E7B8C(void) {
    D_801F5548[0].unk124 += 10;
    if (D_801F5548[0].unk124 >= 15) {
        D_801F5548[0].unk124 = 14;
        D_801F5478.unkC1 = 3;
        D_801F0056 = 1;
        D_801F5478.unkBB = 0;
    }
}

void func_801E7BE4(void) {
    D_801F5548[0].unk124 -= 10;
    if (D_801F5548[0].unk124 < -0x58) {
        D_801F5548[0].unk124 = -0x58;
        D_801F5478.unkC1 = 0;
        D_801F5250->vars[8] = -1;
    }
}

void func_801E7C3C(void) {
    D_801F5548[0].unk124 -= 10;
    if (D_801F5548[0].unk124 < -0x78) {
        D_801F5548[0].unk124 = -0x78;
        D_801F5548[1].unk124 += 10;
        if (D_801F5548[1].unk124 >= 15) {
            D_801F5548[1].unk124 = 14;
            D_801F5478.unkC1 = 3;
            D_801F0056 = 1;
        }
    }
}

void func_801E7CB4(void) {
    D_801F5548[1].unk124 -= 10;
    if (D_801F5548[1].unk124 < -0x78) {
        D_801F5548[1].unk124 = -0x78;
        D_801F5548[0].unk124 += 10;
        if (D_801F5548[0].unk124 >= 15) {
            D_801F5548[0].unk124 = 14;
            D_801F5478.unkC1 = 3;
            D_801F0056 = 1;
        }
    }
}

void func_801E7D2C(s32 active) {
    if (active != 0) {
        D_801F5548[0].unk124 -= 10;
        D_801F5548[1].unk124 -= 10;
        if (D_801F5548[0].unk124 < -0x58) {
            D_801F5548[0].unk124 = -0x58;
        }
        if (D_801F5548[1].unk124 < 14) {
            D_801F5548[1].unk124 = 14;
        }
        if (D_801F5548[0].unk124 == -0x58 && D_801F5548[1].unk124 == 14) {
            D_801F5478.unkC4 = 2;
            D_801F5250->vars[8] = -1;
            D_801F5478.unkBB = 0;
            D_801F5478.unkC1 = 0;
            D_801F4E34[D_801F5478.unkAE]++;
        }
    }
}

void func_801E7E10(s8 mode) {
    if (mode == 0) {
        animateWindowTo(&D_801F5008, &D_801F0168);
        animateWindowTo(&D_801F5050, &D_801F0184);
    } else if (mode == 1) {
        animateWindowTo(&D_801F5008, (Rect16 *)-1);
        animateWindowTo(&D_801F5050, (Rect16 *)-1);
    }
}

void func_801E7E8C(s32 mode) {
    initDialog((u8 *)&D_801F57A8, NULL, 1);
    if (mode != 1) {
        D_801F57A8.choice = 1;
    }
    runDialog(&D_801F57A8);
    switch (D_801F57A8.choice) {
    case 0:
        D_801F5250->vars[1] = 0;
        break;
    case 1:
        D_801F5250->vars[1] = 1;
        break;
    case 2:
        D_801F5250->vars[1] = 2;
        break;
    }
}

void func_801E7F64(void) {
    if (D_801F53C8.unk42 == 0) {
        D_801F53C8.unk40 -= 8;
        if (D_801F53C8.unk40 < 0) {
            D_801F53C8.unk40 = 0;
        }
    } else {
        D_801F53C8.unk40 += 8;
        if (D_801F53C8.unk40 >= 0x100) {
            D_801F53C8.unk40 = 0xFF;
        }
    }
    if (D_801F53C8.unk40 != 0) {
        D_801F53C8.poly[FRAME_BUFFER_INDEX].r0 = D_801F53C8.poly[FRAME_BUFFER_INDEX].g0 =
            D_801F53C8.poly[FRAME_BUFFER_INDEX].b0 = D_801F53C8.unk40;
        addPrim(&CURRENT_FRAME_BUFFER->ot[25], &D_801F53C8.poly[FRAME_BUFFER_INDEX]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[25], &D_801F53C8.tpage[FRAME_BUFFER_INDEX]);
    }
}

void func_801E80E4(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        func_80067784(&D_801F53C8.poly[i]);
        SetSemiTrans(&D_801F53C8.poly[i], 1);
        setPrimQuadRect(&D_801F53C8.poly[i], 0, 0, 320, 240);
        SetDrawTPage(&D_801F53C8.tpage[i], 0, 0, 0x20);
        D_801F53C8.poly[i].b0 = 0;
        D_801F53C8.poly[i].g0 = 0;
        D_801F53C8.poly[i].r0 = 0;
    }
    D_801F53C8.unk42 = 0;
    D_801F53C8.unk40 = 0;
    addFrameCallback((s32)func_801E7F64);
}

void func_801E81C4(EvoProgram *data) {
    s32 result;

    if (D_801F4E4C == 1) {
        return;
    }
    do {
        result = runScriptToNextEvent(data->script, data->vars);
        if (result == 1) {
            switch (data->script->eventOp) {
            case 10:
                switch (data->script->eventArg) {
                case 0:
                    if (func_801E943C((u8 *)data->vars[4]) == -1) {
                        D_801F5478.unkBB = 1;
                        return;
                    }
                    break;
                case 1:
                    return;
                case 2:
                    func_801E7E10(0);
                    break;
                case 4:
                    func_801EA110();
                    return;
                case 5:
                    func_801EA2D4();
                    return;
                case 11:
                    D_801F5478.unkBB = 2;
                    return;
                case 12:
                    func_801E9390(D_801F5258);
                    break;
                case 8:
                    func_801EA414();
                    break;
                case 9:
                    func_801EA530();
                    break;
                case 10:
                    func_801EA790();
                    break;
                case 13:
                    func_801EA820();
                    break;
                case 6:
                    func_801ECBE8();
                    break;
                case 7:
                    func_801ECC24();
                    break;
                case 14:
                    func_801ECC6C();
                    break;
                case 15:
                    D_801F5478.unkC4 = 1;
                    break;
                case 16:
                    func_801EB234();
                    break;
                case 17:
                    func_801EB0F4();
                    return;
                case 18:
                    func_801EB1D0();
                    return;
                case 19:
                    D_801F5478.unkA8 = D_801F5250->script->pc - D_801F5250->script->start;
                    D_801F5478.unkBB = 4;
                    D_801F5478.unkC8 = 0;
                    D_801F5478.unkC1 = 10;
                    return;
                case 20:
                    D_801F5478.unkBB = 4;
                    D_801F5478.unkC8 = 0;
                    D_801F5478.unkC1 = 12;
                    return;
                case 21:
                    D_801F5478.unkBB = 4;
                    D_801F5478.unkC1 = 13;
                    return;
                case 22:
                    D_801F5478.unkBB = 4;
                    D_801F5478.unkC1 = 5;
                    animateWindowTo(&D_801F43D0, &D_801F0038);
                    D_801F0056 = 1;
                    return;
                default:
                    D_801F4E4C = 0;
                    break;
                }
                break;
            case 11:
                switch (data->script->eventArg) {
                case 0:
                    D_801F5478.unkBB = 4;
                    D_801F5478.unkC1 = 15;
                    return;
                case 1:
                    D_801F5478.unkBB = 4;
                    D_801F5478.unkC1 = 14;
                    return;
                case 2:
                    func_801E7E8C((s16)data->script->params[0]);
                    break;
                case 3:
                    animateWindowTo(&D_801F4E58[(s16)data->script->params[0]].win, (Rect16 *)-1);
                    if ((s16)data->script->params[0] == -1) {
                        animateWindowTo(&D_801F43D0, &D_801F0038);
                        D_801F0056 = 1;
                    }
                    break;
                case 4:
                    D_801F5478.unkBB = 4;
                    D_801F5478.unkC1 = 16;
                    return;
                case 5:
                    playSoundEffect((s16)data->script->params[0]);
                    break;
                case 6:
                    func_80014C08((s16)data->script->params[0]);
                    break;
                case 7:
                    D_801F5478.unkB7 = data->script->params[0];
                    return;
                case 8:
                    if ((s16)data->script->params[0] == 0) {
                        D_801F5478.busy[1] = 0;
                    } else {
                        D_801F5478.busy[2] = 0;
                    }
                    break;
                }
                break;
            case 12:
            case 13:
                break;
            }
        }
        clearScriptBusy(data->script);
    } while (result != 0);
}

s32 func_801E8614(EvoProgram *program) {
    *program->vars = 1;
    func_801E81C4(program);
    return *program->vars;
}

Script *func_801E8650(EvoMsd *data) {
    Script *script;

    script = allocHeapBlock(sizeof(Script), 0x2C);
    script->base = (u8 *)data;
    script->start = data->code;
    script->pc = data->code;
    script->offset = 0;
    script->size = data->size;
    clearScriptBusy(script);
    return script;
}

s32 *func_801E86B4(s32 count) {
    s32 *flags;
    s32 *p;
    s32 i;

    flags = allocHeapBlock(count * 4, 0x2C);
    p = flags;
    for (i = 0; i < count; i++) {
        *p++ = 0;
    }
    return flags;
}

EvoProgram *func_801E8708(s32 index) {
    char path[24];
    EvoMsd *data;
    EvoProgram *program;

    sprintf(path, "C:\\EVENT\\unit0%d.MSD", index);
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    data = (EvoMsd *)func_80014C08(0x7FFFFFFF);
    program = allocHeapBlock(sizeof(EvoProgram), 0x2C);
    program->data = data;
    program->script = func_801E8650(data);
    return program;
}

void func_801E87A8(void) {
    EvoWindowDef *def;
    s32 i;

    for (def = D_801F00C0, i = 0; i < 14; i++, def++) {
        D_801F4E58[i].z = 30;
        openWindow(&D_801F4E58[i].win, def, -1, (s16 *)-1, def->flags, def->style, def->brightness, 12);
        if (def->label != 0) {
            D_801F4E58[i].win.label = def->label;
        }
        D_801F4E58[i].win.labelPalette = def->labelPalette;
        animateWindowTo(&D_801F4E58[i].win, (Rect16 *)-1);
    }
    for (i = 0; i < 3; i++) {
        D_801F4E58[i].unk44 = 0;
        D_801F4E58[i + 3].unk44 = 1;
    }
    openMenu(&D_801F0030, &D_801F43D0, &D_801F4470, (Bytes4 *)-1);
    animateWindowTo(&D_801F43D0, (Rect16 *)-1);
    D_801F43D0.labelPalette = 8;
    D_801F43D0.label = (s32)"CARD LIST";
    D_801F0030.active = 0;
    openMenu(&D_801F005C, &D_801F4380, &D_801F4420, (Bytes4 *)-1);
    animateWindowTo(&D_801F4380, (Rect16 *)-1);
    D_801F4380.label = (s32)"SORT MENU";
    D_801F4380.labelPalette = 8;
    D_801F4E58[12].z = 0x1B;
    D_801F4E58[13].z = 5;
    D_801F4E58[13].win.palette = 2;
    D_801F57A0 = 0;
    openWindow(&D_801F5410, &D_801F00B8, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    animateWindowTo(&D_801F5410, (Rect16 *)-1);
    D_801F5410.palette = 2;
}

void func_801E89E8(void) {
}

void func_801E89F0(void) {
    s32 i;
    s32 t;

    if (D_801F5478.unkC8 == 1 && D_801F5548[1].unk120 == 1 && D_801F5548[0].unk120 < 30) {
        D_801F5548[0].unk120++;
        if (D_801F5548[0].unk120 >= 30) {
            D_801F5548[0].unk120 = 30;
            D_801F5478.unkC6 = 0;
        }
        t = D_801F5548[0].unk120;
        D_801F5548[0].unk124 = (t * 58 + (30 - t) * 14) / 30;
        D_801F5548[1].unk124 = (t * 58 + (30 - t) * 102) / 30;
    }
    for (i = 0; i < 14; i++) {
        drawWindow(&D_801F4E58[i].win, D_801F0248[i], D_801F4E58[i].z);
    }
    drawWindow(&D_801F5410, func_801E9B94, 5);
    drawWindow(&D_801F43D0, func_801E7178, 0x1D);
    drawWindow(&D_801F4380, func_801E6FCC, 0x1C);
    for (i = 0; i < 2; i++) {
        func_801E780C(&D_801F5548[i]);
    }
}

void func_801E8BD8(void) {
    if (D_801F5478.unkC9 == 0 && (PAD_STATES[0]->pressed & 0x40)) {
        func_801E9390(D_801F5258);
        if (D_801F5478.unkBB == 1) {
            func_801E943C((u8 *)D_801F5250->vars[4]);
        }
        D_801F5478.unkBB = 0;
        playSoundEffect(0);
    }
}

void func_801E8C74(void) {
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\M_CARD.ARC", getCurrentTaskId());
    D_801F5518 = func_80014C08(0x7FFFFFFF);
}

void func_801E8CD8(s32 id, s32 slot) {
    char path[64];
    u32 *tim;

    D_801F5478.busy[0] = 1;
    sprintf(path, "B:\\CARD\\LC%3.3d.TIM", id);
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tim = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTim(tim, (slot & 1) * 32 + 0x240, (slot >> 1) * 64 + 0x100, 0x180, slot + 0x1E8);
    DrawSync(0);
    freeHeapBlock(tim);
    D_801F5478.busy[0] = 0;
}

void func_801E8DC0(void) {
    s32 i;
    s32 bit;
    PlayerProfile *profile;

    for (i = 20, bit = 0, profile = (PlayerProfile *)PLAYER_PROFILES; i < 30; i++, bit++) {
        if ((1 << bit) & profile->unk2C) {
            D_801F5250->vars[i] = 1;
        }
    }
}

void func_801E8E1C(void) {
    s32 i;
    s32 bit;

    i = 20;
    bit = 0;
    while (bit < 32) {
        if (D_801F5250->vars[i] != 0) {
            ((PlayerProfile *)PLAYER_PROFILES)->unk2C |= 1 << bit;
        }
        i++;
        bit++;
        if (i >= 30) {
            break;
        }
    }
}

void func_801E8E88(s32 program) {
    s32 running = 1;
    s32 i;

    func_801E623C();
    func_801E8C74();
    func_801E62D0();
    func_801EE248();
    for (i = 0; i < 3; i++) {
        D_801F4E40[i] = allocTaskHeapBlock(0x12D);
    }
    D_801F4E34 = allocTaskHeapBlock(0x12D);
    func_801E6398();
    func_801E87A8();
    func_801E9390(D_801F5258);
    if (program >= 0) {
        D_801F5478.unkCA = program;
        func_801E80E4();
    }
    D_801F5250 = func_801E8708(D_801F5478.unkCA);
    D_801F5250->vars = func_801E86B4(30);
    if (program < 0) {
        playMusic(0, D_801F5478.unkCA + 0x85, 100);
        D_801F5478.unkC1 = 17;
        D_801F5478.unkBB = 0;
        D_801F5478.unkC6 = 0;
        D_801F5250->script->pc = D_801F5250->script->start + D_801F5478.unkA8;
        func_801E7E10(0);
    } else {
        func_801E65A8();
        D_801F5478.unkC1 = 0;
        D_801F5478.unkBB = 0;
        D_801F5478.unkC6 = 1;
    }
    D_801F5478.unkA4 = 0;
    D_801F5478.unkBC = 0;
    D_801F5478.unkBD = 0;
    D_801F5478.unkBE = 0;
    D_801F5478.unkBA = 0;
    D_801F5478.unkC2 = 0;
    D_801F5478.unkC8 = 0;
    for (i = 0; i < 2; i++) {
        D_801F5548[i].unk124 = -0x78;
        D_801F5548[i].unk126 = 0x29;
    }
    addFrameCallback((s32)func_801E89F0);
    func_801E8DC0();
    do {
        func_80014C08(1);
        switch (D_801F5478.unkBB) {
        case 0:
            running = func_801E8614(D_801F5250);
            break;
        case 1:
        case 2:
            func_801E8BD8();
            break;
        }
        switch (D_801F5478.unkC1) {
        case 0:
            break;
        case 1:
            func_801E9C88();
            break;
        case 2:
            func_801EA598();
            break;
        case 3:
            func_801EABB0();
            break;
        case 4:
            func_801EB440();
            break;
        case 5:
            func_801E7B8C();
            break;
        case 6:
            func_801E7CB4();
            break;
        case 7:
            func_801E7C3C();
            break;
        case 8:
            func_801E7BE4();
            break;
        case 10:
            func_801EBA08();
            break;
        case 11:
            func_801EA9AC();
            break;
        case 12:
            func_801EAA5C();
            break;
        case 13:
            func_801EBE08();
            break;
        case 14:
            func_801EA864((s16)D_801F5250->script->params[0]);
            break;
        case 15:
            func_801EA934((s16)D_801F5250->script->params[0]);
            break;
        case 16:
            func_801E7D2C((s16)D_801F5250->script->params[0]);
            break;
        case 17:
            func_801EBD64();
            break;
        case 18:
            func_801EBD04();
            break;
        }
    } while (running != 0 && D_801F5478.unkC7 == 0);
    if (running == 0) {
        animateWindowTo(&D_801F5008, (Rect16 *)-1);
        animateWindowTo(&D_801F5050, (Rect16 *)-1);
        func_80014C08(20);
    }
    func_801E8E1C();
    removeFrameCallback((s32)func_801E89F0);
    removeFrameCallback((s32)renderSceneModels);
    func_80014C08(1);
    func_80014A00(0x1B);
    freeHeapBlock(D_801F5478.unkA0);
    freeHeapBlock(D_801F5250->data);
    freeHeapBlocksByTag(0x2C);
    freeHeapBlock(D_801F59A8);
    freeHeapBlocksByTag(0x7F);
    if (D_801F5478.unkC7 != 0) {
        func_80014C08(60);
        hideScrollingBackground();
        func_800149B8(0, -1, 0, 0x400, func_801DFF78, 0, getCurrentTaskId(), 0, 0);
    } else {
        removeFrameCallback((s32)func_801E7F64);
        func_800149B8(0, -1, 0, 0x400, returnToWorldMap, 0, 0, 0, 0);
    }
}

void func_801E9390(EvoText *slot) {
    s32 i;

    for (i = 0; i < 4; i++) {
        slot->active = 0;
        slot++;
    }
}

EvoText *func_801E93B0(EvoText *slot) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (slot->active == 0) {
            bzero((Scene3D *)slot, 0x3C);
            slot->active = 1;
            if (i == 0) {
                slot->pos = -1;
            } else {
                slot->pos = 0;
            }
            return slot;
        }
        slot++;
    }
    return NULL;
}

s32 func_801E943C(u8 *src) {
    u8 *playerName;
    u8 *cardName;
    EvoText *t;
    u8 *dst;
    s32 i;
    s32 end;

    playerName = (u8 *)PLAYER_PROFILES;
    cardName = D_801F4980[D_801F5478.unkB0]->name;
    t = func_801E93B0(D_801F5258);
    if (t == NULL) {
        /* Shift-JIS: "the text has run out of lines" */
        printf("\x83" "e\x83L\x83X\x83g\x82\xCC\x8Ds\x90\x94\x82\xAA\x82\xA2\x82\xC1\x82\xCF\x82\xA2\x82\xC9\x82\xC8\x82\xE8\x82\xDC\x82\xB5\x82\xBD\n");
        return -1;
    }
    t->text[0] = '*';
    t->text[1] = 'w';
    t->text[2] = '1';
    dst = t->text + 3;
    while (*src != 0) {
        if (*src < 0x81 || *src > 0x98) {
            if (*src == '*') {
                if (src[1] == 'h') {
                    if (src[2] == '0') {
                        src += 3;
                        for (i = 0; i < 12 && *playerName != 0; i++) {
                            *dst++ = *playerName++;
                        }
                        continue;
                    } else if (src[2] == '1') {
                        src += 3;
                        for (i = 0; i < 21 && *cardName != 0; i++) {
                            *dst++ = *cardName++;
                        }
                        continue;
                    } else if (src[2] == '2' || src[2] == '3') {
                        src += 3;
                        for (i = 0; i < 21 && *cardName != 0; i++) {
                            if (i <= 0) {
                                *dst++ = *cardName++;
                            } else {
                                *dst++ = '?';
                                cardName++;
                                i++;
                            }
                        }
                        continue;
                    }
                }
            } else if (*src == '\\') {
                end = 0;
                for (i = 1; i < 5; i++) {
                    if (end == 0 && src[i] == 0) {
                        end = 1;
                        break;
                    }
                }
                if (end != 1 && src[1] == '0' && src[2] == 'x' && src[3] == '2' && src[4] == '2') {
                    src += 5;
                    *dst++ = '"';
                    continue;
                }
            }
        } else {
            *dst++ = *src++;
            *dst++ = *src++;
            continue;
        }
        *dst++ = *src++;
    }
    *dst = 0;
    dst = t->text;
    for (i = 0; i < 60 && *dst != 0; i++) {
        dst++;
    }
    if (t->pos == -1) {
        t->pos = i;
    }
    if (i >= 60) {
        t->active = 0;
    } else {
        t->active = 1;
    }
    t->len = i;
    for (i = 0; i < 4 && t != D_801F5258; i++) {
        t--;
    }
    return i;
}

const char D_801DF3B0[] = "";

s32 func_801E97E4(s32 x, s32 y, EvoText *t, s32 z) {
    u8 buf[64];
    u8 *dst;
    u8 *src;
    s8 i;
    u8 c;

    dst = buf;
    if (t->len == t->pos) {
        drawText(x, y, (s32)t, 7, z);
        return -1;
    }
    src = t->text;
    for (i = 0; i < t->pos; i++) {
        *dst++ = *src++;
    }
    c = *src;
    if (c == 0) {
        return 1;
    }
    if (*src < 0x81 || *src > 0x98) {
        if (c == '*') {
            switch (src[1]) {
            case 'a':
            case 'b':
            case 'c':
            case 'e':
            case 's':
            case 'w':
                if (src[2] >= '0' && src[2] <= '9') {
                    *dst++ = *src++;
                    *dst++ = *src++;
                    t->pos += 2;
                }
                break;
            }
        }
        *dst = *src;
        dst[1] = 0;
        drawText(x, y, (s32)buf, 7, z);
        t->pos += 1;
    } else {
        *dst++ = src[0];
        *dst = src[1];
        dst[1] = 0;
        drawText(x, y, (s32)buf, 7, z);
        t->pos += 2;
    }
    return 1;
}

s32 func_801E99A0(s16 x, s16 y, s32 z) {
    EvoText *t;
    s32 i;

    for (t = D_801F5258, i = 0; i < 4; i++, t++) {
        if (t->active != 0 && func_801E97E4(x, y + i * 12, t, z) == 1) {
            return 1;
        }
    }
    return 0;
}

void func_801E9A58(UiWindow *w) {
    s32 x = w->originX;
    s32 y = w->originY;
    s32 z = w->z;
    u8 unused[0x48]; /* stack space the original reserves but never touches */

    D_801F5478.unkC9 = func_801E99A0(x, y, z);
    if (D_801F5478.unkC9 == 0 && ((u8)D_801F5478.unkBB == 2 || (u8)D_801F5478.unkBB == 3)) {
        if (++D_801F5478.unkCB & 0x10) {
            drawIcon(x + 200, y + 0x25, 0, 0x1B, z);
        }
    } else {
        D_801F5478.unkCB = 0;
    }
}

void func_801E9B0C(UiWindow *w) {
    Rect16 uv;
    s32 x = w->originX;
    s32 y = w->originY;
    s32 z = w->z;

    uv.x = 0x28;
    uv.y = D_801F5478.unkCA * 56;
    uv.w = 0x40;
    uv.h = 0x38;
    drawTexturedSprite(x, y, &uv, 0x98, 0x7C18, z, w->brightness, -1);
}

void func_801E9B94(UiWindow *w) {
    s32 x = w->originX;
    s32 y = w->originY;
    s32 z = w->z;

    if (D_801F57A0 == 2) {
        drawLargeText(x + 1, y + 1, (s32)"RANK MAX!", 7, z);
    } else if (D_801F57A0 == 1) {
        drawLargeText(x + 1, y + 1, (s32)"RANK UP!", 7, z);
    }
}

void func_801E9C18(UiWindow *w) {
    s32 x = w->originX;
    s32 y = w->originY;
    s32 z = w->z;

    if (D_801F5360 == 0) {
        drawLargeText(x + 1, y + 1, (s32)"FULL SET!", 7, z);
    } else {
        drawLargeText(x + 1, y + 1, (s32)"RECEIVED!", 7, z);
    }
}

void func_801E9C88(void) {
    Rect16 rects[6];
    s32 i;
    s32 dx;
    s32 dy;

    if (D_801F5478.unkB9 == 0) {
        if (PAD_STATES[0]->pressed & 0x5000) {
            playSoundEffect(2);
            if (D_801F5478.unkBA == 0) {
                D_801F5478.unkB9 = 1;
            } else if (D_801F5478.unkBA == 1) {
                D_801F5478.unkB9 = 2;
            }
        } else if (PAD_STATES[0]->pressed & 0x40) {
            playSoundEffect(0);
            D_801F5250->vars[8] = 1;
            D_801F5250->vars[1] = D_801F5380.side;
            D_801F5478.unkBB = 0;
        } else if (PAD_STATES[0]->pressed & 0x10) {
            playSoundEffect(1);
            D_801F5250->vars[8] = 2;
            D_801F5478.unkBB = 0;
        }
    } else if (D_801F5478.unkB9 == 1) {
        D_801F5478.unkA4++;
        if (D_801F5478.unkA4 >= 21) {
            D_801F5478.unkA4 = 20;
            D_801F5478.unkB9 = 0;
            D_801F5478.unkBA = 1;
        }
    } else if (D_801F5478.unkB9 == 2) {
        D_801F5478.unkA4++;
        if (D_801F5478.unkA4 >= 41) {
            D_801F5478.unkA4 = 0;
            D_801F5478.unkB9 = 0;
            D_801F5478.unkBA = 0;
        }
    }
    D_801F5380.side = D_801F5478.unkBA;
    for (i = 0; i < 3; i++) {
        D_801F4E58[i].win.brightness = 0x80 - D_801F5380.side * 64;
        D_801F4E58[i + 3].win.brightness = D_801F5380.side * 64 + 64;
    }
    dy = rsin(D_801F5478.unkA4 * 1024 / 20) * 40 / 4096;
    dx = rsin(D_801F5478.unkA4 * 2048 / 20) * 60 / 4096;
    for (i = 0; i < 3; i++) {
        rects[i] = D_801F00C0[i].rect;
        rects[i + 3] = D_801F00C0[i + 3].rect;
        rects[i].x += dx;
        rects[i].y += dy;
        rects[i + 3].x -= dx;
        rects[i + 3].y -= dy;
        D_801F4E58[i].win.animFrame = D_801F4E58[i].win.animFrames;
        D_801F4E58[i + 3].win.animFrame = D_801F4E58[i + 3].win.animFrames;
        animateWindowTo(&D_801F4E58[i].win, &rects[i]);
        animateWindowTo(&D_801F4E58[i + 3].win, &rects[i + 3]);
    }
    for (i = 0; i < 6; i++) {
        if (i < 3) {
            D_801F4E58[i].z = D_801F5380.side + 30;
        } else {
            D_801F4E58[i].z = (D_801F5380.side + 30) ^ 1;
        }
    }
}

void func_801EA110(void) {
    s32 i;

    D_801F5358 = D_801F5250->vars[12];
    for (i = 0; i < 3; i++) {
        D_801F4E58[i].win.brightness = 0x80 - D_801F53B3 * 64;
        D_801F4E58[i + 3].win.brightness = D_801F53B3 * 64 + 64;
    }
    if (D_801F5478.unkBC == 0) {
        D_801F5478.unkBC = 1;
        D_801F5478.unkB9 = 0;
        D_801F5478.unkC1 = 1;
        D_801F5250->vars[8] = -1;
        for (i = 0; i < 6; i++) {
            if (D_801F5478.unkBA == 0) {
                animateWindowTo(&D_801F4E58[i].win, &D_801F00C0[i].rect);
            } else if (i < 3) {
                animateWindowTo(&D_801F4E58[i].win, &D_801F0114[i].rect);
            } else {
                animateWindowTo(&D_801F4E58[i].win, &D_801F006C[i].rect);
            }
        }
        func_80014C08(30);
    }
}

void func_801EA2D4(void) {
    s32 i;

    if (D_801F5478.unkBC == 1) {
        D_801F5478.unkBC = 0;
        D_801F5478.unkC1 = 0;
        for (i = 0; i < 6; i++) {
            D_801F4E58[i].win.animFrame = D_801F4E58[i].win.animFrames - 1;
            animateWindowTo(&D_801F4E58[i].win, (Rect16 *)-1);
        }
    }
}

void func_801EA358(EvoWindow *w) {
    Rect16 uv;
    s32 x = w->win.originX;
    s32 y = w->win.originY;
    s32 z = w->win.z;

    if (w->unk44 == 0) {
        uv.x = 0;
        uv.y = 0;
        uv.w = 0x40;
        uv.h = 0x40;
        drawTexturedSprite(x, y, &uv, 0x97, 0x7C58, z, w->win.brightness, -1);
    } else {
        uv.x = 0x40;
        uv.y = 0;
        uv.w = 0x40;
        uv.h = 0x40;
        drawTexturedSprite(x, y, &uv, 0x97, 0x7C58, z, w->win.brightness, -1);
    }
}

void func_801EA414(void) {
    s32 i;

    if (D_801F5478.unkBE == 0) {
        D_801F5478.unkBE = 1;
        D_801F5478.unkBF = 0;
        D_801F5478.unkC0 = 0;
        for (i = 10; i < 12; i++) {
            animateWindowTo(&D_801F4E58[i].win, &D_801F00C0[i].rect);
        }
        for (i = 0; i < 3; i++) {
            if (((PlayerProfile *)PLAYER_PROFILES)->partners[i].cardId != 0) {
                D_801F5478.unkBF++;
            }
        }
    }
    D_801F5250->vars[8] = -1;
    D_801F5478.unkC1 = 2;
}

void func_801EA530(void) {
    s32 i;

    D_801F5478.unkBE = 0;
    D_801F5478.unkC1 = 0;
    for (i = 10; i < 12; i++) {
        animateWindowTo(&D_801F4E58[i].win, (Rect16 *)-1);
    }
}

void func_801EA598(void) {
    if (PAD_STATES[0]->pressed & 0x40) {
        playSoundEffect(0);
        D_801F5478.unkC1 = 0;
        D_801F5250->vars[8] = 1;
        D_801F5478.unkAC = ((PlayerProfile *)PLAYER_PROFILES)->partners[D_801F5478.unkC0].cardId;
    } else if (PAD_STATES[0]->pressed & 0x10) {
        playSoundEffect(1);
        D_801F5478.unkC1 = 0;
        D_801F5250->vars[8] = 2;
    } else if (PAD_STATES[0]->repeat & 0x4000) {
        D_801F5478.unkC0++;
        if (D_801F5478.unkC0 >= D_801F5478.unkBF) {
            D_801F5478.unkC0 = 0;
        } else {
            playSoundEffect(2);
        }
        if (D_801F5478.unkBF == 0 || D_801F5478.unkC0 >= D_801F5478.unkBF - 1) {
            PAD_STATES[0]->repeatEnabled = 0;
        }
    } else if (PAD_STATES[0]->repeat & 0x1000) {
        D_801F5478.unkC0--;
        if (D_801F5478.unkC0 < 0) {
            D_801F5478.unkC0 = D_801F5478.unkBF - 1;
        } else {
            playSoundEffect(2);
        }
        if (D_801F5478.unkC0 <= 0) {
            PAD_STATES[0]->repeatEnabled = 0;
        }
    }
}

void func_801EA790(void) {
    animateWindowTo(&D_801F5128, (Rect16 *)-1);
    animateWindowTo(&D_801F43D0, &D_801F0038);
    D_801F0030.active = 1;
    D_801F5478.unkC2 = 0;
    D_801F5478.unkC1 = 3;
    D_801F5478.unkC3 = 0;
    D_801F0030.row = 0;
    scrollWindowTo((s16 *)D_801F0030.win, 0, 0);
    D_801F5250->vars[8] = -1;
}

void func_801EA820(void) {
    animateWindowTo(&D_801F5128, &D_801F01D8);
    animateWindowTo(&D_801F43D0, (Rect16 *)-1);
    D_801F0056 = 0;
}

void func_801EA864(s32 index) {
    EvoScene *scenes = D_801F5548;

    scenes[index].unk124 -= 10;
    if (scenes[index].unk124 < -0x58) {
        scenes[index].unk124 = -0x58;
        D_801F5478.unkC1 = 0;
        D_801F5478.unkBB = 0;
        D_801F5250->vars[8] = -1;
        if (index == 0) {
            D_801F4E34[D_801F5478.unkAC]--;
        } else {
            D_801F4E34[D_801F5478.unkAC]++;
        }
    }
}

void func_801EA934(s32 index) {
    EvoScene *scenes = D_801F5548;

    scenes[index].unk124 += 10;
    if (scenes[index].unk124 >= 15) {
        scenes[index].unk124 = 14;
        D_801F5478.unkC1 = 0;
        D_801F5478.unkBB = 0;
        D_801F5250->vars[8] = -1;
    }
}

void func_801EA9AC(void) {
    D_801F5548[0].unk124 += 10;
    D_801F5548[1].unk124 += 10;
    if (D_801F5548[0].unk124 >= 15) {
        D_801F5548[0].unk124 = 14;
    }
    if (D_801F5548[1].unk124 >= 0x67) {
        D_801F5548[1].unk124 = 0x66;
    }
    if (D_801F5548[0].unk124 == 14 && D_801F5548[1].unk124 == 0x66) {
        D_801F5478.unkC1 = 0;
        D_801F5250->vars[8] = 1;
    }
}

void func_801EAA5C(void) {
    D_801F5548[0].unk124 -= 10;
    D_801F5548[1].unk124 -= 10;
    if (D_801F5548[1].unk124 < 14) {
        D_801F5548[0].unk124 = -0x58;
        D_801F5548[1].unk124 = 14;
        D_801F5478.unkC1 = 0;
        D_801F5478.unkBB = 0;
        animateWindowTo(&D_801F43D0, &D_801F0038);
        func_801ECC6C();
    }
}

void func_801EAAE8(s16 cardId) {
    if (cardId < 0) {
        cardId = -1;
    }
    if (D_801F5250->vars[13] != 0) {
        D_801F5250->vars[11] = 0;
        return;
    }
    D_801F5250->vars[11] = getOwnedCardCount(0, cardId);
    if ((((PlayerProfile *)PLAYER_PROFILES)->cardCollection[cardId] & 7) + 1 >= 7) {
        D_801F5250->vars[11] = -1;
    } else {
        D_801F5250->vars[11] = 0;
    }
}

void func_801EABB0(void) {
    s16 cardId;
    s32 blocked = 0;
    s32 i;

    cardId = D_801F44C0[D_801F0030.row]->id;
    D_801F5458 = cardId;
    if (PAD_STATES[0]->pressed & 0x100) {
        if (D_801F5478.unkC3 != 0) {
            return;
        }
        if (D_801F5478.unkC2 == 0) {
            D_801F5478.unkC2 = 1;
            D_801F0030.active = 0;
            animateWindowTo(&D_801F4380, &D_801F0064);
            playSoundEffect(3);
            return;
        }
        playSoundEffect(4);
        D_801F5478.unkC2 = 0;
        D_801F0056 = 1;
        animateWindowTo(&D_801F4380, (Rect16 *)-1);
    } else if (D_801F5478.unkC2 == 0) {
        if (D_801F5478.unkC3 == 0) {
            if (PAD_STATES[0]->pressed & 0x40) {
                if (D_801F44C0[D_801F0030.row]->unk18 == 0) {
                    blocked = 1;
                } else if (D_801F44C0[D_801F0030.row]->type == 0 &&
                           (D_801F44C0[D_801F0030.row]->attr & 0xF) > D_801F5358) {
                    blocked = 1;
                }
                if (D_801F4E34[cardId] == 0) {
                    return;
                }
                if (cardId >= 0xAC && cardId < 0xBF) {
                    for (i = 0; i < 6; i++) {
                        if (D_801EFED4[i] == cardId) {
                            i = -1;
                            break;
                        }
                    }
                    if (i == -1) {
                        initDialog((u8 *)&D_801F57A8, "You can't use Partner Cards\nin Fusion.", 0);
                    } else {
                        initDialog((u8 *)&D_801F57A8, "You can't use that Card in Fusion.", 0);
                    }
                    runDialog(&D_801F57A8);
                    return;
                }
                if (blocked) {
                    return;
                }
                playSoundEffect(0);
                if (D_801F5478.unkBA == 0) {
                    if (D_801F5478.unkC4 != 2) {
                        D_801F0056 = 0;
                        D_801F5250->vars[8] = 1;
                        D_801F5478.unkC1 = 0;
                        func_801E8CD8(cardId, 0);
                        D_801F5478.unkAC = cardId;
                        D_801F5478.busy[1] = 1;
                        return;
                    }
                    D_801F0056 = 0;
                    func_801E8CD8(cardId, 1);
                    do {
                        func_80014C08(1);
                    } while (D_801F5478.busy[0] != 0);
                    D_801F5478.unkAE = cardId;
                    func_801ECCA0();
                    func_801EAAE8(D_801F5478.unkB0);
                    D_801F5478.unkC1 = 11;
                    animateWindowTo(&D_801F43D0, (Rect16 *)-1);
                    D_801F4E34[D_801F5478.unkAE]--;
                    D_801F5478.busy[2] = 1;
                    func_801E8CD8(D_801F5478.unkB0, 2);
                    do {
                        func_80014C08(1);
                    } while (D_801F5478.busy[0] != 0);
                } else if (D_801F5478.unkBA == 1) {
                    D_801F0056 = 0;
                    uploadTim((u32 *)((u8 *)D_801F5478.unkA0 + D_801F5478.unkA0[cardId]), 0x1C0, 0x190, 0x180, 0x1FC);
                    D_801F5478.unkAE = cardId;
                    D_801F5478.unkC3 = 1;
                    animateWindowTo(&D_801F51B8, &D_801F0210);
                }
            } else if (PAD_STATES[0]->pressed & 0x10) {
                D_801F5478.unkC3 = 0;
                D_801F5478.unkC1 = 0;
                D_801F5250->vars[8] = 2;
                playSoundEffect(1);
            }
        } else if (PAD_STATES[0]->pressed & 0x40) {
            playSoundEffect(0);
            D_801F5478.unkC1 = 0;
            D_801F5250->vars[8] = 1;
        } else if (PAD_STATES[0]->pressed & 0x10) {
            playSoundEffect(1);
            D_801F0056 = 1;
            D_801F5478.unkC3 = 0;
            animateWindowTo(&D_801F51B8, (Rect16 *)-1);
        }
    } else if (PAD_STATES[0]->pressed & 0x10) {
        playSoundEffect(4);
        D_801F5478.unkC2 = 0;
        D_801F0056 = 1;
        animateWindowTo(&D_801F4380, (Rect16 *)-1);
    }
}

void func_801EB0F4(void) {
    PlayerProfile *profile;

    D_801F5250->vars[8] = -2;
    D_801F5250->vars[18] = 0;
    D_801F5250->vars[19] = 0;
    D_801F5250->vars[7] = 1;
    func_801EE2DC(0, 0);
    func_801EB2A0();
    D_801F5478.unkC1 = 4;
    removeCardFromCollection(0, D_801F5478.unkAE, 1);
    D_801F4E34[D_801F5478.unkAE]--;
    profile = (PlayerProfile *)PLAYER_PROFILES;
    profile->unk52++;
    if ((u16)profile->unk52 >= 10000) {
        profile->unk52 = 9999;
    }
}

void func_801EB1D0(void) {
    D_801F5250->vars[8] = -1;
    D_801F0056 = 1;
    D_801F5478.unkC3 = 0;
    animateWindowTo(&D_801F51B8, (Rect16 *)-1);
    D_801F5478.unkC1 = 3;
}

void func_801EB234(void) {
    setScreenFadeParams(0, 2, 6);
    func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 1, 6, 0);
    D_801F553F = 1;
}

void func_801EB2A0(void) {
    s32 i;
    s32 ability;

    D_801F5478.unkB6 = -1;
    ability = -1;
    for (i = 0; i < 6; i++) {
        if (D_801EFED4[i] == D_801F5478.unkAC && D_801F5478.unkB6 == -1) {
            D_801F5478.unkB6 = i;
        }
    }
    if (D_801F5478.unkB6 != -1) {
        for (i = 0; i < 5; i++) {
            if (D_801EFF1C[D_801F5478.unkB6][i].card == D_801F5478.unkAE && ability == -1) {
                ability = D_801EFF1C[D_801F5478.unkB6][i].ability;
                if (getPartnerAbilityState(0, ability) == 0) {
                    grantPartnerAbility(0, ability);
                    D_801F5478.unkB7 = 0;
                } else {
                    ability = -1;
                }
                D_801F5478.unkB0 = ability;
            }
        }
    }
    if (ability == -1) {
        D_801F5478.unkB7 = 3;
        D_801F5250->vars[19] = ability;
        D_801F5478.unkB6 = ability;
        D_801F5478.unkB0 = D_801F4980[D_801F5478.unkAE]->unk18;
    }
}

void func_801EB440(void) {
    char text[168];
    s32 i;

    switch (D_801F5478.unkB7) {
    case 0:
        D_801F5250->vars[18] = 2;
        break;
    case 1:
        sprintf(text, "Obtained\nDigi-Part *c5 No.%3.3d\n*c6[%s]", D_801F5478.unkB0, D_801EF8D4[D_801F5478.unkB0].name);
        initDialog((u8 *)&D_801F57A8, text, 0x80);
        runDialog(&D_801F57A8);
        D_801F5478.unkB7 = 10;
        D_801F5478.unkC1 = 3;
        D_801F0056 = 1;
        D_801F5478.unkC3 = 0;
        for (i = 0; i < 4; i++) {
            D_801F5860[i] = 0;
        }
        D_801F5250->vars[8] = -1;
        break;
    case 2:
        if (D_801F5250->vars[19] == -1) {
            func_801EB670();
        } else if (D_801F5478.unkBB == 0) {
            if (D_801F57A1 == 0) {
                if (D_801F4E50 != -1) {
                    grantPartnerAbility(0, D_801F4E50);
                    sprintf(text, "Obtained\nDigi-Part *c5 No.%3.3d\n*c6[%s]", D_801F4E50, D_801EF8D4[D_801F4E50].name);
                    initDialog((u8 *)&D_801F57A8, text, 0x80);
                    runDialog(&D_801F57A8);
                }
                D_801F5250->vars[19] = -1;
                if (D_801F57A0 != 0) {
                    animateWindowTo(&D_801F5410, (Rect16 *)-1);
                }
            } else {
                D_801F57A1 = 0;
            }
        }
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        break;
    }
}

void func_801EB670(void) {
    char text[168];
    Partner *partner;
    s32 ability;
    s32 i;

    partner = &((PlayerProfile *)PLAYER_PROFILES)->partners[D_801F5478.unkC0];
    bzero((Scene3D *)text, 0xA1);
    if (D_801F5478.unkB6 >= 0) {
        D_801F5250->vars[18] = 2;
        sprintf(text, "Obtained\nDigi-Part *c5 No.%3.3d\n*c7[%s]", D_801F5478.unkB0, D_801EF8D4[D_801F5478.unkB0].name);
        initDialog((u8 *)&D_801F57A8, text, 0x80);
        runDialog(&D_801F57A8);
        D_801F5478.unkB6 = -2;
        return;
    }
    if (D_801F5478.unkB6 == -1) {
        if (D_801F5478.unkB0 > 0) {
            D_801F5478.unkB0--;
            if ((s8)partner->level < 99) {
                partner->exp++;
                if (getExpForNextLevel((s8)partner->level) - (u16)partner->exp > 0) {
                    return;
                }
                partner->level++;
                if ((s8)partner->level >= 99) {
                    D_801F5478.unkB0 = 0;
                }
                ability = findNewPartnerAbility((AbilityLearnEntry *)D_801EF8D4, 0, D_801F5478.unkC0);
                D_801F5250->vars[19] = 1;
                D_801F4E50 = -1;
                D_801F57A1 = 1;
                D_801F57A0 = 1;
                animateWindowTo(&D_801F5410, &D_801F00B8);
                if (ability >= 0) {
                    D_801F5250->vars[19] = 2;
                    D_801F4E50 = ability;
                }
                ability = func_8004994C(0, D_801F5478.unkC0);
                if (ability >= 0) {
                    D_801F5860[ability] += 10;
                }
            } else {
                D_801F57A0 = 2;
                animateWindowTo(&D_801F5410, &D_801F00B8);
                do {
                    func_80014C08(1);
                } while (!(PAD_STATES[0]->pressed & 0x40));
                playMenuSound(1);
                animateWindowTo(&D_801F5410, (Rect16 *)-1);
                D_801F5478.unkB0 = 0;
            }
        } else {
            for (i = 0; i < 4; i++) {
                if (i == 0) {
                    partner->hpBonus += D_801F5860[0];
                } else {
                    partner->attackBonus[i - 1] += D_801F5860[i];
                }
            }
            updatePartnerStats(0, D_801F5478.unkC0);
            D_801F5478.unkB6 = -2;
        }
        return;
    }
    D_801F5478.unkB7 = 10;
    D_801F5478.unkC1 = 3;
    D_801F0056 = 1;
    D_801F5478.unkC3 = 0;
    for (i = 0; i < 4; i++) {
        D_801F5860[i] = 0;
    }
    D_801F5250->vars[8] = -1;
}

void func_801EBA08(void) {
    s32 i;

    if (D_801F5478.unkC8 == 0) {
        D_801F5478.unkC8 = 1;
        for (i = 0; i < 2; i++) {
            D_801F5548[i].unk120 = 0;
        }
        D_801F5250->vars[8] = -1;
        return;
    }
    if (D_801F5478.unkC8 == 1) {
        if (D_801F5478.unkC5 == 1) {
            func_801EE2DC(7, 0);
        } else {
            func_801EE2DC(6, 0);
        }
        removeCardFromCollection(0, D_801F5478.unkAC, 1);
        removeCardFromCollection(0, D_801F5478.unkAE, 1);
        if (addCardToCollection(0, D_801F5478.unkB0, 1) >= 0) {
            D_801F4E34[D_801F5478.unkB0]++;
            D_801F5360 = 1;
        } else {
            D_801F5360 = 0;
        }
        ((PlayerProfile *)PLAYER_PROFILES)->unk50++;
        if ((u16)((PlayerProfile *)PLAYER_PROFILES)->unk50 >= 10000) {
            ((PlayerProfile *)PLAYER_PROFILES)->unk50 = 9999;
        }
        ((PlayerProfile *)PLAYER_PROFILES)->unk52 += 2;
        if ((u16)((PlayerProfile *)PLAYER_PROFILES)->unk52 >= 10000) {
            ((PlayerProfile *)PLAYER_PROFILES)->unk52 = 9999;
        }
        if (D_801F5250->vars[13] != 0) {
            ((PlayerProfile *)PLAYER_PROFILES)->unk54++;
            if ((u16)((PlayerProfile *)PLAYER_PROFILES)->unk54 >= 10000) {
                ((PlayerProfile *)PLAYER_PROFILES)->unk54 = 9999;
            }
        }
        if (D_801F5478.unkC5 == 1) {
            D_801F5478.unkC7 = 1;
            D_801F2BFC[0] = D_801F4980[D_801F5478.unkAC]->modelId;
            D_801F2BFC[1] = D_801F4980[D_801F5478.unkAE]->modelId;
            return;
        }
        animateWindowTo(&D_801F5200, &D_801F022C);
        D_801F5478.unkC8 = 2;
    }
    D_801F5548[0].unk124 = 0x3A;
    D_801F5548[1].unk124 = -0x58;
    D_801F5478.unkBB = 0;
    if (PAD_STATES[0]->pressed & 0x40) {
        animateWindowTo(&D_801F5200, (Rect16 *)-1);
        playSoundEffect(0);
        D_801F5478.unkC1 = 0x12;
        D_801F5478.unkC8 = 3;
    }
}

void func_801EBD04(void) {
    D_801F5548[0].unk124 -= 10;
    if (D_801F5548[0].unk124 < -0x58) {
        D_801F5548[0].unk124 = -0x58;
        D_801F5478.unkC1 = 0;
        D_801F5250->vars[8] = 1;
        D_801F5478.unkC8 = 0;
    }
}

void func_801EBD64(void) {
    if (D_801F5478.unkC7 != 0) {
        func_801EE2DC(8, 0);
        D_801F5478.unkC7 = 0;
        animateWindowTo(&D_801F5200, &D_801F022C);
    }
    if (PAD_STATES[0]->pressed & 0x40) {
        animateWindowTo(&D_801F5200, (Rect16 *)-1);
        playSoundEffect(0);
        D_801F5478.unkC1 = 0x12;
        D_801F5478.unkC8 = 3;
    }
}

void func_801EBE08(void) {
    D_801F5548[0].unk124 -= 10;
    if (D_801F5548[0].unk124 < -0x58) {
        D_801F5548[0].unk124 = -0x58;
        D_801F5478.unkC1 = 0;
        D_801F5478.unkBB = 0;
        D_801F5250->vars[8] = 1;
    }
}

#define PARTNER(i) (((PlayerProfile *)PLAYER_PROFILES)->partners[i])

void func_801EBE68(UiWindow *w) {
    Rect16 rect;
    char text[72];
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 dim;
    s32 level;
    s32 index;
    s32 next;

    x = w->originX;
    y = w->originY;
    z = w->z;
    for (i = 0; i < 3; i++) {
        if (PARTNER(i).cardId == 0) {
            continue;
        }
        if (D_801F5478.unkC0 == i) {
            dim = 0;
            level = 0; /* dead store, but it keeps the branch the original has */
        } else {
            dim = 1;
        }
        level = PARTNER(i).card[0].attr >> 4;
        if (level != 0) {
            level--;
        }
        index = getSlotPartnerIndex(0, i);
        rect.x = (index % 3) * 40;
        rect.y = (index / 3) * 40 + 0x140;
        rect.w = 40;
        rect.h = 40;
        drawTexturedSprite(x, y, &rect, 0x97, ((level + 0x1F8) << 6) | 0x18, z, dim ? 0x40 : 0x80, -1);
        drawTextColored(x + 0x2C, y + 2, PARTNER(i).card[0].name, D_801EFF58[dim].b, 8, z);
        drawLargeTextColored(x + 0x82, y + 4, "NEXT", 6, D_801EFF58[dim].b, z);
        next = 0;
        if ((s8)PARTNER(i).level < 99) {
            next = getExpForNextLevel((s8)PARTNER(i).level) - (u16)PARTNER(i).exp;
        }
        sprintf(text, "*s0%3d", next);
        drawTextColored(x + 0xB0, y + 2, text, D_801EFF58[dim].b, 8, z);
        y += 2;
        drawLargeTextColored(x + 0x2C, y + 0x10, "RANK", 6, D_801EFF58[dim].b, z);
        sprintf(text, "*s0%2d", (s8)PARTNER(i).level);
        drawTextColored(x + 0x56, y + 0xE, text, D_801EFF58[dim].b, 8, z);
        drawIconColored(x + 0x6C, y + 0xE, 0, 0x1A, D_801EFF58[dim].b, z);
        sprintf(text, "*s0%4d", PARTNER(i).card[0].hp);
        drawTextColored(x + 0x7A, y + 0xE, text, D_801EFF58[dim].b, 8, z);
        drawIconColored(x + 0x9C, y + 0xE, 0, 7, D_801EFF58[dim].b, z);
        sprintf(text, "*s0%4d", PARTNER(i).card[0].attack[0].power);
        drawTextColored(x + 0xAA, y + 0xE, text, D_801EFF58[dim].b, 8, z);
        drawIconColored(x + 0x6C, y + 0x1A, 0, 8, D_801EFF58[dim].b, z);
        sprintf(text, "*s0%4d", PARTNER(i).card[0].attack[1].power);
        drawTextColored(x + 0x7A, y + 0x1A, text, D_801EFF58[dim].b, 8, z);
        drawIconColored(x + 0x9C, y + 0x1A, 0, 9, D_801EFF58[dim].b, z);
        sprintf(text, "*s0%4d", PARTNER(i).card[0].attack[2].power);
        drawTextColored(x + 0xAA, y + 0x1A, text, D_801EFF58[dim].b, 8, z);
        drawLargeTextColored(x + 0x2C, y + 0x1C, "EXP", 6, D_801EFF58[dim].b, z);
        sprintf(text, "*s0%4d", (u16)PARTNER(i).exp);
        drawTextColored(x + 0x4A, y + 0x1A, text, D_801EFF58[dim].b, 8, z);
        y += 0x28;
    }
}

void func_801EC434(UiWindow *w) {
    u8 palettes[8] = { 2, 1, 4, 9, 6, 8, 8, 8 };
    char text[72];
    Rect16 rect;
    Partner *partner;
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 index;
    s32 next;

    x = w->originX;
    y = w->originY;
    z = w->z;
    partner = &PARTNER(D_801F5478.unkC0);
    w->palette = palettes[partner->card[0].attr >> 4];
    drawTextColored(x, y, partner->card[0].name, D_801EFF58[0].b, 7, z);
    drawLargeTextColored(x, y + 0x10, "RANK", 6, D_801EFF58[0].b, z);
    sprintf(text, "*s0%2d", (s8)partner->level);
    drawTextColored(x + 0x34, y + 0xE, text, D_801EFF58[0].b, 7, z);
    drawLargeTextColored(x, y + 0x1E, "EXP", 6, D_801EFF58[0].b, z);
    sprintf(text, "*s0%4d", (u16)partner->exp);
    drawTextColored(x + 0x28, y + 0x1C, text, D_801EFF58[0].b, 7, z);
    drawLargeTextColored(x, y + 0x2C, "NEXT", 6, D_801EFF58[0].b, z);
    next = 0;
    if ((s8)partner->level < 99) {
        next = getExpForNextLevel((s8)partner->level) - (u16)partner->exp;
    }
    sprintf(text, "*s0%3d", next);
    drawTextColored(x + 0x2E, y + 0x2A, text, D_801EFF58[0].b, 7, z);
    drawIconColored(x, y + 0x38, 0, 0x1A, D_801EFF58[0].b, z);
    sprintf(text, "*s0%4d", partner->card[0].hp);
    drawTextColored(x + 0x28, y + 0x38, text, D_801EFF58[0].b, 7, z);
    drawIconColored(x, y + 0x46, 0, 7, D_801EFF58[0].b, z);
    sprintf(text, "*s0%4d", partner->card[0].attack[0].power);
    drawTextColored(x + 0x28, y + 0x46, text, D_801EFF58[0].b, 7, z);
    drawIconColored(x, y + 0x54, 0, 8, D_801EFF58[0].b, z);
    sprintf(text, "*s0%4d", partner->card[0].attack[1].power);
    drawTextColored(x + 0x28, y + 0x54, text, D_801EFF58[0].b, 7, z);
    drawIconColored(x, y + 0x62, 0, 9, D_801EFF58[0].b, z);
    sprintf(text, "*s0%4d", partner->card[0].attack[2].power);
    drawTextColored(x + 0x28, y + 0x62, text, D_801EFF58[0].b, 7, z);
    for (i = 0; i < 4; i++) {
        if (D_801F5860[i] > 0) {
            sprintf(text, "+%d", D_801F5860[i]);
            drawTextColored(x + 0x42, y + (i + 4) * 14, text, D_801EFF58[0].b, 5, z);
        }
    }
    index = getSlotPartnerIndex(0, D_801F5478.unkC0);
    rect.x = (index % 3) * 84;
    rect.y = (index / 3) * 123;
    rect.w = 0x54;
    rect.h = 0x7B;
    drawTexturedSprite(x, y, &rect, 0x1A, getClut(index * 16 + 0x190, 0x1EF), z, 0x80, -1);
}

void func_801EC8DC(UiWindow *w) {
    Rect16 rect;
    char text[72];
    s32 x;
    s32 y;
    s32 z;
    s16 id;

    x = w->originX;
    y = w->originY;
    z = w->z;
    id = D_801F5478.unkAE;
    if (id < 0xBF) {
        sprintf(text, D_801DF15C, ((DigimonCardData *)(DIGIMON_CARDS + id * 0x13C))->id);
        drawText(x + 0x40, y + 0xD, (s32)text, 7, z);
        drawText(x, y, (s32)((DigimonCardData *)(DIGIMON_CARDS + id * 0x13C))->name, 7, z);
        drawIcon(x + 0x46, y + 0x1A, 0, ((DigimonCardData *)(DIGIMON_CARDS + id * 0x13C))->attr >> 4, z);
        drawIcon(x + 0x46, y + 0x27, 0, (((DigimonCardData *)(DIGIMON_CARDS + id * 0x13C))->attr & 0xF) + 0x10, z);
        drawLargeText(x, y + 0x2A, (s32)"Level", 6, z);
    } else if ((id -= 0xBF) < 0x66) {
        sprintf(text, D_801DF15C, ((DigimonCardData *)(OPTION_CARDS + id * 0xE2))->id);
        drawText(x + 0x40, y + 0xD, (s32)text, 7, z);
        drawText(x, y, (s32)((DigimonCardData *)(OPTION_CARDS + id * 0xE2))->name, 7, z);
        drawIcon(x + 0x46, y + 0x1A, 0, 5, z);
    } else {
        id -= 0x66;
        sprintf(text, D_801DF15C, ((DigimonCardData *)(DIGIVOLVE_CARDS + id * 0x70))->id);
        drawText(x + 0x40, y + 0xD, (s32)text, 7, z);
        drawText(x, y, (s32)((DigimonCardData *)(DIGIVOLVE_CARDS + id * 0x70))->name, 7, z);
        drawIcon(x + 0x46, y + 0x1A, 0, 6, z);
    }
    drawLargeText(x, y + 0x12, (s32)D_801DF13C, 6, z);
    drawLargeText(x, y + 0x1E, (s32)D_801DF548, 6, z);
    rect.x = 0;
    rect.y = 0x90;
    rect.w = 0x28;
    rect.h = 0x28;
    drawTexturedSprite(x + 0x60, y + 0xD, &rect, 0x97, 0x7F18, z, 0x80, -1);
}

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF548);

void func_801ECBE8(void) {
    D_801F5478.unkAE = -1;
    D_801F5478.unkAC = -1;
    D_801F5478.unkC6 = 1;
    D_801F5478.unkBB = 4;
    D_801F5478.unkC2 = 0;
    D_801F5478.unkC3 = 0;
    D_801F5478.unkC4 = 1;
    D_801F5478.busy[1] = 0;
    D_801F5478.busy[2] = 0;
}

void func_801ECC24(void) {
    D_801F5478.unkC4 = 1;
    D_801F5478.unkC1 = 8;
    animateWindowTo(&D_801F43D0, (Rect16 *)-1);
    D_801F0056 = 0;
}

void func_801ECC6C(void) {
    D_801F5478.unkC4 = 2;
    D_801F5250->vars[8] = -1;
    D_801F5478.unkC1 = 7;
}

s16 func_801ECCA0(void) {
    s8 types[2];
    s8 i;
    s8 j;
    s8 level;
    s8 kind;

    D_801F5478.unkB0 = -1;
    level = D_801F4980[D_801F5478.unkAC]->unk18 + D_801F4980[D_801F5478.unkAE]->unk18;
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            types[0] = D_801F4980[D_801F5478.unkAC]->type;
        } else {
            types[i] = D_801F4980[D_801F5478.unkAE]->type;
        }
        switch (types[i]) {
        case 0:
            if (i == 0) {
                types[0] = D_801F4980[D_801F5478.unkAC]->attr >> 4;
            } else {
                types[i] = D_801F4980[D_801F5478.unkAE]->attr >> 4;
            }
            break;
        case 1:
        case 2:
            types[i] = 5;
            break;
        }
    }
    kind = D_801EFEDC[types[0]][types[1]];
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 20; i++) {
            if (D_801DF03C[i][j] == D_801F5478.unkAC && D_801DF03C[i][1 - j] == D_801F5478.unkAE) {
                D_801F5478.unkB0 = D_801DF03C[i][2];
                D_801F2BFC[2] = D_801DF03C[i][3];
            }
        }
    }
    D_801F5250->vars[14] = 0;
    D_801F5250->vars[13] = 0;
    if (D_801F5478.unkB0 == -1) {
        D_801F5478.unkB8 = rand() % 100;
        if (D_801F5478.unkB8 < 0) {
            D_801F5478.unkB8 = D_801F5478.unkB8 * -1;
        }
        if (D_801F5478.unkB8 <= (s8)(level / 10)) {
            D_801F5250->vars[13] = 1;
            D_801F5478.unkC5 = 2;
            D_801F5478.unkB8 = rand() % 100;
            if (D_801F5478.unkB8 < 0) {
                D_801F5478.unkB8 = D_801F5478.unkB8 * -1;
            }
            if (D_801F5478.unkB8 < 21) {
                D_801F5478.unkB0 = rand() % 12 + 0x111;
                if (getOwnedCardCount(0, D_801F5478.unkB0) >= 6) {
                    D_801F5478.unkB0 = 200;
                }
            } else if (D_801F5478.unkB8 < 61) {
                D_801F5478.unkB0 = 200;
            } else {
                D_801F5478.unkB8 = rand() % 3 + 1;
                func_801ED5F4(kind, level + D_801F5478.unkB8, &D_801F5478.unkB0, D_801F5478.unkAC, D_801F5478.unkAE);
                if (D_801F5478.unkB0 == D_801F5478.unkAC || D_801F5478.unkB0 == D_801F5478.unkAE) {
                    D_801F5478.unkB0 = -1;
                }
                for (i = 0; D_801F5478.unkB0 == -1;) {
                    i++;
                    func_801ED5F4(kind, level + D_801F5478.unkB8 + i, &D_801F5478.candidates[0], 500, 500);
                    func_801ED5F4(kind, level + D_801F5478.unkB8 - i, &D_801F5478.candidates[1], 500, 500);
                    if (D_801F5478.candidates[0] != -1 && D_801F5478.candidates[1] != -1) {
                        D_801F5478.unkB0 = D_801F5478.candidates[rand() % 2];
                    } else if (D_801F5478.candidates[0] != -1) {
                        D_801F5478.unkB0 = D_801F5478.candidates[0];
                    } else if (D_801F5478.candidates[1] != -1) {
                        D_801F5478.unkB0 = D_801F5478.candidates[1];
                    }
                    if (D_801F5478.unkB0 == D_801F5478.unkAC || D_801F5478.unkB0 == D_801F5478.unkAE) {
                        D_801F5478.unkB0 = -1;
                    }
                }
            }
        } else {
            func_801ED5F4(kind, level, &D_801F5478.unkB0, D_801F5478.unkAC, D_801F5478.unkAE);
            if (D_801F5478.unkB0 == D_801F5478.unkAC || D_801F5478.unkB0 == D_801F5478.unkAE) {
                D_801F5478.unkB0 = -1;
            }
            for (i = 0; D_801F5478.unkB0 == -1;) {
                i++;
                func_801ED5F4(kind, level + i, &D_801F5478.candidates[0], 500, 500);
                func_801ED5F4(kind, level - i, &D_801F5478.candidates[1], 500, 500);
                if (D_801F5478.candidates[0] != -1 && D_801F5478.candidates[1] != -1) {
                    D_801F5478.unkB0 = D_801F5478.candidates[rand() % 2];
                } else if (D_801F5478.candidates[0] != -1) {
                    D_801F5478.unkB0 = D_801F5478.candidates[0];
                } else if (D_801F5478.candidates[1] != -1) {
                    D_801F5478.unkB0 = D_801F5478.candidates[1];
                }
                if (D_801F5478.unkB0 == D_801F5478.unkAC || D_801F5478.unkB0 == D_801F5478.unkAE) {
                    D_801F5478.unkB0 = -1;
                }
            }
            D_801F5478.unkC5 = 0;
        }
    } else {
        D_801F5478.unkC5 = 1;
        D_801F5250->vars[14] = 1;
    }
    switch (D_801F4980[D_801F5478.unkB0]->type) {
    case 0:
        D_801F5250->vars[15] = D_801F4980[D_801F5478.unkB0]->attr & 0xF;
        D_801F5250->vars[16] = D_801F4980[D_801F5478.unkB0]->attr >> 4;
        break;
    case 1:
        D_801F5250->vars[15] = 5;
        D_801F5250->vars[16] = 5;
        break;
    case 2:
        D_801F5250->vars[15] = 6;
        D_801F5250->vars[16] = 6;
        break;
    }
    D_801F5250->vars[17] = D_801F5478.unkC5;
    D_801F5250->vars[11] = D_801F5478.unkB0;
    return D_801F5478.unkB0;
}

void func_801ED5F4(u8 type, u8 level, s16 *out, s16 excludeA, s16 excludeB) {
    s16 candidates[10];
    s16 i;
    s16 j;
    s16 tmp;
    s8 count;

    *out = -1;
    if (level < 2) {
        level = 2;
    }
    if (level >= 36) {
        level = 35;
    }
    count = 0;
    for (i = 0; i < 10; i++) {
        candidates[i] = -1;
    }
    if (type == 5) {
        for (i = D_801EFF00[5].first; i <= D_801EFF00[5].last; i++) {
            if (D_801F4980[i]->level == level && i != excludeA && i != excludeB) {
                candidates[count] = i;
                count++;
            }
        }
        for (i = D_801EFF00[6].first; i <= D_801EFF00[6].last; i++) {
            if (D_801F4980[i]->level == level && i != excludeA && i != excludeB) {
                candidates[count] = i;
                count++;
            }
        }
        for (i = 0; i < count; i++) {
            j = rand() % count;
            tmp = candidates[i];
            candidates[i] = candidates[j];
            candidates[j] = tmp;
        }
        *out = candidates[0];
    } else {
        for (i = D_801EFF00[type].first; i <= D_801EFF00[type].last; i++) {
            if (D_801F4980[i]->level == level) {
                *out = i;
                return;
            }
        }
    }
}

void func_801ED8B0(EvoClut *clut, u16 flags) {
    u16 *src;
    u16 *dst;
    s32 brighten;
    s32 level;
    s32 i;
    s32 color;
    s32 r;
    s32 g;
    s32 b;

    brighten = clut->brighten;
    level = clut->level;
    src = clut->src;
    dst = clut->dst;
    for (i = 0; i < clut->rect.w * clut->rect.h; i++) {
        color = *src++;
        if (color != 0) {
            r = color & 0x1F;
            g = (color >> 5) & 0x1F;
            b = (color >> 10) & 0x1F;
            if (brighten == 0) {
                r = r * level / 255;
                g = g * level / 255;
                b = b * level / 255;
            } else {
                r += (31 - r) * level / 255;
                g += (31 - g) * level / 255;
                b += (31 - b) * level / 255;
            }
            if (r < 0) {
                r = 0;
            } else if (r >= 32) {
                r = 31;
            }
            if (g < 0) {
                g = 0;
            } else if (g >= 32) {
                g = 31;
            }
            if (b < 0) {
                b = 0;
            } else if (b >= 32) {
                b = 31;
            }
            color = flags | (color & 0x8000) | (b << 10) | (g << 5) | r;
        }
        *dst++ = color;
    }
    LoadImage((s16 *)&clut->rect, (s32)clut->dst);
    DrawSync(0);
}

EvoModelFx *func_801EDAE8(s16 level, EvoFx *fx, s32 modelId, s32 anim, s32 unused, s32 vramSlot, u8 arg6,
                          s32 loop, s32 pak, s32 arg9) {
    EvoModelFx *obj;
    s32 slot;

    obj = allocTaskHeapBlock(sizeof(EvoModelFx));
    for (slot = 2; slot < 23; slot++) {
        if (SCENE_3D->modelState[slot] == 0) {
            break;
        }
    }
    if (slot >= 23 || loadModel(slot, modelId, vramSlot, pak, 0) == 0) {
        freeHeapBlock(obj);
        return NULL;
    }
    obj->slot = slot;
    obj->model = SCENE_3D->models[slot];
    if (anim >= 0 && loadModelAnimation(slot, anim, 0, pak) != 0) {
        applyAnimationFirstFrame(slot, 0);
        startModelAnimation(slot, 0, -2, loop ^ 1);
    }
    if (fx != NULL) {
        obj->fx = *fx;
        initEffectObject(obj);
        SCENE_3D->modelState[obj->slot] = -1;
        obj->active = 1;
    } else {
        SCENE_3D->modelState[obj->slot] = 1;
        obj->active = 0;
    }
    obj->unk574 = arg9;
    obj->model->link = (s32)obj;
    obj->unk570 = -1;
    {
        Rect16 rect = { 0x30, 0x70, 0x10, 0x10 };

        if (obj->unk574 == 0) {
            rect.y = 0xF0;
        }
        rect.x += ((obj->model->tpageOffset / 0x10000 + 5) & 0xF) << 6;
        rect.y += ((obj->model->tpageOffset / 0x10000 + 5) >> 4) << 8;
        StoreImage2(&rect, (u32 *)obj->clut.src);
        obj->clut.rect = rect;
        obj->clut.brighten = 0;
        obj->prevLevel = level;
        obj->clut.level = level;
        if (level != 0xFF) {
            func_801ED8B0(&obj->clut, 0x8000);
        }
    }
    obj->unk56F = arg6;
    return obj;
}

void func_801EDE18(EvoModelFx *obj) {
    if (obj->active != 0) {
        if (obj->fx.unk139 != 0) {
            tickEffectStartDelay(obj);
            SCENE_3D->modelState[obj->slot] = -1;
        } else {
            obj->clut.level = updateEffectBrightness(obj, obj->clut.level);
            if (obj->clut.level != obj->prevLevel) {
                obj->prevLevel = obj->clut.level;
                func_801ED8B0(&obj->clut, 0x8000);
            }
            if (obj->clut.level == 0) {
                SCENE_3D->modelState[obj->slot] = -1;
            } else {
                SCENE_3D->modelState[obj->slot] = 3;
            }
        }
    }
}

void func_801EDEF8(EvoModelFx *obj) {
    unloadModel(obj->slot);
    if (obj->clut.level != 0xFF) {
        obj->clut.level = 0xFF;
        func_801ED8B0(&obj->clut, 0x8000);
    }
    freeHeapBlock(obj);
}

EvoFadeRect *func_801EDF48(s16 *rect, Bytes4 *from, Bytes4 *to, u8 blendMode, s16 speed, u8 mode) {
    EvoFadeRect *f;

    f = allocTaskHeapBlock(sizeof(EvoFadeRect));
    initPolyF4Pair(&f->poly[0], &f->poly[1], from, blendMode, &f->tpage[0], &f->tpage[1], rect, 1, 1);
    f->from = *from;
    f->to = *to;
    f->speed = speed;
    f->mode = mode;
    if (mode < 2 || mode == 3) {
        f->state = 1;
    } else {
        f->state = 0;
    }
    return f;
}

s32 func_801EE040(EvoFadeRect *f) {
    PolyF4 *poly;
    DrTPage *tpage;
    s32 doneIn = 0;
    s32 doneOut = 0;

    if (f->state == 0) {
        return -1;
    }
    poly = &f->poly[FRAME_BUFFER_INDEX];
    tpage = &f->tpage[FRAME_BUFFER_INDEX];
    if (f->state == 1) {
        doneIn = stepColorToward(f->speed, &poly->r0, f->to.b[0], &poly->g0, f->to.b[1], &poly->b0, f->to.b[2]);
    } else if (f->state == 2) {
        doneOut = stepColorToward(f->speed, &poly->r0, f->from.b[0], &poly->g0, f->from.b[1], &poly->b0, f->from.b[2]);
        if (doneOut != 0 && f->mode == 3) {
            f->state = 1;
        }
    }
    if (*(u32 *)&poly->r0 & 0xFFFFFF) {
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[1], (s32)poly);
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[1], (s32)tpage);
    }
    if (doneIn != 0 && (f->mode == 1 || f->mode == 3)) {
        f->state = 2;
    }
    if (doneIn == 0 && doneOut == 0) {
        return 0;
    }
    if (doneIn != 0) {
        return 1;
    }
    if (doneOut != 0) {
        return 2;
    }
    return -1;
}

s32 func_801EE1F4(s32 index) {
    char path[32];
    s32 file;

    sprintf(path, "C:\\EVO_PAK\\%d.PAK", index);
    file = loadFileTagged((s32 *)path, getCurrentTaskId(), 0x12C);
    if (file != 0) {
        return file;
    }
    return 0;
}

void func_801EE248(void) {
    func_800149B8(0, -1, 0, 0x800, loadFileTagged, "C:\\Unit_eff.arc", getCurrentTaskId(), -2);
    D_801F59A8 = (u8 *)func_80014C08(0x7FFFFFFF);
}

void func_801EE2B4(void) {
    freeHeapBlock(D_801F59A8);
}

void func_801EE2DC(s32 index, s32 arg) {
    func_801EE330(index, arg, arg, 0, 0);
}

void func_801EE304(s32 index, s32 arg, s32 arg2) {
    func_801EE330(index, arg, arg, arg2, arg2);
}

void func_801EE330(s32 index, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 data;

    D_801F59A4 = 0;
    D_801F59A5 = 0;
    D_801F59A6 = 0;
    data = decompressArchiveEntry((s32)D_801F59A8, index);
    func_800149B8(0, 0x1F, 0, 0x800, func_801EF7DC, data, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    freeHeapBlock((void *)data);
}

s32 func_801EE3BC(EvoLoader *loader) {
    s32 i;

    PushMatrix();
    tickEffectMotion((s32)&D_801F5868, 0);
    PopMatrix();
    loader->vars[0] = 1;
    func_801EE69C(loader);
    for (i = 0; i < 16; i++) {
        if (loader->entries[i].active != 0 && D_801F0280[loader->entries[i].kind] != NULL) {
            D_801F0280[loader->entries[i].kind](loader->entries[i].handle);
            if (loader->entries[i].kind > 0) {
                loader->vars[i + 88] = loader->entries[i].handle->unk118;
                loader->vars[i + 120] = loader->entries[i].handle->unk11C;
            }
        }
    }
    return loader->vars[0];
}

void func_801EE4E8(EvoLoader *loader) {
    s32 i;

    func_80014C08(FRAME_INTERVAL);
    for (i = 0; i < 16; i++) {
        if (loader->entries[i].kind != -1) {
            D_801F02A8[loader->entries[i].kind](loader->entries[i].handle);
        }
    }
    func_80014C08(FRAME_INTERVAL);
    if (loader->buffer != NULL) {
        freeHeapBlock(loader->buffer);
    }
}

void func_801EE5B0(s32 kind, EvoObject *obj) {
    switch (kind) {
    case 0:
        obj->x = 28;
        obj->y = -13;
        obj->z = 0;
        break;
    case 1:
        obj->x = -106;
        obj->y = -13;
        obj->z = 0;
        break;
    case 2:
        obj->x = -12;
        obj->y = -21;
        obj->z = 0;
        break;
    case 3:
        obj->x = -90;
        obj->y = -21;
        obj->z = 0;
        break;
    case 4:
        obj->x = -59;
        obj->y = -21;
        obj->z = 0;
        break;
    default:
        obj->x = 0;
        obj->y = 0;
        obj->z = 0;
        break;
    }
}

void func_801EE638(s32 index, EvoLight *light) {
    Color color;

    if (index >= 0) {
        color.r = light->r;
        color.g = light->g;
        color.b = light->b;
        func_801F893C(SPRITE(index), &color);
    }
}

void func_801EE69C(EvoLoader *loader) {
    s32 *vars;
    s32 result;
    s32 index;

    if (loader->counter != 0) {
        loader->counter--;
        return;
    }
    vars = loader->vars;
    do {
        result = runScriptToNextEvent(loader->script, vars);
        if (result == 1) {
            switch (loader->script->eventOp) {
            case 10:
                switch (loader->script->eventArg) {
                case 0:
                    func_801EED18(&D_801F5868, (EvoFxParams *)vars, 0);
                    break;
                case 1:
                    func_801EEF24(&D_801F5868, (EvoFxParams *)vars);
                    restartEffectMotion((u8 *)&D_801F5868);
                    break;
                case 2:
                    D_800794E7 = 1;
                    break;
                case 3:
                    D_800794E7 = 0;
                    break;
                case 4:
                    D_800795A8 = 0;
                    break;
                case 5:
                    D_800795A8 = 1;
                    break;
                case 6:
                    func_801EE5B0(D_801F59A5, (EvoObject *)vars);
                    break;
                case 7:
                    func_801EE5B0(D_801F59A6, (EvoObject *)vars);
                    break;
                case 8:
                    func_801EE638(D_801F59A5, (EvoLight *)vars);
                    break;
                case 9:
                    func_801EE638(D_801F59A6, (EvoLight *)vars);
                    break;
                case 10:
                    index = D_801F59A5;
                    if (index >= 0) {
                        func_801F8928(SPRITE(index));
                    }
                    break;
                case 11:
                    index = D_801F59A6;
                    if (index >= 0) {
                        func_801F8928(SPRITE(index));
                    }
                    break;
                case 12:
                    D_801F5548[0].unk120 = 0;
                    D_801F5548[1].unk120 = 1;
                    break;
                case 13:
                    D_801F540A = 1;
                    break;
                case 14:
                    D_801F540A = 0;
                    break;
                case 15:
                    D_801F5548[0].unk124 = 0x3A;
                    D_801F5548[1].unk124 = -0x58;
                    break;
                }
                break;
            case 11:
                switch (loader->script->eventArg) {
                case 0:
                    playSoundEffect((s16)loader->script->params[0]);
                    break;
                case 1:
                    func_801EED18(loader->entries[(s16)loader->script->params[0]].handle, (EvoFxParams *)vars, 0);
                    break;
                case 2:
                    func_801EF108(loader->entries[(s16)loader->script->params[0]].handle, vars, loader);
                    initEffectObject(loader->entries[(s16)loader->script->params[0]].handle);
                    break;
                case 3:
                    stopSoundVoice(loader->script->params[0]);
                    break;
                case 4:
                    if (loader->entries[(s16)loader->script->params[0]].handle != NULL) {
                        loader->entries[(s16)loader->script->params[0]].active = 1;
                    }
                    break;
                case 5:
                    loader->entries[(s16)loader->script->params[0]].active = 0;
                    break;
                case 6:
                    loader->counter = (s16)loader->script->params[0] - 1;
                    return;
                case 7:
                    func_801EF0BC(loader->entries[(s16)loader->script->params[0]].handle, (EvoObject *)vars);
                    break;
                case 8:
                    func_801EED18(loader->entries[(s16)loader->script->params[0]].handle, (EvoFxParams *)vars, 1);
                    break;
                case 9:
                    func_801EE5B0(getActiveDigimonCard(D_801F59A4 ^ (s16)loader->script->params[0]), (EvoObject *)vars);
                    break;
                case 10:
                    func_801EE5B0(getPlayedCard(D_801F59A4 ^ (s16)loader->script->params[0]), (EvoObject *)vars);
                    break;
                case 11:
                    func_801EE5B0(peekOnlineDeckTop(D_801F59A4 ^ (s16)loader->script->params[0]), (EvoObject *)vars);
                    break;
                case 12:
                    func_801EE5B0(peekOfflineDeckTop(D_801F59A4 ^ (s16)loader->script->params[0]), (EvoObject *)vars);
                    break;
                case 15:
                    func_801EE638(peekOnlineDeckTop(D_801F59A4 ^ (s16)loader->script->params[0]), (EvoLight *)vars);
                    break;
                case 16:
                    func_801EE638(peekOfflineDeckTop(D_801F59A4 ^ (s16)loader->script->params[0]), (EvoLight *)vars);
                    break;
                case 18:
                    printf("NO USE\n");
                    break;
                case 19:
                    index = D_801F59A5;
                    if (index >= 0) {
                        func_801F8910(SPRITE(index), (s16)loader->script->params[0]);
                    }
                    break;
                case 20:
                    index = D_801F59A6;
                    if (index >= 0) {
                        func_801F8910(SPRITE(index), (s16)loader->script->params[0]);
                    }
                    break;
                case 21:
                    func_801EE5B0((s16)loader->script->params[0], (EvoObject *)vars);
                    break;
                case 22:
                    loader->buffer = (void *)func_801EE1F4((s16)loader->script->params[0]);
                    break;
                case 23:
                    animateWindowTo(&D_801F51B8, (Rect16 *)-1);
                    break;
                }
                break;
            case 12:
                switch (loader->script->eventArg) {
                case 6:
                    func_801EF5C8((s16)loader->script->params[0], (s16)loader->script->params[1], vars, loader);
                    break;
                case 1:
                    vars[1] = rsin((s16)loader->script->params[1]) * (s16)loader->script->params[0] / 4096;
                    break;
                case 2:
                    vars[1] = rcos((s16)loader->script->params[1]) * (s16)loader->script->params[0] / 4096;
                    break;
                case 3:
                    playSoundEffectOnVoice((s16)loader->script->params[0], (s16)loader->script->params[1]);
                    break;
                case 5:
                    printf("NO USE\n");
                    break;
                }
                break;
            }
        }
        clearScriptBusy(loader->script);
    } while (result != 0);
}

void func_801EED18(EvoFx *fx, EvoFxParams *params, s32 current) {
    if (current == 0) {
        params->args[0] = fx->rot.vx;
        params->args[1] = fx->rot.vy;
        params->args[2] = fx->rot.vz;
        params->args[8] = fx->scale.vx;
        params->args[9] = fx->scale.vy;
        params->args[10] = fx->scale.vz;
        params->args[17] = fx->pos.vx;
        params->args[18] = fx->pos.vy;
        params->args[19] = fx->pos.vz;
    } else {
        params->args[0] = fx->curRot.vx;
        params->args[1] = fx->curRot.vy;
        params->args[2] = fx->curRot.vz;
        params->args[8] = fx->curScale.vx;
        params->args[9] = fx->curScale.vy;
        params->args[10] = fx->curScale.vz;
        params->args[17] = fx->curPos[0];
        params->args[18] = fx->curPos[1];
        params->args[19] = fx->curPos[2];
    }
    params->args[3] = fx->rotVel.vx;
    params->args[4] = fx->rotVel.vy;
    params->args[5] = fx->rotVel.vz;
    params->args[6] = fx->unk120;
    params->args[7] = fx->unk122;
    params->args[11] = fx->scaleVel.vx;
    params->args[12] = fx->scaleVel.vy;
    params->args[13] = fx->scaleVel.vz;
    params->args[14] = fx->scaleAccel.vx;
    params->args[15] = fx->scaleAccel.vy;
    params->args[16] = fx->scaleAccel.vz;
    params->args[20] = fx->vel.vx;
    params->args[21] = fx->vel.vy;
    params->args[22] = fx->vel.vz;
    params->args[23] = fx->accel.vx;
    params->args[24] = fx->accel.vy;
    params->args[25] = fx->accel.vz;
    params->args[26] = fx->unk12C;
    params->args[27] = fx->unk124;
    params->args[28] = fx->unk137;
    params->args[29] = fx->unk130;
    params->args[31] = fx->unk128;
    params->args[32] = fx->unk12A;
    params->args[33] = fx->unk126;
    params->args[30] = fx->unk12E;
}

void func_801EEF24(EvoFx *fx, EvoFxParams *params) {
    fx->rot.vx = params->args[0];
    fx->rot.vy = params->args[1];
    fx->rot.vz = params->args[2];
    fx->rotVel.vx = params->args[3];
    fx->rotVel.vy = params->args[4];
    fx->rotVel.vz = params->args[5];
    fx->unk120 = params->args[6];
    fx->unk122 = params->args[7];
    fx->scale.vx = params->args[8];
    fx->scale.vy = params->args[9];
    fx->scale.vz = params->args[10];
    fx->scaleVel.vx = params->args[11];
    fx->scaleVel.vy = params->args[12];
    fx->scaleVel.vz = params->args[13];
    fx->scaleAccel.vx = params->args[14];
    fx->scaleAccel.vy = params->args[15];
    fx->scaleAccel.vz = params->args[16];
    fx->pos.vx = params->args[17];
    fx->pos.vy = params->args[18];
    fx->pos.vz = params->args[19];
    fx->vel.vx = params->args[20];
    fx->vel.vy = params->args[21];
    fx->vel.vz = params->args[22];
    fx->accel.vx = params->args[23];
    fx->accel.vy = params->args[24];
    fx->accel.vz = params->args[25];
    fx->unk12C = params->args[26];
    fx->unk124 = params->args[27];
    fx->unk137 = params->args[28];
    fx->unk130 = params->args[29];
    fx->unk128 = params->args[31];
    fx->unk12A = params->args[32];
    fx->unk126 = params->args[33];
    fx->unk12E = params->args[30];
}

void func_801EF0BC(void *xform, EvoObject *obj) {
    SVECTOR pos;

    getTransformWorldPos(xform, &pos);
    obj->x = pos.vx;
    obj->y = pos.vy;
    obj->z = pos.vz;
}

void func_801EF108(EvoFx *fx, s32 *vars, EvoLoader *loader) {
    func_801EEF24(fx, (EvoFxParams *)vars);
    if (vars[186] == -2) {
        fx->unk98 = NULL;
    } else if (vars[186] == -1) {
        fx->unk98 = &D_801F5868;
    } else {
        fx->unk98 = loader->entries[vars[186]].handle;
    }
}

EvoFx *func_801EF188(s32 *vars) {
    Rect16 rect;
    Bytes4 from;
    Bytes4 to;

    rect.x = vars[75];
    rect.y = vars[76];
    rect.w = vars[77];
    rect.h = vars[78];
    from.b[0] = vars[38];
    from.b[1] = vars[39];
    from.b[2] = vars[40];
    to.b[0] = vars[41];
    to.b[1] = vars[42];
    to.b[2] = vars[43];
    return (EvoFx *)func_801EDF48((s16 *)&rect, &from, &to, vars[28], vars[29], vars[69]);
}

EvoFx *func_801EF244(s32 *vars, EvoLoader *loader) {
    Bytes4 inner;
    Bytes4 mid;
    Bytes4 outer;
    EvoFx fx;

    func_801EF108(&fx, vars, loader);
    inner.b[0] = vars[38];
    inner.b[1] = vars[39];
    inner.b[2] = vars[40];
    mid.b[0] = vars[41];
    mid.b[1] = vars[42];
    mid.b[2] = vars[43];
    outer.b[0] = vars[44];
    outer.b[1] = vars[45];
    outer.b[2] = vars[46];
    return (EvoFx *)createRingEffect(vars[56], &inner, &mid, &outer, (EffectTemplate *)&fx, vars[22], vars[27], vars[28],
                                     vars[71], vars[62], vars[63], vars[66], vars[64], vars[65], NULL, 0, 0, 0,
                                     vars[70], vars[73], vars[74], 0);
}

EvoFx *func_801EF3A8(s32 *vars, EvoLoader *loader) {
    EvoFx fx;

    func_801EF108(&fx, vars, loader);
    return (EvoFx *)cloneEffectObject((EffectTemplate *)&fx);
}

EvoFx *func_801EF3DC(s32 *vars, EvoLoader *loader) {
    EvoFx fx;
    EvoFx *template = &fx;

    func_801EF108(template, vars, loader);
    return (EvoFx *)func_801EDAE8(vars[56], template, vars[84], vars[85], -1, vars[86], vars[70], vars[87], (s32)loader->buffer, 0);
}

EvoFx *func_801EF474(s32 *vars, EvoLoader *loader) {
    EvoFx fx;
    Bytes4 start;
    Bytes4 end;

    start.b[0] = vars[38];
    start.b[1] = vars[39];
    start.b[2] = vars[40];
    end.b[0] = vars[41];
    end.b[1] = vars[42];
    end.b[2] = vars[43];
    func_801EF108(&fx, vars, loader);
    return (EvoFx *)createStreakParticles(start.b, end.b, (EffectTemplate *)&fx, vars[189], vars[188], vars[190], vars[195],
                                          vars[191], vars[192], vars[193], vars[194], vars[80], vars[81], vars[79],
                                          vars[82], vars[27], vars[70], vars[74]);
}

void func_801EF5A8(void *obj) {
    freeHeapBlock(obj);
}

void func_801EF5C8(s32 index, s32 kind, s32 *vars, EvoLoader *loader) {
    if (D_801F0294[kind] != NULL) {
        loader->entries[index].kind = kind;
        loader->entries[index].active = 0;
        loader->entries[index].handle = D_801F0294[kind](vars, loader);
        loader->counter++;
        if ((loader->counter & 0xF) == 0) {
            func_80014C08(FRAME_INTERVAL);
        }
    }
}

EvoLoader *func_801EF65C(EvoMsd *data) {
    EvoFx fx;
    EvoLoader *loader;
    s32 i;

    loader = allocTaskHeapBlock(sizeof(EvoLoader));
    loader->data = data;
    loader->script = createScriptContext(data);
    loader->vars = allocScriptRegisters(0xC4);
    for (i = 0; i < 16; i++) {
        loader->entries[i].kind = -1;
        loader->entries[i].active = 0;
        loader->entries[i].handle = NULL;
    }
    loader->counter = 0;
    loader->buffer = NULL;
    fx.rot.vx = 0;
    fx.rot.vy = 0;
    fx.rot.vz = 0;
    fx.rotVel.vx = 0;
    fx.rotVel.vy = 0;
    fx.rotVel.vz = 0;
    fx.scale.vx = 0;
    fx.scale.vy = 0;
    fx.scale.vz = 0;
    fx.scaleVel.vx = 0;
    fx.scaleVel.vy = 0;
    fx.scaleVel.vz = 0;
    fx.scaleAccel.vx = 0;
    fx.scaleAccel.vy = 0;
    fx.scaleAccel.vz = 0;
    fx.pos.vx = 0x1000;
    fx.pos.vy = 0x1000;
    fx.pos.vz = 0x1000;
    fx.vel.vx = 0x1000;
    fx.vel.vy = 0x1000;
    fx.vel.vz = 0x1000;
    fx.accel.vx = 0;
    fx.accel.vy = 0;
    fx.accel.vz = 0;
    fx.unk137 = 0;
    fx.unk130 = 0;
    fx.unk12C = 0x80;
    fx.unk98 = SCENE_3D->unk78;
    fx.unk12E = 0;
    D_801F5868 = fx;
    initEffectObject(&D_801F5868);
    func_801EE69C(loader);
    return loader;
}

void func_801EF7DC(EvoMsd *data, s32 parentTask) {
    EvoLoader *loader;

    loader = func_801EF65C(data);
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (func_801EE3BC(loader) != 0);
    func_801EE4E8(loader);
    freeScriptContext(loader->script, loader->vars);
    freeHeapBlock(loader);
    func_80014A48(parentTask);
}

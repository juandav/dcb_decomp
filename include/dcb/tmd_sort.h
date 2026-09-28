#ifndef DCB_TMD_SORT_H
#define DCB_TMD_SORT_H

#include "game.h"

#define SORT_WORK ((SortWork *)0x1F800000)
#define STRIP_DRAW(draw)                                                                                            \
    {                                                                                                                \
        gte_avsz3();                                                                                                 \
        packet = draw(emitEnvMapTriangle(packet, SORT_WORK->ot, gouraud, SORT_WORK->code), SORT_WORK->ot, gouraud,           \
                   SORT_WORK->code);                                                                                 \
    }

typedef struct {
    /* 0x00 */ u32 *data;
    /* 0x04 */ u32 *work;
    /* 0x08 */ u32 *ot;
    /* 0x0C */ u32 packet;
    /* 0x10 */ s32 pass;
    /* 0x14 */ u32 code;
    /* 0x18 */ u32 textured;
    /* 0x1C */ u32 quad;
    /* 0x20 */ void *otSize;
    /* 0x24 */ u32 count[2];
    /* 0x2C */ u32 envRgbCode;
    /* 0x30 */ u32 tpage;
    /* 0x34 */ u32 clut;
    /* 0x38 */ u32 inlineTexture;
} SortWork;

extern ModelTextureSlot MODEL_TEXTURE_SLOTS[];

void loadTriangleToGte(u32 index0, u32 *indices, u8 *workBuf);
void loadGteVertex0(s32 gouraud, u32 index, u8 *workBuf);
void loadGteVertex0Nclip(s32 gouraud, u32 index, u8 *workBuf);
void loadGteVertex1(s32 gouraud, u32 index, u8 *workBuf);
void loadGteVertex1Nclip(s32 gouraud, u32 index, u8 *workBuf);
void loadGteVertex2(s32 gouraud, u32 index, u8 *workBuf);
void loadGteVertex2Nclip(s32 gouraud, u32 index, u8 *workBuf);
void loadGteQuadVertex3(s32 gouraud, u32 index, u8 *workBuf);
u32 *emitTexturedTriangle(u32 *packet, u32 *ot, s32 gouraud, u32 code);
u32 *emitTexturedQuad(u32 *packet, u32 *ot, s32 gouraud, u32 code);
u32 *emitUntexturedTriangle(u32 *packet, u32 *ot, s32 gouraud, u32 code);
u32 *emitUntexturedQuad(u32 *packet, u32 *ot, s32 gouraud, u32 code);
u32 *transformAndLightVertices(u32 *, u32 *);
void sortModelPrimitives(SortWork *);
u32 *transformVerticesWithEnvMap(u32 *, u32 *);
void sortEnvMappedPrimitives(SortWork *w);
u32 sortModelObject(u32 *data, u32 *ot, u32 packet, void *otSize);
u32 sortEnvMappedModelObject(u32 *data, u32 *ot, u32 packet, void *otSize);
void loadEnvGteVertex0(s32 gouraud, u32 index, u8 *workBuf);
void loadEnvGteVertex1(s32 gouraud, u32 index, u8 *workBuf);
void loadEnvGteVertex2(s32 gouraud, u32 index, u8 *workBuf);
u32 *emitEnvMapTriangle(u32 *packet, u32 *ot, s32 gouraud, u32 code);

#endif /* DCB_TMD_SORT_H */

#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/tmd_sort.h"
#include "dcb/main.h"
#include "dcb/task.h"

Unk8006DF60 MODEL_TEXTURE_SLOTS[4] = {
    { 0x002F0000, 0x103C0000 },
    { 0x002F0040, 0x103D0040 },
    { 0x002F0080, 0x103E0080 },
    { 0x002F00C0, 0x103F00C0 },
};

INCLUDE_ASM("asm/main/nonmatchings/gfx/tmd_sort", transformAndLightVertices);

void loadTriangleToGte(u32 index0, u32 *indices, u8 *workBuf) {
    u8 *vert0;
    u8 *vert1;
    u8 *vert2;
    u8 *color0;
    u8 *color1;
    u32 index1;
    u32 index2;

    vert0 = workBuf + (index0 >> 16);
    gte_lwc2(12, 0, vert0);
    gte_lwc2(17, 4, vert0);
    index1 = indices[1];
    index2 = indices[2];
    vert1 = workBuf + (index1 >> 16);
    vert2 = workBuf + (index2 >> 16);
    gte_lwc2(13, 0, vert1);
    gte_lwc2(18, 4, vert1);
    gte_lwc2(14, 0, vert2);
    gte_lwc2(19, 4, vert2);
    color0 = workBuf + (index0 & 0xFFFF);
    gte_nclip();
    color1 = workBuf + (index1 & 0xFFFF);
    workBuf += index2 & 0xFFFF;
    gte_lwc2(20, 0, color0);
    gte_lwc2(21, 0, color1);
    gte_lwc2(22, 0, workBuf);
}

void loadGteVertex0(s32 gouraud, u32 index, u8 *workBuf) {
    u8 *vertex;

    vertex = workBuf + (index >> 16);
    workBuf += index & 0xFFFF;
    gte_lwc2(12, 0, vertex);
    gte_lwc2(17, 4, vertex);
    gte_lwc2(20, 0, workBuf);
}

void loadGteVertex0Nclip(s32 gouraud, u32 index, u8 *workBuf) {
    u8 *vertex;

    vertex = workBuf + (index >> 16);
    gte_lwc2(12, 0, vertex);
    gte_lwc2(17, 4, vertex);
    workBuf += index & 0xFFFF;
    gte_nclip();
    gte_lwc2(20, 0, workBuf);
}

void loadGteVertex1(s32 gouraud, u32 index, u8 *workBuf) {
    u8 *vertex;

    vertex = workBuf + (index >> 16);
    workBuf += index & 0xFFFF;
    gte_lwc2(13, 0, vertex);
    gte_lwc2(18, 4, vertex);
    if (gouraud == 0) {
        gte_lwc2(20, 0, workBuf);
    }
    gte_lwc2(21, 0, workBuf);
}

void loadGteVertex1Nclip(s32 gouraud, u32 index, u8 *workBuf) {
    u8 *vertex;

    vertex = workBuf + (index >> 16);
    gte_lwc2(13, 0, vertex);
    gte_lwc2(18, 4, vertex);
    workBuf += index & 0xFFFF;
    gte_nclip();
    gte_lwc2(21, 0, workBuf);
    if (gouraud == 0) {
        gte_lwc2(20, 0, workBuf);
    }
}

void loadGteVertex2(s32 gouraud, u32 index, u8 *workBuf) {
    u8 *vertex;

    vertex = workBuf + (index >> 16);
    workBuf += index & 0xFFFF;
    gte_lwc2(14, 0, vertex);
    gte_lwc2(19, 4, vertex);
    if (gouraud == 0) {
        gte_lwc2(20, 0, workBuf);
    }
    gte_lwc2(22, 0, workBuf);
}

void loadGteVertex2Nclip(s32 gouraud, u32 index, u8 *workBuf) {
    u8 *vertex;

    vertex = workBuf + (index >> 16);
    gte_lwc2(14, 0, vertex);
    gte_lwc2(19, 4, vertex);
    workBuf += index & 0xFFFF;
    gte_nclip();
    gte_lwc2(22, 0, workBuf);
    if (gouraud == 0) {
        gte_lwc2(20, 0, workBuf);
    }
}

void loadGteQuadVertex3(s32 gouraud, u32 index, u8 *workBuf) {
    u8 *vertex;

    vertex = workBuf + (index >> 16);
    workBuf += index & 0xFFFF;
    gte_lwc2(0, 0, vertex);
    gte_lwc2(16, 4, vertex);
    if (gouraud == 0) {
        gte_lwc2(20, 0, workBuf);
    }
    gte_lwc2(6, 0, workBuf);
}

u32 *emitTexturedTriangle(u32 *packet, u32 *ot, s32 gouraud, u32 code) {
    u32 *next;
    register u32 rgb asm("$8");
    u32 len;
    u32 otz;
    u32 tag;

    gte_mfc2(20, rgb);
    gte_swc2(12, 8, packet);
    packet[1] = rgb | code;
    next = packet;
    if (gouraud) {
        gte_swc2(2, 12, packet);
        gte_swc2(21, 16, packet);
        gte_swc2(13, 20, packet);
        gte_swc2(4, 24, packet);
        gte_swc2(22, 28, packet);
        gte_swc2(14, 32, packet);
        gte_swc2(3, 36, packet);
        gte_mfc2(7, otz);
        len = 0x09000000;
        next = packet + 10;
    } else {
        gte_swc2(2, 12, packet);
        gte_swc2(13, 16, packet);
        gte_swc2(4, 20, packet);
        gte_swc2(14, 24, packet);
        gte_swc2(3, 28, packet);
        gte_mfc2(7, otz);
        len = 0x07000000;
        next += 8;
    }
    if (otz >= (u32)SORT_WORK->unk20) {
        return packet;
    }
    tag = len | ot[otz];
    ot[otz] = (u32)packet;
    *packet = tag;
    return next;
}

u32 *emitTexturedQuad(u32 *packet, u32 *ot, s32 gouraud, u32 code) {
    register u32 rgb asm("$8");
    u32 *next;
    u32 len;
    u32 otz;
    u32 tag;

    gte_mfc2(20, rgb);
    gte_swc2(12, 8, packet);
    packet[1] = rgb | code;
    next = packet;
    if (gouraud) {
        gte_swc2(2, 12, packet);
        gte_swc2(21, 16, packet);
        gte_swc2(13, 20, packet);
        gte_swc2(4, 24, packet);
        gte_swc2(22, 28, packet);
        gte_swc2(14, 32, packet);
        gte_swc2(3, 36, packet);
        gte_mfc2(7, otz);
        len = 0x0C000000;
        next = packet + 13;
        gte_swc2(6, 40, packet);
        gte_swc2(0, 44, packet);
        gte_swc2(5, 48, packet);
    } else {
        gte_swc2(2, 12, packet);
        gte_swc2(13, 16, packet);
        gte_swc2(4, 20, packet);
        gte_swc2(14, 24, packet);
        gte_swc2(3, 28, packet);
        gte_swc2(0, 32, packet);
        gte_swc2(5, 36, packet);
        gte_mfc2(7, otz);
        len = 0x09000000;
        next += 10;
    }
    if (otz >= (u32)SORT_WORK->unk20) {
        return packet;
    }
    tag = len | ot[otz];
    ot[otz] = (u32)packet;
    *packet = tag;
    return next;
}

u32 *emitUntexturedTriangle(u32 *packet, u32 *ot, s32 gouraud, u32 code) {
    register u32 rgb asm("$8");
    u32 *next;
    u32 len;
    u32 otz;
    u32 tag;

    gte_mfc2(20, rgb);
    gte_swc2(12, 8, packet);
    packet[1] = rgb | code;
    next = packet;
    if (gouraud) {
        gte_swc2(21, 12, packet);
        gte_swc2(13, 16, packet);
        gte_swc2(22, 20, packet);
        gte_swc2(14, 24, packet);
        gte_mfc2(7, otz);
        len = 0x06000000;
        next = packet + 7;
    } else {
        gte_swc2(13, 12, packet);
        gte_swc2(14, 16, packet);
        gte_mfc2(7, otz);
        len = 0x04000000;
        next += 5;
    }
    if (otz >= (u32)SORT_WORK->unk20) {
        return packet;
    }
    tag = len | ot[otz];
    ot[otz] = (u32)packet;
    *packet = tag;
    return next;
}

u32 *emitUntexturedQuad(u32 *packet, u32 *ot, s32 gouraud, u32 code) {
    register u32 rgb asm("$8");
    u32 *next;
    u32 len;
    u32 otz;
    u32 tag;

    gte_mfc2(20, rgb);
    gte_swc2(12, 8, packet);
    packet[1] = rgb | code;
    next = packet;
    if (gouraud) {
        gte_swc2(21, 12, packet);
        gte_swc2(13, 16, packet);
        gte_swc2(22, 20, packet);
        gte_swc2(14, 24, packet);
        gte_mfc2(7, otz);
        len = 0x08000000;
        next = packet + 9;
        gte_swc2(6, 28, packet);
        gte_swc2(0, 32, packet);
    } else {
        gte_swc2(13, 12, packet);
        gte_swc2(14, 16, packet);
        gte_swc2(0, 20, packet);
        gte_mfc2(7, otz);
        len = 0x05000000;
        next += 6;
    }
    if (otz >= (u32)SORT_WORK->unk20) {
        return packet;
    }
    tag = len | ot[otz];
    ot[otz] = (u32)packet;
    *packet = tag;
    return next;
}

INCLUDE_ASM("asm/main/nonmatchings/gfx/tmd_sort", sortModelPrimitives);

u32 sortModelObject(u32 *data, u32 *ot, u32 packet, void *otSize) {
    SortWork *work;
    s32 partCount;

    work = SORT_WORK;
    work->data = data;
    work->ot = ot;
    work->packet = packet & 0xFFFFFF;
    work->work = (u32 *)0x1F80007C;
    work->unk20 = otSize;
    partCount = *data++;
    work->data = data;
    for (; partCount > 0; partCount--) {
        SORT_WORK->data = transformAndLightVertices(SORT_WORK->data, SORT_WORK->work);
        sortModelPrimitives(work);
    }
    return SORT_WORK->packet;
}

u32 sortEnvMappedModelObject(u32 *data, u32 *ot, u32 packet, void *otSize) {
    SortWork *work;
    s32 partCount;

    work = SORT_WORK;
    work->data = data;
    work->ot = ot;
    work->packet = packet & 0xFFFFFF;
    work->work = (u32 *)0x1F80007C;
    work->unk20 = otSize;
    partCount = *data++;
    work->data = data;
    for (; partCount > 0; partCount--) {
        if (*SORT_WORK->data++ != 0) {
            SORT_WORK->data = transformVerticesWithEnvMap(SORT_WORK->data, SORT_WORK->work);
            sortEnvMappedPrimitives(work);
        } else {
            SORT_WORK->data = transformAndLightVertices(SORT_WORK->data, SORT_WORK->work);
            sortModelPrimitives(work);
        }
    }
    return SORT_WORK->packet;
}

INCLUDE_ASM("asm/main/nonmatchings/gfx/tmd_sort", transformVerticesWithEnvMap);

void loadEnvGteVertex0(s32 gouraud, u32 index, u8 *workBuf) {
    u8 *vertex;

    vertex = workBuf + (index >> 16);
    gte_lwc2(12, 0, vertex);
    gte_lwc2(17, 4, vertex);
    workBuf += index & 0xFFFF;
    gte_nclip();
    gte_lwc2(20, 0, workBuf);
    gte_lwc2(25, 4, workBuf);
}

void loadEnvGteVertex1(s32 gouraud, u32 index, u8 *workBuf) {
    u8 *vertex;

    vertex = workBuf + (index >> 16);
    gte_lwc2(13, 0, vertex);
    gte_lwc2(18, 4, vertex);
    workBuf += index & 0xFFFF;
    gte_nclip();
    gte_lwc2(21, 0, workBuf);
    gte_lwc2(26, 4, workBuf);
    if (gouraud == 0) {
        gte_lwc2(20, 0, workBuf);
        gte_lwc2(25, 4, workBuf);
    }
}

void loadEnvGteVertex2(s32 gouraud, u32 index, u8 *workBuf) {
    u8 *vertex;

    vertex = workBuf + (index >> 16);
    gte_lwc2(14, 0, vertex);
    gte_lwc2(19, 4, vertex);
    workBuf += index & 0xFFFF;
    gte_nclip();
    gte_lwc2(22, 0, workBuf);
    gte_lwc2(27, 4, workBuf);
    if (gouraud == 0) {
        gte_lwc2(20, 0, workBuf);
        gte_lwc2(25, 4, workBuf);
    }
}

u32 *emitEnvMapTriangle(u32 *packet, u32 *ot, s32 gouraud, u32 code) {
    SortWork *work;
    s32 envUv0;
    s32 envUv1;
    s32 envUv2;
    u32 otz;
    u32 tag;

    gte_mfc2(25, envUv0);
    gte_mfc2(26, envUv1);
    gte_mfc2(27, envUv2);
    if (envUv0 > 0 && envUv1 > 0 && envUv2 > 0) {
        return packet;
    }
    work = SORT_WORK;
    packet[1] = work->unk2C;
    packet[3] = (envUv0 & 0xFFFF) | work->clut;
    packet[5] = (envUv1 & 0xFFFF) | work->tpage;
    packet[7] = (envUv2 | work->tpage) & 0xFFFF;
    gte_swc2(12, 8, packet);
    gte_swc2(13, 16, packet);
    gte_swc2(14, 24, packet);
    gte_mfc2(7, otz);
    if (otz >= (u32)work->unk20) {
        return packet;
    }
    tag = ot[otz] | 0x07000000;
    ot[otz] = (u32)packet;
    *packet = tag;
    return packet + 8;
}

void sortEnvMappedPrimitives(SortWork *w) {
    u32 *packet;
    u32 *cursor;
    u32 header;
    s32 texInfo;
    u32 code;
    s32 gouraud;
    u32 idx;
    u32 stripCount;
    u32 stripLength;
    u32 k;
    register s32 nclip asm("$4");
    u8 *base;
    u8 *p0;
    u8 *p1;
    u8 *p2;
    u8 *q0;
    u8 *q1;
    u8 *q2;
    u32 i1;
    u32 i2;

    packet = (u32 *)SORT_WORK->packet;
    cursor = SORT_WORK->data;
    while ((header = *cursor++) != 0) {
        SORT_WORK->count[0] = header >> 20;
        SORT_WORK->count[1] = (header >> 8) & 0xFFF;
        code = header << 24;
        SORT_WORK->code = code;
        SORT_WORK->quad = code & 0x08000000;
        SORT_WORK->textured = code & 0x04000000;
        gouraud = code & 0x10000000;
        texInfo = *cursor++;
        if (texInfo >= 0) {
            SORT_WORK->unk38 = 0;
            SORT_WORK->unk2C = (texInfo & 0xFFFFFF) | 0x26000000;
            SORT_WORK->clut = MODEL_TEXTURE_SLOTS[texInfo >> 24].clut;
            SORT_WORK->tpage = MODEL_TEXTURE_SLOTS[texInfo >> 24].tpage;
        } else {
            SORT_WORK->unk38 = 1;
            SORT_WORK->unk2C = (texInfo & 0xFFFFFF) | 0x26000000;
            SORT_WORK->clut = *cursor++;
            SORT_WORK->tpage = *cursor++;
        }
        idx = *cursor;
        for (SORT_WORK->pass = 0; SORT_WORK->pass != 2; SORT_WORK->pass++) {
            while (SORT_WORK->count[SORT_WORK->pass] != 0) {
                stripCount = idx;
                stripLength = idx >> 16;
                idx = *++cursor;
                for (stripCount &= 0xFFFF; stripCount != 0; stripCount--) {
                    base = (u8 *)SORT_WORK->work;
                    p0 = base + (idx >> 16);
                    gte_lwc2(12, 0, p0);
                    gte_lwc2(17, 4, p0);
                    i1 = cursor[1];
                    i2 = cursor[2];
                    p1 = base + (i1 >> 16);
                    p2 = base + (i2 >> 16);
                    gte_lwc2(13, 0, p1);
                    gte_lwc2(18, 4, p1);
                    gte_lwc2(14, 0, p2);
                    gte_lwc2(19, 4, p2);
                    q0 = base + (idx & 0xFFFF);
                    gte_nclip();
                    q1 = base + (i1 & 0xFFFF);
                    q2 = base + (i2 & 0xFFFF);
                    gte_lwc2(20, 0, q0);
                    gte_lwc2(21, 0, q1);
                    gte_lwc2(22, 0, q2);
                    gte_lwc2(25, 4, q0);
                    gte_lwc2(26, 4, q1);
                    gte_lwc2(27, 4, q2);
                    gte_mfc2(24, nclip);
                    if (SORT_WORK->textured) {
                        gte_lwc2(2, 12, cursor);
                        gte_lwc2(4, 16, cursor);
                        idx = cursor[6];
                        if (nclip > 0) {
                            gte_avsz3();
                            gte_lwc2(3, 20, cursor);
                            packet = emitTexturedTriangle(emitEnvMapTriangle(packet, SORT_WORK->ot, gouraud, SORT_WORK->code),
                                                SORT_WORK->ot, gouraud, SORT_WORK->code);
                        }
                        cursor += 6;
                        for (k = 1; k != stripLength;) {
                            loadEnvGteVertex2(gouraud, idx, (u8 *)SORT_WORK->work);
                            gte_lwc2(3, 4, cursor);
                            idx = cursor[2];
                            gte_mfc2(24, nclip);
                            if ((k & 1) ? nclip < 0 : nclip > 0) {
                                STRIP_DRAW(emitTexturedTriangle);
                            }
                            cursor += 2;
                            if (++k == stripLength) {
                                break;
                            }
                            if (SORT_WORK->pass != 0) {
                                loadEnvGteVertex0(gouraud, idx, (u8 *)SORT_WORK->work);
                                gte_lwc2(2, 4, cursor);
                                gte_mfc2(24, nclip);
                                idx = cursor[2];
                                if ((k & 1) ? nclip < 0 : nclip > 0) {
                                    STRIP_DRAW(emitTexturedTriangle);
                                }
                                cursor += 2;
                                if (++k == stripLength) {
                                    break;
                                }
                            }
                            loadEnvGteVertex1(gouraud, idx, (u8 *)SORT_WORK->work);
                            gte_lwc2(4, 4, cursor);
                            gte_mfc2(24, nclip);
                            idx = cursor[2];
                            if ((k & 1) ? nclip < 0 : nclip > 0) {
                                STRIP_DRAW(emitTexturedTriangle);
                            }
                            cursor += 2;
                            k++;
                        }
                    } else {
                        idx = cursor[3];
                        if (nclip > 0) {
                            STRIP_DRAW(emitUntexturedTriangle);
                        }
                        cursor += 3;
                        for (k = 1; k != stripLength;) {
                            loadEnvGteVertex2(gouraud, idx, (u8 *)SORT_WORK->work);
                            idx = *++cursor;
                            gte_mfc2(24, nclip);
                            if ((k & 1) ? nclip < 0 : nclip > 0) {
                                STRIP_DRAW(emitUntexturedTriangle);
                            }
                            if (++k == stripLength) {
                                break;
                            }
                            if (SORT_WORK->pass != 0) {
                                loadEnvGteVertex0(gouraud, idx, (u8 *)SORT_WORK->work);
                                idx = *++cursor;
                                gte_mfc2(24, nclip);
                                if ((k & 1) ? nclip < 0 : nclip > 0) {
                                    STRIP_DRAW(emitUntexturedTriangle);
                                }
                                if (++k == stripLength) {
                                    break;
                                }
                            }
                            loadEnvGteVertex1(gouraud, idx, (u8 *)SORT_WORK->work);
                            idx = *++cursor;
                            gte_mfc2(24, nclip);
                            if ((k & 1) ? nclip < 0 : nclip > 0) {
                                STRIP_DRAW(emitUntexturedTriangle);
                            }
                            k++;
                        }
                    }
                }
                SORT_WORK->count[SORT_WORK->pass]--;
            }
        }
    }
    SORT_WORK->packet = (u32)packet;
    SORT_WORK->data = cursor;
}

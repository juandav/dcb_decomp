#ifndef DCB_PRIM3D_H
#define DCB_PRIM3D_H

#include "game.h"

void transformAndAddPolyFT3(s32 poly, s32 vert0, s32 vert1, s32 vert2, u8 cullBackface, s32 fixedOtz);
void transformAndAddPolyFT4(s32 poly, s32 vert0, s32 vert1, s32 vert2, s32 vert3, u8 cullBackface, s32 fixedOtz);
void transformAndAddPolyGT4(s32 poly, s32 vert0, s32 vert1, s32 vert2, s32 vert3, u8 cullBackface, s32 fixedOtz);
void transformAndAddPolyF3(s32 poly, s32 tpagePrim, s32 vert0, s32 vert1, s32 vert2, u8 semiTrans, u8 cullBackface, s32 fixedOtz);
void transformAndAddPolyF4(s32 poly, s32 tpagePrim, s32 vert0, s32 vert1, s32 vert2, s32 vert3, u8 semiTrans, u8 cullBackface, s32 fixedOtz);
void transformAndAddPolyGT3(s32 poly, s32 vert0, s32 vert1, s32 vert2, u8 cullBackface, s32 fixedOtz);
void transformAndAddPolyG3(s32 poly, s32 tpagePrim, s32 vert0, s32 vert1, s32 vert2, u8 semiTrans, u8 cullBackface, s32 fixedOtz);
void transformAndAddPolyG4(s32 poly, s32 tpagePrim, s32 vert0, s32 vert1, s32 vert2, s32 vert3, u8 semiTrans, u8 cullBackface, s32 fixedOtz);
void transformAndAddLineF2(s32 line, s32 tpagePrim, s32 vert0, s32 vert1, u8 semiTrans, s32 fixedOtz);
void transformAndAddLineG2(s32 line, s32 tpagePrim, s32 vert0, s32 vert1, u8 semiTrans, s32 fixedOtz);

#endif /* DCB_PRIM3D_H */

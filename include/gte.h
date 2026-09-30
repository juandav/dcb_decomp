#ifndef GTE_H
#define GTE_H

/*
 * GTE access written inline in C. The mnemonics come from gte_macros.inc,
 * which include/labels.inc brings into every translation unit.
 */

/* Load GTE data register `reg` from `ofs(p)` */
#define gte_lwc2(reg, ofs, p) __asm__ volatile("lwc2 $" #reg ", " #ofs "(%0)" : : "r"(p))

#define gte_nclip() __asm__ volatile("nclip")

/* Store GTE data register `reg` to `ofs(p)` */
#define gte_swc2(reg, ofs, p) __asm__ volatile("swc2 $" #reg ", " #ofs "(%0)" : : "r"(p))

/* Read GTE data register `reg` into `v` */
#define gte_mfc2(reg, v) __asm__ volatile("mfc2 %0, $" #reg : "=r"(v))

#define gte_avsz3() __asm__ volatile("avsz3")
#define gte_avsz4() __asm__ volatile("avsz4")

/* Write general register `v` to GTE data register `reg` */
#define gte_mtc2(reg, v) __asm__ volatile("mtc2 %0, $" #reg : : "r"(v))

/* Load V0-V2 from the three SVECTORs at `p` */
#define gte_ldv3c(p)                                                                                                 \
    __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0); lwc2 $2, 8(%0); lwc2 $3, 12(%0); lwc2 $4, 16(%0); lwc2 $5, 20(%0)" \
                     :                                                                                               \
                     : "r"(p))

/* Load V0 from two registers: vx/vy packed in `xy`, vz in `z` */
#define gte_ldv0_reg(xy, z) __asm__ volatile("mtc2 %0, $0; mtc2 %1, $1" : : "r"(xy), "r"(z))

/*
 * Software pipelining for a GTE loop: read the next three SVECTORs at `p`
 * into $8-$13 while the GTE works, then hand them to V0-V2.
 */
#define gte_prefetchv3c(p)                                                                                           \
    __asm__ volatile("lw $8, 0(%0); lw $9, 4(%0); lw $10, 8(%0); lw $11, 12(%0); lw $12, 16(%0); lw $13, 20(%0)"     \
                     :                                                                                               \
                     : "r"(p)                                                                                        \
                     : "$8", "$9", "$10", "$11", "$12", "$13")
#define gte_ldv3_prefetched()                                                                                        \
    __asm__ volatile("mtc2 $8, $0; mtc2 $9, $1; mtc2 $10, $2; mtc2 $11, $3; mtc2 $12, $4; mtc2 $13, $5"               \
                     :                                                                                               \
                     :                                                                                               \
                     : "$8", "$9", "$10", "$11", "$12", "$13")

/*
 * Store the screen coordinates of the three vertices of the last rtpt, each
 * SXY followed by its Z (SXY0, SZ1, SXY1, SZ2, SXY2, SZ3), as six words at `p`
 */
#define gte_stsxysz3c(p)                                                                                             \
    __asm__ volatile("swc2 $12, 0(%0); swc2 $17, 4(%0); swc2 $13, 8(%0); swc2 $18, 12(%0); swc2 $14, 16(%0); "       \
                     "swc2 $19, 20(%0)"                                                                              \
                     :                                                                                               \
                     : "r"(p))

/* Store the three lit colours of the last ncct (RGB0-RGB2) as three words at `p` */
#define gte_strgb3c(p) __asm__ volatile("swc2 $20, 0(%0); swc2 $21, 4(%0); swc2 $22, 8(%0)" : : "r"(p))

/* Set the colour and code (RGBC) the lighting commands start from */
#define gte_ldrgbc(v) __asm__ volatile("mtc2 %0, $6" : : "r"(v))

/* Read the result of the last nclip (MAC0, what gte_stopz stores) into `v` */
#define gte_stopz_reg(v) __asm__ volatile("mfc2 %0, $24" : "=r"(v))

/*
 * A textured packet's UV words wait in V0-V2 until emitTextured* stores them:
 * VXY1 = UV0 + CLUT, VXY2 = UV1 + TPAGE, VZ1 = UV2 and VZ2 = UV3 (quads).
 * The CLUT and TPAGE words are added to the word p[i] in a scratch register,
 * %0, that comes from the "r"(0) input (hence the `move $n, $0` in front).
 */
#define gte_lduv0(p, i, clut)                                                                                        \
    __asm__ volatile("lw %0, %c3(%1)\n\taddu %0, %0, %2\n\tmtc2 %0, $2" : : "r"(0), "r"(p), "r"(clut), "i"((i) * 4))
#define gte_lduv1(p, i, tpage)                                                                                       \
    __asm__ volatile("lw %0, %c3(%1)\n\taddu %0, %0, %2\n\tmtc2 %0, $4" : : "r"(0), "r"(p), "r"(tpage), "i"((i) * 4))
/* gte_lduv0 from p[i] and gte_lduv1 from p[i + 1] */
#define gte_lduv01(p, i, clut, tpage)                                                                                \
    __asm__ volatile("lw %0, %c4(%1)\n\taddu %0, %0, %2\n\tmtc2 %0, $2\n\t"                                          \
                     "lw %0, %c5(%1)\n\taddu %0, %0, %3\n\tmtc2 %0, $4"                                              \
                     :                                                                                               \
                     : "r"(0), "r"(p), "r"(clut), "r"(tpage), "i"((i) * 4), "i"((i) * 4 + 4))
#define gte_lduv2(p, i) __asm__ volatile("lwc2 $3, %c1(%0)" : : "r"(p), "i"((i) * 4))
#define gte_lduv3(p, i) __asm__ volatile("lwc2 $5, %c1(%0)" : : "r"(p), "i"((i) * 4))

/* Load V0 from the SVECTOR at `p` (the inline_c.h form: `p` is an operand) */
#define gte_ldv0c(p) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(p))

/* Read the three results of the last mvmva (MAC1-MAC3) */
#define gte_stmac123(x, y, z) __asm__ volatile("mfc2 %0, $25; mfc2 %1, $26; mfc2 %2, $27" : "=r"(x), "=r"(y), "=r"(z))

/* Store the last lit colour (RGB2) as a word at `p` */
#define gte_strgb(p) __asm__ volatile("swc2 $22, 0(%0)" : : "r"(p))

/*
 * Start a primitive packet at `p` from the last GTE results: SXY0 goes to
 * p[2] and RGB0 | `code` (the colour and GPU code word) to p[1]. RGB0 is read
 * into $8 first, so the SXY0 store covers the mfc2 delay.
 */
#define gte_stsxy0_rgbcode(p, code)                                                                                  \
    __asm__ volatile("mfc2 $8, $20; swc2 $12, 8(%0); or $8, $8, %1; sw $8, 4(%0)"                                    \
                     :                                                                                               \
                     : "r"(p), "r"(code)                                                                             \
                     : "$8", "memory")

/* GTE commands without the nops in front (the caller keeps the pipeline safe) */
#define gte_nop() __asm__ volatile("nop")
#define gte_rtpt() __asm__ volatile("rtpt")
#define gte_ncct() __asm__ volatile("ncct")
#define gte_nccs() __asm__ volatile("nccs")
#define gte_mvmva(sf, mx, v, cv, lm) __asm__ volatile("mvmva " #sf ", " #mx ", " #v ", " #cv ", " #lm)
/* V0 times the rotation matrix, without the translation (MAC1-3 = R * V0) */
#define gte_rtv0() gte_mvmva(1, 0, 0, 3, 0)

/* The inline_c.h forms: the pointer is an operand and $12-$14 are scratch */
#define gte_SetRotMatrix_c(r)                                                                                        \
    __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); "      \
                     "lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4"                                      \
                     :                                                                                               \
                     : "r"(r)                                                                                        \
                     : "$12", "$13", "$14")
#define gte_SetTransMatrix_c(r)                                                                                      \
    __asm__ volatile("lw $12, 20(%0); lw $13, 24(%0); ctc2 $12, $5; lw $14, 28(%0); ctc2 $13, $6; ctc2 $14, $7"      \
                     :                                                                                               \
                     : "r"(r)                                                                                        \
                     : "$12", "$13", "$14")

/*
 * The inline_o.h forms of DMPSX 3: the pointer goes to $12 first, every
 * instruction is its own volatile asm, $12-$15 are scratch, and a GTE command
 * gets two nops in front of it.
 */
#define GTE_O(s) __asm__ volatile(s : : : "$12", "$13", "$14", "$15", "memory")
#define GTE_O_PTR(p) __asm__ volatile("move $12,%0" : : "r"(p) : "$12", "$13", "$14", "$15", "memory")

#define gte_SetRotMatrix(r)                                                                                          \
    {                                                                                                                \
        GTE_O_PTR(r);                                                                                                \
        GTE_O("lw $13,0($12)");                                                                                      \
        GTE_O("lw $14,4($12)");                                                                                      \
        GTE_O("ctc2 $13,$0");                                                                                        \
        GTE_O("ctc2 $14,$1");                                                                                        \
        GTE_O("lw $13,8($12)");                                                                                      \
        GTE_O("lw $14,12($12)");                                                                                     \
        GTE_O("lw $15,16($12)");                                                                                     \
        GTE_O("ctc2 $13,$2");                                                                                        \
        GTE_O("ctc2 $14,$3");                                                                                        \
        GTE_O("ctc2 $15,$4");                                                                                        \
    }

#define gte_SetLightMatrix(r)                                                                                        \
    {                                                                                                                \
        GTE_O_PTR(r);                                                                                                \
        GTE_O("lw $13,0($12)");                                                                                      \
        GTE_O("lw $14,4($12)");                                                                                      \
        GTE_O("ctc2 $13,$8");                                                                                        \
        GTE_O("ctc2 $14,$9");                                                                                        \
        GTE_O("lw $13,8($12)");                                                                                      \
        GTE_O("lw $14,12($12)");                                                                                     \
        GTE_O("lw $15,16($12)");                                                                                     \
        GTE_O("ctc2 $13,$10");                                                                                       \
        GTE_O("ctc2 $14,$11");                                                                                       \
        GTE_O("ctc2 $15,$12");                                                                                       \
    }

#define gte_SetColorMatrix(r)                                                                                        \
    {                                                                                                                \
        GTE_O_PTR(r);                                                                                                \
        GTE_O("lw $13,0($12)");                                                                                      \
        GTE_O("lw $14,4($12)");                                                                                      \
        GTE_O("ctc2 $13,$16");                                                                                       \
        GTE_O("ctc2 $14,$17");                                                                                       \
        GTE_O("lw $13,8($12)");                                                                                      \
        GTE_O("lw $14,12($12)");                                                                                     \
        GTE_O("lw $15,16($12)");                                                                                     \
        GTE_O("ctc2 $13,$18");                                                                                       \
        GTE_O("ctc2 $14,$19");                                                                                       \
        GTE_O("ctc2 $15,$20");                                                                                       \
    }

#define gte_SetTransMatrix(r)                                                                                        \
    {                                                                                                                \
        GTE_O_PTR(r);                                                                                                \
        GTE_O("lw $13,20($12)");                                                                                     \
        GTE_O("lw $14,24($12)");                                                                                     \
        GTE_O("ctc2 $13,$5");                                                                                        \
        GTE_O("lw $15,28($12)");                                                                                     \
        GTE_O("ctc2 $14,$6");                                                                                        \
        GTE_O("ctc2 $15,$7");                                                                                        \
    }

#define gte_ldv0(r)                                                                                                  \
    {                                                                                                                \
        GTE_O_PTR(r);                                                                                                \
        GTE_O("lwc2 $0,0($12)");                                                                                     \
        GTE_O("lwc2 $1,4($12)");                                                                                     \
    }

#define gte_rtv0tr()                                                                                                   \
    {                                                                                                                \
        GTE_O("nop");                                                                                                \
        GTE_O("nop");                                                                                                \
        GTE_O("rtv0tr");                                                                                               \
    }

#define gte_stlvnl(r)                                                                                                \
    {                                                                                                                \
        GTE_O_PTR(r);                                                                                                \
        GTE_O("swc2 $25,0($12)");                                                                                    \
        GTE_O("swc2 $26,4($12)");                                                                                    \
        GTE_O("swc2 $27,8($12)");                                                                                    \
    }

#define gte_stflg(r)                                                                                                 \
    {                                                                                                                \
        GTE_O_PTR(r);                                                                                                \
        GTE_O("cfc2 $13,$31");                                                                                       \
        GTE_O("nop");                                                                                                \
        GTE_O("sw $13,0($12)");                                                                                      \
    }

#endif /* GTE_H */

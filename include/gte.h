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

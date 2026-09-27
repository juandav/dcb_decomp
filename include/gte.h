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

#endif /* GTE_H */

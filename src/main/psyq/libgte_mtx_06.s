/*
 * libgte MTX_06: ApplyMatrixSV and TransMatrix.
 *
 * Hand-written assembly, not compiler output:
 * - ApplyMatrixSV issues `mvmva` with a single nop in front. PsyQ's C GTE
 *   macros (inline_c.h and inline_o.h, DMPSX 3) always emit two nops before
 *   every GTE command; only inline_a.h, the macros for assembler programs,
 *   has the bare command.
 * - Both load into $t0-$t4 while $v0/$v1 are free, where GCC hands out $v0,
 *   $v1 and the free $a registers first.
 * - Both end in `addu $v0,...; jr $ra; nop`. The PsyQ build (GCC 2.7.2 +
 *   ASPSX in reorder mode) moves that addu into the delay slot of `jr $ra`
 *   (TransMatrix written in C comes out as `jr $ra; sw ...`, with the loads
 *   in $v1); the unfilled slot is `.set noreorder` code.
 */

.include "macro.inc"

.set noat
.set noreorder

.section .text

glabel ApplyMatrixSV
    lw         $t0, 0x0($a0)
    lw         $t1, 0x4($a0)
    lw         $t2, 0x8($a0)
    lw         $t3, 0xC($a0)
    lw         $t4, 0x10($a0)
    ctc2       $t0, $0
    ctc2       $t1, $1
    ctc2       $t2, $2
    ctc2       $t3, $3
    ctc2       $t4, $4
    lwc2       $0, 0x0($a1)
    lwc2       $1, 0x4($a1)
    nop
    mvmva      1, 0, 0, 3, 0
    mfc2       $t0, $9
    mfc2       $t1, $10
    mfc2       $t2, $11
    sh         $t0, 0x0($a2)
    sh         $t1, 0x2($a2)
    sh         $t2, 0x4($a2)
    addu       $v0, $a2, $zero
    jr         $ra
     nop
endlabel ApplyMatrixSV
    nop

glabel TransMatrix
    lw         $t0, 0x0($a1)
    lw         $t1, 0x4($a1)
    lw         $t2, 0x8($a1)
    sw         $t0, 0x14($a0)
    sw         $t1, 0x18($a0)
    sw         $t2, 0x1C($a0)
    addu       $v0, $a0, $zero
    jr         $ra
     nop
endlabel TransMatrix
    nop
    nop
    nop

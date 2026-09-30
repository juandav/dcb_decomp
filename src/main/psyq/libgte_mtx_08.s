/*
 * libgte MTX_08: ScaleMatrix, SetRotMatrix, SetLightMatrix, SetTransMatrix
 * and SetBackColor.
 *
 * Hand-written assembly, not compiler output:
 * - The four GTE setters are lw/ctc2 (or sll/ctc2) sequences into $t0-$t4
 *   with $v0/$v1 free, where GCC hands out $v0, $v1 and the free $a
 *   registers first, and each ends in `jr $ra; nop` after a ctc2 that the
 *   PsyQ build (GCC 2.7.2 + ASPSX in reorder mode) would have moved into
 *   the delay slot.
 * - ScaleMatrix sign-extends each half-word with `andi $t1,$t0,0xFFFF;
 *   sll 16; sra 16` (GCC drops the redundant andi), multiplies the signed
 *   values with multu, and works only in $t0-$t5 with $v1 unused.
 */

.include "macro.inc"

.set noat
.set noreorder

.section .text

glabel ScaleMatrix
    lw         $t3, 0x0($a1)
    lw         $t4, 0x4($a1)
    lw         $t5, 0x8($a1)
    lw         $t0, 0x0($a0)
    nop
    andi       $t1, $t0, 0xFFFF
    sll        $t1, $t1, 16
    sra        $t1, $t1, 16
    multu      $t1, $t3
    mflo       $t1
    sra        $t1, $t1, 12
    andi       $t1, $t1, 0xFFFF
    sra        $t2, $t0, 16
    multu      $t2, $t4
    mflo       $t2
    sra        $t2, $t2, 12
    sll        $t2, $t2, 16
    or         $t1, $t1, $t2
    sw         $t1, 0x0($a0)
    lw         $t0, 0x4($a0)
    nop
    andi       $t1, $t0, 0xFFFF
    sll        $t1, $t1, 16
    sra        $t1, $t1, 16
    multu      $t1, $t5
    mflo       $t1
    sra        $t1, $t1, 12
    andi       $t1, $t1, 0xFFFF
    sra        $t2, $t0, 16
    multu      $t2, $t3
    mflo       $t2
    sra        $t2, $t2, 12
    sll        $t2, $t2, 16
    or         $t1, $t1, $t2
    sw         $t1, 0x4($a0)
    lw         $t0, 0x8($a0)
    nop
    andi       $t1, $t0, 0xFFFF
    sll        $t1, $t1, 16
    sra        $t1, $t1, 16
    multu      $t1, $t4
    mflo       $t1
    sra        $t1, $t1, 12
    andi       $t1, $t1, 0xFFFF
    sra        $t2, $t0, 16
    multu      $t2, $t5
    mflo       $t2
    sra        $t2, $t2, 12
    sll        $t2, $t2, 16
    or         $t1, $t1, $t2
    sw         $t1, 0x8($a0)
    lw         $t0, 0xC($a0)
    nop
    andi       $t1, $t0, 0xFFFF
    sll        $t1, $t1, 16
    sra        $t1, $t1, 16
    multu      $t1, $t3
    mflo       $t1
    sra        $t1, $t1, 12
    andi       $t1, $t1, 0xFFFF
    sra        $t2, $t0, 16
    multu      $t2, $t4
    mflo       $t2
    sra        $t2, $t2, 12
    sll        $t2, $t2, 16
    or         $t1, $t1, $t2
    sw         $t1, 0xC($a0)
    lw         $t0, 0x10($a0)
    nop
    andi       $t1, $t0, 0xFFFF
    sll        $t1, $t1, 16
    sra        $t1, $t1, 16
    multu      $t1, $t5
    mflo       $t1
    sra        $t1, $t1, 12
    sw         $t1, 0x10($a0)
    jr         $ra
     addu      $v0, $a0, $zero
endlabel ScaleMatrix
    nop
    nop

glabel SetRotMatrix
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
    jr         $ra
     nop
endlabel SetRotMatrix

glabel SetLightMatrix
    lw         $t0, 0x0($a0)
    lw         $t1, 0x4($a0)
    lw         $t2, 0x8($a0)
    lw         $t3, 0xC($a0)
    lw         $t4, 0x10($a0)
    ctc2       $t0, $8
    ctc2       $t1, $9
    ctc2       $t2, $10
    ctc2       $t3, $11
    ctc2       $t4, $12
    jr         $ra
     nop
endlabel SetLightMatrix

glabel SetTransMatrix
    lw         $t0, 0x14($a0)
    lw         $t1, 0x18($a0)
    lw         $t2, 0x1C($a0)
    ctc2       $t0, $5
    ctc2       $t1, $6
    ctc2       $t2, $7
    jr         $ra
     nop
endlabel SetTransMatrix

glabel SetBackColor
    sll        $a0, $a0, 4
    sll        $a1, $a1, 4
    sll        $a2, $a2, 4
    ctc2       $a0, $13
    ctc2       $a1, $14
    ctc2       $a2, $15
    jr         $ra
     nop
endlabel SetBackColor

/*
 * libgte MTX_004: ApplyMatrixLV.
 *
 * Hand-written assembly, not compiler output: both `mvmva`s have a single
 * nop in front, where PsyQ's C GTE macros (inline_c.h and inline_o.h,
 * DMPSX 3) always emit two (only inline_a.h, for assembler programs, has
 * the bare command); the second mvmva is issued before the results of the
 * first are read back, and the sign of each part is handled by
 * `bgez; nop; negu ...; b; negu` blocks, which is scheduling by hand for
 * the GTE's latency; and it works only in $t0-$t5 with $v1 unused, where
 * GCC hands out $v0, $v1 and the free $a registers first.
 */

.include "macro.inc"

.set noat
.set noreorder

.section .text

glabel ApplyMatrixLV
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
    lw         $t0, 0x0($a1)
    lw         $t1, 0x4($a1)
    lw         $t2, 0x8($a1)
    bgez       $t0, .L80062288
     nop
    negu       $t0, $t0
    sra        $t3, $t0, 15
    negu       $t3, $t3
    andi       $t0, $t0, 0x7FFF
    b          .L80062290
     negu      $t0, $t0
  .L80062288:
    sra        $t3, $t0, 15
    andi       $t0, $t0, 0x7FFF
  .L80062290:
    bgez       $t1, .L800622B0
     nop
    negu       $t1, $t1
    sra        $t4, $t1, 15
    negu       $t4, $t4
    andi       $t1, $t1, 0x7FFF
    b          .L800622B8
     negu      $t1, $t1
  .L800622B0:
    sra        $t4, $t1, 15
    andi       $t1, $t1, 0x7FFF
  .L800622B8:
    bgez       $t2, .L800622D8
     nop
    negu       $t2, $t2
    sra        $t5, $t2, 15
    negu       $t5, $t5
    andi       $t2, $t2, 0x7FFF
    b          .L800622E0
     negu      $t2, $t2
  .L800622D8:
    sra        $t5, $t2, 15
    andi       $t2, $t2, 0x7FFF
  .L800622E0:
    mtc2       $t3, $9
    mtc2       $t4, $10
    mtc2       $t5, $11
    nop
    mvmva      0, 0, 3, 3, 0
    mfc2       $t3, $25
    mfc2       $t4, $26
    mfc2       $t5, $27
    mtc2       $t0, $9
    mtc2       $t1, $10
    mtc2       $t2, $11
    nop
    mvmva      1, 0, 3, 3, 0
    bgez       $t3, .L8006232C
     nop
    negu       $t3, $t3
    sll        $t3, $t3, 3
    b          .L80062330
     negu      $t3, $t3
  .L8006232C:
    sll        $t3, $t3, 3
  .L80062330:
    bgez       $t4, .L80062348
     nop
    negu       $t4, $t4
    sll        $t4, $t4, 3
    b          .L8006234C
     negu      $t4, $t4
  .L80062348:
    sll        $t4, $t4, 3
  .L8006234C:
    bgez       $t5, .L80062364
     nop
    negu       $t5, $t5
    sll        $t5, $t5, 3
    b          .L80062368
     negu      $t5, $t5
  .L80062364:
    sll        $t5, $t5, 3
  .L80062368:
    mfc2       $t0, $25
    mfc2       $t1, $26
    mfc2       $t2, $27
    addu       $t0, $t0, $t3
    addu       $t1, $t1, $t4
    addu       $t2, $t2, $t5
    sw         $t0, 0x0($a2)
    sw         $t1, 0x4($a2)
    sw         $t2, 0x8($a2)
    jr         $ra
     addu      $v0, $a2, $zero
endlabel ApplyMatrixLV

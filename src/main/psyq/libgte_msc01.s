/*
 * libgte MSC01: SquareRoot0, InvSquareRoot, VectorNormalS, VectorNormal,
 * VectorNormalSS, their helper func_8005BA84, MatrixNormal, MulMatrix0,
 * CompMatrix, PushMatrix and PopMatrix.
 *
 * Hand-written assembly, not compiler output:
 * - $at is an ordinary temporary (`addiu $at,$zero,0x20; beq $v0,$at`,
 *   `addiu $at,$zero,-0x2; and $t2,$v0,$at`, `slti $at,$t6,0x280; bnez $at`,
 *   `lui $at,0xFFFF; and $t1,$t1,$at`), and the arithmetic uses the
 *   trapping add/addi/sub, which GCC never emits for C integers.
 * - func_8005BA84 takes its vector in $t0-$t2 and returns it there; the
 *   VectorNormal* entry points load $t0-$t2, keep $ra in $a3 across
 *   `jal func_8005BA84` without a stack frame, and VectorNormalS has no
 *   return of its own: it branches into the middle of VectorNormalSS.
 *   PushMatrix and PopMatrix save $ra in a global around their printf.
 * - The sqr, op and mvmva commands have one nop in front, where PsyQ's C
 *   GTE macros (inline_c.h and inline_o.h, DMPSX 3) always emit two; only
 *   inline_a.h, the macros for assembler programs, has the bare command.
 *   The leading-zero count goes through GTE registers 30/31 with two nops
 *   placed by hand.
 */

.include "macro.inc"

.set noat
.set noreorder

.section .text

glabel SquareRoot0
    mtc2       $a0, $30
    nop
    nop
    mfc2       $v0, $31
    addiu      $at, $zero, 0x20
    beq        $v0, $at, .L8005B970
     nop
    andi       $t0, $v0, 0x1
    addiu      $at, $zero, -0x2
    and        $t2, $v0, $at
    addiu      $t1, $zero, 0x1F
    sub        $t1, $t1, $t2
    sra        $t1, $t1, 1
    addi       $t3, $t2, -0x18
    bltz       $t3, .L8005B93C
     nop
    sllv       $t4, $a0, $t3
    b          .L8005B948
  .L8005B93C:
     addiu     $t3, $zero, 0x18
    sub        $t3, $t3, $t2
    srav       $t4, $a0, $t3
  .L8005B948:
    addi       $t4, $t4, -0x40
    sll        $t4, $t4, 1
    lui        $t5, %hi(D_800718A8)
    addu       $t5, $t5, $t4
    lh         $t5, %lo(D_800718A8)($t5)
    nop
    sllv       $t5, $t5, $t1
    srl        $v0, $t5, 12
    jr         $ra
     nop
  .L8005B970:
    jr         $ra
     addiu     $v0, $zero, 0x0
endlabel SquareRoot0
    nop
    nop
    nop

glabel InvSquareRoot
    mtc2       $a0, $30
    nop
    nop
    mfc2       $v0, $31
    addiu      $at, $zero, 0x20
    beq        $v0, $at, .L8005BA08
     nop
    beqz       $v0, .L8005BA08
     nop
    andi       $t0, $v0, 0x1
    addiu      $at, $zero, -0x2
    and        $t2, $v0, $at
    addiu      $t1, $zero, 0x1F
    sub        $t1, $t1, $t2
    sra        $t1, $t1, 1
    addi       $t3, $t2, -0x18
    bltz       $t3, .L8005B9D4
     nop
    sllv       $t4, $a0, $t3
    b          .L8005B9E0
  .L8005B9D4:
     addiu     $t3, $zero, 0x18
    sub        $t3, $t3, $t2
    srav       $t4, $a0, $t3
  .L8005B9E0:
    addi       $t4, $t4, -0x40
    sll        $t4, $t4, 1
    lui        $t5, %hi(D_80071A3C)
    addu       $t5, $t5, $t4
    lh         $t5, %lo(D_80071A3C)($t5)
    sw         $t1, 0x0($a2)
    sw         $t5, 0x0($a1)
    addiu      $v0, $zero, 0x1
    jr         $ra
     nop
  .L8005BA08:
    jr         $ra
     addiu     $v0, $zero, -0x1
endlabel InvSquareRoot

glabel VectorNormalS
    lw         $t0, 0x0($a0)
    lw         $t1, 0x4($a0)
    lw         $t2, 0x8($a0)
    b          .L8005BA60
     nop
endlabel VectorNormalS

glabel VectorNormal
    lw         $t0, 0x0($a0)
    lw         $t1, 0x4($a0)
    lw         $t2, 0x8($a0)
    addu       $a3, $ra, $zero
    jal        func_8005BA84
     nop
    sw         $t0, 0x0($a1)
    sw         $t1, 0x4($a1)
    sw         $t2, 0x8($a1)
    addu       $ra, $a3, $zero
    jr         $ra
     nop
endlabel VectorNormal

glabel VectorNormalSS
    lh         $t0, 0x0($a0)
    lh         $t1, 0x2($a0)
    lh         $t2, 0x4($a0)
  .L8005BA60:
    addu       $a3, $ra, $zero
    jal        func_8005BA84
     nop
    sh         $t0, 0x0($a1)
    sh         $t1, 0x2($a1)
    sh         $t2, 0x4($a1)
    addu       $ra, $a3, $zero
    jr         $ra
     nop
endlabel VectorNormalSS

glabel func_8005BA84
    mtc2       $t0, $9
    mtc2       $t1, $10
    mtc2       $t2, $11
    nop
    sqr        0
    mfc2       $t3, $25
    mfc2       $t4, $26
    mfc2       $t5, $27
    add        $t3, $t3, $t4
    add        $v0, $t3, $t5
    mtc2       $v0, $30
    nop
    nop
    mfc2       $v1, $31
    addiu      $at, $zero, -0x2
    and        $v1, $v1, $at
    addiu      $t6, $zero, 0x1F
    sub        $t6, $t6, $v1
    sra        $t6, $t6, 1
    addi       $t3, $v1, -0x18
    bltz       $t3, .L8005BAE4
     nop
    b          .L8005BAF0
     sllv      $t4, $v0, $t3
  .L8005BAE4:
    addiu      $t3, $zero, 0x18
    sub        $t3, $t3, $v1
    srav       $t4, $v0, $t3
  .L8005BAF0:
    addi       $t4, $t4, -0x40
    sll        $t4, $t4, 1
    lui        $t5, %hi(D_80071A3C)
    addu       $t5, $t5, $t4
    lh         $t5, %lo(D_80071A3C)($t5)
    nop
    mtc2       $t5, $8
    mtc2       $t0, $9
    mtc2       $t1, $10
    mtc2       $t2, $11
    nop
    nop
    gpf        0
    mfc2       $t0, $25
    mfc2       $t1, $26
    mfc2       $t2, $27
    srav       $t0, $t0, $t6
    srav       $t1, $t1, $t6
    srav       $t2, $t2, $t6
    jr         $ra
     nop
endlabel func_8005BA84

glabel MatrixNormal
    lh         $t0, 0x0($a0)
    lh         $t1, 0x2($a0)
    lh         $t2, 0x4($a0)
    lh         $t3, 0x6($a0)
    lh         $t4, 0x8($a0)
    lh         $t5, 0xA($a0)
    cfc2       $v0, $0
    cfc2       $v1, $2
    cfc2       $a2, $4
    ctc2       $t0, $0
    ctc2       $t1, $2
    ctc2       $t2, $4
    mtc2       $t5, $11
    mtc2       $t3, $9
    mtc2       $t4, $10
    nop
    op         1
    mfc2       $t7, $25
    mfc2       $t8, $26
    mfc2       $t9, $27
    ctc2       $t3, $0
    ctc2       $t4, $2
    ctc2       $t5, $4
    nop
    op         1
    mtc2       $t3, $0
    mtc2       $t4, $1
    mtc2       $t5, $2
    mfc2       $t0, $25
    mfc2       $t1, $26
    mfc2       $t2, $27
    ctc2       $v0, $0
    ctc2       $v1, $2
    ctc2       $a2, $4
    addu       $a3, $ra, $zero
    jal        func_8005BA84
     nop
    sh         $t0, 0x0($a1)
    sh         $t1, 0x2($a1)
    sh         $t2, 0x4($a1)
    mfc2       $t0, $0
    mfc2       $t1, $1
    mfc2       $t2, $2
    jal        func_8005BA84
     nop
    sh         $t0, 0x6($a1)
    sh         $t1, 0x8($a1)
    sh         $t2, 0xA($a1)
    addu       $t0, $t7, $zero
    addu       $t1, $t8, $zero
    jal        func_8005BA84
     addu      $t2, $t9, $zero
    sh         $t0, 0xC($a1)
    sh         $t1, 0xE($a1)
    sh         $t2, 0x10($a1)
    addu       $ra, $a3, $zero
    jr         $ra
     nop
endlabel MatrixNormal
    nop
    nop

glabel MulMatrix0
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
    lhu        $t0, 0x0($a1)
    lw         $t1, 0x4($a1)
    lw         $t2, 0xC($a1)
    lui        $at, (0xFFFF0000 >> 16)
    and        $t1, $t1, $at
    or         $t0, $t0, $t1
    mtc2       $t0, $0
    mtc2       $t2, $1
    nop
    mvmva      1, 0, 0, 3, 0
    lhu        $t0, 0x2($a1)
    lw         $t1, 0x8($a1)
    lh         $t2, 0xE($a1)
    sll        $t1, $t1, 16
    or         $t0, $t0, $t1
    mfc2       $t3, $9
    mfc2       $t4, $10
    mfc2       $t5, $11
    mtc2       $t0, $0
    mtc2       $t2, $1
    nop
    mvmva      1, 0, 0, 3, 0
    lhu        $t0, 0x4($a1)
    lw         $t1, 0x8($a1)
    lw         $t2, 0x10($a1)
    lui        $at, (0xFFFF0000 >> 16)
    and        $t1, $t1, $at
    or         $t0, $t0, $t1
    mfc2       $t6, $9
    mfc2       $t7, $10
    mfc2       $t8, $11
    mtc2       $t0, $0
    mtc2       $t2, $1
    nop
    mvmva      1, 0, 0, 3, 0
    andi       $t3, $t3, 0xFFFF
    sll        $t6, $t6, 16
    or         $t6, $t6, $t3
    sw         $t6, 0x0($a2)
    andi       $t5, $t5, 0xFFFF
    sll        $t8, $t8, 16
    or         $t8, $t8, $t5
    sw         $t8, 0xC($a2)
    mfc2       $t0, $9
    mfc2       $t1, $10
    andi       $t0, $t0, 0xFFFF
    sll        $t4, $t4, 16
    or         $t0, $t0, $t4
    sw         $t0, 0x4($a2)
    andi       $t7, $t7, 0xFFFF
    sll        $t1, $t1, 16
    or         $t1, $t1, $t7
    sw         $t1, 0x8($a2)
    swc2       $11, 0x10($a2)
    addu       $v0, $a2, $zero
    jr         $ra
     nop
endlabel MulMatrix0
    nop

glabel CompMatrix
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
    lhu        $t0, 0x0($a1)
    lw         $t1, 0x4($a1)
    lw         $t2, 0xC($a1)
    lui        $at, (0xFFFF0000 >> 16)
    and        $t1, $t1, $at
    or         $t0, $t0, $t1
    mtc2       $t0, $0
    mtc2       $t2, $1
    nop
    mvmva      1, 0, 0, 3, 0
    lhu        $t0, 0x2($a1)
    lw         $t1, 0x8($a1)
    lh         $t2, 0xE($a1)
    sll        $t1, $t1, 16
    or         $t0, $t0, $t1
    mfc2       $t3, $9
    mfc2       $t4, $10
    mfc2       $t5, $11
    mtc2       $t0, $0
    mtc2       $t2, $1
    nop
    mvmva      1, 0, 0, 3, 0
    lhu        $t0, 0x4($a1)
    lw         $t1, 0x8($a1)
    lw         $t2, 0x10($a1)
    lui        $at, (0xFFFF0000 >> 16)
    and        $t1, $t1, $at
    or         $t0, $t0, $t1
    mfc2       $t6, $9
    mfc2       $t7, $10
    mfc2       $t8, $11
    mtc2       $t0, $0
    mtc2       $t2, $1
    nop
    mvmva      1, 0, 0, 3, 0
    andi       $t3, $t3, 0xFFFF
    sll        $t6, $t6, 16
    or         $t6, $t6, $t3
    sw         $t6, 0x0($a2)
    andi       $t5, $t5, 0xFFFF
    sll        $t8, $t8, 16
    or         $t8, $t8, $t5
    sw         $t8, 0xC($a2)
    mfc2       $t0, $9
    mfc2       $t1, $10
    swc2       $11, 0x10($a2)
    lhu        $t5, 0x14($a1)
    lw         $t6, 0x18($a1)
    lw         $t2, 0x1C($a1)
    sll        $t6, $t6, 16
    or         $t5, $t5, $t6
    mtc2       $t5, $0
    mtc2       $t2, $1
    nop
    mvmva      1, 0, 0, 3, 0
    sll        $t4, $t4, 16
    andi       $t0, $t0, 0xFFFF
    or         $t0, $t0, $t4
    sw         $t0, 0x4($a2)
    andi       $t7, $t7, 0xFFFF
    sll        $t1, $t1, 16
    or         $t1, $t1, $t7
    sw         $t1, 0x8($a2)
    mfc2       $t0, $25
    mfc2       $t1, $26
    mfc2       $t2, $27
    lw         $t3, 0x14($a0)
    lw         $t4, 0x18($a0)
    lw         $t5, 0x1C($a0)
    add        $t0, $t0, $t3
    add        $t1, $t1, $t4
    add        $t2, $t2, $t5
    sw         $t0, 0x14($a2)
    sw         $t1, 0x18($a2)
    sw         $t2, 0x1C($a2)
    addu       $v0, $a2, $zero
    jr         $ra
     nop
endlabel CompMatrix

glabel PushMatrix
    lui        $t6, %hi(D_80071BD4)
    lw         $t6, %lo(D_80071BD4)($t6)
    nop
    slti       $at, $t6, 0x280
    bnez       $at, .L8005BEE4
     nop
    lui        $at, %hi(D_80071BC8)
    sw         $ra, %lo(D_80071BC8)($at)
    lui        $a0, %hi(D_80071E58)
    jal        printf
     addiu     $a0, $a0, %lo(D_80071E58)
    lui        $ra, %hi(D_80071BC8)
    lw         $ra, %lo(D_80071BC8)($ra)
    nop
    jr         $ra
     nop
  .L8005BEE4:
    lui        $t7, %hi(D_80071BD8)
    addiu      $t7, $t7, %lo(D_80071BD8)
    addu       $t7, $t7, $t6
    cfc2       $t0, $0
    cfc2       $t1, $1
    sw         $t0, 0x0($t7)
    sw         $t1, 0x4($t7)
    cfc2       $t0, $2
    cfc2       $t1, $3
    sw         $t0, 0x8($t7)
    sw         $t1, 0xC($t7)
    cfc2       $t0, $4
    nop
    sw         $t0, 0x10($t7)
    cfc2       $t0, $5
    cfc2       $t1, $6
    cfc2       $t2, $7
    sw         $t0, 0x14($t7)
    sw         $t1, 0x18($t7)
    sw         $t2, 0x1C($t7)
    addi       $t6, $t6, 0x20
    lui        $at, %hi(D_80071BD4)
    sw         $t6, %lo(D_80071BD4)($at)
    jr         $ra
     nop
endlabel PushMatrix

glabel PopMatrix
    lui        $t6, %hi(D_80071BD4)
    lw         $t6, %lo(D_80071BD4)($t6)
    nop
    bgtz       $t6, .L8005BF84
     nop
    lui        $at, %hi(D_80071BC8)
    sw         $ra, %lo(D_80071BC8)($at)
    lui        $a0, %hi(D_80071E89)
    jal        printf
     addiu     $a0, $a0, %lo(D_80071E89)
    lui        $ra, %hi(D_80071BC8)
    lw         $ra, %lo(D_80071BC8)($ra)
    nop
    jr         $ra
     nop
  .L8005BF84:
    addi       $t6, $t6, -0x20
    lui        $at, %hi(D_80071BD4)
    sw         $t6, %lo(D_80071BD4)($at)
    lui        $t7, %hi(D_80071BD8)
    addiu      $t7, $t7, %lo(D_80071BD8)
    addu       $t7, $t7, $t6
    lw         $t0, 0x0($t7)
    lw         $t1, 0x4($t7)
    ctc2       $t0, $0
    ctc2       $t1, $1
    lw         $t0, 0x8($t7)
    lw         $t1, 0xC($t7)
    ctc2       $t0, $2
    ctc2       $t1, $3
    lw         $t0, 0x10($t7)
    nop
    ctc2       $t0, $4
    nop
    lw         $t0, 0x14($t7)
    lw         $t1, 0x18($t7)
    lw         $t2, 0x1C($t7)
    ctc2       $t0, $5
    ctc2       $t1, $6
    ctc2       $t2, $7
    jr         $ra
     nop
endlabel PopMatrix
    nop
    nop

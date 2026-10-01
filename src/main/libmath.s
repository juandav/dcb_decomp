/*
 * Soft-float library linked in with the game code: double and float
 * arithmetic, conversions and compares (the libgcc entry points) and their
 * exception hooks. The square root at its end is C, in libmath.c.
 *
 * The arithmetic routines are hand-written assembly, not compiler output:
 * they use $at as an ordinary temporary (`addiu $at,$zero,0x0` then
 * `beq $x,$at`, `slti $at,$x,0` then `bnez $at`), which no compiler does
 * since $at belongs to the assembler; they pass results to each other in
 * $a0/$a1 and keep the caller's $ra in $t8 (`jr $t8`); __subdf3 has no
 * return and runs into __adddf3, and more entry points sit unlabelled
 * after __cmpdf2 and __fixdfsi; they open a private 0xC-byte frame in the
 * middle of the code around each call; and __divdf3 and __muldf3 save $ra
 * at 4($sp) of a 0x18 frame, where GCC puts it at the top.
 *
 * The two exception hooks, raiseSoftFloatException and
 * raiseSoftFloatUnimplemented, are compiled,
 * but not by any GCC the game or PsyQ were built with (2.7.2, 2.8.x and
 * 2.95.2 at any -O): $fp points at the caller's $sp, the saves go upwards
 * from 8($sp) with $ra lowest, moves are `addu $s0,$zero,$a0`, and the
 * stack drops 8 bytes around each call instead of keeping the argument
 * area in the frame. __fixunsdfsi has the same saves and moves, a 0x24
 * frame, not a multiple of 8, and its constant in .text. With no compiler
 * at hand that produces them, they stay assembly too.
 */

.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

glabel __negdf2
    lui        $at, (0x80000000 >> 16)
    xor        $v1, $a1, $at
    jr         $ra
     addu      $v0, $a0, $zero
endlabel __negdf2

glabel __subdf3
    lui        $at, (0x80000000 >> 16)
    xor        $a3, $a3, $at
endlabel __subdf3

glabel __adddf3
    addu       $t8, $ra, $zero
    jal        addDoubleCore
     nop
    addu       $v1, $a1, $zero
    jr         $t8
     addu      $v0, $a0, $zero
    lui        $at, (0x80000000 >> 16)
    xor        $a3, $a3, $at
endlabel __adddf3

glabel addDoubleCore
    lui        $t6, (0x200000 >> 16)
    sll        $v0, $a1, 1
    addu       $t7, $t6, $v0
    sltu       $at, $t6, $t7
    beqz       $at, .L80025AF4
     sll       $v1, $a3, 1
    addu       $t7, $t6, $v1
    sltu       $at, $t6, $t7
    beqz       $at, .L80025AF4
     nop
  .L8002589C:
    sll        $t1, $a1, 11
    srl        $t6, $a0, 22
    sll        $t3, $a3, 11
    lui        $at, (0x80000000 >> 16)
    or         $t1, $t1, $at
    or         $t3, $t3, $at
    srl        $t1, $t1, 1
    or         $t1, $t1, $t6
    srl        $t3, $t3, 1
    srl        $t6, $a2, 22
    srl        $v0, $v0, 21
    srl        $v1, $v1, 21
    srl        $t4, $a1, 31
    sll        $t0, $a0, 10
    sll        $t2, $a2, 10
    or         $t3, $t3, $t6
    sll        $t4, $t4, 31
    beq        $v0, $v1, .L80025974
     addiu     $t7, $zero, 0x20
    sltu       $at, $v0, $v1
    bnez       $at, .L80025934
     nop
    subu       $t6, $v0, $v1
    sltu       $at, $t6, $t7
    bnez       $at, .L8002591C
     nop
    addu       $t2, $t3, $zero
    subu       $t6, $t6, $t7
    sltu       $at, $t6, $t7
    bnez       $at, .L8002591C
     addiu     $t3, $zero, 0x0
    addiu      $t2, $zero, 0x0
  .L8002591C:
    subu       $t7, $t7, $t6
    sllv       $t7, $t3, $t7
    srlv       $t2, $t2, $t6
    srlv       $t3, $t3, $t6
    b          .L80025974
     or        $t2, $t2, $t7
  .L80025934:
    subu       $t6, $v1, $v0
    sltu       $at, $t6, $t7
    bnez       $at, .L8002595C
     nop
    addu       $t0, $t1, $zero
    subu       $t6, $t6, $t7
    sltu       $at, $t6, $t7
    bnez       $at, .L8002595C
     addiu     $t1, $zero, 0x0
    addiu      $t0, $zero, 0x0
  .L8002595C:
    subu       $t7, $t7, $t6
    sllv       $t7, $t1, $t7
    srlv       $t0, $t0, $t6
    srlv       $t1, $t1, $t6
    or         $t0, $t0, $t7
    addu       $v0, $v1, $zero
  .L80025974:
    xor        $t6, $a3, $a1
    slti       $at, $t6, 0x0
    bnez       $at, .L80025A00
     nop
    addu       $t0, $t0, $t2
    sltu       $t6, $t0, $t2
    addu       $t1, $t1, $t6
    addu       $t1, $t1, $t3
    slti       $at, $t1, 0x0
    bnez       $at, .L800259B4
     nop
    srl        $t6, $t0, 31
    sll        $t1, $t1, 1
    sll        $t0, $t0, 1
    b          .L80025AA4
     or        $t1, $t1, $t6
  .L800259B4:
    addiu      $v0, $v0, 0x1
    addiu      $t6, $zero, 0x7FF
    sltu       $at, $v0, $t6
    bnez       $at, .L80025AA4
     nop
    addiu      $sp, $sp, -0xC
    sw         $ra, 0x4($sp)
    addiu      $a0, $zero, 0x280
    addu       $a1, $t8, $zero
    jal        raiseSoftFloatException
     sw        $t8, 0x8($sp)
    lw         $t8, 0x8($sp)
    lw         $ra, 0x4($sp)
    bgtz       $v0, .L80025AD4
     addiu     $sp, $sp, 0xC
    addu       $v0, $t6, $zero
    addiu      $t0, $zero, 0x0
    b          .L80025AA4
     addiu     $t1, $zero, 0x0
  .L80025A00:
    sltu       $t6, $t0, $t2
    subu       $t1, $t1, $t3
    subu       $t1, $t1, $t6
    slti       $at, $t1, 0x0
    beqz       $at, .L80025A30
     subu      $t0, $t0, $t2
    sltu       $t6, $zero, $t0
    negu       $t1, $t1
    lui        $at, (0x80000000 >> 16)
    negu       $t0, $t0
    subu       $t1, $t1, $t6
    xor        $t4, $t4, $at
  .L80025A30:
    slti       $at, $t1, 0x0
    bnez       $at, .L80025AA4
     addiu     $t6, $zero, -0x1
    addiu      $at, $zero, 0x0
    bne        $t1, $at, .L80025A78
     slti      $at, $t0, 0x0
    bnez       $at, .L80025A6C
     addiu     $t6, $zero, 0x1F
    addiu      $at, $zero, 0x0
    beq        $t0, $at, .L80025ADC
     nop
  .L80025A5C:
    sll        $t0, $t0, 1
    slti       $at, $t0, 0x1
    beqz       $at, .L80025A5C
     addiu     $t6, $t6, 0x1
  .L80025A6C:
    addu       $t1, $t0, $zero
    b          .L80025A94
     addiu     $t0, $zero, 0x0
  .L80025A78:
    sll        $t1, $t1, 1
    srl        $t7, $t0, 31
    or         $t1, $t1, $t7
    slti       $at, $t1, 0x1
    sll        $t0, $t0, 1
    beqz       $at, .L80025A78
     addiu     $t6, $t6, 0x1
  .L80025A94:
    subu       $v0, $v0, $t6
    slti       $at, $v0, 0x0
    bnez       $at, .L80025BAC
     nop
  .L80025AA4:
    sll        $t1, $t1, 1
    srl        $t1, $t1, 1
    addiu      $t0, $t0, 0x400
    sltiu      $t7, $t0, 0x400
    addu       $t1, $t1, $t7
    srl        $a1, $t1, 11
    srl        $a0, $t0, 11
    sll        $t7, $t1, 21
    sll        $v0, $v0, 20
    addu       $a1, $v0, $a1
    or         $a0, $a0, $t7
    or         $a1, $a1, $t4
  .L80025AD4:
    jr         $ra
     nop
  .L80025ADC:
    addiu      $a0, $zero, 0x0
    b          .L80025AD4
     addiu     $a1, $zero, 0x0
    addiu      $a0, $zero, 0x0
    b          .L80025AD4
     addu      $a1, $t4, $zero
  .L80025AF4:
    lui        $t6, (0xFFE00000 >> 16)
    beq        $t6, $v0, .L80025B58
     sltu      $at, $t6, $v0
    bnez       $at, .L80025BAC
     nop
    beq        $t6, $v1, .L80025B4C
     sltu      $at, $t6, $v1
    bnez       $at, .L80025BAC
     nop
    or         $t7, $a0, $v0
    addiu      $at, $zero, 0x0
    beq        $t7, $at, .L80025B3C
     nop
    or         $t7, $a2, $v1
    bne        $t7, $at, .L8002589C
     nop
    b          .L80025AD4
     nop
  .L80025B3C:
    or         $t7, $a2, $v1
    addiu      $at, $zero, 0x0
    beq        $t7, $at, .L80025BD0
     nop
  .L80025B4C:
    addu       $a0, $a2, $zero
    b          .L80025AD4
     addu      $a1, $a3, $zero
  .L80025B58:
    sltu       $at, $t6, $v1
    bnez       $at, .L80025BAC
     sltu      $at, $v1, $t6
    bnez       $at, .L80025AD4
     nop
    xor        $t6, $a3, $a1
    slti       $at, $t6, 0x0
    beqz       $at, .L80025AD4
     nop
    addiu      $sp, $sp, -0xC
    sw         $ra, 0x4($sp)
    addiu      $a0, $zero, 0x800
    addu       $a1, $t8, $zero
    jal        raiseSoftFloatException
     sw        $t8, 0x8($sp)
    lw         $t8, 0x8($sp)
    lw         $ra, 0x4($sp)
    bgtz       $v0, .L80025AD4
     addiu     $sp, $sp, 0xC
    b          .L80025AD4
     addiu     $a0, $zero, -0x1
  .L80025BAC:
    addiu      $sp, $sp, -0xC
    sw         $ra, 0x4($sp)
    addu       $a0, $t8, $zero
    jal        raiseSoftFloatUnimplemented
     sw        $t8, 0x8($sp)
    lw         $t8, 0x8($sp)
    lw         $ra, 0x4($sp)
    b          .L80025AD4
     addiu     $sp, $sp, 0xC
  .L80025BD0:
    and        $a1, $a3, $a1
    b          .L80025AD4
     addiu     $a0, $zero, 0x0
endlabel addDoubleCore

glabel __divdf3
    addiu      $sp, $sp, -0x18
    sw         $ra, 0x4($sp)
    jal        divDoubleCore
     nop
    lw         $ra, 0x4($sp)
    addiu      $sp, $sp, 0x18
    addu       $v1, $a1, $zero
    jr         $ra
     addu      $v0, $a0, $zero
endlabel __divdf3

glabel divDoubleCore
    xor        $t9, $a3, $a1
    lui        $t6, (0x200000 >> 16)
    srl        $t9, $t9, 31
    sll        $v0, $a1, 1
    addu       $t7, $t6, $v0
    sltu       $at, $t6, $t7
    sll        $t9, $t9, 31
    beqz       $at, .L80025E0C
     sll       $v1, $a3, 1
    addu       $t7, $t6, $v1
    sltu       $at, $t6, $t7
    beqz       $at, .L80025E0C
     nop
  .L80025C34:
    srl        $t7, $a0, 21
    sll        $t3, $a3, 11
    lui        $at, %hi(D_80000000)
    sll        $t1, $a1, 11
    or         $t1, $t1, $t7
    srl        $t7, $a2, 21
    or         $t3, $t3, $t7
    addiu      $t7, $at, %lo(D_80000000)
    or         $t3, $t3, $at
    srl        $t6, $t3, 16
    divu       $zero, $t7, $t6
    or         $t1, $t1, $at
    addiu      $at, $zero, 0x0
    sll        $t0, $a0, 11
    sll        $t2, $a2, 11
    srl        $v0, $v0, 21
    mflo       $t7
    sll        $t7, $t7, 16
    nop
    multu      $t7, $t3
    mfhi       $a3
    negu       $a3, $a3
    nop
    multu      $a3, $t7
    mfhi       $a3
    sll        $a3, $a3, 1
    bne        $a3, $at, .L80025CA8
     srl       $v1, $v1, 21
    addiu      $a3, $zero, -0x1
  .L80025CA8:
    multu      $a3, $t3
    mflo       $t6
    mfhi       $t7
    nop
    nop
    multu      $a3, $t2
    mfhi       $t5
    addu       $t6, $t5, $t6
    sltu       $a2, $t6, $t5
    addu       $t7, $a2, $t7
    negu       $t7, $t7
    sltu       $a2, $zero, $t6
    subu       $t7, $t7, $a2
    multu      $t7, $a3
    negu       $t6, $t6
    mflo       $t2
    mfhi       $t3
    nop
    nop
    multu      $t6, $a3
    mfhi       $t5
    addu       $t2, $t5, $t2
    sltu       $t4, $t2, $t5
    addu       $t3, $t4, $t3
    sll        $t3, $t3, 1
    srl        $t6, $t2, 31
    or         $t3, $t6, $t3
    multu      $t1, $t3
    sll        $t2, $t2, 1
    mflo       $t6
    mfhi       $t7
    nop
    nop
    multu      $t0, $t3
    mfhi       $t5
    addu       $t6, $t5, $t6
    sltu       $a2, $t6, $t5
    multu      $t1, $t2
    addu       $t7, $a2, $t7
    mfhi       $a3
    addu       $t4, $a3, $t6
    sltu       $a2, $t4, $a3
    addu       $t5, $a2, $t7
    slti       $at, $t5, 0x0
    bnez       $at, .L80025D74
     nop
    srl        $t6, $t4, 31
    sll        $t5, $t5, 1
    sll        $t4, $t4, 1
    or         $t5, $t5, $t6
    addiu      $v0, $v0, -0x1
  .L80025D74:
    addiu      $v0, $v0, 0x3FF
    subu       $v0, $v0, $v1
    addiu      $t6, $zero, 0x7FE
    sltu       $at, $t6, $v0
    bnez       $at, .L80025DC4
     nop
  .L80025D8C:
    sll        $t5, $t5, 1
    srl        $t5, $t5, 1
    addiu      $t4, $t4, 0x400
    sltiu      $t7, $t4, 0x400
    addu       $t5, $t5, $t7
    srl        $a1, $t5, 11
    srl        $a0, $t4, 11
    sll        $t7, $t5, 21
    sll        $v0, $v0, 20
    addu       $a1, $v0, $a1
    or         $a0, $a0, $t7
    or         $a1, $a1, $t9
  .L80025DBC:
    jr         $ra
     nop
  .L80025DC4:
    slti       $at, $v0, 0x0
    bnez       $at, .L80025ECC
     nop
    sw         $ra, 0xC($sp)
    lw         $a1, 0x4($sp)
    jal        raiseSoftFloatException
     addiu     $a0, $zero, 0x280
    lw         $ra, 0xC($sp)
    bgtz       $v0, .L80025DBC
     nop
    addiu      $v0, $t6, 0x1
    addiu      $t4, $zero, 0x0
    b          .L80025D8C
     addiu     $t5, $zero, 0x0
    addiu      $v0, $zero, 0x0
    addiu      $t4, $zero, 0x0
    b          .L80025D8C
     addiu     $t5, $zero, 0x0
  .L80025E0C:
    lui        $t6, (0xFFE00000 >> 16)
    beq        $t6, $v0, .L80025EA0
     sltu      $at, $t6, $v0
    bnez       $at, .L80025ECC
     nop
    beq        $t6, $v1, .L80025E90
     sltu      $at, $t6, $v1
    bnez       $at, .L80025ECC
     nop
    or         $t7, $a0, $v0
    addiu      $at, $zero, 0x0
    beq        $t7, $at, .L80025E80
     nop
    or         $t7, $a2, $v1
    bne        $t7, $at, .L80025C34
     nop
  .L80025E4C:
    sw         $t9, 0x8($sp)
    sw         $ra, 0xC($sp)
    lw         $a1, 0x4($sp)
    jal        raiseSoftFloatException
     addiu     $a0, $zero, 0x400
    lw         $ra, 0xC($sp)
    bgtz       $v0, .L80025DBC
     nop
    lw         $t9, 0x8($sp)
    lui        $a1, (0x7FF00000 >> 16)
    addiu      $a0, $zero, 0x0
    b          .L80025DBC
     or        $a1, $a1, $t9
  .L80025E80:
    or         $t7, $a2, $v1
    addiu      $at, $zero, 0x0
    beq        $t7, $at, .L80025EAC
     nop
  .L80025E90:
    addiu      $a1, $zero, 0x0
    addiu      $a0, $zero, 0x0
    b          .L80025DBC
     or        $a1, $a1, $t9
  .L80025EA0:
    sltu       $at, $v1, $t6
    bnez       $at, .L80025E4C
     nop
  .L80025EAC:
    sw         $ra, 0xC($sp)
    lw         $a1, 0x4($sp)
    jal        raiseSoftFloatException
     addiu     $a0, $zero, 0x800
    lw         $ra, 0xC($sp)
    bgtz       $v0, .L80025DBC
     nop
    addiu      $a0, $zero, -0x1
  .L80025ECC:
    sw         $ra, 0xC($sp)
    lw         $a0, 0x4($sp)
    jal        raiseSoftFloatUnimplemented
     nop
    lw         $ra, 0xC($sp)
endlabel divDoubleCore
    nop

glabel __muldf3
    addiu      $sp, $sp, -0x18
    sw         $ra, 0x4($sp)
    jal        mulDoubleCore
     nop
    lw         $ra, 0x4($sp)
    addiu      $sp, $sp, 0x18
    addu       $v1, $a1, $zero
    jr         $ra
     addu      $v0, $a0, $zero
endlabel __muldf3

glabel mulDoubleCore
    xor        $t9, $a3, $a1
    lui        $t6, (0x200000 >> 16)
    srl        $t9, $t9, 31
    sll        $v0, $a1, 1
    addu       $t7, $t6, $v0
    sltu       $at, $t6, $t7
    sll        $t9, $t9, 31
    beqz       $at, .L80026060
     sll       $v1, $a3, 1
    addu       $t7, $t6, $v1
    sltu       $at, $t6, $t7
    beqz       $at, .L80026060
     nop
  .L80025F3C:
    sll        $t3, $a3, 11
    sll        $t1, $a1, 11
    srl        $t7, $a0, 21
    or         $t1, $t1, $t7
    srl        $t7, $a2, 21
    or         $t3, $t3, $t7
    sll        $t2, $a2, 11
    lui        $at, (0x80000000 >> 16)
    or         $t3, $t3, $at
    or         $t1, $t1, $at
    multu      $t1, $t3
    sll        $t0, $a0, 11
    srl        $v0, $v0, 21
    mflo       $t6
    mfhi       $t7
    nop
    nop
    multu      $t0, $t3
    mfhi       $t5
    addu       $t6, $t5, $t6
    sltu       $a2, $t6, $t5
    multu      $t1, $t2
    addu       $t7, $a2, $t7
    mfhi       $a3
    addu       $t6, $a3, $t6
    sltu       $a2, $t6, $a3
    addu       $t7, $a2, $t7
    slti       $at, $t7, 0x0
    bnez       $at, .L80025FC8
     srl       $v1, $v1, 21
    srl        $a2, $t6, 31
    sll        $t7, $t7, 1
    sll        $t6, $t6, 1
    or         $t7, $t7, $a2
    addiu      $v0, $v0, -0x1
  .L80025FC8:
    addiu      $v0, $v0, -0x3FE
    addu       $v0, $v0, $v1
    addiu      $t4, $zero, 0x7FE
    sltu       $at, $t4, $v0
    bnez       $at, .L80026018
     nop
  .L80025FE0:
    sll        $t7, $t7, 1
    srl        $t7, $t7, 1
    addiu      $t6, $t6, 0x400
    sltiu      $t4, $t6, 0x400
    addu       $t7, $t7, $t4
    srl        $a1, $t7, 11
    srl        $a0, $t6, 11
    sll        $t4, $t7, 21
    sll        $v0, $v0, 20
    addu       $a1, $v0, $a1
    or         $a0, $a0, $t4
    or         $a1, $a1, $t9
  .L80026010:
    jr         $ra
     nop
  .L80026018:
    slti       $at, $v0, 0x0
    bnez       $at, .L800260EC
     nop
    sw         $ra, 0xC($sp)
    lw         $a1, 0x4($sp)
    jal        raiseSoftFloatException
     addiu     $a0, $zero, 0x280
    lw         $ra, 0xC($sp)
    bgtz       $v0, .L80026010
     nop
    addiu      $v0, $t4, 0x1
    addiu      $t6, $zero, 0x0
    b          .L80025FE0
     addiu     $t7, $zero, 0x0
    addiu      $v0, $zero, 0x0
    addiu      $t6, $zero, 0x0
    b          .L80025FE0
     addiu     $t7, $zero, 0x0
  .L80026060:
    lui        $t6, (0xFFE00000 >> 16)
    beq        $t6, $v0, .L800260B0
     sltu      $at, $t6, $v0
    bnez       $at, .L800260EC
     sltu      $at, $t6, $v1
    bnez       $at, .L800260EC
     nop
    beq        $t6, $v1, .L80026108
     nop
    or         $t7, $a0, $v0
    addiu      $at, $zero, 0x0
    beq        $t7, $at, .L800260A0
     nop
    or         $t7, $a2, $v1
    bne        $t7, $at, .L80025F3C
     nop
  .L800260A0:
    addiu      $a1, $zero, 0x0
    addiu      $a0, $zero, 0x0
    b          .L80026010
     or        $a1, $a1, $t9
  .L800260B0:
    sltu       $at, $t6, $v1
    bnez       $at, .L800260EC
     addiu     $at, $zero, 0x0
    bne        $v1, $at, .L80026118
     nop
  .L800260C4:
    sw         $ra, 0xC($sp)
    lw         $a1, 0x4($sp)
    jal        raiseSoftFloatException
     addiu     $a0, $zero, 0x800
    lw         $ra, 0xC($sp)
    bgtz       $v0, .L80026010
     nop
    addiu      $a0, $zero, -0x1
    b          .L80026010
     addiu     $a1, $zero, -0x1
  .L800260EC:
    sw         $ra, 0xC($sp)
    lw         $a0, 0x4($sp)
    jal        raiseSoftFloatUnimplemented
     nop
    lw         $ra, 0xC($sp)
    b          .L80026010
     nop
  .L80026108:
    or         $t7, $a0, $v0
    addiu      $at, $zero, 0x0
    beq        $t7, $at, .L800260C4
     nop
  .L80026118:
    lui        $a1, (0x7FF00000 >> 16)
    or         $a1, $a1, $t9
    b          .L80026010
     addiu     $a0, $zero, 0x0
endlabel mulDoubleCore

glabel __cmpdf2
    addu       $t8, $a0, $zero
    addu       $a0, $a1, $zero
    addu       $a1, $t8, $zero
    addu       $t8, $a2, $zero
    addu       $a2, $a3, $zero
    sll        $v0, $a0, 1
    lui        $t6, (0xFFE00000 >> 16)
    sltu       $at, $v0, $t6
    addu       $a3, $t8, $zero
    beqz       $at, .L800261E0
     sll       $t0, $a2, 1
    sltu       $at, $t0, $t6
    beqz       $at, .L80026248
     nop
    sra        $t6, $a0, 31
    lui        $at, (0x7FFFFFFF >> 16)
    ori        $at, $at, (0x7FFFFFFF & 0xFFFF)
    xor        $t0, $t6, $a1
    sltu       $t7, $t0, $t6
    and        $t1, $a0, $at
    xor        $t1, $t6, $t1
    and        $t3, $a2, $at
    subu       $t1, $t1, $t6
    subu       $t0, $t0, $t6
    sra        $t6, $a2, 31
    xor        $t2, $t6, $a3
    xor        $t3, $t6, $t3
    subu       $t1, $t1, $t7
    sltu       $t7, $t2, $t6
    subu       $t3, $t3, $t6
    subu       $t2, $t2, $t6
    subu       $t3, $t3, $t7
    slt        $at, $t1, $t3
    bnez       $at, .L800261D8
     addiu     $a0, $zero, -0x1
    slt        $at, $t3, $t1
    bnez       $at, .L800261D8
     addiu     $a0, $zero, 0x1
    sltu       $at, $t2, $t0
    bnez       $at, .L800261D8
     sltu      $at, $t0, $t2
    bnez       $at, .L800261D8
     addiu     $a0, $zero, -0x1
    addiu      $a0, $zero, 0x0
  .L800261D8:
    jr         $ra
     addu      $v0, $a0, $zero
  .L800261E0:
    beq        $t6, $v0, .L80026200
     nop
  .L800261E8:
    b          .L800261D8
     addiu     $a0, $zero, 0x3
  .L800261F0:
    b          .L800261D8
     addiu     $a0, $zero, 0x1
  .L800261F8:
    b          .L800261D8
     addiu     $a0, $zero, -0x1
  .L80026200:
    sltu       $at, $t6, $t0
    bnez       $at, .L800261E8
     sltu      $at, $t0, $t6
    bnez       $at, .L80026234
     nop
    xor        $t6, $a2, $a0
    slti       $at, $t6, 0x0
    beqz       $at, .L800261E8
     slti      $at, $a0, 0x0
    beqz       $at, .L800261F0
     nop
    b          .L800261F8
     nop
  .L80026234:
    slti       $at, $a0, 0x0
    bnez       $at, .L800261F8
     nop
    b          .L800261F0
     nop
  .L80026248:
    sltu       $at, $t6, $t0
    bnez       $at, .L800261E8
     slti      $at, $a2, 0x0
    bnez       $at, .L800261F0
     nop
    b          .L800261F8
     nop
  /* the libgcc entry, GCC's int to double conversion: returns the double in
     $v0/$v1 */
  alabel __floatsidf
    addu       $t8, $ra, $zero
    jal        intToDoubleCore
     nop
    addu       $v1, $a1, $zero
    jr         $t8
     addu      $v0, $a0, $zero
endlabel __cmpdf2

glabel intToDoubleCore
    sra        $t4, $a0, 31
    xor        $v1, $a0, $t4
    subu       $v1, $v1, $t4
    addu       $t7, $v1, $zero
    slti       $at, $t7, 0x0
    addiu      $v0, $zero, 0x413
    bnez       $at, .L800262B8
     addiu     $t6, $zero, 0x1F
    addiu      $at, $zero, 0x0
    beq        $t7, $at, .L80026310
     nop
  .L800262A8:
    sll        $t7, $t7, 1
    slti       $at, $t7, 0x1
    beqz       $at, .L800262A8
     addiu     $t6, $t6, -0x1
  .L800262B8:
    addiu      $at, $zero, 0x14
    subu       $t6, $at, $t6
    slti       $at, $t6, 0x0
    bnez       $at, .L800262D8
     nop
    sllv       $a1, $v1, $t6
    b          .L800262E8
     addiu     $a0, $zero, 0x0
  .L800262D8:
    negu       $t7, $t6
    srlv       $a1, $v1, $t7
    addiu      $t7, $t6, 0x20
    sllv       $a0, $v1, $t7
  .L800262E8:
    lui        $at, (0xFFEFFFFF >> 16)
    ori        $at, $at, (0xFFEFFFFF & 0xFFFF)
    subu       $v0, $v0, $t6
    and        $a1, $a1, $at
    sll        $v0, $v0, 20
    or         $a1, $a1, $v0
    sll        $t4, $t4, 31
    or         $a1, $a1, $t4
  .L80026308:
    jr         $ra
     nop
  .L80026310:
    addiu      $a0, $zero, 0x0
    b          .L80026308
     addiu     $a1, $zero, 0x0
endlabel intToDoubleCore

glabel __fixdfsi
    sll        $v0, $a1, 1
    beqz       $v0, .L80026400
     nop
    lui        $t6, (0xFFE00000 >> 16)
    sltu       $at, $t6, $v0
    bnez       $at, .L80026400
     lui       $at, (0x40000000 >> 16)
    sll        $v1, $a1, 10
    or         $v1, $v1, $at
    lui        $at, (0x7FFFFFFF >> 16)
    ori        $at, $at, (0x7FFFFFFF & 0xFFFF)
    and        $v1, $v1, $at
    srl        $t6, $a0, 22
    or         $v1, $t6, $v1
    slti       $at, $a1, 0x0
    srl        $v0, $v0, 21
    bnez       $at, .L8002638C
     addiu     $t6, $zero, 0x41D
    sltu       $at, $t6, $v0
    bnez       $at, .L800263B4
     nop
    subu       $v0, $t6, $v0
    sltiu      $at, $v0, 0x20
    beqz       $at, .L80026400
     nop
    srlv       $a0, $v1, $v0
  .L80026384:
    jr         $ra
     addu      $v0, $a0, $zero
  .L8002638C:
    sltu       $at, $t6, $v0
    bnez       $at, .L800263D8
     nop
    subu       $v0, $t6, $v0
    sltiu      $at, $v0, 0x20
    beqz       $at, .L80026400
     nop
    srlv       $v1, $v1, $v0
    b          .L80026384
     negu      $a0, $v1
  .L800263B4:
    addiu      $sp, $sp, -0xC
    sw         $ra, 0x8($sp)
    addu       $a1, $ra, $zero
    jal        raiseSoftFloatException
     addiu     $a0, $zero, 0x800
    lw         $ra, 0x8($sp)
    bgtz       $v0, .L80026384
     addiu     $sp, $sp, 0xC
    addiu      $a0, $zero, -0x1
  .L800263D8:
    addiu      $sp, $sp, -0xC
    sw         $ra, 0x8($sp)
    addu       $a1, $ra, $zero
    jal        raiseSoftFloatException
     addiu     $a0, $zero, 0x800
    lw         $ra, 0x8($sp)
    bgtz       $v0, .L80026384
     addiu     $sp, $sp, 0xC
    b          .L80026384
     lui       $a0, (0x80000000 >> 16)
  .L80026400:
    addiu      $sp, $sp, -0xC
    sw         $ra, 0x8($sp)
    addu       $a1, $ra, $zero
    jal        raiseSoftFloatException
     addiu     $a0, $zero, 0x800
    lw         $ra, 0x8($sp)
    bgtz       $v0, .L80026384
     addiu     $sp, $sp, 0xC
    b          .L80026384
     addiu     $a0, $zero, 0x0
    addu       $t8, $a0, $zero
    addu       $a0, $a1, $zero
    lui        $t6, (0x200000 >> 16)
    sll        $v0, $a0, 1
    srl        $t4, $a0, 31
    addu       $t7, $t6, $v0
    sltu       $at, $t6, $t7
    addu       $a1, $t8, $zero
    beqz       $at, .L800264B0
     sll       $t4, $t4, 31
    addiu      $t6, $zero, 0x380
    srl        $v0, $v0, 21
    subu       $v0, $v0, $t6
    addiu      $t6, $zero, 0xFF
    sltu       $at, $v0, $t6
    beqz       $at, .L8002649C
     nop
    sll        $v1, $a0, 12
    srl        $v1, $v1, 9
    sll        $v0, $v0, 23
    or         $v0, $v1, $v0
    srl        $t7, $a1, 27
    srl        $v1, $a1, 29
    andi       $t7, $t7, 0x1
    addu       $a0, $v0, $v1
    addu       $a0, $a0, $t7
  .L80026490:
    or         $a0, $a0, $t4
  .L80026494:
    jr         $ra
     addu      $v0, $a0, $zero
  .L8002649C:
    slti       $at, $v0, 0x0
    bnez       $at, .L800264D8
     nop
    b          .L800264C8
     nop
  .L800264B0:
    beq        $t7, $t6, .L800264D8
     nop
    lui        $t6, (0xFFE00000 >> 16)
    sltu       $at, $t6, $v0
    bnez       $at, .L800264D0
     nop
  .L800264C8:
    b          .L80026490
     lui       $a0, (0x7F800000 >> 16)
  .L800264D0:
    b          .L80026494
     addiu     $a0, $zero, -0x1
  .L800264D8:
    b          .L80026490
     addiu     $a0, $zero, 0x0
    addu       $a1, $a0, $zero
    lui        $t6, (0x1000000 >> 16)
    srl        $t4, $a1, 31
    sll        $v0, $a1, 1
    addu       $t7, $t6, $v0
    sltu       $at, $t6, $t7
    beqz       $at, .L8002653C
     sll       $t4, $t4, 31
    srl        $v0, $v0, 24
    sll        $v1, $a1, 9
    addiu      $t6, $zero, -0x380
    subu       $v0, $v0, $t6
    sll        $v0, $v0, 20
    srl        $v1, $v1, 12
    or         $t0, $v1, $v0
    sll        $t1, $a1, 29
  .L80026520:
    or         $t0, $t4, $t0
  .L80026524:
    addu       $v1, $t0, $zero
    jr         $ra
     addu      $v0, $t1, $zero
  .L80026530:
    addiu      $t0, $zero, 0x0
    b          .L80026520
     addiu     $t1, $zero, 0x0
  .L8002653C:
    beq        $t7, $t6, .L80026530
     nop
    lui        $t6, (0xFF000000 >> 16)
    sltu       $at, $t6, $v0
    bnez       $at, .L80026560
     nop
    lui        $t0, (0x7FF00000 >> 16)
    b          .L80026520
     addiu     $t1, $zero, 0x0
  .L80026560:
    addiu      $t0, $zero, -0x1
    b          .L80026524
     addiu     $t1, $zero, -0x1
    lui        $at, (0x80000000 >> 16)
    jr         $ra
     xor       $v0, $a0, $at
endlabel __fixdfsi

glabel __subsf3
    lui        $at, (0x80000000 >> 16)
    xor        $a1, $a1, $at
    lui        $t6, (0x1000000 >> 16)
    sll        $a2, $a0, 1
    addu       $t7, $t6, $a2
    sltu       $at, $t6, $t7
    beqz       $at, .L800266E0
     sll       $t0, $a1, 1
    addu       $t7, $t6, $t0
    sltu       $at, $t6, $t7
    beqz       $at, .L800266E0
     lui       $at, (0x80000000 >> 16)
    sll        $a3, $a0, 8
    sll        $t1, $a1, 8
    or         $a3, $a3, $at
    or         $t1, $t1, $at
    srl        $t4, $a0, 31
    srl        $a2, $a2, 24
    srl        $t0, $t0, 24
    sltu       $at, $a2, $t0
    bnez       $at, .L800265E8
     sll       $t4, $t4, 31
    subu       $t6, $a2, $t0
    sltiu      $at, $t6, 0x20
    bnez       $at, .L80026600
     srlv      $t1, $t1, $t6
    b          .L80026600
     addiu     $t1, $zero, 0x0
  .L800265E8:
    subu       $t6, $t0, $a2
    sltiu      $at, $t6, 0x20
    addu       $a2, $t0, $zero
    bnez       $at, .L80026600
     srlv      $a3, $a3, $t6
    addiu      $a3, $zero, 0x0
  .L80026600:
    xor        $t6, $a1, $a0
    slti       $at, $t6, 0x0
    bnez       $at, .L80026660
     nop
    addu       $a3, $a3, $t1
    sltu       $at, $a3, $t1
    beqz       $at, .L800266AC
     nop
    addiu      $a2, $a2, 0x1
    addiu      $t6, $zero, 0xFF
    sltu       $at, $a2, $t6
    bnez       $at, .L800266AC
     srl       $a3, $a3, 1
    addiu      $sp, $sp, -0xC
    sw         $ra, 0x8($sp)
    addu       $a1, $ra, $zero
    jal        raiseSoftFloatException
     addiu     $a0, $zero, 0x280
    lw         $ra, 0x8($sp)
    bgtz       $v0, .L800266C8
     addiu     $sp, $sp, 0xC
    addu       $a2, $t6, $zero
    b          .L800266AC
     addiu     $a3, $zero, 0x0
  .L80026660:
    sltu       $at, $a3, $t1
    beqz       $at, .L80026678
     lui       $at, (0x80000000 >> 16)
    subu       $a3, $t1, $a3
    b          .L8002667C
     xor       $t4, $t4, $at
  .L80026678:
    subu       $a3, $a3, $t1
  .L8002667C:
    slti       $at, $a3, 0x0
    bnez       $at, .L800266AC
     addiu     $at, $zero, 0x0
    beq        $a3, $at, .L800266D0
     nop
  .L80026690:
    sll        $a3, $a3, 1
    slti       $at, $a3, 0x1
    beqz       $at, .L80026690
     addiu     $a2, $a2, -0x1
    slti       $at, $a2, 0x0
    bnez       $at, .L80026768
     nop
  .L800266AC:
    sll        $a3, $a3, 1
    srl        $a3, $a3, 1
    addiu      $a3, $a3, 0x80
    sll        $a0, $a2, 23
    srl        $a3, $a3, 8
    addu       $a0, $a0, $a3
    or         $a0, $a0, $t4
  .L800266C8:
    jr         $ra
     addu      $v0, $a0, $zero
  .L800266D0:
    b          .L800266C8
     addiu     $a0, $zero, 0x0
    b          .L800266C8
     addu      $a0, $t4, $zero
  .L800266E0:
    lui        $t6, (0xFF000000 >> 16)
    beq        $t6, $a2, .L8002671C
     sltu      $at, $t6, $a2
    bnez       $at, .L80026768
     nop
    beq        $t6, $t0, .L80026714
     sltu      $at, $t6, $t0
    bnez       $at, .L80026768
     addiu     $at, $zero, 0x0
    bne        $a2, $at, .L800266C8
     nop
    beq        $t0, $at, .L80026788
     nop
  .L80026714:
    b          .L800266C8
     addu      $a0, $a1, $zero
  .L8002671C:
    sltu       $at, $t6, $t0
    bnez       $at, .L80026768
     sltu      $at, $t0, $t6
    bnez       $at, .L800266C8
     nop
    xor        $t6, $a1, $a0
    slti       $at, $t6, 0x0
    beqz       $at, .L800266C8
     nop
    addiu      $sp, $sp, -0xC
    sw         $ra, 0x8($sp)
    addu       $a1, $ra, $zero
    jal        raiseSoftFloatException
     addiu     $a0, $zero, 0x800
    lw         $ra, 0x8($sp)
    bgtz       $v0, .L800266C8
     addiu     $sp, $sp, 0xC
    b          .L800266C8
     addiu     $a0, $zero, -0x1
  .L80026768:
    addiu      $sp, $sp, -0xC
    sw         $ra, 0x8($sp)
    addu       $a0, $ra, $zero
    jal        raiseSoftFloatUnimplemented
     nop
    lw         $ra, 0x8($sp)
    b          .L800266C8
     addiu     $sp, $sp, 0xC
  .L80026788:
    b          .L800266C8
     and       $a0, $a1, $a0
    xor        $t4, $a1, $a0
    lui        $t6, (0x1000000 >> 16)
    srl        $t4, $t4, 31
    sll        $a2, $a0, 1
    addu       $t7, $t6, $a2
    sltu       $at, $t6, $t7
    sll        $t4, $t4, 31
    beqz       $at, .L800268B8
     sll       $t0, $a1, 1
    addu       $t7, $t6, $t0
    sltu       $at, $t6, $t7
    beqz       $at, .L800268B8
     lui       $at, (0x80000000 >> 16)
    srl        $a2, $a2, 24
    addiu      $a2, $a2, 0x7F
    srl        $t0, $t0, 24
    sll        $t3, $a0, 8
    or         $t3, $t3, $at
    sll        $t1, $a1, 8
    or         $t1, $t1, $at
    subu       $a2, $a2, $t0
    addiu      $t6, $zero, 0xFE
    sltu       $at, $t6, $a2
    bnez       $at, .L80026874
     nop
    lui        $t7, (0x80000000 >> 16)
    srl        $t6, $t1, 16
    divu       $zero, $t7, $t6
    mflo       $t7
    sll        $t7, $t7, 16
    bnez       $t7, .L80026814
     nop
    addiu      $t7, $zero, -0x1
  .L80026814:
    multu      $t7, $t1
    mfhi       $a3
    negu       $a3, $a3
    nop
    multu      $a3, $t7
    mfhi       $a3
    nop
    nop
    multu      $a3, $t3
    mfhi       $a3
    sll        $a3, $a3, 1
    bltz       $a3, .L80026850
     nop
    sll        $a3, $a3, 1
    addiu      $a2, $a2, -0x1
  .L80026850:
    sll        $a3, $a3, 1
    srl        $a3, $a3, 1
    addiu      $a3, $a3, 0x80
    srl        $a3, $a3, 8
  .L80026860:
    sll        $a2, $a2, 23
    add        $a0, $a2, $a3
    or         $a0, $a0, $t4
  .L8002686C:
    jr         $ra
     addu      $v0, $a0, $zero
  .L80026874:
    slti       $at, $a2, 0x0
    bnez       $at, .L80026928
     nop
    addiu      $sp, $sp, -0xC
    sw         $ra, 0x8($sp)
    addu       $a1, $ra, $zero
    jal        raiseSoftFloatException
     addiu     $a0, $zero, 0x280
    lw         $ra, 0x8($sp)
    bgtz       $v0, .L8002686C
     addiu     $sp, $sp, 0xC
    addiu      $a2, $t6, 0x1
    b          .L80026860
     addiu     $t3, $zero, 0x0
    addiu      $a2, $zero, 0x0
    b          .L80026860
     addiu     $t3, $zero, 0x0
  .L800268B8:
    lui        $t6, (0xFF000000 >> 16)
    beq        $t6, $a2, .L800268F4
     sltu      $at, $t6, $a2
    bnez       $at, .L80026928
     nop
    beq        $t6, $t0, .L800268EC
     sltu      $at, $t6, $t0
    bnez       $at, .L80026928
     addiu     $at, $zero, 0x0
    bne        $a2, $at, .L80026948
     nop
    beq        $t0, $at, .L80026900
     nop
  .L800268EC:
    b          .L8002686C
     addu      $a0, $t4, $zero
  .L800268F4:
    sltu       $at, $t0, $t6
    bnez       $at, .L80026948
     nop
  .L80026900:
    addiu      $sp, $sp, -0xC
    sw         $ra, 0x8($sp)
    addu       $a1, $ra, $zero
    jal        raiseSoftFloatException
     addiu     $a0, $zero, 0x800
    lw         $ra, 0x8($sp)
    bgtz       $v0, .L8002686C
     addiu     $sp, $sp, 0xC
    b          .L8002686C
     addiu     $a0, $zero, -0x1
  .L80026928:
    addiu      $sp, $sp, -0xC
    sw         $ra, 0x8($sp)
    addu       $a0, $ra, $zero
    jal        raiseSoftFloatUnimplemented
     nop
    lw         $ra, 0x8($sp)
    b          .L8002686C
     addiu     $sp, $sp, 0xC
  .L80026948:
    addiu      $sp, $sp, -0xC
    sw         $ra, 0x8($sp)
    addu       $a1, $ra, $zero
    jal        raiseSoftFloatException
     addiu     $a0, $zero, 0x400
    lw         $ra, 0x8($sp)
    bgtz       $v0, .L8002686C
     addiu     $sp, $sp, 0xC
    lui        $a0, (0x7F800000 >> 16)
    b          .L8002686C
     or        $a0, $a0, $t4
endlabel __subsf3

glabel __mulsf3
    xor        $t4, $a1, $a0
    lui        $t6, (0x1000000 >> 16)
    srl        $t4, $t4, 31
    sll        $a2, $a0, 1
    addu       $t7, $t6, $a2
    sltu       $at, $t6, $t7
    sll        $t4, $t4, 31
    beqz       $at, .L80026A58
     sll       $t0, $a1, 1
    addu       $t7, $t6, $t0
    sltu       $at, $t6, $t7
    beqz       $at, .L80026A58
     nop
    srl        $a2, $a2, 24
    lui        $at, (0x80000000 >> 16)
    sll        $a3, $a0, 8
    or         $a3, $a3, $at
    sll        $t1, $a1, 8
    or         $t1, $t1, $at
    multu      $a3, $t1
    srl        $t0, $t0, 24
    mfhi       $t7
    srl        $t6, $t7, 31
    addu       $a2, $a2, $t6
    addu       $a2, $a2, $t0
    srlv       $t7, $t7, $t6
    addiu      $a2, $a2, -0x7F
    addiu      $t6, $zero, 0xFE
    sltu       $at, $t6, $a2
    bnez       $at, .L80026A14
     nop
  .L800269F0:
    sll        $t7, $t7, 2
    srl        $t7, $t7, 2
    addiu      $t7, $t7, 0x40
    srl        $t7, $t7, 7
    sll        $a2, $a2, 23
    addu       $a0, $a2, $t7
    or         $a0, $a0, $t4
  .L80026A0C:
    jr         $ra
     addu      $v0, $a0, $zero
  .L80026A14:
    slti       $at, $a2, 0x0
    bnez       $at, .L80026AC0
     nop
    addiu      $sp, $sp, -0xC
    sw         $ra, 0x8($sp)
    addu       $a1, $ra, $zero
    jal        raiseSoftFloatException
     addiu     $a0, $zero, 0x280
    lw         $ra, 0x8($sp)
    bgtz       $v0, .L80026A0C
     addiu     $sp, $sp, 0xC
    addiu      $a2, $t6, 0x1
    b          .L800269F0
     addiu     $t7, $zero, 0x0
    addiu      $a2, $zero, 0x0
    b          .L800269F0
     addiu     $t7, $zero, 0x0
  .L80026A58:
    lui        $t6, (0xFF000000 >> 16)
    beq        $t6, $a2, .L80026A84
     sltu      $at, $t6, $a2
    bnez       $at, .L80026AC0
     nop
    beq        $t6, $t0, .L80026AE0
     sltu      $at, $t6, $t0
    bnez       $at, .L80026AC0
     nop
    b          .L80026A0C
     addu      $a0, $t4, $zero
  .L80026A84:
    sltu       $at, $t6, $t0
    bnez       $at, .L80026AC0
     addiu     $at, $zero, 0x0
    bne        $t0, $at, .L80026AEC
     nop
  .L80026A98:
    addiu      $sp, $sp, -0xC
    sw         $ra, 0x8($sp)
    addu       $a1, $ra, $zero
    jal        raiseSoftFloatException
     addiu     $a0, $zero, 0x800
    lw         $ra, 0x8($sp)
    bgtz       $v0, .L80026A0C
     addiu     $sp, $sp, 0xC
    b          .L80026A0C
     addiu     $a0, $zero, -0x1
  .L80026AC0:
    addiu      $sp, $sp, -0xC
    sw         $ra, 0x8($sp)
    addu       $a0, $ra, $zero
    jal        raiseSoftFloatUnimplemented
     nop
    lw         $ra, 0x8($sp)
    b          .L80026A0C
     addiu     $sp, $sp, 0xC
  .L80026AE0:
    addiu      $at, $zero, 0x0
    beq        $a2, $at, .L80026A98
     nop
  .L80026AEC:
    lui        $a0, (0x7F800000 >> 16)
    b          .L80026A0C
     or        $a0, $a0, $t4
    sll        $a2, $a0, 1
    lui        $t6, (0xFF000000 >> 16)
    sltu       $at, $a2, $t6
    beqz       $at, .L80026B58
     sll       $t0, $a1, 1
    sltu       $at, $t0, $t6
    beqz       $at, .L80026BC0
     nop
    sra        $t6, $a0, 31
    srl        $a3, $a2, 1
    xor        $a3, $t6, $a3
    srl        $t1, $t0, 1
    subu       $a3, $a3, $t6
    sra        $t6, $a1, 31
    xor        $t1, $t6, $t1
    subu       $t1, $t1, $t6
    slt        $at, $a3, $t1
    bnez       $at, .L80026B50
     addiu     $a0, $zero, -0x1
    bne        $a3, $t1, .L80026B50
     addiu     $a0, $zero, 0x1
    addiu      $a0, $zero, 0x0
  .L80026B50:
    jr         $ra
     addu      $v0, $a0, $zero
  .L80026B58:
    beq        $t6, $a2, .L80026B8C
     nop
  .L80026B60:
    b          .L80026B50
     addiu     $a0, $zero, 0x3
  .L80026B68:
    b          .L80026B50
     addiu     $a0, $zero, 0x1
  .L80026B70:
    b          .L80026B50
     addiu     $a0, $zero, -0x1
  .L80026B78:
    slti       $at, $a0, 0x0
    bnez       $at, .L80026B70
     nop
    b          .L80026B68
     nop
  .L80026B8C:
    sltu       $at, $t6, $t0
    bnez       $at, .L80026B60
     sltu      $at, $t0, $t6
    bnez       $at, .L80026B78
     nop
    xor        $t6, $a1, $a0
    slti       $at, $t6, 0x0
    beqz       $at, .L80026B60
     slti      $at, $a0, 0x0
    beqz       $at, .L80026B68
     nop
    b          .L80026B70
     nop
  .L80026BC0:
    sltu       $at, $t6, $t0
    bnez       $at, .L80026B60
     slti      $at, $a1, 0x0
    bnez       $at, .L80026B68
     nop
    b          .L80026B70
     nop
endlabel __mulsf3

/* 2^31, for __fixunsdfsi */
dlabel TWO_POW_31
    .double 2147483648.0
enddlabel TWO_POW_31

glabel __fixunsdfsi
    addiu      $sp, $sp, -0x24
    sw         $ra, 0xC($sp)
    sw         $s1, 0x10($sp)
    sw         $s0, 0x14($sp)
    lui        $a2, %hi(TWO_POW_31)
    lw         $a2, %lo(TWO_POW_31)($a2)
    lui        $a3, %hi(TWO_POW_31 + 0x4)
    lw         $a3, %lo(TWO_POW_31 + 0x4)($a3)
    addu       $s0, $zero, $a0
    jal        __cmpdf2
     addu      $s1, $zero, $a1
    bgez       $v0, .L80026C2C
     nop
    addu       $a0, $zero, $s0
    jal        __fixdfsi
     addu      $a1, $zero, $s1
    j          .L80026C5C
     nop
  .L80026C2C:
    lui        $a2, %hi(TWO_POW_31)
    lw         $a2, %lo(TWO_POW_31)($a2)
    lui        $a3, %hi(TWO_POW_31 + 0x4)
    lw         $a3, %lo(TWO_POW_31 + 0x4)($a3)
    addu       $a0, $zero, $s0
    jal        __subdf3
     addu      $a1, $zero, $s1
    addu       $a0, $zero, $v0
    jal        __fixdfsi
     addu      $a1, $zero, $v1
    lui        $at, (0x80000000 >> 16)
    addu       $v0, $v0, $at
  .L80026C5C:
    lw         $ra, 0xC($sp)
    lw         $s1, 0x10($sp)
    lw         $s0, 0x14($sp)
    jr         $ra
     addiu     $sp, $sp, 0x24
endlabel __fixunsdfsi

glabel raiseSoftFloatException
    addiu      $sp, $sp, -0x20
    sw         $ra, 0x8($sp)
    sw         $fp, 0xC($sp)
    sw         $s1, 0x10($sp)
    sw         $s0, 0x14($sp)
    addu       $s0, $zero, $a0
    addiu      $fp, $sp, 0x20
    lui        $a0, %hi(SOFT_FLOAT_TRAP_INFO)
    addiu      $a0, $a0, %lo(SOFT_FLOAT_TRAP_INFO)
    addu       $s1, $zero, $a1
    addiu      $sp, $sp, -0x8
    jal        handleSoftFloatTrap
     sll       $v0, $s0, 5
    lui        $a0, %hi(SOFT_FLOAT_STATUS)
    addiu      $a0, $a0, %lo(SOFT_FLOAT_STATUS)
    lw         $a2, 0x0($a0)
    sra        $v1, $s0, 5
    or         $v0, $v0, $v1
    or         $v1, $v0, $a2
    sw         $v1, 0x0($a0)
    lw         $v0, -0x8($a0)
    addiu      $sp, $sp, 0x8
    andi       $v0, $v0, 0x200
    beqz       $v0, .L80026D10
     nop
    lw         $a2, 0x0($a0)
    nop
    and        $v0, $s0, $a2
    beqz       $v0, .L80026D10
     nop
    sw         $v1, 0x4($a0)
    sw         $s1, -0x4($a0)
    addiu      $sp, $sp, -0x8
    jal        handleSoftFloatTrap
     addiu     $a0, $a0, -0xC
    jal        handleSoftFloatTrap
     addiu     $a0, $zero, 0x200
    addiu      $v0, $zero, 0x1
    j          .L80026D14
     addiu     $sp, $sp, 0x8
  .L80026D10:
    addiu      $v0, $zero, 0x0
  .L80026D14:
    addiu      $sp, $fp, -0x20
    lw         $ra, 0x8($sp)
    lw         $fp, 0xC($sp)
    lw         $s1, 0x10($sp)
    lw         $s0, 0x14($sp)
    jr         $ra
     addiu     $sp, $sp, 0x20
endlabel raiseSoftFloatException

glabel raiseSoftFloatUnimplemented
    lui        $v0, %hi(SOFT_FLOAT_STATUS)
    addiu      $v0, $v0, %lo(SOFT_FLOAT_STATUS)
    addiu      $sp, $sp, -0x18
    sw         $ra, 0x8($sp)
    sw         $fp, 0xC($sp)
    lw         $v1, 0x0($v0)
    addiu      $fp, $sp, 0x18
    lui        $at, (0x20000 >> 16)
    or         $v1, $v1, $at
    sw         $v1, 0x0($v0)
    sw         $v1, 0x4($v0)
    sw         $a0, -0x4($v0)
    addiu      $sp, $sp, -0x8
    jal        handleSoftFloatTrap
     addiu     $a0, $zero, 0x200
    addiu      $sp, $sp, 0x8
    addiu      $sp, $fp, -0x18
    lw         $ra, 0x8($sp)
    lw         $fp, 0xC($sp)
    jr         $ra
     addiu     $sp, $sp, 0x18
endlabel raiseSoftFloatUnimplemented

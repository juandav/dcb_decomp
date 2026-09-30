/*
 * The game's task switching and interrupt glue, written in assembly: the
 * routines save and restore a task's registers in its control block
 * (CURRENT_TASK), read and write coprocessor 0's Status and EPC (mfc0 and
 * mtc0 on $12 and $13) and return with `jr ... rfe`, use $k1 and $at as
 * ordinary registers, and have several entry points into one body, none of
 * which a C compiler produces.
 */

.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

glabel disableInterrupts
    mfc0       $t0, $12
    nop
    addu       $t1, $t0, $zero
    addiu      $at, $zero, -0x40
    and        $t0, $t0, $at
    andi       $t1, $t1, 0xF
    sll        $t1, $t1, 2
    or         $t0, $t0, $t1
    mtc0       $t0, $12
    nop
    jr         $ra
     nop
endlabel disableInterrupts

glabel restoreInterrupts
    jr         $ra
     rfe
endlabel restoreInterrupts

glabel launchTaskScheduler
    lui        $at, %hi(TASK_GP)
    sw         $gp, %lo(TASK_GP)($at)
    j          startTaskScheduler
     nop
endlabel launchTaskScheduler

glabel spawnTask
    mfc0       $t0, $12
    nop
    addu       $t1, $t0, $zero
    addiu      $at, $zero, -0x40
    and        $t0, $t0, $at
    andi       $t1, $t1, 0xF
    sll        $t1, $t1, 2
    or         $t0, $t0, $t1
    mtc0       $t0, $12
    nop
    addiu      $sp, $sp, -0x4
    sw         $ra, 0x0($sp)
    jal        createTask
     nop
    lw         $ra, 0x0($sp)
    addiu      $sp, $sp, 0x4
    jr         $ra
     rfe
endlabel spawnTask

glabel endTask
    mfc0       $t0, $12
    nop
    addu       $t1, $t0, $zero
    addiu      $at, $zero, -0x40
    and        $t0, $t0, $at
    andi       $t1, $t1, 0xF
    sll        $t1, $t1, 2
    or         $t0, $t0, $t1
    mtc0       $t0, $12
    nop
    addiu      $sp, $sp, -0x4
    sw         $ra, 0x0($sp)
    jal        killTask
     nop
    lw         $ra, 0x0($sp)
    addiu      $sp, $sp, 0x4
    jr         $ra
     rfe
endlabel endTask

glabel resumeTask
    mfc0       $t0, $12
    nop
    addu       $t1, $t0, $zero
    addiu      $at, $zero, -0x40
    and        $t0, $t0, $at
    andi       $t1, $t1, 0xF
    sll        $t1, $t1, 2
    or         $t0, $t0, $t1
    mtc0       $t0, $12
    nop
    addiu      $sp, $sp, -0x4
    sw         $ra, 0x0($sp)
    jal        wakeTask
     nop
    lw         $ra, 0x0($sp)
    addiu      $sp, $sp, 0x4
    jr         $ra
     rfe
endlabel resumeTask

glabel exitTask
    mfc0       $t0, $12
    nop
    addu       $t1, $t0, $zero
    addiu      $at, $zero, -0x40
    and        $t0, $t0, $at
    andi       $t1, $t1, 0xF
    sll        $t1, $t1, 2
    or         $t0, $t0, $t1
    mtc0       $t0, $12
    nop
    jal        exitCurrentTask
     nop
    j          .L80014B3C
     nop
  alabel yieldTask
    mfc0       $t0, $12
    nop
    addu       $t1, $t0, $zero
    addiu      $at, $zero, -0x40
    and        $t0, $t0, $at
    andi       $t1, $t1, 0xF
    sll        $t1, $t1, 2
    or         $t0, $t0, $t1
    mtc0       $t0, $12
    nop
    lui        $a0, %hi(CURRENT_TASK)
    lw         $a0, %lo(CURRENT_TASK)($a0)
    nop
    sw         $t0, 0xAC($a0)
    sw         $s0, 0x60($a0)
    sw         $s1, 0x64($a0)
    sw         $s2, 0x68($a0)
    sw         $s3, 0x6C($a0)
    sw         $s4, 0x70($a0)
    sw         $s5, 0x74($a0)
    sw         $s6, 0x78($a0)
    sw         $s7, 0x7C($a0)
    sw         $gp, 0x90($a0)
    sw         $sp, 0x94($a0)
    sw         $fp, 0x98($a0)
    sw         $ra, 0xA0($a0)
    addiu      $t0, $zero, -0x8000
    jal        selectNextTask
     sh        $t0, 0x2($a0)
  .L80014B3C:
    lw         $s0, 0x60($v0)
    lw         $s1, 0x64($v0)
    lw         $s2, 0x68($v0)
    lw         $s3, 0x6C($v0)
    lw         $s4, 0x70($v0)
    lw         $s5, 0x74($v0)
    lw         $s6, 0x78($v0)
    lw         $s7, 0x7C($v0)
    lw         $gp, 0x90($v0)
    lw         $t0, 0x0($v0)
    lw         $sp, 0x94($v0)
    sll        $t0, $t0, 2
    bltz       $t0, .L80014B94
     lw        $fp, 0x98($v0)
    lw         $t0, 0xB0($v0)
    lw         $t1, 0xAC($v0)
    mtc0       $t0, $13
    mtc0       $t1, $12
    lw         $ra, 0xA0($v0)
    lw         $v0, 0x28($v0)
    jr         $ra
     rfe
  .L80014B94:
    lw         $a0, 0x30($v0)
    lw         $a1, 0x34($v0)
    lw         $a2, 0x38($v0)
    lw         $a3, 0x3C($v0)
    lw         $t2, 0x48($v0)
    lw         $t3, 0x4C($v0)
    lw         $t4, 0x50($v0)
    lw         $t5, 0x54($v0)
    lw         $t6, 0x58($v0)
    lw         $t7, 0x5C($v0)
    lw         $t8, 0x80($v0)
    lw         $t9, 0x84($v0)
    lw         $ra, 0x9C($v0)
    lw         $t0, 0xA8($v0)
    lw         $t1, 0xA4($v0)
    mtlo       $t0
    mthi       $t1
    lw         $t0, 0xB0($v0)
    lw         $t1, 0xAC($v0)
    mtc0       $t0, $13
    mtc0       $t1, $12
    lw         $t0, 0x40($v0)
    lw         $t1, 0x44($v0)
    lw         $v1, 0x2C($v0)
    lw         $at, 0x24($v0)
    lw         $k1, 0xA0($v0)
    lw         $v0, 0x28($v0)
    jr         $k1
     rfe
  alabel waitFrames
    mfc0       $t0, $12
    nop
    addu       $t1, $t0, $zero
    addiu      $at, $zero, -0x40
    and        $t0, $t0, $at
    andi       $t1, $t1, 0xF
    sll        $t1, $t1, 2
    or         $t0, $t0, $t1
    mtc0       $t0, $12
    nop
    addiu      $at, $zero, -0x5
    and        $t0, $t0, $at
    lui        $k1, %hi(CURRENT_TASK)
    lw         $k1, %lo(CURRENT_TASK)($k1)
    nop
    sw         $t0, 0xAC($k1)
    sw         $s0, 0x60($k1)
    sw         $s1, 0x64($k1)
    sw         $s2, 0x68($k1)
    sw         $s3, 0x6C($k1)
    sw         $s4, 0x70($k1)
    sw         $s5, 0x74($k1)
    sw         $s6, 0x78($k1)
    sw         $s7, 0x7C($k1)
    sw         $gp, 0x90($k1)
    sw         $sp, 0x94($k1)
    lw         $t1, 0x18($k1)
    sw         $fp, 0x98($k1)
    bnez       $t1, .L80014CC8
     sw        $ra, 0x9C($k1)
    lui        $t0, %hi(waitFramesResume)
    addiu      $t0, $t0, %lo(waitFramesResume)
    sw         $t0, 0xA0($k1)
    sw         $a0, 0x4($k1)
    addiu      $t0, $zero, -0x8000
    sh         $t0, 0x2($k1)
    j          .L80014CB4
     nop
  alabel waitFramesResume
    lui        $k1, %hi(CURRENT_TASK)
    lw         $k1, %lo(CURRENT_TASK)($k1)
    nop
    lw         $a0, 0x4($k1)
    nop
  .L80014CB4:
    addiu      $a0, $a0, -0x1
    bgtz       $a0, .L80014CE0
     sw        $a0, 0x4($k1)
    lw         $t0, 0xAC($k1)
    lw         $t1, 0x18($k1)
  .L80014CC8:
    ori        $t0, $t0, 0x4
    lw         $t2, 0x9C($k1)
    sw         $t0, 0xAC($k1)
    sw         $t1, 0x28($k1)
    sw         $t2, 0xA0($k1)
    sw         $zero, 0x18($k1)
  .L80014CE0:
    jal        selectNextTask
     addu      $a0, $k1, $zero
    j          .L80014B3C
     nop
endlabel exitTask

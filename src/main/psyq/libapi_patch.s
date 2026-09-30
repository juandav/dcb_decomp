/*
 * libapi's controller patch: _patch_pad hooks the BIOS pad handler and
 * EnablePAD/DisablePAD jump through the two entry points it finds.
 *
 * Hand-written assembly, not compiler output: _patch_pad uses $at as an
 * ordinary register (`lui $at` / `sw $ra, %lo(..)($at)`), saves $ra in a
 * global instead of a stack frame and reloads it into $ra itself, calls
 * the BIOS through a constant (`addiu $t2,$zero,0xB0; jalr $t2` with the
 * function number in $t1), uses the trapping `addi` that GCC never emits
 * for pointer arithmetic, and counts its clear loop in $t1; EnablePAD and
 * DisablePAD tail-jump through a loaded pointer (`jr $t1`) with no frame,
 * which GCC 2.x (no sibling calls) cannot produce.
 */

.include "macro.inc"

.set noat
.set noreorder

.section .text

glabel EnablePAD
    lui        $t1, %hi(jtbl_801DDC88)
    lw         $t1, %lo(jtbl_801DDC88)($t1)
    nop
    jr         $t1
     nop
endlabel EnablePAD

glabel DisablePAD
    lui        $t1, %hi(jtbl_801DDC8C)
    lw         $t1, %lo(jtbl_801DDC8C)($t1)
    nop
    jr         $t1
     nop
endlabel DisablePAD

glabel _patch_pad
    lui        $at, %hi(D_801DDC80)
    sw         $ra, %lo(D_801DDC80)($at)
    jal        EnterCriticalSection
     nop
    addiu      $t1, $zero, 0x57
    addiu      $t2, $zero, 0xB0
    jalr       $t2
     nop
    lw         $v0, 0x16C($v0)
    addiu      $t1, $zero, 0xB
    addi       $v1, $v0, 0x884
    lui        $at, %hi(jtbl_801DDC88)
    sw         $v1, %lo(jtbl_801DDC88)($at)
    addi       $v1, $v0, 0x894
    lui        $at, %hi(jtbl_801DDC8C)
    sw         $v1, %lo(jtbl_801DDC8C)($at)
  .L8006B00C:
    sw         $zero, 0x594($v0)
    addiu      $v0, $v0, 0x4
    addiu      $t1, $t1, -0x1
    bnez       $t1, .L8006B00C
     nop
    jal        FlushCache
     nop
    lui        $ra, %hi(D_801DDC80)
    lw         $ra, %lo(D_801DDC80)($ra)
    nop
    jr         $ra
     nop
endlabel _patch_pad
    nop
    nop

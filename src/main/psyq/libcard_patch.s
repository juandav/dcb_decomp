/*
 * libcard's BIOS patches: _patch_card, _patch_card2 and _patch_card_info
 * rewrite code inside the BIOS memory card driver, and _copy_memcard_patch
 * copies the replacement code (func_80068A08 .. func_80068A78) to 0xDF80.
 *
 * Hand-written assembly, not compiler output: the patchers keep the
 * caller's $ra in a global (`lui $at; sw $ra,%lo(D_801DD940)($at)`, then
 * `lw $ra` back from it) instead of a stack frame, call the BIOS through
 * the B0 table with the function number in $t1 (`li $t2,0xB0; jalr $t2`),
 * use `addi` (GCC only emits addiu) and copy code with $t2/$t1 as a
 * source/end pair. The copied fragments are not functions at all: they
 * read the BIOS's own $v1 without setting it, jump through absolute BIOS
 * addresses (`lw $v0,-0x2004($v0); jr $v0`, 0xA000DFAC/0xA000DF80) and
 * contain a delay loop on $t0 ending in a bare `jr $ra`.
 */

.include "macro.inc"

.set noat
.set noreorder

.section .text

glabel _patch_card_info
    lui        $at, %hi(D_801DD940)
    sw         $ra, %lo(D_801DD940)($at)
    addiu      $t1, $zero, 0x57
    addiu      $t2, $zero, 0xB0
    jalr       $t2
     nop
    addiu      $t2, $zero, 0x9
    lw         $v0, 0x16C($v0)
    nop
    addi       $v1, $v0, 0x1988
    jal        FlushCache
     sw        $zero, 0x0($v1)
    lui        $ra, %hi(D_801DD940)
    lw         $ra, %lo(D_801DD940)($ra)
    nop
    jr         $ra
     nop
endlabel _patch_card_info

glabel func_80068A08
    lhu        $t7, 0xA($v1)
    lui        $t0, (0x0 >> 16)
    or         $t8, $t7, $v0
    ori        $t9, $t8, 0x12
    sh         $t9, 0xA($v1)
    addiu      $t0, $zero, 0x28
  .L80068A20:
    addiu      $t0, $t0, -0x1
    bnez       $t0, .L80068A20
     nop
    jr         $ra
     nop
endlabel func_80068A08

glabel func_80068A34
    lw         $v0, 0x1074($v1)
    nop
    andi       $v0, $v0, 0x80
    beqz       $v0, .L80068A70
     nop
  .L80068A48:
    lw         $v0, 0x1044($v1)
    nop
    andi       $v0, $v0, 0x80
    bnez       $v0, .L80068A48
     nop
    lui        $v0, (0x10000 >> 16)
    lw         $v0, -0x2004($v0)
    nop
    jr         $v0
     nop
  .L80068A70:
    jr         $ra
     nop
endlabel func_80068A34

glabel func_80068A78
    lui        $v0, %hi(D_A000DFAC)
    addiu      $v0, $v0, %lo(D_A000DFAC)
    jr         $v0
     nop
    nop
    lui        $t0, %hi(D_A000DF80)
    addiu      $t0, $t0, %lo(D_A000DF80)
    jalr       $t0
     nop
endlabel func_80068A78
    nop

glabel _patch_card
    lui        $at, %hi(D_801DD940)
    sw         $ra, %lo(D_801DD940)($at)
    jal        EnterCriticalSection
     nop
    addiu      $t1, $zero, 0x56
    addiu      $t2, $zero, 0xB0
    jalr       $t2
     nop
    lw         $v0, 0x18($v0)
    nop
    lw         $v1, 0x70($v0)
    nop
    andi       $t1, $v1, 0xFFFF
    sll        $t1, $t1, 16
    lw         $v1, 0x74($v0)
    nop
    andi       $t2, $v1, 0xFFFF
    addu       $v1, $t1, $t2
    addiu      $v0, $v1, 0x28
    lui        $t2, %hi(func_80068A78)
    addiu      $t2, $t2, %lo(func_80068A78)
    lui        $t1, %hi(func_80068A78 + 0x14)
    addiu      $t1, $t1, %lo(func_80068A78 + 0x14)
  .L80068AFC:
    lw         $v1, 0x0($t2)
    nop
    sw         $v1, 0x0($v0)
    addiu      $t2, $t2, 0x4
    bne        $t2, $t1, .L80068AFC
     addiu     $v0, $v0, 0x4
    lui        $at, (0x10000 >> 16)
    jal        FlushCache
     sw        $v0, -0x2004($at)
    lui        $ra, %hi(D_801DD940)
    lw         $ra, %lo(D_801DD940)($ra)
    nop
    jr         $ra
     nop
endlabel _patch_card

glabel _patch_card2
    lui        $at, %hi(D_801DD940)
    sw         $ra, %lo(D_801DD940)($at)
    jal        EnterCriticalSection
     nop
    addiu      $t1, $zero, 0x57
    addiu      $t2, $zero, 0xB0
    jalr       $t2
     nop
    lw         $v0, 0x16C($v0)
    nop
    lw         $v1, 0x9C8($v0)
    lui        $t2, %hi(func_80068A78 + 0x14)
    addiu      $t2, $t2, %lo(func_80068A78 + 0x14)
    lui        $t1, %hi(_patch_card)
    addiu      $t1, $t1, %lo(_patch_card)
  .L80068B70:
    lw         $t0, 0x0($t2)
    nop
    sw         $t0, 0x9C8($v0)
    addiu      $t2, $t2, 0x4
    bne        $t2, $t1, .L80068B70
     addiu     $v0, $v0, 0x4
    jal        FlushCache
     nop
    lui        $ra, %hi(D_801DD940)
    lw         $ra, %lo(D_801DD940)($ra)
    nop
    jr         $ra
     nop
endlabel _patch_card2

glabel _copy_memcard_patch
    ori        $v0, $zero, 0xDF80
    lui        $t2, %hi(func_80068A08)
    addiu      $t2, $t2, %lo(func_80068A08)
    lui        $t1, %hi(func_80068A78)
    addiu      $t1, $t1, %lo(func_80068A78)
  .L80068BB8:
    lw         $v1, 0x0($t2)
    nop
    sw         $v1, 0x0($v0)
    addiu      $t2, $t2, 0x4
    bne        $t2, $t1, .L80068BB8
     addiu     $v0, $v0, 0x4
    jr         $ra
     nop
endlabel _copy_memcard_patch
    nop
    nop
    nop

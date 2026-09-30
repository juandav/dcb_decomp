/*
 * libgte PATCHGTE and the libgs GsTMDfast* packet builders that follow it:
 * _patch_gte, the exception-handler code it copies (func_8005FBB0),
 * NormalColorCol, NormalColorCol3, func_8005FC54 and GsTMDfastF3L through
 * GsTMDfastTG4L.
 *
 * Hand-written assembly, not compiler output:
 * - _patch_gte calls the BIOS through `li $t2,0xB0; jalr $t2; li $t1,0x56`,
 *   keeps $ra in a global instead of a stack frame, and copies the words
 *   between func_8005FBB0 and D_8005FBE0 over the BIOS exception handler;
 *   those words are cop0 code (`mfc0 $v1,$14`, stores through $k0).
 * - Every GTE command (rtpt, rtps, nclip, avsz3, avsz4, nccs, ncct) has one
 *   nop or none in front. PsyQ's C GTE macros (inline_c.h and inline_o.h,
 *   DMPSX 3) always emit two nops before each command; only inline_a.h,
 *   the macros for assembler programs, has the bare command.
 * - The GsTMDfast* functions load their stack arguments into $t7-$t9
 *   while $v0/$v1 and $t0 are free, count down with `addiu $t8,$t8,-1`
 *   (or $a3), and end in `addu $v0,...; jr $ra; nop`, a delay slot the
 *   PsyQ build (GCC 2.7.2 + ASPSX in reorder mode) fills with the addu.
 * - func_8005FC54 is the GTE leading-zero count (mtc2 to register 30, two
 *   nops placed by hand, mfc2 from register 31), the same sequence that
 *   opens SquareRoot0 in MSC01.
 */

.include "macro.inc"

.set noat
.set noreorder

.section .text

glabel _patch_gte
    lui        $at, %hi(D_801DBD58)
    sw         $ra, %lo(D_801DBD58)($at)
    jal        EnterCriticalSection
     nop
    addiu      $t1, $zero, 0x56
    addiu      $t2, $zero, 0xB0
    jalr       $t2
     nop
    lw         $v0, 0x18($v0)
    nop
    addiu      $v0, $v0, 0x28
    addu       $t7, $v0, $zero
    lui        $t2, %hi(func_8005FBB0)
    addiu      $t2, $t2, %lo(func_8005FBB0)
    lui        $t1, %hi(D_8005FBC8)
    addiu      $t1, $t1, %lo(D_8005FBC8)
  .L8005FB44:
    lw         $v1, 0x0($t2)
    lw         $t3, 0x0($v0)
    addiu      $t2, $t2, 0x4
    bne        $v1, $t3, .L8005FB8C
     addiu     $v0, $v0, 0x4
    bne        $t2, $t1, .L8005FB44
     nop
    addu       $v0, $t7, $zero
    lui        $t2, %hi(D_8005FBC8)
    addiu      $t2, $t2, %lo(D_8005FBC8)
    lui        $t1, %hi(D_8005FBE0)
    addiu      $t1, $t1, %lo(D_8005FBE0)
  .L8005FB74:
    lw         $v1, 0x0($t2)
    nop
    sw         $v1, 0x0($v0)
    addiu      $t2, $t2, 0x4
    bne        $t2, $t1, .L8005FB74
     addiu     $v0, $v0, 0x4
  .L8005FB8C:
    jal        FlushCache
     nop
    jal        ExitCriticalSection
     nop
    lui        $ra, %hi(D_801DBD58)
    lw         $ra, %lo(D_801DBD58)($ra)
    nop
    jr         $ra
     nop
endlabel _patch_gte

glabel func_8005FBB0
    sw         $at, 0x4($k0)
    sw         $v0, 0x8($k0)
    sw         $v1, 0xC($k0)
    sw         $ra, 0x7C($k0)
    mfc0       $v1, $14
    nop
  alabel D_8005FBC8
    sw         $at, 0x4($k0)
    sw         $v0, 0x8($k0)
    mfc0       $v0, $13
    sw         $v1, 0xC($k0)
    mfc0       $v1, $14
    sw         $ra, 0x7C($k0)
endlabel func_8005FBB0
  alabel D_8005FBE0
    nop

glabel NormalColorCol
    lwc2       $0, 0x0($a0)
    lwc2       $1, 0x4($a0)
    lwc2       $6, 0x0($a1)
    nop
    nccs
    swc2       $22, 0x0($a2)
    jr         $ra
     nop
endlabel NormalColorCol

glabel NormalColorCol3
    lwc2       $0, 0x0($a0)
    lwc2       $1, 0x4($a0)
    lwc2       $2, 0x0($a1)
    lwc2       $3, 0x4($a1)
    lwc2       $4, 0x0($a2)
    lwc2       $5, 0x4($a2)
    lwc2       $6, 0x0($a3)
    nop
    ncct
    lw         $t0, 0x10($sp)
    lw         $t1, 0x14($sp)
    lw         $t2, 0x18($sp)
    swc2       $20, 0x0($t0)
    swc2       $21, 0x0($t1)
    swc2       $22, 0x0($t2)
    jr         $ra
     nop
endlabel NormalColorCol3
    nop
    nop
    nop

glabel func_8005FC54
    mtc2       $a0, $30
    nop
    nop
    mfc2       $v0, $31
    jr         $ra
     nop
endlabel func_8005FC54
    nop
    nop

glabel GsTMDfastF3L
    lw         $t8, 0x10($sp)
    lw         $t9, 0x18($sp)
    lw         $t7, 0x14($sp)
    beqz       $t8, .L8005FDB0
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
    lw         $t2, 0x8($a0)
    lw         $t3, 0xC($a0)
    srl        $t4, $t2, 16
    sll        $t4, $t4, 3
    addu       $t4, $t4, $a1
  .L8005FCA0:
    lwc2       $0, 0x0($t4)
    lwc2       $1, 0x4($t4)
    sll        $t5, $t3, 16
    srl        $t5, $t5, 13
    addu       $t5, $t5, $a1
    lwc2       $2, 0x0($t5)
    lwc2       $3, 0x4($t5)
    srl        $t5, $t3, 16
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lwc2       $4, 0x0($t5)
    lwc2       $5, 0x4($t5)
    nop
    rtpt
    sll        $t5, $t2, 16
    srl        $t5, $t5, 13
    addu       $t5, $t5, $a2
    lui        $t3, %hi(D_801DBFA4)
    lw         $t3, %lo(D_801DBFA4)($t3)
    lw         $t2, 0x4($a0)
    sll        $t3, $t3, 25
    or         $t2, $t2, $t3
    mtc2       $t2, $6
    addiu      $a0, $a0, 0x10
    lw         $t2, 0x8($a0)
    lw         $t3, 0xC($a0)
    srl        $t4, $t2, 16
    sll        $t4, $t4, 3
    addu       $t4, $t4, $a1
    cfc2       $v0, $31
    nop
    bltz       $v0, .L8005FDA4
     nop
    nclip
    mfc2       $v0, $24
    nop
    blez       $v0, .L8005FDA4
     nop
    avsz3
    lwc2       $0, 0x0($t5)
    lwc2       $1, 0x4($t5)
    mfc2       $t6, $7
    nop
    nccs
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0x4000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a3)
    xor        $v0, $a3, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    swc2       $12, 0x8($a3)
    swc2       $13, 0xC($a3)
    swc2       $14, 0x10($a3)
    swc2       $22, 0x4($a3)
    addiu      $a3, $a3, 0x14
  .L8005FDA4:
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L8005FCA0
     nop
  .L8005FDB0:
    addu       $v0, $a3, $zero
    jr         $ra
     nop
endlabel GsTMDfastF3L
    nop
    nop

glabel GsTMDfastNF4
    lw         $t9, 0x14($sp)
    lw         $t7, 0x10($sp)
    beqz       $a3, .L8005FF20
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
    lw         $t2, 0x8($a0)
    lhu        $t4, 0xC($a0)
    srl        $t3, $t2, 16
    sll        $t2, $t2, 16
    srl        $t2, $t2, 13
    sll        $t3, $t3, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
  .L8005FE00:
    lwc2       $0, 0x0($t2)
    lwc2       $1, 0x4($t2)
    lwc2       $2, 0x0($t3)
    lwc2       $3, 0x4($t3)
    lwc2       $4, 0x0($t4)
    lwc2       $5, 0x4($t4)
    nop
    rtpt
    lui        $t3, %hi(D_801DBFA4)
    lw         $t3, %lo(D_801DBFA4)($t3)
    lw         $t2, 0x4($a0)
    sll        $t3, $t3, 25
    or         $t2, $t2, $t3
    mtc2       $t2, $22
    lhu        $v1, 0xE($a0)
    addiu      $a0, $a0, 0x10
    sll        $v1, $v1, 3
    addu       $v1, $v1, $a1
    lw         $t2, 0x8($a0)
    lhu        $t4, 0xC($a0)
    srl        $t3, $t2, 16
    sll        $t2, $t2, 16
    srl        $t2, $t2, 13
    sll        $t3, $t3, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
    cfc2       $v0, $31
    nop
    bltz       $v0, .L8005FF14
     nop
    nclip
    lwc2       $0, 0x0($v1)
    lwc2       $1, 0x4($v1)
    mfc2       $v0, $24
    nop
    blez       $v0, .L8005FF14
     nop
    swc2       $22, 0x4($a2)
    swc2       $12, 0x8($a2)
    swc2       $13, 0xC($a2)
    swc2       $14, 0x10($a2)
    nop
    rtps
    cfc2       $v0, $31
    nop
    bltz       $v0, .L8005FF14
     nop
    avsz4
    mfc2       $t6, $7
    swc2       $14, 0x14($a2)
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0x5000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a2)
    xor        $v0, $a2, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    addiu      $a2, $a2, 0x18
  .L8005FF14:
    addiu      $a3, $a3, -0x1
    bnez       $a3, .L8005FE00
     nop
  .L8005FF20:
    addu       $v0, $a2, $zero
    jr         $ra
     nop
endlabel GsTMDfastNF4
    nop
    nop

glabel GsTMDfastF4NL
    lw         $t8, 0x10($sp)
    lw         $t9, 0x18($sp)
    lw         $t7, 0x14($sp)
    beqz       $t8, .L80060094
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
    lw         $t3, 0xC($a0)
    lhu        $t2, 0xA($a0)
    srl        $t4, $t3, 16
    sll        $t3, $t3, 16
    srl        $t3, $t3, 13
    sll        $t2, $t2, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
  .L8005FF74:
    lwc2       $0, 0x0($t2)
    lwc2       $1, 0x4($t2)
    lwc2       $2, 0x0($t3)
    lwc2       $3, 0x4($t3)
    lwc2       $4, 0x0($t4)
    lwc2       $5, 0x4($t4)
    nop
    rtpt
    lui        $t3, %hi(D_801DBFA4)
    lw         $t3, %lo(D_801DBFA4)($t3)
    lw         $t2, 0x4($a0)
    sll        $t3, $t3, 25
    or         $t2, $t2, $t3
    mtc2       $t2, $22
    lhu        $v1, 0x10($a0)
    addiu      $a0, $a0, 0x14
    sll        $v1, $v1, 3
    addu       $v1, $v1, $a1
    lw         $t3, 0xC($a0)
    lhu        $t2, 0xA($a0)
    srl        $t4, $t3, 16
    sll        $t3, $t3, 16
    srl        $t3, $t3, 13
    sll        $t2, $t2, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80060088
     nop
    nclip
    lwc2       $0, 0x0($v1)
    lwc2       $1, 0x4($v1)
    mfc2       $v0, $24
    nop
    blez       $v0, .L80060088
     nop
    swc2       $22, 0x4($a3)
    swc2       $12, 0x8($a3)
    swc2       $13, 0xC($a3)
    swc2       $14, 0x10($a3)
    nop
    rtps
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80060088
     nop
    avsz4
    mfc2       $t6, $7
    swc2       $14, 0x14($a3)
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0x5000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a3)
    xor        $v0, $a3, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    addiu      $a3, $a3, 0x18
  .L80060088:
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L8005FF74
     nop
  .L80060094:
    addu       $v0, $a3, $zero
    jr         $ra
     nop
endlabel GsTMDfastF4NL
    nop

glabel GsTMDfastF4L
    lw         $t8, 0x10($sp)
    lw         $t9, 0x18($sp)
    lw         $t7, 0x14($sp)
    beqz       $t8, .L80060210
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
    lw         $t2, 0x8($a0)
    lw         $t3, 0xC($a0)
    srl        $t4, $t2, 16
    sll        $t4, $t4, 3
    addu       $t4, $t4, $a1
  .L800600D0:
    lwc2       $0, 0x0($t4)
    lwc2       $1, 0x4($t4)
    sll        $t5, $t3, 16
    srl        $t5, $t5, 13
    addu       $t5, $t5, $a1
    lwc2       $2, 0x0($t5)
    lwc2       $3, 0x4($t5)
    srl        $t5, $t3, 16
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lwc2       $4, 0x0($t5)
    lwc2       $5, 0x4($t5)
    nop
    rtpt
    sll        $t5, $t2, 16
    srl        $t5, $t5, 13
    addu       $t5, $t5, $a2
    lhu        $v1, 0x10($a0)
    lui        $t3, %hi(D_801DBFA4)
    lw         $t3, %lo(D_801DBFA4)($t3)
    lw         $t2, 0x4($a0)
    sll        $t3, $t3, 25
    or         $t2, $t2, $t3
    mtc2       $t2, $6
    addiu      $a0, $a0, 0x14
    sll        $v1, $v1, 3
    addu       $v1, $v1, $a1
    lw         $t2, 0x8($a0)
    lw         $t3, 0xC($a0)
    srl        $t4, $t2, 16
    sll        $t4, $t4, 3
    addu       $t4, $t4, $a1
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80060204
     nop
    nclip
    lwc2       $0, 0x0($v1)
    lwc2       $1, 0x4($v1)
    mfc2       $v0, $24
    nop
    blez       $v0, .L80060204
     nop
    swc2       $12, 0x8($a3)
    swc2       $13, 0xC($a3)
    swc2       $14, 0x10($a3)
    nop
    rtps
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80060204
     nop
    avsz4
    lwc2       $0, 0x0($t5)
    lwc2       $1, 0x4($t5)
    mfc2       $t6, $7
    nop
    nccs
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0x5000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a3)
    xor        $v0, $a3, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    swc2       $14, 0x14($a3)
    swc2       $22, 0x4($a3)
    addiu      $a3, $a3, 0x18
  .L80060204:
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L800600D0
     nop
  .L80060210:
    addu       $v0, $a3, $zero
    jr         $ra
     nop
endlabel GsTMDfastF4L
    nop
    nop

glabel GsTMDfastG3L
    lw         $t8, 0x10($sp)
    lw         $t9, 0x18($sp)
    lw         $t7, 0x14($sp)
    beqz       $t8, .L800603A8
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
  .L8006023C:
    lw         $t2, 0x8($a0)
    lw         $t3, 0xC($a0)
    lw         $t4, 0x10($a0)
  .L80060248:
    srl        $t5, $t2, 16
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lwc2       $0, 0x0($t5)
    lwc2       $1, 0x4($t5)
    srl        $t5, $t3, 16
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lwc2       $2, 0x0($t5)
    lwc2       $3, 0x4($t5)
    srl        $t5, $t4, 16
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lwc2       $4, 0x0($t5)
    lwc2       $5, 0x4($t5)
    nop
    rtpt
    sll        $t2, $t2, 16
    srl        $t2, $t2, 13
    addu       $t2, $t2, $a2
    sll        $t3, $t3, 16
    srl        $t3, $t3, 13
    addu       $t3, $t3, $a2
    sll        $t4, $t4, 16
    srl        $t4, $t4, 13
    addu       $t4, $t4, $a2
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80060398
     nop
    nclip
    lui        $t6, %hi(D_801DBFA4)
    lw         $t6, %lo(D_801DBFA4)($t6)
    lw         $t5, 0x4($a0)
    sll        $t6, $t6, 25
    or         $t5, $t5, $t6
    mtc2       $t5, $6
    mfc2       $v0, $24
    nop
    blez       $v0, .L80060398
     nop
    avsz3
    lwc2       $0, 0x0($t2)
    lwc2       $1, 0x4($t2)
    lwc2       $2, 0x0($t3)
    lwc2       $3, 0x4($t3)
    lwc2       $4, 0x0($t4)
    lwc2       $5, 0x4($t4)
    mfc2       $t6, $7
    nop
    ncct
    addiu      $a0, $a0, 0x14
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0x6000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a3)
    xor        $v0, $a3, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    lw         $t2, 0x8($a0)
    lw         $t3, 0xC($a0)
    lw         $t4, 0x10($a0)
    nop
    swc2       $12, 0x8($a3)
    swc2       $13, 0x10($a3)
    swc2       $14, 0x18($a3)
    swc2       $20, 0x4($a3)
    swc2       $21, 0xC($a3)
    swc2       $22, 0x14($a3)
    addiu      $a3, $a3, 0x1C
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L80060248
     nop
    b          .L800603A8
     nop
  .L80060398:
    addiu      $a0, $a0, 0x14
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L8006023C
     nop
  .L800603A8:
    addu       $v0, $a3, $zero
    jr         $ra
     nop
endlabel GsTMDfastG3L

glabel GsTMDfastG4L
    lw         $t8, 0x10($sp)
    lw         $t9, 0x18($sp)
    lw         $t7, 0x14($sp)
    beqz       $t8, .L80060588
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
  .L800603CC:
    lw         $t2, 0x8($a0)
    lw         $t3, 0xC($a0)
    lw         $t4, 0x10($a0)
  .L800603D8:
    srl        $t5, $t2, 16
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lwc2       $0, 0x0($t5)
    lwc2       $1, 0x4($t5)
    srl        $t5, $t3, 16
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lwc2       $2, 0x0($t5)
    lwc2       $3, 0x4($t5)
    srl        $t5, $t4, 16
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lwc2       $4, 0x0($t5)
    lwc2       $5, 0x4($t5)
    nop
    rtpt
    lw         $t5, 0x14($a0)
    sll        $t2, $t2, 16
    srl        $t2, $t2, 13
    addu       $t2, $t2, $a2
    sll        $t3, $t3, 16
    srl        $t3, $t3, 13
    addu       $t3, $t3, $a2
    sll        $t4, $t4, 16
    srl        $t4, $t4, 13
    addu       $t4, $t4, $a2
    srl        $v1, $t5, 16
    sll        $v1, $v1, 3
    addu       $v1, $v1, $a1
    sll        $t5, $t5, 16
    srl        $t5, $t5, 13
    addu       $t5, $t5, $a2
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80060578
     nop
    nclip
    lwc2       $0, 0x0($v1)
    lwc2       $1, 0x4($v1)
    mfc2       $v1, $24
    nop
    blez       $v1, .L80060578
     nop
    swc2       $12, 0x8($a3)
    swc2       $13, 0x10($a3)
    swc2       $14, 0x18($a3)
    nop
    rtps
    lui        $t6, %hi(D_801DBFA4)
    lw         $t6, %lo(D_801DBFA4)($t6)
    lw         $v0, 0x4($a0)
    sll        $t6, $t6, 25
    or         $v0, $v0, $t6
    mtc2       $v0, $6
    lwc2       $2, 0x0($t3)
    lwc2       $3, 0x4($t3)
    lwc2       $4, 0x0($t4)
    lwc2       $5, 0x4($t4)
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80060578
     nop
    avsz4
    lwc2       $0, 0x0($t5)
    lwc2       $1, 0x4($t5)
    mfc2       $t6, $7
    nop
    nccs
    swc2       $22, 0x1C($a3)
    lwc2       $0, 0x0($t2)
    lwc2       $1, 0x4($t2)
    ncct
    addiu      $a0, $a0, 0x18
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0x8000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a3)
    xor        $v0, $a3, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    lw         $t2, 0x8($a0)
    lw         $t3, 0xC($a0)
    lw         $t4, 0x10($a0)
    nop
    swc2       $14, 0x20($a3)
    swc2       $20, 0x4($a3)
    swc2       $21, 0xC($a3)
    swc2       $22, 0x14($a3)
    addiu      $a3, $a3, 0x24
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L800603D8
     nop
    b          .L80060588
     nop
  .L80060578:
    addiu      $a0, $a0, 0x18
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L800603CC
     nop
  .L80060588:
    addu       $v0, $a3, $zero
    jr         $ra
     nop
endlabel GsTMDfastG4L

glabel GsTMDfastTNF3
    lw         $t9, 0x14($sp)
    lw         $t7, 0x10($sp)
    beqz       $a3, .L800606F4
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
    lw         $t2, 0x14($a0)
    lhu        $t4, 0x18($a0)
    srl        $t3, $t2, 16
    sll        $t2, $t2, 16
    srl        $t2, $t2, 13
    sll        $t3, $t3, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
  .L800605D0:
    lwc2       $0, 0x0($t2)
    lwc2       $1, 0x4($t2)
    lwc2       $2, 0x0($t3)
    lwc2       $3, 0x4($t3)
    lwc2       $4, 0x0($t4)
    lwc2       $5, 0x4($t4)
    nop
    rtpt
    lw         $v0, 0x10($a0)
    lui        $t3, %hi(D_801DBFA4)
    lw         $t3, %lo(D_801DBFA4)($t3)
    lb         $t2, 0x3($a0)
    sll        $t3, $t3, 1
    or         $t2, $t2, $t3
    sll        $v0, $v0, 8
    srl        $v0, $v0, 8
    srl        $t2, $t2, 1
    sll        $t2, $t2, 25
    or         $t2, $t2, $v0
    mtc2       $t2, $22
    lw         $t2, 0x4($a0)
    lw         $t3, 0x8($a0)
    lw         $t4, 0xC($a0)
    sw         $t2, 0xC($a2)
    sw         $t3, 0x14($a2)
    sw         $t4, 0x1C($a2)
    addiu      $a0, $a0, 0x1C
    lw         $t2, 0x14($a0)
    lhu        $t4, 0x18($a0)
    srl        $t3, $t2, 16
    sll        $t2, $t2, 16
    srl        $t2, $t2, 13
    sll        $t3, $t3, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
    cfc2       $v0, $31
    nop
    bltz       $v0, .L800606E8
     nop
    nclip
    mfc2       $v0, $24
    nop
    blez       $v0, .L800606E8
     nop
    avsz3
    mfc2       $t6, $7
    nop
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0x7000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a2)
    xor        $v0, $a2, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    swc2       $22, 0x4($a2)
    swc2       $12, 0x8($a2)
    swc2       $13, 0x10($a2)
    swc2       $14, 0x18($a2)
    addiu      $a2, $a2, 0x20
  .L800606E8:
    addiu      $a3, $a3, -0x1
    bnez       $a3, .L800605D0
     nop
  .L800606F4:
    addu       $v0, $a2, $zero
    jr         $ra
     nop
endlabel GsTMDfastTNF3
    nop

glabel GsTMDfastTF3NL
    lw         $t8, 0x10($sp)
    lw         $t9, 0x18($sp)
    lw         $t7, 0x14($sp)
    beqz       $t8, .L8006085C
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
    lw         $t3, 0x14($a0)
    lhu        $t2, 0x12($a0)
    srl        $t4, $t3, 16
    sll        $t3, $t3, 16
    srl        $t3, $t3, 13
    sll        $t2, $t2, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
  .L80060744:
    lwc2       $0, 0x0($t2)
    lwc2       $1, 0x4($t2)
    lwc2       $2, 0x0($t3)
    lwc2       $3, 0x4($t3)
    lwc2       $4, 0x0($t4)
    lwc2       $5, 0x4($t4)
    nop
    rtpt
    lui        $v0, (0x808080 >> 16)
    ori        $v0, $v0, (0x808080 & 0xFFFF)
    lui        $t3, %hi(D_801DBFA4)
    lw         $t3, %lo(D_801DBFA4)($t3)
    lb         $t2, 0x3($a0)
    sll        $t3, $t3, 1
    or         $t2, $t2, $t3
    sll        $t2, $t2, 24
    or         $v0, $v0, $t2
    lw         $t2, 0x4($a0)
    lw         $t3, 0x8($a0)
    lw         $t4, 0xC($a0)
    sw         $t2, 0xC($a3)
    sw         $t3, 0x14($a3)
    sw         $t4, 0x1C($a3)
    sw         $v0, 0x4($a3)
    addiu      $a0, $a0, 0x18
    lw         $t3, 0x14($a0)
    lhu        $t2, 0x12($a0)
    srl        $t4, $t3, 16
    sll        $t3, $t3, 16
    srl        $t3, $t3, 13
    sll        $t2, $t2, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80060850
     nop
    nclip
    mfc2       $v0, $24
    nop
    blez       $v0, .L80060850
     nop
    avsz3
    mfc2       $t6, $7
    nop
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0x7000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a3)
    xor        $v0, $a3, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    swc2       $12, 0x8($a3)
    swc2       $13, 0x10($a3)
    swc2       $14, 0x18($a3)
    addiu      $a3, $a3, 0x20
  .L80060850:
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L80060744
     nop
  .L8006085C:
    addu       $v0, $a3, $zero
    jr         $ra
     nop
endlabel GsTMDfastTF3NL
    nop
    nop
    nop

glabel GsTMDfastTF3L
    lw         $t8, 0x10($sp)
    lw         $t9, 0x18($sp)
    lw         $t7, 0x14($sp)
    beqz       $t8, .L800609D8
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
    lw         $t2, 0x10($a0)
    lw         $t3, 0x14($a0)
    srl        $t4, $t2, 16
    sll        $t4, $t4, 3
    addu       $t4, $t4, $a1
  .L800608A0:
    lwc2       $0, 0x0($t4)
    lwc2       $1, 0x4($t4)
    sll        $t5, $t3, 16
    srl        $t5, $t5, 13
    addu       $t5, $t5, $a1
    lwc2       $2, 0x0($t5)
    lwc2       $3, 0x4($t5)
    srl        $t5, $t3, 16
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lwc2       $4, 0x0($t5)
    lwc2       $5, 0x4($t5)
    nop
    rtpt
    sll        $t5, $t2, 16
    srl        $t5, $t5, 13
    addu       $t5, $t5, $a2
    lui        $v0, (0x808080 >> 16)
    ori        $v0, $v0, (0x808080 & 0xFFFF)
    lui        $t3, %hi(D_801DBFA4)
    lw         $t3, %lo(D_801DBFA4)($t3)
    lb         $t2, 0x3($a0)
    sll        $t3, $t3, 1
    or         $t2, $t2, $t3
    sll        $t2, $t2, 24
    or         $v0, $v0, $t2
    mtc2       $v0, $6
    lw         $t2, 0x4($a0)
    lw         $t3, 0x8($a0)
    lw         $t4, 0xC($a0)
    sw         $t2, 0xC($a3)
    sw         $t3, 0x14($a3)
    sw         $t4, 0x1C($a3)
    addiu      $a0, $a0, 0x18
    lw         $t2, 0x10($a0)
    lw         $t3, 0x14($a0)
    srl        $t4, $t2, 16
    sll        $t4, $t4, 3
    addu       $t4, $t4, $a1
    cfc2       $v0, $31
    nop
    bltz       $v0, .L800609CC
     nop
    nclip
    mfc2       $v0, $24
    nop
    blez       $v0, .L800609CC
     nop
    avsz3
    lwc2       $0, 0x0($t5)
    lwc2       $1, 0x4($t5)
    mfc2       $t6, $7
    nop
    nccs
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0x7000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a3)
    xor        $v0, $a3, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    swc2       $12, 0x8($a3)
    swc2       $13, 0x10($a3)
    swc2       $14, 0x18($a3)
    swc2       $22, 0x4($a3)
    addiu      $a3, $a3, 0x20
  .L800609CC:
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L800608A0
     nop
  .L800609D8:
    addu       $v0, $a3, $zero
    jr         $ra
     nop
endlabel GsTMDfastTF3L

glabel GsTMDfastTNF4
    lw         $t9, 0x14($sp)
    lw         $t7, 0x10($sp)
    beqz       $a3, .L80060B78
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
    lw         $t2, 0x18($a0)
    lhu        $t4, 0x1C($a0)
    srl        $t3, $t2, 16
    sll        $t2, $t2, 16
    srl        $t2, $t2, 13
    sll        $t3, $t3, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
  .L80060A20:
    lwc2       $0, 0x0($t2)
    lwc2       $1, 0x4($t2)
    lwc2       $2, 0x0($t3)
    lwc2       $3, 0x4($t3)
    lwc2       $4, 0x0($t4)
    lwc2       $5, 0x4($t4)
    nop
    rtpt
    lw         $v0, 0x14($a0)
    lhu        $v1, 0x1E($a0)
    lui        $t3, %hi(D_801DBFA4)
    lw         $t3, %lo(D_801DBFA4)($t3)
    lb         $t2, 0x3($a0)
    sll        $t3, $t3, 1
    or         $t2, $t2, $t3
    sll        $v0, $v0, 8
    srl        $v0, $v0, 8
    srl        $t2, $t2, 1
    sll        $t2, $t2, 25
    or         $v0, $v0, $t2
    sw         $v0, 0x4($a2)
    addiu      $a0, $a0, 0x20
    sll        $v1, $v1, 3
    addu       $v1, $v1, $a1
    lw         $t2, 0x18($a0)
    lhu        $t4, 0x1C($a0)
    srl        $t3, $t2, 16
    sll        $t2, $t2, 16
    srl        $t2, $t2, 13
    sll        $t3, $t3, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80060B6C
     nop
    nclip
    lwc2       $0, 0x0($v1)
    lwc2       $1, 0x4($v1)
    mfc2       $v0, $24
    nop
    blez       $v0, .L80060B6C
     nop
    swc2       $12, 0x8($a2)
    swc2       $13, 0x10($a2)
    swc2       $14, 0x18($a2)
    nop
    rtps
    addiu      $v0, $a0, -0x20
    lw         $v1, 0x4($v0)
    lw         $t6, 0x8($v0)
    sw         $v1, 0xC($a2)
    sw         $t6, 0x14($a2)
    lw         $v1, 0xC($v0)
    lw         $t6, 0x10($v0)
    sw         $v1, 0x1C($a2)
    sw         $t6, 0x24($a2)
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80060B6C
     nop
    avsz4
    mfc2       $t6, $7
    swc2       $14, 0x20($a2)
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0x9000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a2)
    xor        $v0, $a2, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    addiu      $a2, $a2, 0x28
  .L80060B6C:
    addiu      $a3, $a3, -0x1
    bnez       $a3, .L80060A20
     nop
  .L80060B78:
    addu       $v0, $a2, $zero
    jr         $ra
     nop
endlabel GsTMDfastTNF4

glabel GsTMDfastTF4NL
    lw         $t8, 0x10($sp)
    lw         $t9, 0x18($sp)
    lw         $t7, 0x14($sp)
    beqz       $t8, .L80060D14
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
    lw         $t3, 0x18($a0)
    lhu        $t2, 0x16($a0)
    srl        $t4, $t3, 16
    sll        $t3, $t3, 16
    srl        $t3, $t3, 13
    sll        $t2, $t2, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
  .L80060BC4:
    lwc2       $0, 0x0($t2)
    lwc2       $1, 0x4($t2)
    lwc2       $2, 0x0($t3)
    lwc2       $3, 0x4($t3)
    lwc2       $4, 0x0($t4)
    lwc2       $5, 0x4($t4)
    nop
    rtpt
    lhu        $v1, 0x1C($a0)
    addiu      $a0, $a0, 0x20
    sll        $v1, $v1, 3
    addu       $v1, $v1, $a1
    lw         $t3, 0x18($a0)
    lhu        $t2, 0x16($a0)
    srl        $t4, $t3, 16
    sll        $t3, $t3, 16
    srl        $t3, $t3, 13
    sll        $t2, $t2, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80060D08
     nop
    nclip
    lwc2       $0, 0x0($v1)
    lwc2       $1, 0x4($v1)
    mfc2       $v0, $24
    nop
    blez       $v0, .L80060D08
     nop
    swc2       $12, 0x8($a3)
    swc2       $13, 0x10($a3)
    swc2       $14, 0x18($a3)
    nop
    rtps
    addiu      $v0, $a0, -0x20
    lw         $v1, 0x4($v0)
    lw         $t6, 0x8($v0)
    sw         $v1, 0xC($a3)
    sw         $t6, 0x14($a3)
    lw         $v1, 0xC($v0)
    lw         $t6, 0x10($v0)
    sw         $v1, 0x1C($a3)
    sw         $t6, 0x24($a3)
    lui        $t6, %hi(D_801DBFA4)
    lw         $t6, %lo(D_801DBFA4)($t6)
    lb         $v1, 0x3($v0)
    sll        $t6, $t6, 1
    or         $v1, $v1, $t6
    sll        $v1, $v1, 24
    lui        $t6, (0x808080 >> 16)
    ori        $t6, $t6, (0x808080 & 0xFFFF)
    or         $v1, $v1, $t6
    sw         $v1, 0x4($a3)
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80060D08
     nop
    avsz4
    mfc2       $t6, $7
    swc2       $14, 0x20($a3)
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0x9000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a3)
    xor        $v0, $a3, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    addiu      $a3, $a3, 0x28
  .L80060D08:
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L80060BC4
     nop
  .L80060D14:
    addu       $v0, $a3, $zero
    jr         $ra
     nop
endlabel GsTMDfastTF4NL
    nop

glabel GsTMDfastTF4L
    lw         $t8, 0x10($sp)
    lw         $t9, 0x18($sp)
    lw         $t7, 0x14($sp)
    beqz       $t8, .L80060EC4
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
    lw         $t2, 0x14($a0)
    lw         $t3, 0x18($a0)
    srl        $t4, $t2, 16
    sll        $t4, $t4, 3
    addu       $t4, $t4, $a1
  .L80060D50:
    lwc2       $0, 0x0($t4)
    lwc2       $1, 0x4($t4)
    sll        $t5, $t3, 16
    srl        $t5, $t5, 13
    addu       $t5, $t5, $a1
    lwc2       $2, 0x0($t5)
    lwc2       $3, 0x4($t5)
    srl        $t5, $t3, 16
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lwc2       $4, 0x0($t5)
    lwc2       $5, 0x4($t5)
    nop
    rtpt
    lui        $t6, %hi(D_801DBFA4)
    lw         $t6, %lo(D_801DBFA4)($t6)
    lb         $v1, 0x3($a0)
    sll        $t6, $t6, 1
    or         $v1, $v1, $t6
    sll        $v1, $v1, 24
    lui        $t6, (0x808080 >> 16)
    ori        $t6, $t6, (0x808080 & 0xFFFF)
    or         $v1, $v1, $t6
    mtc2       $v1, $6
    sll        $t5, $t2, 16
    srl        $t5, $t5, 13
    addu       $t5, $t5, $a2
    lhu        $v1, 0x1C($a0)
    addiu      $a0, $a0, 0x20
    sll        $v1, $v1, 3
    addu       $v1, $v1, $a1
    lw         $t2, 0x14($a0)
    lw         $t3, 0x18($a0)
    srl        $t4, $t2, 16
    sll        $t4, $t4, 3
    addu       $t4, $t4, $a1
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80060EB8
     nop
    nclip
    lwc2       $0, 0x0($v1)
    lwc2       $1, 0x4($v1)
    mfc2       $v0, $24
    nop
    blez       $v0, .L80060EB8
     nop
    swc2       $12, 0x8($a3)
    swc2       $13, 0x10($a3)
    swc2       $14, 0x18($a3)
    nop
    rtps
    addiu      $v0, $a0, -0x20
    lw         $v1, 0x4($v0)
    lw         $t6, 0x8($v0)
    sw         $v1, 0xC($a3)
    sw         $t6, 0x14($a3)
    lw         $v1, 0xC($v0)
    lw         $t6, 0x10($v0)
    sw         $v1, 0x1C($a3)
    sw         $t6, 0x24($a3)
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80060EB8
     nop
    avsz4
    lwc2       $0, 0x0($t5)
    lwc2       $1, 0x4($t5)
    mfc2       $t6, $7
    nop
    nccs
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0x9000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a3)
    xor        $v0, $a3, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    swc2       $14, 0x20($a3)
    swc2       $22, 0x4($a3)
    addiu      $a3, $a3, 0x28
  .L80060EB8:
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L80060D50
     nop
  .L80060EC4:
    addu       $v0, $a3, $zero
    jr         $ra
     nop
endlabel GsTMDfastTF4L
    nop

glabel GsTMDfastTNG3
    lw         $t9, 0x14($sp)
    lw         $t7, 0x10($sp)
    beqz       $a3, .L80061040
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
    lw         $t2, 0x1C($a0)
    lhu        $t4, 0x20($a0)
    srl        $t3, $t2, 16
    sll        $t2, $t2, 16
    srl        $t2, $t2, 13
    sll        $t3, $t3, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
  .L80060F10:
    lwc2       $0, 0x0($t2)
    lwc2       $1, 0x4($t2)
    lwc2       $2, 0x0($t3)
    lwc2       $3, 0x4($t3)
    lwc2       $4, 0x0($t4)
    lwc2       $5, 0x4($t4)
    nop
    rtpt
    lui        $t3, %hi(D_801DBFA4)
    lw         $t3, %lo(D_801DBFA4)($t3)
    lb         $t2, 0x3($a0)
    sll        $t3, $t3, 1
    or         $t2, $t2, $t3
    lw         $t3, 0x10($a0)
    srl        $t2, $t2, 1
    sll        $t2, $t2, 25
    sll        $t3, $t3, 8
    srl        $t3, $t3, 8
    or         $t2, $t2, $t3
    mtc2       $t2, $20
    lwc2       $21, 0x14($a0)
    lwc2       $22, 0x18($a0)
    lw         $t2, 0x4($a0)
    lw         $t3, 0x8($a0)
    lw         $t4, 0xC($a0)
    sw         $t2, 0xC($a2)
    sw         $t3, 0x18($a2)
    sw         $t4, 0x24($a2)
    addiu      $a0, $a0, 0x24
    lw         $t2, 0x1C($a0)
    lhu        $t4, 0x20($a0)
    srl        $t3, $t2, 16
    sll        $t2, $t2, 16
    srl        $t2, $t2, 13
    sll        $t3, $t3, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80061034
     nop
    nclip
    mfc2       $v0, $24
    nop
    blez       $v0, .L80061034
     nop
    avsz3
    mfc2       $t6, $7
    swc2       $12, 0x8($a2)
    swc2       $13, 0x14($a2)
    swc2       $14, 0x20($a2)
    swc2       $20, 0x4($a2)
    swc2       $21, 0x10($a2)
    swc2       $22, 0x1C($a2)
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0x9000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a2)
    xor        $v0, $a2, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    addiu      $a2, $a2, 0x28
  .L80061034:
    addiu      $a3, $a3, -0x1
    bnez       $a3, .L80060F10
     nop
  .L80061040:
    addu       $v0, $a2, $zero
    jr         $ra
     nop
endlabel GsTMDfastTNG3
    nop
    nop

glabel GsTMDfastTG3NL
    lw         $t8, 0x10($sp)
    lw         $t9, 0x18($sp)
    lw         $t7, 0x14($sp)
    beqz       $t8, .L800611A4
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
    lhu        $t2, 0x12($a0)
    lhu        $t3, 0x16($a0)
    lhu        $t4, 0x1A($a0)
    sll        $t2, $t2, 3
    sll        $t3, $t3, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
  .L80061090:
    lwc2       $0, 0x0($t2)
    lwc2       $1, 0x4($t2)
    lwc2       $2, 0x0($t3)
    lwc2       $3, 0x4($t3)
    lwc2       $4, 0x0($t4)
    lwc2       $5, 0x4($t4)
    nop
    rtpt
    lwc2       $20, 0x4($a0)
    lwc2       $21, 0x8($a0)
    lwc2       $22, 0xC($a0)
    lb         $t2, 0x3($a0)
    lui        $t3, %hi(D_801DBFA4)
    lw         $t3, %lo(D_801DBFA4)($t3)
    andi       $t2, $t2, 0x2
    sll        $t3, $t3, 1
    or         $t2, $t3, $t2
    sll        $t2, $t2, 24
    lui        $v1, (0x25808080 >> 16)
    ori        $v1, $v1, (0x25808080 & 0xFFFF)
    or         $v1, $v1, $t2
    sw         $v1, 0x4($a3)
    addiu      $a0, $a0, 0x1C
    lhu        $t2, 0x12($a0)
    lhu        $t3, 0x16($a0)
    lhu        $t4, 0x1A($a0)
    sll        $t2, $t2, 3
    sll        $t3, $t3, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80061198
     nop
    nclip
    mfc2       $v0, $24
    nop
    blez       $v0, .L80061198
     nop
    avsz3
    mfc2       $t6, $7
    swc2       $20, 0xC($a3)
    swc2       $21, 0x14($a3)
    swc2       $22, 0x1C($a3)
    swc2       $12, 0x8($a3)
    swc2       $13, 0x10($a3)
    swc2       $14, 0x18($a3)
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0x7000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a3)
    xor        $v0, $a3, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    addiu      $a3, $a3, 0x20
  .L80061198:
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L80061090
     nop
  .L800611A4:
    addu       $v0, $a3, $zero
    jr         $ra
     nop
endlabel GsTMDfastTG3NL
    nop

glabel GsTMDfastTG3L
    lw         $t8, 0x10($sp)
    lw         $t9, 0x18($sp)
    lw         $t7, 0x14($sp)
    beqz       $t8, .L8006135C
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
  .L800611CC:
    lw         $t2, 0x10($a0)
    lw         $t3, 0x14($a0)
    lw         $t4, 0x18($a0)
  .L800611D8:
    srl        $t5, $t2, 16
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lwc2       $0, 0x0($t5)
    lwc2       $1, 0x4($t5)
    srl        $t5, $t3, 16
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lwc2       $2, 0x0($t5)
    lwc2       $3, 0x4($t5)
    srl        $t5, $t4, 16
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lwc2       $4, 0x0($t5)
    lwc2       $5, 0x4($t5)
    nop
    rtpt
    lui        $t6, %hi(D_801DBFA4)
    lw         $t6, %lo(D_801DBFA4)($t6)
    lb         $v0, 0x3($a0)
    sll        $t6, $t6, 1
    or         $v0, $v0, $t6
    sll        $v0, $v0, 24
    lui        $t6, (0x808080 >> 16)
    ori        $t6, $t6, (0x808080 & 0xFFFF)
    or         $v0, $v0, $t6
    mtc2       $v0, $6
    sll        $t2, $t2, 16
    srl        $t2, $t2, 13
    addu       $t2, $t2, $a2
    sll        $t3, $t3, 16
    srl        $t3, $t3, 13
    addu       $t3, $t3, $a2
    sll        $t4, $t4, 16
    srl        $t4, $t4, 13
    addu       $t4, $t4, $a2
    cfc2       $v0, $31
    nop
    bltz       $v0, .L8006134C
     nop
    nclip
    mfc2       $v0, $24
    nop
    blez       $v0, .L8006134C
     nop
    avsz3
    lwc2       $0, 0x0($t2)
    lwc2       $1, 0x4($t2)
    lwc2       $2, 0x0($t3)
    lwc2       $3, 0x4($t3)
    lwc2       $4, 0x0($t4)
    lwc2       $5, 0x4($t4)
    mfc2       $t6, $7
    nop
    ncct
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0x9000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a3)
    xor        $v0, $a3, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    lw         $t2, 0x4($a0)
    lw         $t3, 0x8($a0)
    lw         $t4, 0xC($a0)
    sw         $t2, 0xC($a3)
    sw         $t3, 0x18($a3)
    sw         $t4, 0x24($a3)
    swc2       $12, 0x8($a3)
    swc2       $13, 0x14($a3)
    swc2       $14, 0x20($a3)
    swc2       $20, 0x4($a3)
    swc2       $21, 0x10($a3)
    swc2       $22, 0x1C($a3)
    addiu      $a0, $a0, 0x1C
    lw         $t2, 0x10($a0)
    lw         $t3, 0x14($a0)
    lw         $t4, 0x18($a0)
    addiu      $a3, $a3, 0x28
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L800611D8
     nop
    b          .L8006135C
     nop
  .L8006134C:
    addiu      $a0, $a0, 0x1C
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L800611CC
     nop
  .L8006135C:
    addu       $v0, $a3, $zero
    jr         $ra
     nop
endlabel GsTMDfastTG3L
    nop
    nop
    nop

glabel GsTMDfastTNG4
    lw         $t9, 0x14($sp)
    lw         $t7, 0x10($sp)
    beqz       $a3, .L80061524
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
    lw         $t2, 0x24($a0)
    lhu        $t4, 0x28($a0)
    srl        $t3, $t2, 16
    sll        $t2, $t2, 16
    srl        $t2, $t2, 13
    sll        $t3, $t3, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
  .L800613B0:
    lwc2       $0, 0x0($t2)
    lwc2       $1, 0x4($t2)
    lwc2       $2, 0x0($t3)
    lwc2       $3, 0x4($t3)
    lwc2       $4, 0x0($t4)
    lwc2       $5, 0x4($t4)
    nop
    rtpt
    lui        $v1, %hi(D_801DBFA4)
    lw         $v1, %lo(D_801DBFA4)($v1)
    lb         $v0, 0x3($a0)
    sll        $v1, $v1, 1
    or         $v0, $v0, $v1
    lw         $v1, 0x14($a0)
    srl        $v0, $v0, 1
    sll        $v0, $v0, 25
    sll        $v1, $v1, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    mtc2       $v0, $20
    lwc2       $21, 0x18($a0)
    lwc2       $22, 0x1C($a0)
    lwc2       $6, 0x20($a0)
    lhu        $t5, 0x2A($a0)
    addu       $v1, $a0, $zero
    addiu      $a0, $a0, 0x2C
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lw         $t2, 0x24($a0)
    lhu        $t4, 0x28($a0)
    srl        $t3, $t2, 16
    sll        $t2, $t2, 16
    srl        $t2, $t2, 13
    sll        $t3, $t3, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80061518
     nop
    nclip
    lwc2       $0, 0x0($t5)
    lwc2       $1, 0x4($t5)
    mfc2       $v0, $24
    nop
    blez       $v0, .L80061518
     nop
    swc2       $12, 0x8($a2)
    swc2       $13, 0x14($a2)
    swc2       $14, 0x20($a2)
    nop
    rtps
    lw         $t5, 0x4($v1)
    lw         $t6, 0x8($v1)
    cfc2       $v0, $31
    nop
    bltz       $v0, .L80061518
     nop
    avsz4
    lw         $v0, 0xC($v1)
    sw         $t5, 0xC($a2)
    sw         $t6, 0x18($a2)
    sw         $v0, 0x24($a2)
    lw         $t5, 0x10($v1)
    mfc2       $t6, $7
    sw         $t5, 0x30($a2)
    swc2       $14, 0x2C($a2)
    swc2       $20, 0x4($a2)
    swc2       $21, 0x10($a2)
    swc2       $22, 0x1C($a2)
    swc2       $6, 0x28($a2)
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0xC000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a2)
    xor        $v0, $a2, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    addiu      $a2, $a2, 0x34
  .L80061518:
    addiu      $a3, $a3, -0x1
    bnez       $a3, .L800613B0
     nop
  .L80061524:
    addu       $v0, $a2, $zero
    jr         $ra
     nop
endlabel GsTMDfastTNG4
    nop

glabel GsTMDfastTG4NL
    lw         $t8, 0x10($sp)
    lw         $t9, 0x18($sp)
    lw         $t7, 0x14($sp)
    beqz       $t8, .L800616C0
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
    lhu        $t2, 0x16($a0)
    lhu        $t3, 0x1A($a0)
    lhu        $t4, 0x1E($a0)
    sll        $t2, $t2, 3
    sll        $t3, $t3, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
  .L80061570:
    lwc2       $0, 0x0($t2)
    lwc2       $1, 0x4($t2)
    lwc2       $2, 0x0($t3)
    lwc2       $3, 0x4($t3)
    lwc2       $4, 0x0($t4)
    lwc2       $5, 0x4($t4)
    nop
    rtpt
    lhu        $t5, 0x22($a0)
    addu       $v1, $a0, $zero
    addiu      $a0, $a0, 0x24
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lhu        $t2, 0x16($a0)
    lhu        $t3, 0x1A($a0)
    lhu        $t4, 0x1E($a0)
    sll        $t2, $t2, 3
    sll        $t3, $t3, 3
    sll        $t4, $t4, 3
    addu       $t2, $t2, $a1
    addu       $t3, $t3, $a1
    addu       $t4, $t4, $a1
    cfc2       $v0, $31
    nop
    bltz       $v0, .L800616B4
     nop
    nclip
    lwc2       $0, 0x0($t5)
    lwc2       $1, 0x4($t5)
    mfc2       $v0, $24
    nop
    blez       $v0, .L800616B4
     nop
    swc2       $12, 0x8($a3)
    swc2       $13, 0x10($a3)
    swc2       $14, 0x18($a3)
    nop
    rtps
    lb         $t5, 0x3($v1)
    lui        $t6, %hi(D_801DBFA4)
    lw         $t6, %lo(D_801DBFA4)($t6)
    andi       $t5, $t5, 0x2
    sll        $t6, $t6, 1
    or         $t5, $t6, $t5
    sll        $t5, $t5, 24
    lui        $t6, (0x2D808080 >> 16)
    ori        $t6, $t6, (0x2D808080 & 0xFFFF)
    or         $t6, $t5, $t6
    sw         $t6, 0x4($a3)
    lw         $t5, 0x4($v1)
    lw         $t6, 0x8($v1)
    cfc2       $v0, $31
    nop
    bltz       $v0, .L800616B4
     nop
    avsz4
    lw         $v0, 0xC($v1)
    sw         $t5, 0xC($a3)
    sw         $t6, 0x14($a3)
    sw         $v0, 0x1C($a3)
    lw         $t5, 0x10($v1)
    mfc2       $t6, $7
    sw         $t5, 0x24($a3)
    swc2       $14, 0x20($a3)
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0x9000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a3)
    xor        $v0, $a3, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    addiu      $a3, $a3, 0x28
  .L800616B4:
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L80061570
     nop
  .L800616C0:
    addu       $v0, $a3, $zero
    jr         $ra
     nop
endlabel GsTMDfastTG4NL
    nop
    nop

glabel GsTMDfastTG4L
    lw         $t8, 0x10($sp)
    lw         $t9, 0x18($sp)
    lw         $t7, 0x14($sp)
    beqz       $t8, .L800618D4
     lw        $t0, 0x4($t9)
    lw         $t1, 0x8($t9)
  .L800616EC:
    lw         $t2, 0x14($a0)
    lw         $t3, 0x18($a0)
    lw         $t4, 0x1C($a0)
  .L800616F8:
    srl        $t5, $t2, 16
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lwc2       $0, 0x0($t5)
    lwc2       $1, 0x4($t5)
    srl        $t5, $t3, 16
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lwc2       $2, 0x0($t5)
    lwc2       $3, 0x4($t5)
    srl        $t5, $t4, 16
    sll        $t5, $t5, 3
    addu       $t5, $t5, $a1
    lwc2       $4, 0x0($t5)
    lwc2       $5, 0x4($t5)
    nop
    rtpt
    lui        $t6, %hi(D_801DBFA4)
    lw         $t6, %lo(D_801DBFA4)($t6)
    lb         $v0, 0x3($a0)
    sll        $t6, $t6, 1
    or         $v0, $v0, $t6
    sll        $v0, $v0, 24
    lui        $t6, (0x808080 >> 16)
    ori        $t6, $t6, (0x808080 & 0xFFFF)
    or         $v0, $v0, $t6
    mtc2       $v0, $6
    lw         $t5, 0x20($a0)
    sll        $t2, $t2, 16
    srl        $t2, $t2, 13
    addu       $t2, $t2, $a2
    sll        $t3, $t3, 16
    srl        $t3, $t3, 13
    addu       $t3, $t3, $a2
    sll        $t4, $t4, 16
    srl        $t4, $t4, 13
    addu       $t4, $t4, $a2
    srl        $v1, $t5, 16
    sll        $v1, $v1, 3
    addu       $v1, $v1, $a1
    sll        $t5, $t5, 16
    srl        $t5, $t5, 13
    addu       $t5, $t5, $a2
    cfc2       $v0, $31
    nop
    bltz       $v0, .L800618C4
     nop
    nclip
    lwc2       $0, 0x0($v1)
    lwc2       $1, 0x4($v1)
    mfc2       $v1, $24
    nop
    blez       $v1, .L800618C4
     nop
    swc2       $12, 0x8($a3)
    swc2       $13, 0x14($a3)
    swc2       $14, 0x20($a3)
    nop
    rtps
    lwc2       $2, 0x0($t3)
    lwc2       $3, 0x4($t3)
    lwc2       $4, 0x0($t4)
    lwc2       $5, 0x4($t4)
    cfc2       $v0, $31
    nop
    bltz       $v0, .L800618C4
     nop
    avsz4
    lwc2       $0, 0x0($t5)
    lwc2       $1, 0x4($t5)
    mfc2       $t6, $7
    nop
    nccs
    lw         $v0, 0x4($a0)
    lw         $t3, 0x8($a0)
    lw         $t4, 0xC($a0)
    lw         $t5, 0x10($a0)
    sw         $v0, 0xC($a3)
    sw         $t3, 0x18($a3)
    sw         $t4, 0x24($a3)
    sw         $t5, 0x30($a3)
    swc2       $22, 0x28($a3)
    lwc2       $0, 0x0($t2)
    lwc2       $1, 0x4($t2)
    ncct
    subu       $t6, $t6, $t1
    srlv       $t6, $t6, $t7
    andi       $t6, $t6, 0xFFFF
    sll        $t6, $t6, 2
    addu       $t6, $t6, $t0
    lw         $t5, 0x0($t6)
    lui        $v0, (0xC000000 >> 16)
    sll        $v1, $t5, 8
    srl        $v1, $v1, 8
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a3)
    xor        $v0, $a3, $t5
    sll        $v1, $v0, 8
    srl        $v0, $v1, 8
    xor        $v1, $v0, $t5
    sw         $v1, 0x0($t6)
    addiu      $a0, $a0, 0x24
    lw         $t2, 0x14($a0)
    lw         $t3, 0x18($a0)
    lw         $t4, 0x1C($a0)
    swc2       $14, 0x2C($a3)
    swc2       $20, 0x4($a3)
    swc2       $21, 0x10($a3)
    swc2       $22, 0x1C($a3)
    addiu      $a3, $a3, 0x34
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L800616F8
     nop
    b          .L800618D4
     nop
  .L800618C4:
    addiu      $a0, $a0, 0x24
    addiu      $t8, $t8, -0x1
    bnez       $t8, .L800616EC
     nop
  .L800618D4:
    addu       $v0, $a3, $zero
    jr         $ra
     nop
endlabel GsTMDfastTG4L
    nop

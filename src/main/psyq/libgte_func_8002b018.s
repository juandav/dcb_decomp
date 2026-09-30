/*
 * libgte perspective transforms: SetGeomScreen (SetGeomScreen),
 * RotTransPers, RotTransPers3, RotTrans, RotTransPers4, RotAverage4,
 * RotNclip3, RotNclip4, RotAverageNclip3 and RotAverageNclip4.
 *
 * Hand-written assembly, not compiler output:
 * - Every GTE command (rtps, rtpt, mvmva, nclip, avsz3, avsz4) has one nop
 *   or none in front. PsyQ's C GTE macros (inline_c.h and inline_o.h,
 *   DMPSX 3) always emit two nops before each command; only inline_a.h,
 *   the macros for assembler programs, has the bare command.
 * - The stack arguments are loaded into $t0-$t3 inside the GTE's latency
 *   window while $v0/$v1 are free, and the RotNclip/RotAverageNclip
 *   variants skip their stores with `bgtz $v0,1f; nop; b end; nop`.
 * - SetGeomScreen is a lone ctc2 followed by `jr $ra; nop`: the PsyQ build
 *   (GCC 2.7.2 + ASPSX in reorder mode) puts that ctc2 in the delay slot.
 */

.include "macro.inc"

.set noat
.set noreorder

.section .text

glabel SetGeomScreen
    ctc2       $a0, $26
    jr         $ra
     nop
endlabel SetGeomScreen
    nop

glabel RotTransPers
    lwc2       $0, 0x0($a0)
    lwc2       $1, 0x4($a0)
    nop
    rtps
    swc2       $14, 0x0($a1)
    swc2       $8, 0x0($a2)
    cfc2       $v1, $31
    mfc2       $v0, $19
    sw         $v1, 0x0($a3)
    jr         $ra
     sra       $v0, $v0, 2
endlabel RotTransPers
    nop

glabel RotTransPers3
    lwc2       $0, 0x0($a0)
    lwc2       $1, 0x4($a0)
    lwc2       $2, 0x0($a1)
    lwc2       $3, 0x4($a1)
    lwc2       $4, 0x0($a2)
    lwc2       $5, 0x4($a2)
    nop
    rtpt
    lw         $t0, 0x10($sp)
    lw         $t1, 0x14($sp)
    lw         $t2, 0x18($sp)
    lw         $t3, 0x1C($sp)
    swc2       $12, 0x0($a3)
    swc2       $13, 0x0($t0)
    swc2       $14, 0x0($t1)
    swc2       $8, 0x0($t2)
    cfc2       $v1, $31
    mfc2       $v0, $19
    sw         $v1, 0x0($t3)
    jr         $ra
     sra       $v0, $v0, 2
endlabel RotTransPers3
    nop
    nop
    nop

glabel RotTrans
    lwc2       $0, 0x0($a0)
    lwc2       $1, 0x4($a0)
    nop
    mvmva      1, 0, 0, 0, 0
    swc2       $25, 0x0($a1)
    swc2       $26, 0x4($a1)
    swc2       $27, 0x8($a1)
    cfc2       $v0, $31
    jr         $ra
     sw        $v0, 0x0($a2)
endlabel RotTrans
    nop
    nop

glabel RotTransPers4
    lwc2       $0, 0x0($a0)
    lwc2       $1, 0x4($a0)
    lwc2       $2, 0x0($a1)
    lwc2       $3, 0x4($a1)
    lwc2       $4, 0x0($a2)
    lwc2       $5, 0x4($a2)
    nop
    rtpt
    lw         $t0, 0x10($sp)
    lw         $t1, 0x14($sp)
    lw         $t2, 0x18($sp)
    swc2       $12, 0x0($t0)
    swc2       $13, 0x0($t1)
    swc2       $14, 0x0($t2)
    cfc2       $v1, $31
    lwc2       $0, 0x0($a3)
    lwc2       $1, 0x4($a3)
    nop
    rtps
    lw         $t0, 0x1C($sp)
    lw         $t1, 0x20($sp)
    lw         $t2, 0x24($sp)
    swc2       $14, 0x0($t0)
    swc2       $8, 0x0($t1)
    cfc2       $t0, $31
    mfc2       $v0, $19
    or         $t0, $t0, $v1
    sw         $t0, 0x0($t2)
    jr         $ra
     sra       $v0, $v0, 2
endlabel RotTransPers4
    nop
    nop

glabel RotAverage4
    lwc2       $0, 0x0($a0)
    lwc2       $1, 0x4($a0)
    lwc2       $2, 0x0($a1)
    lwc2       $3, 0x4($a1)
    lwc2       $4, 0x0($a2)
    lwc2       $5, 0x4($a2)
    nop
    rtpt
    lw         $t0, 0x10($sp)
    lw         $t1, 0x14($sp)
    lw         $t2, 0x18($sp)
    swc2       $12, 0x0($t0)
    swc2       $13, 0x0($t1)
    swc2       $14, 0x0($t2)
    cfc2       $v1, $31
    lwc2       $0, 0x0($a3)
    lwc2       $1, 0x4($a3)
    nop
    rtps
    lw         $t0, 0x1C($sp)
    lw         $t1, 0x20($sp)
    lw         $t2, 0x24($sp)
    swc2       $14, 0x0($t0)
    cfc2       $t0, $31
    swc2       $8, 0x0($t1)
    or         $t0, $t0, $v1
    sw         $t0, 0x0($t2)
    avsz4
    mfc2       $v0, $7
    jr         $ra
     nop
endlabel RotAverage4
    nop

glabel RotNclip3
    lwc2       $0, 0x0($a0)
    lwc2       $1, 0x4($a0)
    lwc2       $2, 0x0($a1)
    lwc2       $3, 0x4($a1)
    lwc2       $4, 0x0($a2)
    lwc2       $5, 0x4($a2)
    nop
    rtpt
    lw         $t0, 0x20($sp)
    cfc2       $v0, $31
    nop
    nclip
    sw         $v0, 0x0($t0)
    nop
    lw         $t0, 0x10($sp)
    lw         $t1, 0x14($sp)
    lw         $t2, 0x18($sp)
    mfc2       $v0, $24
    nop
    bgtz       $v0, .L8005C6D0
     nop
    b          .L8005C6F0
     nop
  .L8005C6D0:
    swc2       $12, 0x0($a3)
    swc2       $13, 0x0($t0)
    swc2       $14, 0x0($t1)
    swc2       $8, 0x0($t2)
    mfc2       $t0, $19
    lw         $t1, 0x1C($sp)
    sra        $t0, $t0, 2
    sw         $t0, 0x0($t1)
  .L8005C6F0:
    jr         $ra
     nop
endlabel RotNclip3
    nop
    nop
    nop

glabel RotNclip4
    lwc2       $0, 0x0($a0)
    lwc2       $1, 0x4($a0)
    lwc2       $2, 0x0($a1)
    lwc2       $3, 0x4($a1)
    lwc2       $4, 0x0($a2)
    lwc2       $5, 0x4($a2)
    nop
    rtpt
    lw         $t0, 0x28($sp)
    cfc2       $v1, $31
    nop
    nclip
    sw         $v1, 0x0($t0)
    nop
    lw         $t0, 0x10($sp)
    lw         $t1, 0x14($sp)
    lw         $t2, 0x18($sp)
    mfc2       $v0, $24
    nop
    bgtz       $v0, .L8005C760
     nop
    b          .L8005C7AC
     nop
  .L8005C760:
    swc2       $12, 0x0($t0)
    swc2       $13, 0x0($t1)
    swc2       $14, 0x0($t2)
    lwc2       $0, 0x0($a3)
    lwc2       $1, 0x4($a3)
    nop
    rtps
    lw         $t0, 0x1C($sp)
    lw         $t1, 0x20($sp)
    lw         $t2, 0x24($sp)
    lw         $t3, 0x28($sp)
    swc2       $14, 0x0($t0)
    mfc2       $t4, $19
    swc2       $8, 0x0($t1)
    cfc2       $v0, $31
    sra        $t4, $t4, 2
    sw         $t4, 0x0($t2)
    or         $v0, $v0, $v1
    sw         $v0, 0x0($t3)
  .L8005C7AC:
    jr         $ra
     nop
endlabel RotNclip4

glabel RotAverageNclip3
    lwc2       $0, 0x0($a0)
    lwc2       $1, 0x4($a0)
    lwc2       $2, 0x0($a1)
    lwc2       $3, 0x4($a1)
    lwc2       $4, 0x0($a2)
    lwc2       $5, 0x4($a2)
    nop
    rtpt
    lw         $t0, 0x20($sp)
    cfc2       $t1, $31
    nop
    sw         $t1, 0x0($t0)
    nclip
    lw         $t0, 0x10($sp)
    lw         $t1, 0x14($sp)
    lw         $t2, 0x18($sp)
    mfc2       $v0, $24
    nop
    bgtz       $v0, .L8005C80C
     nop
    b          .L8005C834
     nop
  .L8005C80C:
    swc2       $12, 0x0($a3)
    swc2       $13, 0x0($t0)
    swc2       $14, 0x0($t1)
    swc2       $8, 0x0($t2)
    nop
    avsz3
    lw         $t1, 0x1C($sp)
    mfc2       $t0, $7
    nop
    sw         $t0, 0x0($t1)
  .L8005C834:
    jr         $ra
     nop
endlabel RotAverageNclip3
    nop
    nop

glabel RotAverageNclip4
    lwc2       $0, 0x0($a0)
    lwc2       $1, 0x4($a0)
    lwc2       $2, 0x0($a1)
    lwc2       $3, 0x4($a1)
    lwc2       $4, 0x0($a2)
    lwc2       $5, 0x4($a2)
    nop
    rtpt
    lw         $t0, 0x28($sp)
    cfc2       $v1, $31
    nop
    sw         $v1, 0x0($t0)
    nclip
    lw         $t0, 0x10($sp)
    lw         $t1, 0x14($sp)
    lw         $t2, 0x18($sp)
    mfc2       $v0, $24
    nop
    bgtz       $v0, .L8005C89C
     nop
    b          .L8005C8EC
     nop
  .L8005C89C:
    swc2       $12, 0x0($t0)
    swc2       $13, 0x0($t1)
    swc2       $14, 0x0($t2)
    lwc2       $0, 0x0($a3)
    lwc2       $1, 0x4($a3)
    nop
    rtps
    lw         $t0, 0x1C($sp)
    lw         $t1, 0x20($sp)
    lw         $t2, 0x28($sp)
    swc2       $14, 0x0($t0)
    cfc2       $t3, $31
    swc2       $8, 0x0($t1)
    or         $t3, $t3, $v1
    sw         $t3, 0x0($t2)
    avsz4
    lw         $t1, 0x24($sp)
    mfc2       $t0, $7
    nop
    sw         $t0, 0x0($t1)
  .L8005C8EC:
    jr         $ra
     nop
endlabel RotAverageNclip4

/*
 * libgte REG12: SetGeomOffset (SetGeomOffset: the screen offset, shifted
 * left 16, into GTE control registers OFX and OFY).
 *
 * Hand-written assembly, not compiler output: the body is two ctc2 moves
 * with nothing C can express, and it ends in `jr $ra; nop` after a ctc2
 * that could have filled the slot. The PsyQ build (GCC 2.7.2 + ASPSX in
 * reorder mode) always moves such an instruction into the delay slot of
 * `jr $ra`; the unfilled slot is `.set noreorder` code.
 */

.include "macro.inc"

.set noat
.set noreorder

.section .text

glabel SetGeomOffset
    sll        $a0, $a0, 16
    sll        $a1, $a1, 16
    ctc2       $a0, $24
    ctc2       $a1, $25
    jr         $ra
     nop
endlabel SetGeomOffset
    nop
    nop

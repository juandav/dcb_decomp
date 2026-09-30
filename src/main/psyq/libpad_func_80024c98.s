/*
 * BIOS call stubs: EnterCriticalSection and ExitCriticalSection
 * (func_8006A804/func_8006A814, `li $a0,1|2; syscall 0; jr $ra`), the B0
 * table's open, lseek, read, write, close, nextfile and
 * ChangeClearPad (func_8006A824 .. func_8006A884) and the C0 table's
 * ChangeClearRCnt (func_8006A894).
 *
 * Hand-written assembly, not compiler output: each stub is
 * `li $t2,0xB0 (or 0xC0); jr $t2; li $t1,N` with no frame, a jump to a
 * constant address with the call number in $t1, which no C compiles to;
 * the other two are a bare `syscall`.
 */

.include "macro.inc"

.set noat
.set noreorder

.section .text

glabel func_8006A804
    addiu      $a0, $zero, 0x1
    syscall    0
    jr         $ra
     nop
endlabel func_8006A804

glabel func_8006A814
    addiu      $a0, $zero, 0x2
    syscall    0
    jr         $ra
     nop
endlabel func_8006A814

glabel func_8006A824
    addiu      $t2, $zero, 0xB0
    jr         $t2
     addiu     $t1, $zero, 0x32
endlabel func_8006A824
    nop

glabel func_8006A834
    addiu      $t2, $zero, 0xB0
    jr         $t2
     addiu     $t1, $zero, 0x33
endlabel func_8006A834
    nop

glabel func_8006A844
    addiu      $t2, $zero, 0xB0
    jr         $t2
     addiu     $t1, $zero, 0x34
endlabel func_8006A844
    nop

glabel func_8006A854
    addiu      $t2, $zero, 0xB0
    jr         $t2
     addiu     $t1, $zero, 0x35
endlabel func_8006A854
    nop

glabel func_8006A864
    addiu      $t2, $zero, 0xB0
    jr         $t2
     addiu     $t1, $zero, 0x36
endlabel func_8006A864
    nop

glabel func_8006A874
    addiu      $t2, $zero, 0xB0
    jr         $t2
     addiu     $t1, $zero, 0x43
endlabel func_8006A874
    nop

glabel func_8006A884
    addiu      $t2, $zero, 0xB0
    jr         $t2
     addiu     $t1, $zero, 0x5B
endlabel func_8006A884
    nop

glabel func_8006A894
    addiu      $t2, $zero, 0xC0
    jr         $t2
     addiu     $t1, $zero, 0xA
endlabel func_8006A894
    nop

/*
 * BIOS call stubs: EnterCriticalSection and ExitCriticalSection
 * (`li $a0,1|2; syscall 0; jr $ra`), the B0 table's open, lseek, read,
 * write, close, nextfile and ChangeClearPad and the C0 table's
 * ChangeClearRCnt.
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

glabel EnterCriticalSection
    addiu      $a0, $zero, 0x1
    syscall    0
    jr         $ra
     nop
endlabel EnterCriticalSection

glabel ExitCriticalSection
    addiu      $a0, $zero, 0x2
    syscall    0
    jr         $ra
     nop
endlabel ExitCriticalSection

glabel open
    addiu      $t2, $zero, 0xB0
    jr         $t2
     addiu     $t1, $zero, 0x32
endlabel open
    nop

glabel lseek
    addiu      $t2, $zero, 0xB0
    jr         $t2
     addiu     $t1, $zero, 0x33
endlabel lseek
    nop

glabel read
    addiu      $t2, $zero, 0xB0
    jr         $t2
     addiu     $t1, $zero, 0x34
endlabel read
    nop

glabel write
    addiu      $t2, $zero, 0xB0
    jr         $t2
     addiu     $t1, $zero, 0x35
endlabel write
    nop

glabel close
    addiu      $t2, $zero, 0xB0
    jr         $t2
     addiu     $t1, $zero, 0x36
endlabel close
    nop

glabel nextfile
    addiu      $t2, $zero, 0xB0
    jr         $t2
     addiu     $t1, $zero, 0x43
endlabel nextfile
    nop

glabel ChangeClearPad
    addiu      $t2, $zero, 0xB0
    jr         $t2
     addiu     $t1, $zero, 0x5B
endlabel ChangeClearPad
    nop

glabel ChangeClearRCnt
    addiu      $t2, $zero, 0xC0
    jr         $t2
     addiu     $t1, $zero, 0xA
endlabel ChangeClearRCnt
    nop

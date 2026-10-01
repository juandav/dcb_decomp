#ifndef INCLUDE_ASM_H
#define INCLUDE_ASM_H

#if !defined(M2CTX) && !defined(PERMUTER) && !defined(SKIP_ASM)

/*
 * FOLDER is relative to the version's splat output, ASM_DIR (asm/<version>),
 * which the Makefile defines: INCLUDE_ASM("main/nonmatchings/psyq", NAME)
 * includes asm/us/main/nonmatchings/psyq/NAME.s when building VERSION=us.
 */
#ifndef ASM_DIR
#error "ASM_DIR is not defined: the Makefile gives it, as asm/<version>"
#endif

#ifndef INCLUDE_ASM
#define INCLUDE_ASM(FOLDER, NAME) \
    __asm__( \
        ".section .text\n" \
        "    .set noat\n" \
        "    .set noreorder\n" \
        "    .include \"" ASM_DIR "/" FOLDER "/" #NAME ".s\"\n" \
        "    .set reorder\n" \
        "    .set at\n" \
    )
#endif
#ifndef INCLUDE_RODATA
#define INCLUDE_RODATA(FOLDER, NAME) \
    __asm__( \
        ".section .rodata\n" \
        "    .include \"" ASM_DIR "/" FOLDER "/" #NAME ".s\"\n" \
        ".section .text" \
    )
#endif

#if defined(INCLUDE_ASM_USE_MACRO_INC) && INCLUDE_ASM_USE_MACRO_INC
__asm__(".include \"include/macro.inc\"\n");
#else
__asm__(".include \"include/labels.inc\"\n");
#endif

#else

#ifndef INCLUDE_ASM
#define INCLUDE_ASM(FOLDER, NAME)
#endif
#ifndef INCLUDE_RODATA
#define INCLUDE_RODATA(FOLDER, NAME)
#endif

#endif /* !defined(M2CTX) && !defined(PERMUTER) */

/*
 * ASPSX pads the .text of every PsyQ library object to a multiple of 16
 * bytes. Put this after the last function of a library object that is
 * written in C, with the number of padding nops that follow it. Not every
 * object before it is a multiple of 16, so .align can't be used.
 */
#if !defined(M2CTX) && !defined(PERMUTER) && !defined(SKIP_ASM)
#define OBJECT_END(NOPS) __asm__(".section .text\n\t.fill " #NOPS ", 4, 0\n")
#else
#define OBJECT_END(NOPS)
#endif

#endif /* INCLUDE_ASM_H */

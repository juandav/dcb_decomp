#ifndef STDARG_H
#define STDARG_H

/* va-mips.h from GCC 2.x: arguments are read from the caller's stack */
typedef char *va_list;

#define __va_rounded_size(TYPE) (((sizeof(TYPE) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(AP, LASTARG) (AP = ((va_list)__builtin_next_arg(LASTARG)))
#define va_arg(AP, TYPE) \
    (AP = (char *)(((int)AP + __va_rounded_size(TYPE) + (__alignof__(TYPE) - 1)) & -__alignof__(TYPE)), \
     *((TYPE *)(AP - __va_rounded_size(TYPE))))
#define va_end(AP) ((void)0)

#endif /* STDARG_H */

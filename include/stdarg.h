#ifndef STDARG_H
#define STDARG_H

/* PsyQ's stdarg.h: the named argument's address is taken, so it gets a home
   slot, and va_arg steps the pointer before reading behind it */
typedef char *va_list;

#define __va_rounded_size(TYPE) (((sizeof(TYPE) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(AP, LASTARG) (AP = ((char *)&(LASTARG) + __va_rounded_size(LASTARG)))
#define va_arg(AP, TYPE) (AP += __va_rounded_size(TYPE), *((TYPE *)(AP - __va_rounded_size(TYPE))))
#define va_end(AP) ((void)0)

#endif /* STDARG_H */

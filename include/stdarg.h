/*$@ stdarg.h : copyright (c) 2018 Hsiang Chen : under GPL $*/
#ifndef _STDARG_H_
#define _STDARG_H_

#ifndef va_list
  typedef char* va_list;
#endif

#ifndef __va_rounded_size
  #define __va_rounded_size(TYPE)  \
      (((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))
#endif

#ifndef va_start
  #ifndef __sparc__
    #define va_start(AP, LASTARG)                       \
      (AP = ((char *) &(LASTARG) + __va_rounded_size (LASTARG)))
  #else
    #define va_start(AP, LASTARG)                       \
      (__builtin_saveregs (),                         \
       AP = ((char *) &(LASTARG) + __va_rounded_size (LASTARG)))
  #endif
#endif

#ifndef va_arg
  #define va_arg(AP, TYPE)                            \
      (AP += __va_rounded_size (TYPE),                \
       *((TYPE *) (AP - __va_rounded_size (TYPE))))
#endif

#ifndef va_end
  #define va_end(AP)
#endif

#endif


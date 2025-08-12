/*

    A Simple stdarg.h

 */

#ifndef STDARG_H
#define STDARG_H

#include <std/stddef.h>

typedef char* va_list;

#define va_start(ap, last_fixed_arg) (ap = (char*)&last_fixed_arg + sizeof(last_fixed_arg))
#define va_arg(ap, type) (*(type*)((ap += sizeof(type)) - sizeof(type)))
#define va_end(ap) (ap = NULL)

#endif

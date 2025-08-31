/*

    My stdarg implimentation. It works decently well.

*/
#ifndef STDARG_H
#define STDARG_H

typedef unsigned char* va_list;

#define _VA_ALIGN(type) (((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#define va_start(ap, last) (ap = (char*)&last + _VA_ALIGN(last))

#define va_arg(ap, type) (*(type*)((ap += _VA_ALIGN(type)) - _VA_ALIGN(type)))

#define va_end(ap) (ap = (va_list)0)


#endif

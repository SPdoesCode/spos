/*

    The standard defenitions.

*/

#ifndef STDDEF_H
#define STDDEF_H

// Unsigned ints
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

// Signed ints
typedef char  int8_t;
typedef short int16_t;
typedef int   int32_t;
typedef long long int64_t;

// Boolean
typedef uint8_t bool;

// Size
typedef unsigned int size_t;

// String (TODO: Get a allocator so this can be used.)
//typedef char* string;

// Misc.
typedef uint8_t byte;
typedef uint16_t lbyte;
typedef uint32_t llbyte;

#endif

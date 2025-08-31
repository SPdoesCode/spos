/*

    String utilitys and manipulation.

*/
#ifndef STRINGS_H
#define STRINGS_H

#include <std/stddef.h>

// Basic string utilitys
size_t strlen(char *str);
char* itoa(uint32_t value);
char* strcat(char* dest, char* src);

#endif

#include <string.h>
#include <stdint.h>

// Get the length of a string
size_t strlen(char *str) {
    size_t size = 0;
    while (str[size]) {
        size++;
    }
    return size;
}

char* strcat(char* dest,char* src) {
    char *d = dest;
    while (*d) {
        d++;
    }
    while (*src) {
        *d++ = *src++;
    }
    *d = '\0';
    return dest;
}
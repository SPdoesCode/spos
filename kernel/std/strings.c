#include <std/strings.h>
#include <std/stddef.h>

// Get the length of a string
size_t strlen(char *str) {
    size_t size = 0;
    while (str[size]) {
        size++;
    }
    return size;
}

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

// Convert a int to a string.
char* itoa(uint32_t value) {
    static char str[16];   // persists between calls
    char temp[16];
    int i = 0;

    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    while (value > 0 && i < 15) {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    }

    for (int j = 0; j < i; j++) {
        str[j] = temp[i - j - 1];
    }

    str[i] = '\0';
    return str;
}

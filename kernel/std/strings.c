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
char itoa_buffer[16];
// Convert a int to a string.
char* itoa(uint32_t value) { // Thank you micl for helping me fix this :)
    int i = 0;
    int j = 0;

    if (value == 0) {
        itoa_buffer[0] = '0';
        itoa_buffer[1] = '\0';
        return itoa_buffer;
    }

    while (value > 0 && i < 15) {
        itoa_buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    for (j = 0; j < i / 2; j++) {
        char temp = itoa_buffer[j];
        itoa_buffer[j] = itoa_buffer[i - j - 1];
        itoa_buffer[i - j - 1] = temp;
    }

    itoa_buffer[i] = '\0';
    return itoa_buffer;
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

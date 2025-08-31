#include <drivers/serialport.h>
#include <sys/portio.h>
#include <std/stddef.h>
#include <std/stdarg.h>
#include <std/strings.h>

void serialportsetup(void) {
    outb(SERIAL_COM1_PORT + 1, 0x00);
    outb(SERIAL_COM1_PORT + 3, SERIAL_ENABLE_DLAB);
    outb(SERIAL_COM1_PORT, 0x03);
    outb(SERIAL_COM1_PORT + 1, 0x00);
    outb(SERIAL_COM1_PORT + 3, 0x03);
    outb(SERIAL_COM1_PORT + 2, 0xC7);
    outb(SERIAL_COM1_PORT + 4, 0x0B);
}

uint8_t transmissionempty(void) {
    return inb(SERIAL_COM1_PORT + 5 & 0x20);
}

void serialwritechar(char c) {
    while (transmissionempty() == 0) {}
    outb(SERIAL_COM1_PORT, c);
}

void serialwriteline(char *msg) {
    size_t x = 0;
    while (msg[x]) {
        serialwritechar(msg[x]);
        x++;
    }
}

void serialprintln(char *msg, ...) {
    va_list ap;
    va_start(ap, msg);

    while (*msg) {
        if (*msg == '%') {
            switch (*(msg + 1)) {
                case 'd':
                    ;
                    char* num = itoa(va_arg(ap, int));
                    for (int i = 0; num[i] != '\0'; i++) {
                        serialwritechar(num[i]);
                    }
                    msg+=2;
                    break;
                case 's':
                    ;
                    char* tmp = va_arg(ap, char*);
                    for (int i = 0; tmp[i] != '\0'; i++) {
                        serialwritechar(tmp[i]);
                    }
                    msg+=2;
                    break;
                default:
                    msg++;
                    break;
            }
        } else {
            serialwritechar(*(msg));
            msg++;
        }
    }
    serialwriteline("\r\n");
    va_end(ap);
}

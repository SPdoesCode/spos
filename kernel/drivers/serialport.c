#include <drivers/serialport.h>
#include <sys/portio.h>
#include <std/stddef.h>

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

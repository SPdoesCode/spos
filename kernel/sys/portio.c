#include <sys/portio.h>
#include <std/stddef.h>

// Get a byte from a port.
uint8_t inb(port16_t p) {
    uint8_t retval;
    __asm__ volatile ("inb %1, %0": "=a"(retval): "Nd"(p));
    return retval;
}

// Get a longer byte from a port.
uint16_t inw(port16_t p) {
    uint16_t retval;
    __asm__ volatile ("inw %1, %0": "=a"(retval): "Nd"(p));
    return retval;
}

// Get a longer long byte from a port.
uint32_t inl(port16_t p) {
    uint32_t retval;
    __asm__ volatile ("inl %1, %0": "=a"(retval): "Nd"(p));
    return retval;
}

// Output a byte to a port.
void outb(port16_t p, uint8_t v) {
    __asm__ volatile ("outb %0, %1":: "a"(v), "Nd"(p));
}

// Output a longer byte to a port.
void outw(port16_t p, uint16_t v) {
     __asm__ volatile ("outw %0, %1":: "a"(v), "Nd"(p));
}

// Output a longer long byte to a port.
void outl(port16_t p, uint32_t v) {
     __asm__ volatile ("outl %0, %1":: "a"(v), "Nd"(p));
}

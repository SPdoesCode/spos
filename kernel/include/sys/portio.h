/*

    A simple input and output port utility.

*/

#ifndef PORTIO_H
#define PORTIO_H

#include <std/stddef.h>

typedef uint16_t port16_t;

// The input functions.
uint8_t inb(port16_t p);
uint16_t inw(port16_t p);
uint32_t inl(port16_t p);

// The output functions.
void outb(port16_t p, uint8_t v);
void outw(port16_t p, uint16_t v);
void outl(port16_t p, uint32_t v);

#endif

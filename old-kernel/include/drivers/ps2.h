/*

    A minimal ps2 driver.

*/

#ifndef PS2_H
#define PS2_H

#include <std/stddef.h>

// Useful for removing garbage from a input.
#define NO_VALUE_INPUT 255

// If it isnt defined already we set it.
// Allows a different port to be used if it is desired.
#ifndef DEFAULT_PS2_PORT
#   define DEFAULT_PS2_PORT 0x60
#endif

// If it isnt definded we can set it.
// Some may need a different PS2 status port.
#ifndef DEFAULT_PS2_STATUS_PORT
#   define DEFAULT_PS2_STATUS_PORT 0x64
#endif

// If it isnt definded we can set it.
// Some may need a different PS2 buffer port.
#ifndef DEFAULT_PS2_BUFFER_PORT
#   define DEFAULT_PS2_BUFFER_PORT 0x01
#endif

// Basic stuff for getting keys and decoding.
uint8_t decodeinput(uint8_t inputcode);
char getkey();

#endif

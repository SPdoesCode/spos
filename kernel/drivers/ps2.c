#include <sys/portio.h>
#include <drivers/ps2.h>
#include <drivers/keymap.h>

// Decode a input normally from DEFAULT_PS2_PORT.
uint8_t decodeinput(uint8_t inputcode) {
    uint8_t retval = NO_VALUE_INPUT;
    if ((inputcode & 0x80) == 0 && inputcode < 128) {
        retval = keymap_lower[inputcode];
    }
    return retval;
}

// Get a input of a char from DEFAULT_PS2_PORT using DEFAULT_PS2_(STATUS and BUFFER)_PORT
char getkey() {
    while ((inb(DEFAULT_PS2_STATUS_PORT) & DEFAULT_PS2_BUFFER_PORT) == 0) {}
    return (char)decodeinput(inb(DEFAULT_PS2_PORT));
}

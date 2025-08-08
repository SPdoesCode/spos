/*

    A Simple VGA driver.

*/

#ifndef VGA_TEXTMODE_H
#define VGA_TEXTMODE_H

#include <std/stddef.h>

#define MAX_VGA_X 80
#define MAX_VGA_Y 25
#define DEF_VGA_CLEAR_COLOR 0x00

typedef uint8_t vga_color;

void writechar(char c, uint16_t x, uint16_t y, vga_color color);
void writeline(char *c, uint16_t x, uint16_t y, vga_color color);
void clearscreen(vga_color color);

#endif

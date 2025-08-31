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
typedef struct vgachar {
    char c;
    vga_color color;
} vgachar_t;

extern vgachar_t vgatxtbuff[MAX_VGA_X * MAX_VGA_Y];
extern uint8_t current_txt_y, current_txt_x;

void txtwritechar(char c, uint16_t x, uint16_t y, vga_color color);
void txtwriteline(char *c, uint16_t x, uint16_t y, vga_color color);
void txtclearscreen(vga_color color);
void txtprintln(vga_color color, char* msg, ...);

#endif

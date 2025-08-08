#include <drivers/vga/textmode.h>
#include <std/stddef.h>

// The VGA Textmode buffer
uint16_t* vga_buffer = (uint16_t*)0xB8000;

// Write a char to a possition on the vga buffer with a color.
void writechar(char c, uint16_t x, uint16_t y, vga_color color) {
    if (x >= MAX_VGA_X || y >= MAX_VGA_Y) return;
    const uint16_t location = y * MAX_VGA_X + x;

    vga_buffer[location] = ((uint16_t)color << 8) | (uint16_t)c;

}

// Write a string to the vga buffer. It will continue on the next y if the x goes past VGA_MAX_X.
void writeline(char *c, uint16_t x, uint16_t y, vga_color color) {
    uint8_t cnum = 0;
    uint8_t cy = 0 + y;
    uint8_t cx = 0 + x;
    while (c[cnum]) {
        if (cx >= MAX_VGA_X) {
            cy++;
            cx = 0;
        }
        if (cy >= MAX_VGA_Y) {
            clearscreen(DEF_VGA_CLEAR_COLOR);
            cx = 0;
            cy = 0;
        }
        writechar(c[cnum], cx, cy, color);
        cnum++;
        cx++;
    }

}

// Clear the screen to a color.
void clearscreen(vga_color color) {
    uint16_t x = 0;
    uint16_t y = 0;
    while (y < MAX_VGA_Y) {
        x = 0;
        while (x < MAX_VGA_X) {
            writechar(' ', x, y, color);
            x++;
        }
        y++;
    }
}

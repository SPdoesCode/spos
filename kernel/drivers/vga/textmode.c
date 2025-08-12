#include <drivers/vga/textmode.h>
#include <std/stddef.h>
#include <std/stdarg.h>
#include <std/strings.h>

// The VGA Textmode buffer
uint16_t* vga_buffer = (uint16_t*)0xB8000;
vgachar_t vgatxtbuff[MAX_VGA_X * MAX_VGA_Y];
uint8_t current_txt_x = 0, current_txt_y = 0;

// Write a char to a possition on the vga buffer with a color.
void txtwritechar(char c, uint16_t x, uint16_t y, vga_color color) {
    if (x >= MAX_VGA_X || y >= MAX_VGA_Y) return;
    const uint16_t location = y * MAX_VGA_X + x;

    vga_buffer[location] = ((uint16_t)color << 8) | (uint16_t)c;
    vgatxtbuff[location].c = c;
    vgatxtbuff[location].color = color;

}

// Write a string to the vga buffer. It will continue on the next y if the x goes past VGA_MAX_X.
void txtwriteline(char *c, uint16_t x, uint16_t y, vga_color color) {
    uint8_t cnum = 0;
    uint8_t cy = 0 + y;
    uint8_t cx = 0 + x;
    while (c[cnum]) {
        if (cx >= MAX_VGA_X) {
            cy++;
            cx = 0;
        }
        if (cy >= MAX_VGA_Y) {
            txtclearscreen(DEF_VGA_CLEAR_COLOR);
            cx = 0;
            cy = 0;
        }
        txtwritechar(c[cnum], cx, cy, color);
        cnum++;
        cx++;
    }

}

// Clear the screen to a color.
void txtclearscreen(vga_color color) {
    uint16_t x = 0;
    uint16_t y = 0;
    while (y < MAX_VGA_Y) {
        x = 0;
        while (x < MAX_VGA_X) {
            txtwritechar(' ', x, y, color);
            x++;
        }
        y++;
    }
}

static void txtinc(void) {
    int i;
    // clear first row
    for (i = 0; i < MAX_VGA_X; i++) {
        vgatxtbuff[i].c = 0;
        vgatxtbuff[i].color = 0x00;
        txtwritechar(vgatxtbuff[i].c, i, 0, 0x00);
    }
    i = MAX_VGA_X;
    for (i = MAX_VGA_X; i < MAX_VGA_X * MAX_VGA_Y; i ++) {
        vgatxtbuff[i - MAX_VGA_X] = vgatxtbuff[i];
        int row = (i - MAX_VGA_X) / MAX_VGA_X;
        int col = (i - MAX_VGA_X) % MAX_VGA_X;
        txtwritechar(vgatxtbuff[i - MAX_VGA_X].c, col, row, vgatxtbuff[i - MAX_VGA_X].color);
    }
    int start_last_row = MAX_VGA_X * (MAX_VGA_Y - 1);
    for (i = start_last_row; i < MAX_VGA_X * MAX_VGA_Y; i++) {
        vgatxtbuff[i].c = 0;
        vgatxtbuff[i].color = 0x00;
        int col = i % MAX_VGA_X;
        int row = i / MAX_VGA_X;
        txtwritechar(vgatxtbuff[i].c, col, row, 0x00);
    }
}

void txtprintln(vga_color color, char *msg, ...) {
    va_list ap;
    va_start(ap, msg);

    while (*msg) {
        if (current_txt_x >= MAX_VGA_X) {
            current_txt_x = 0;
            current_txt_y ++;
        }
        if (current_txt_y >= MAX_VGA_Y) {
            current_txt_y = MAX_VGA_Y - 1;
            txtinc();
        }
        if (*msg == '%') {
            switch (*(msg + 1)) {
                case 'd':
                    ;
                    char* num = itoa(va_arg(ap, int));
                    for (int i = 0; num[i] != '\0'; i++) {
                        if (current_txt_x >= MAX_VGA_X) {
                            current_txt_x = 0;
                            current_txt_y ++;
                        }
                        if (current_txt_y >= MAX_VGA_Y) {
                            current_txt_y = MAX_VGA_Y - 1;
                            txtinc();
                        }
                        txtwritechar(num[i], current_txt_x, current_txt_y, color);
                        current_txt_x ++;
                    }
                    msg+=2;
                    break;
                case 's':
                    ;
                    char* tmp = va_arg(ap, char*);
                    for (int i = 0; tmp[i] != '\0'; i++) {
                        if (current_txt_x >= MAX_VGA_X) {
                            current_txt_x = 0;
                            current_txt_y ++;
                        }
                        if (current_txt_y >= MAX_VGA_Y) {
                            current_txt_y = MAX_VGA_Y - 1;
                            txtinc();
                        }
                        txtwritechar(tmp[i], current_txt_x, current_txt_y, color);
                        current_txt_x ++;
                    }
                    msg+=2;
                    break;
                default:
                    msg++;
                    break;
            }
        } else {
            txtwritechar(*(msg), current_txt_x++, current_txt_y, color);
            msg++;
        }
    }
    current_txt_y ++;
    current_txt_x = 0;
    va_end(ap);
}

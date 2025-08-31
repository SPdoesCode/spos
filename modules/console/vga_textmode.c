#include <loader.h>
#include <kmodules.h>
#include <kstdio.h>

static int MAX_VGA_X = 80;
static int MAX_VGA_Y = 25;

static void init(void) {
    kprintf("Loaded vga textmode driver...");
}

static void exit(void) {
    kprintf("");
}

static void put_char(char c, int x, int y, uint8_t color) {
    static uint16_t* vga_buffer = (uint16_t*)0xB8000;

    if (x >= MAX_VGA_X || y >= MAX_VGA_Y) return;
    uint16_t location = y * MAX_VGA_X + x;

    vga_buffer[location] = ((uint16_t)color << 8) | (uint16_t)c;
}

static console_driver_t textmode = {
    .init     = init,
    .put_char = put_char,
    .quit     = exit,
};

kernel_driver_t vga_textmode = {
    .name = "VGA Textmode driver",
    .type = CONSOLE,
    .driver.console = &textmode,
};
#include <drivers/vga/textmode.h>
#include <drivers/serialport.h>

void halt(void) {
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

void kmain(void) {
    clearscreen(0x00);
    writeline("Hello World!", 0, 0, 0x0F);
    writeline("This is spos!", 0, 1, 0x0F);
    serialportsetup();
    serialwriteline("Hello world!");
    serialwriteline("This is spos, but from serial output!");
    halt();
}

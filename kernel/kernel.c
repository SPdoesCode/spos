#include <drivers/vga/textmode.h>
#include <drivers/serialport.h>
#include <sys/sysinfo.h>
#include <std/stddef.h>
#include <std/strings.h>

void halt(void) {
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

void kmain(void) {
    clearscreen(0x00);
    serialportsetup();
    writeline("found mem (kb):", 0, 0, 0x0F);
    serialwriteline("Found mem (kb): ");


    halt();
}

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
    fillramentrys();
    writeline("found mem (kb):", 0, 0, 0x0F);
    serialwriteline("Found mem (kb): ");
    writeline(itoa((total_memory_bytes / 1024)), 1, 1, 0x0F);
    serialwriteline(itoa((total_memory_bytes / 1024)));

    halt();
}

#include <sys/sysinfo.h>
#include <std/stddef.h>
#include <sys/portio.h>

ram_entry_t ramenterys[16];
size_t ram_entry_num = 0;

// Grabs the ammount of ram.
uint32_t getrambytes(void) {
    uint16_t total;
    uint8_t lowmem, highmem;

    outb(0x70, 0x30);
    lowmem = inb(0x71);
    outb(0x70, 0x31);
    highmem = inb(0x71);

    total = lowmem | highmem << 8;
    return (uint32_t) total * 1024;
}

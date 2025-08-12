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
    txtclearscreen(0x00);
    serialportsetup();
    fillramentrys();
    pci_scan();
    serialprintln("Found ram (kb): %d", (total_memory_bytes / 1024));
    serialprintln("Found device(s):");
    serialprintln("#. <device id> : <vendor id> : <class code> : <slot> : <subclass> : <prog if>");
    for (size_t i = 0; i < pci_entry_num; i++) {
        serialprintln("%d. %s : %s : %s : %s : %s : %s", i, itoa(pcientrys[i].device_id), itoa(pcientrys[i].vendor_id), pciclasstostring(pcientrys[i].class_code), itoa((uint16_t)pcientrys[i].slot), itoa((uint16_t)pcientrys[i].subclass), itoa((uint16_t)pcientrys[i].prog_if));
    }

    txtprintln(0x0F, "Found ram (kb): %d", (total_memory_bytes / 1024));
    txtprintln(0x0F, "Found device(s): ");
    txtprintln(0x0F, "#. <device id> : <vendor id> : <class code> : <slot> : <subclass> : <prog if>");
    for (size_t i = 0; i < pci_entry_num; i++) {
        txtprintln(0x0F, "%d. %s : %s : %s : %s : %s : %s", i, itoa(pcientrys[i].device_id), itoa(pcientrys[i].vendor_id), pciclasstostring(pcientrys[i].class_code), itoa((uint16_t)pcientrys[i].slot), itoa((uint16_t)pcientrys[i].subclass), itoa((uint16_t)pcientrys[i].prog_if));
    }

    halt();
}

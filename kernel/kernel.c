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
        serialprintln("%d. %d : %d : %s : %d : %d : %d",
            i,
            pcientrys[i].device_id,
            pcientrys[i].vendor_id,
            pciclasstostring(pcientrys[i].class_code),
            pcientrys[i].slot,
            pcientrys[i].subclass,
            pcientrys[i].prog_if
        );
    }

    txtprintln(0x0F, "Found ram (kb): %d", (total_memory_bytes / 1024));
    txtprintln(0x0F, "Found device(s): ");
    txtprintln(0x0F, "#. <device id> : <vendor id> : <class code> : <slot> : <subclass> : <prog if>");
    for (size_t i = 0; i < pci_entry_num; i++) {
        txtprintln(0x0F, "%d. %d : %d : %s : %d : %d : %d",
            i,
            pcientrys[i].device_id,
            pcientrys[i].vendor_id,
            pciclasstostring(pcientrys[i].class_code),
            pcientrys[i].slot,
            pcientrys[i].subclass,
            pcientrys[i].prog_if
        );
    }

    halt();
}

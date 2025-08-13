#include <drivers/vga/textmode.h>
#include <drivers/serialport.h>
#include <sys/sysinfo.h>
#include <std/stddef.h>
#include <std/strings.h>
#include <drivers/ide/ide.h>

#define LEGACY_PRIMARY_IO   0x1F0
#define LEGACY_PRIMARY_CTRL 0x3F6
#define LEGACY_SECOND_IO    0x170
#define LEGACY_SECOND_CTRL  0x376
#define LEGACY_BMIDE        0x0000

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

    ide_initialize(
        LEGACY_PRIMARY_IO,
        LEGACY_PRIMARY_CTRL,
        LEGACY_SECOND_IO,
        LEGACY_SECOND_CTRL,
        LEGACY_BMIDE
    );

    for (int i = 0; i < 4; i++) {
        if (ide_dev_buff[i].reserve == 0) continue;  // Skip non-existing devices

        serialprintln("Device %d:", i);
        serialprintln("  Channel: %d, Drive: %d, Type: %s",
                  ide_dev_buff[i].channel,
                  ide_dev_buff[i].drive,
                  ide_dev_buff[i].type == IDE_ATA ? "ATA" : "ATAPI");
        serialprintln("  Size: %d sectors", ide_dev_buff[i].size);
        serialprintln("  Model: %s", ide_dev_buff[i].model);

        txtprintln(0x0F, "Device %d:", i);
        txtprintln(0x0F,"  Channel: %d, Drive: %d, Type: %s",
                  ide_dev_buff[i].channel,
                  ide_dev_buff[i].drive,
                  ide_dev_buff[i].type == IDE_ATA ? "ATA" : "ATAPI");
        txtprintln(0x0F,"  Size: %d sectors", ide_dev_buff[i].size);
        txtprintln(0x0F,"  Model: %s", ide_dev_buff[i].model);

    }

    halt();
}

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
    pci_scan();
    serialwriteline("Found mem (kb): ");
    serialwriteline(itoa((total_memory_bytes / 1024)));
    serialwriteline("\r\n");
    serialwriteline("Found device(s): \r\n");
    serialwriteline("#. <device id> : <vendor id> : <class code> : <slot> : <subclass> : <prog if>\r\n");
    for (size_t i = 0; i < pci_entry_num; i++) {
        serialwriteline(itoa(i));
        serialwriteline(". ");
        serialwriteline(itoa(pcientrys[i].device_id));
        serialwriteline(" : ");
        serialwriteline(itoa(pcientrys[i].vendor_id));
        serialwriteline(" : ");
        serialwriteline(pciclasstostring(pcientrys[i].class_code));
        serialwriteline(" : ");
        serialwriteline(itoa((uint16_t)pcientrys[i].slot));
        serialwriteline(" : ");
        serialwriteline(itoa((uint16_t)pcientrys[i].subclass));
        serialwriteline(" : ");
        serialwriteline(itoa((uint16_t)pcientrys[i].prog_if));
        serialwriteline("\r\n");
    }

    writeline("Found mem (kb): ", 0, 0, 0x0F);
    writeline(itoa((total_memory_bytes / 1024)), 18, 0, 0x0F);
    writeline("Found device(s): ", 0, 1, 0x0F);
    writeline("#. <device id> : <vendor id> : <class code> : <slot> : <subclass> : <prog if>", 0, 2, 0x0F);

    uint8_t y = 3;

    for (size_t i = 0; i < pci_entry_num; i++) {
        char buf[50];
        strcat(buf, itoa(i));
        strcat(buf, ". ");
        strcat(buf, itoa(pcientrys[i].device_id));
        strcat(buf, " : ");
        strcat(buf, itoa(pcientrys[i].vendor_id));
        strcat(buf, " : ");
        strcat(buf, pciclasstostring(pcientrys[i].class_code));
        strcat(buf, " : ");
        strcat(buf, itoa((uint16_t)pcientrys[i].slot));
        strcat(buf, " : ");
        strcat(buf, itoa((uint16_t)pcientrys[i].subclass));
        strcat(buf, " : ");
        strcat(buf, itoa((uint16_t)pcientrys[i].prog_if));
        writeline(buf, 0, y, 0x0F);
        for (size_t i = 0; i < strlen(buf); i++) {
            buf[i] = '\0';
        }
        y++;
    }

    halt();
}

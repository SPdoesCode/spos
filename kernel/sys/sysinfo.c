#include <sys/sysinfo.h>
#include <std/stddef.h>
#include <sys/portio.h>

ram_entry_t ramenterys[MAX_RAM_ENTRIES];
size_t ram_entry_num = 0;
uint64_t total_memory_bytes = 0;

pci_entry_t pcientrys[MAX_DEV_ENTRIES];
size_t pci_entry_num = 0;

void fillramentrys(void) {
    // E820 map stored at 0x8000 in boot code
    uint16_t* entry_count_ptr = (uint16_t*)0x8000;
    uint8_t* entry_ptr = (uint8_t*)0x8004;

    ram_entry_num = *entry_count_ptr;

    if (ram_entry_num == 0 || ram_entry_num > MAX_RAM_ENTRIES) {
        ram_entry_num = 0;
        return;
    }

    total_memory_bytes = 0;
    uint64_t usable_memory = 0;

    for (size_t i = 0; i < ram_entry_num; i++) {
        // read base addr (should be 8 bytes)
        ramenterys[i].base_addr = *(uint64_t*)(entry_ptr);
        entry_ptr += 8;

        // get length (8 bytes)
        ramenterys[i].region_len = *(uint64_t*)(entry_ptr);
        entry_ptr += 8;

        // get type
        ramenterys[i].type = *(uint32_t*)(entry_ptr);
        entry_ptr += 4;

        // read ACPI attr (4 bytes)
        ramenterys[i].attrib = *(uint32_t*)(entry_ptr);
        entry_ptr += 4;

        // next pointer isn't used, so clear it
        ramenterys[i].next = 0;

        // type 1 = usable RAM
        if (ramenterys[i].type == 1) {
            usable_memory += ramenterys[i].region_len;
        }
    }

    total_memory_bytes = usable_memory;
}

uint32_t readpciconfig(uint16_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address =
        (1U << 31) |
        ((uint32_t)bus << 16) |
        ((uint32_t)slot << 11) |
        ((uint32_t)func << 8) |
        (offset & 0xFC);
    outl(PCI_CONFIG_ADDRESS, address);
    return inl(PCI_CONFIG_DATA);
}

pci_type_t translateclass(uint8_t class_code) {
    switch(class_code) {
        case 0x00: return unclassified;
        case 0x01: return mass_storage;
        case 0x02: return network;
        case 0x03: return display;
        case 0x04: return multimedia;
        case 0x05: return memory;
        case 0x06: return bridge;
        case 0x07: return simple_comm;
        case 0x08: return base_system;
        case 0x09: return input;
        case 0x0a: return docking;
        case 0x0b: return processor;
        case 0x0c: return serial_bus;
        case 0x0d: return wireless;
        case 0x0e: return intelligent_io;
        case 0x0f: return satellite;
        case 0x10: return encryption;
        case 0x11: return data_acq;
        default:   return unknown;
    }
}

char* pciclasstostring(pci_type_t class_code) {
    switch (class_code) {
        case unclassified:   return "unclassified";
        case mass_storage:   return "mass storage";
        case network:        return "network";
        case display:        return "display";
        case multimedia:     return "multimedia";
        case memory:         return "memory";
        case bridge:         return "bridge";
        case simple_comm:    return "simple comm";
        case base_system:    return "base system";
        case input:          return "input";
        case docking:        return "docking";
        case processor:      return "processor";
        case serial_bus:     return "serial bus";
        case wireless:       return "wireless";
        case intelligent_io: return "intelligent io";
        case satellite:      return "satellite";
        case encryption:     return "encryption";
        case data_acq:       return "data acquisition";
        case unknown:
        default:             return "unknown";
    }
}

void pci_scan(void) {
    int count = 0;

    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            for (uint8_t func = 0; func < 8; func++) {
                if (count >= MAX_DEV_ENTRIES) {
                    pci_entry_num = count;
                    return;
                }

                uint32_t vendor_data = readpciconfig(bus, slot, func, 0x00);
                uint16_t vendor_id = (uint16_t)(vendor_data & 0xFFFF);
                if (vendor_id == 0xFFFF) continue; // No device

                uint16_t device_id = (uint16_t)((vendor_data >> 16) & 0xFFFF);
                uint32_t class_data = readpciconfig(bus, slot, func, 0x08);
                uint8_t class_code = (uint8_t)((class_data >> 24) & 0xFF);
                uint8_t subclass = (uint8_t)((class_data >> 16) & 0xFF);
                uint8_t prog_if = (uint8_t)((class_data >> 8) & 0xFF);

                pcientrys[count].vendor_id = vendor_id;
                pcientrys[count].device_id = device_id;
                pcientrys[count].class_code = translateclass(class_code);
                pcientrys[count].subclass = subclass;
                pcientrys[count].prog_if = prog_if;
                pcientrys[count].bus = bus;
                pcientrys[count].slot = slot;
                pcientrys[count].func = func;

                count++;
            }
        }
    }

    pci_entry_num = count;
}

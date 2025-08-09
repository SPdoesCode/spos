#include <sys/sysinfo.h>
#include <std/stddef.h>
#include <sys/portio.h>

ram_entry_t ramenterys[MAX_RAM_ENTRIES];
size_t ram_entry_num = 0;
uint64_t total_memory_bytes = 0;

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

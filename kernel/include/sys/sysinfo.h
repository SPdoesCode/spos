/*

    A set of utilitys to get info from the system.
    (Thanks to micl for helping me a lot here!)

*/

#ifndef SYSINFO_H
#define SYSINFO_H

#include <std/stddef.h>

#define MAX_RAM_ENTRIES 128

typedef struct ram_entry {
	uint64_t base_addr;
	uint64_t region_len;
	uint32_t type;
	uint32_t attrib;
	struct ram_entry *next;
} ram_entry_t;

extern ram_entry_t ramenterys[MAX_RAM_ENTRIES];
extern size_t ram_entry_num;
extern uint64_t total_memory_bytes;

void fillramentrys(void);

#endif

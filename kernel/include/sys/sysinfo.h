/*

    A set of utilitys to get info from the system.

*/

#ifndef SYSINFO_H
#define SYSINFO_H

#include <std/stddef.h>

typedef struct ram_entry {
	uint64_t base_addr;
	uint64_t region_len;
	uint32_t type;
	uint32_t attrib;
	struct ram_entry *next;
} ram_entry_t;

extern ram_entry_t ramenterys[16];
extern size_t ram_entry_num;

void fillramentrys(void);
uint32_t getrambytes(void);

#endif

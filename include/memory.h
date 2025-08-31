/*

    A set of utils for the memory management in the kernel.

*/
#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

typedef struct ram_entry {
    uint32_t base_addr;
    uint32_t region_len;
    uint32_t type;
    uint32_t attrib;
    struct ram_entry* next;
} ram_entry_t;

typedef struct mem_block {
    size_t size;
    struct mem_block* next;
} mem_block_t;

extern mem_block_t* memory_blocks;
extern uint32_t max_size;
extern uintptr_t heap_start;
extern uintptr_t heap_end;

extern uint8_t __kernel_end;

extern ram_entry_t* ramenterys;
extern size_t ram_entry_num;
extern uint32_t total_memory_bytes;
extern uint32_t total_kern_memory_bytes;

void get_ram(void);
int init_memory(void* heap_start, size_t heap_size);

void* kalloc(size_t size);
void kfree(void* ptr);

#endif
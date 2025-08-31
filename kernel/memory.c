#include <memory.h>
#include <stdint.h>

extern uint8_t __kernel_end;

uintptr_t heap_start = (uintptr_t)&__kernel_end;
uintptr_t heap_end = 0;

mem_block_t* memory_blocks = 0;

void get_ram(void) {
    uint16_t* entry_count_ptr = (uint16_t*)0x8000;
    uint8_t* entry_ptr = (uint8_t*)0x8004;

    ram_entry_num = *entry_count_ptr;

    total_memory_bytes = 0;
    uint32_t usable_memory = 0;
    uint32_t kernel_memory = 0;

    for (size_t i = 0; i < ram_entry_num; i++) {
        ramenterys[i].base_addr = *(uint32_t*)entry_ptr; entry_ptr += 4;
        ramenterys[i].region_len = *(uint32_t*)entry_ptr; entry_ptr += 4;
        ramenterys[i].type = *(uint32_t*)entry_ptr; entry_ptr += 4;
        ramenterys[i].attrib = *(uint32_t*)entry_ptr; entry_ptr += 4;
        ramenterys[i].next = 0;

        if (ramenterys[i].type == 1) {
            usable_memory += ramenterys[i].region_len;

            uintptr_t region_start = ramenterys[i].base_addr;
            uintptr_t region_end = region_start + ramenterys[i].region_len;

            if (region_end > heap_start && heap_end == 0) {
                heap_start = region_start > heap_start ? region_start : heap_start;
                heap_end = region_end;
            }
        }

        if (ramenterys[i].type == 0) {
            kernel_memory += ramenterys[i].region_len;
        }
    }

    total_kern_memory_bytes = kernel_memory;
    total_memory_bytes = usable_memory;
}

int init_memory(void* start_ptr, size_t heap_size) {
    if (!start_ptr || heap_size < sizeof(mem_block_t))
        return -1;

    heap_start = (uintptr_t)start_ptr;
    heap_end   = heap_start + heap_size;

    memory_blocks = (mem_block_t*)heap_start;
    memory_blocks->size = heap_size - sizeof(mem_block_t);
    memory_blocks->next = 0;

    return 0;
}

void* kalloc(size_t size) {
    mem_block_t* prev = 0;
    mem_block_t* curr = memory_blocks;

    while (curr) {
        if (curr->size >= size) {

            if (curr->size >= size + sizeof(mem_block_t) + 4) {
                mem_block_t* new_block = (mem_block_t*)((uintptr_t)curr + sizeof(mem_block_t) + size);
                new_block->size = curr->size - size - sizeof(mem_block_t);
                new_block->next = curr->next;

                curr->size = size;
                curr->next = new_block;
            }

            if (prev)
                prev->next = curr->next;
            else
                memory_blocks = curr->next;

            return (void*)((uintptr_t)curr + sizeof(mem_block_t));
        }

        prev = curr;
        curr = curr->next;
    }

    return 0;
}

void kfree(void* ptr) {
    if (!ptr) return;

    mem_block_t* block = (mem_block_t*)((uintptr_t)ptr - sizeof(mem_block_t));
    block->next = memory_blocks;
    memory_blocks = block;
}

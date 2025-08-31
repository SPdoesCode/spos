#include <kstdio.h>
#include <memory.h>
#include <loader.h>

void kmain(void) {
    ekprintf("Booted spos...");

    ekprintf("Getting ram...");
    get_ram();
    ekprintf("Found %d usable ram (kb)...", (total_memory_bytes / 1024));

    ekprintf("Initing memory...");
    init_memory((void*)heap_start, heap_end - heap_start);
    ekprintf("Initing device drivers...");


}
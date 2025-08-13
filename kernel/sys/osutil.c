#include <sys/osutil.h>
#include <std/stddef.h>

void sleep(uint32_t s) {
    for (uint32_t i = 0; i < s; i++) {
        // abt 1 second prob
        for (volatile uint32_t j = 0; j < 100000000; j++) {
            __asm__ volatile("nop");
        }
    }
}
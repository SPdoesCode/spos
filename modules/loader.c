#include <loader.h>
#include <kmodules.h>

// Storage device drivers:
extern kernel_driver_t ide;

// Console drivers:
extern kernel_driver_t vga_textmode;

// Video drivers:
//extern kernel_driver_t vga_vesa;

// File system drivers:
//extern kernel_driver_t fat16;
//extern kernel_driver_t ext2;

// List of activated drivers:
kernel_driver_t* DRIVER_LIST[] = {
    &ide,
    &vga_textmode,
//    &vesa,
//    &fat16,
//    &ext2,
    NULL // MUST be here
};
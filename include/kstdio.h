/*

    kstdio.h: houses standard io for the kernel.

*/

#ifndef KSTDIO_H
#define KSTDIO_H

// Early kernel printf, uses a vga textmode buffer.
void ekprintf(char* msg, ...);

// Prints to the kernel buffer.
void kprintf(char* msg, ...);

#endif
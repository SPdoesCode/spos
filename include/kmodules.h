/*

    The Kernel module system for spos

*/
#ifndef KMODULES_H
#define KMODULES_H

#include <stdint.h>

typedef enum driver_type {
    STORAGE,
    FS,
    INPUT,
    VIDEO,
    CONSOLE,
    MISC,
} driver_type_t;

typedef struct misc_driver {
    void (*init)(void);
    void (*exit)(void);
} misc_driver_t;

typedef struct storage_driver {
    void (*init)(void);
    int  (*write)(const void* buffer, size_t size, uint32_t offset);
    int  (*read)(void* buffer, size_t size, uint32_t offset);
    void (*exit)(void);
} storage_driver_t;

typedef struct fs_driver {
    void (*init)(void);
    int  (*write)(const char* path, const void* buffer, size_t size);
    int  (*read)(const char* path, void* buffer, size_t size);
    int  (*mount)(const char* device, const char* mountpoint);
    int  (*unmount)(const char* mountpoint);
    void (*exit)(void);
} fs_driver_t;

typedef struct input_driver {
    void (*init)(void);
    int  (*read)(void);
    void (*quit)(void);
} input_driver_t;

typedef struct video_driver {
    void (*init)(void);
    void (*put_pixel)(int x, int y, uint8_t color);
    void (*quit)(void);
} video_driver_t;

typedef struct console_driver {
    void (*init)(void);
    void (*put_char)(char c, int x, int y, uint8_t color);
    void (*quit)(void);
} console_driver_t;

typedef struct kernel_driver {
    char* name;
    driver_type_t type;
    union {
        storage_driver_t* storage;
        fs_driver_t*      fs;
        input_driver_t*   input;
        video_driver_t*   video;
        console_driver_t* console;
        misc_driver_t*    misc;
    } driver;
} kernel_driver_t;

#endif
/*

    A set of utilitys to get info from the system.
    (Thanks to micl for helping me a lot here!)

*/

#ifndef SYSINFO_H
#define SYSINFO_H

#include <std/stddef.h>

#define MAX_RAM_ENTRIES 128

#define MAX_DEV_ENTRIES 128
#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

typedef struct ram_entry {
	uint64_t base_addr;
	uint64_t region_len;
	uint32_t type;
	uint32_t attrib;
	struct ram_entry *next;
} ram_entry_t;

typedef enum {
    unclassified = 0x00,
    mass_storage = 0x01,
    network = 0x02,
    display = 0x03,
    multimedia = 0x04,
    memory = 0x05,
    bridge = 0x06,
    simple_comm = 0x07,
    base_system = 0x08,
    input = 0x09,
    docking = 0x0a,
    processor = 0x0b,
    serial_bus = 0x0c,
    wireless = 0x0d,
    intelligent_io = 0x0e,
    satellite = 0x0f,
    encryption = 0x10,
    data_acq = 0x11,
    unknown = 0xff,
} pci_type_t;

typedef struct pci_entry {
    uint16_t vendor_id;
    uint16_t device_id;
    pci_type_t class_code;
    uint8_t subclass;
    uint8_t prog_if;
    uint8_t bus;
    uint8_t slot;
    uint8_t func;
} pci_entry_t;

extern ram_entry_t ramenterys[MAX_RAM_ENTRIES];
extern size_t ram_entry_num;
extern uint64_t total_memory_bytes;

extern pci_entry_t pcientrys[MAX_DEV_ENTRIES];
extern size_t pci_entry_num;


void fillramentrys(void);

uint32_t readpciconfig(uint16_t bus, uint8_t slot, uint8_t func, uint8_t offset);
pci_type_t translateclass(uint8_t class_code);
char* pciclasstostring(pci_type_t class_code);
void pci_scan(void);

#endif

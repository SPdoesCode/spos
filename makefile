# Tools
CC      = gcc
NASM    = nasm
LD      = ld

# Flags
CFLAGS  = -m32 -ffreestanding -nostdlib -Wall -Wextra -fno-pic -fno-pie -no-pie -Ikernel/include
NASM_BIN = -f bin
NASM_ELF = -f elf32
LDFLAGS  = -m elf_i386 -T boot/linker.ld --oformat binary

# Directories
BUILD = .build

# Sources
BOOT_SECTOR   = boot/boot.asm
KERNEL_ENTRY  = boot/kernel_entry.asm

KERNEL_C_SRCS := $(shell find kernel/ -type f -name "*.c")

KERNEL_OBJS = $(KERNEL_C_SRCS:%.c=$(BUILD)/%.o)
ENTRY_OBJ   = $(BUILD)/kernel_entry.o
BOOT_BIN    = $(BUILD)/boot.bin
KERNEL_BIN  = $(BUILD)/kernel.bin
OS_IMAGE    = $(BUILD)/os.img

# Default target
all: $(OS_IMAGE)

# Create build directory
$(BUILD):
	mkdir -p $(BUILD)

# Compile .c files
$(BUILD)/%.o: %.c | $(BUILD)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble kernel entry
$(ENTRY_OBJ): $(KERNEL_ENTRY) | $(BUILD)
	$(NASM) $(NASM_ELF) $< -o $@

# Assemble boot sector
$(BOOT_BIN): $(BOOT_SECTOR) | $(BUILD)
	$(NASM) $(NASM_BIN) $< -o $@

# Link kernel
$(KERNEL_BIN): $(ENTRY_OBJ) $(KERNEL_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Create OS floppy image
$(OS_IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$(BOOT_BIN) of=$@ bs=512 conv=notrunc
	dd if=$(KERNEL_BIN) of=$@ bs=512 seek=1 conv=notrunc

# Run with QEMU
run: $(OS_IMAGE)
	qemu-system-x86_64 -fda $< -m 1G -vga std -serial stdio

# Clean build
clean:
	rm -rf $(BUILD)

ZIG      = zig
NASM     = nasm
LD       = ld
BUILD    = .build

TARGET   = x86-freestanding
ZIGFLAGS = build-obj -target x86-freestanding -mcpu=i386 -O ReleaseSmall

BOOT_SECTOR     = boot.asm
KERNEL_ENTRY    = kernel_entry.asm
MAIN_ZIG        = main.zig
LINKER_SCRIPT   = linker.ld

BOOT_BIN        = $(BUILD)/boot.bin
ENTRY_OBJ       = $(BUILD)/kernel_entry.o
MAIN_OBJ        = $(BUILD)/main.o
KERNEL_BIN      = $(BUILD)/kernel.bin
OS_IMAGE        = $(BUILD)/os.img

all: $(OS_IMAGE)

$(BUILD):
	mkdir -p $(BUILD)

$(BOOT_BIN): $(BOOT_SECTOR) | $(BUILD)
	$(NASM) -f bin $< -o $@

$(ENTRY_OBJ): $(KERNEL_ENTRY) | $(BUILD)
	$(NASM) -f elf32 $< -o $@

$(MAIN_OBJ): $(MAIN_ZIG) imports/vga.zig imports/input.zig | $(BUILD)
	$(ZIG) build-obj $(MAIN_ZIG) -target x86-freestanding -mcpu=i386 -O ReleaseSmall -fno-stack-protector -femit-bin=$@

$(KERNEL_BIN): $(ENTRY_OBJ) $(MAIN_OBJ)
	$(LD) -m elf_i386 -T $(LINKER_SCRIPT) --oformat binary -o $@ $^

$(OS_IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$(BOOT_BIN) of=$@ bs=512 conv=notrunc
	dd if=$(KERNEL_BIN) of=$@ bs=512 seek=1 conv=notrunc

run: $(OS_IMAGE)
	qemu-system-x86_64 -fda $< -m 128 -vga std

clean:
	rm -rf $(BUILD)

; Boot sector that loads kernel only (skip video for now)
BITS 16
org 0x7C00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Load kernel (128 sectors starting at sector 2)
    mov ah, 0x02        ; BIOS read sectors
    mov al, 128         ; Number of sectors to read
    mov ch, 0x00        ; Cylinder 0
    mov cl, 0x02        ; Start from sector 2
    mov dh, 0x00        ; Head 0
    mov bx, 0x1000      ; Load at 0x1000:0x0000
    mov es, bx
    xor bx, bx
    int 0x13
    jc disk_error

    ; Switch to protected mode
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEG:init_pm

disk_error:
    mov si, disk_error_msg
    call print_string_16
    jmp $

print_string_16:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print_string_16
.done:
    ret

disk_error_msg: db "Disk error!", 0

BITS 32
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    ; Jump to kernel
    jmp 0x10000

; GDT
gdt_start:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xFFFF       ; Limit (bits 0-15)
    dw 0x0          ; Base (bits 0-15)
    db 0x0          ; Base (bits 16-23)
    db 10011010b    ; Access byte
    db 11001111b    ; Flags + Limit (bits 16-19)
    db 0x0          ; Base (bits 24-31)

gdt_data:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

times 510 - ($ - $$) db 0
dw 0xAA55

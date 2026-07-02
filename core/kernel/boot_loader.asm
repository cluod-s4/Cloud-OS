; core/kernel/boot_loader.asm
; Real bootloader for Cloud-OS

[org 0x7c00]
[bits 16]

start:
    mov ax, 0x0003
    int 0x10
    
    mov si, boot_msg
    call print_string
    
    call load_kernel
    call switch_to_protected
    jmp 0x08:0x1000

print_string:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0e
    int 0x10
    jmp print_string
.done:
    ret

load_kernel:
    mov ah, 0x02
    mov al, 0x10
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    mov dl, 0x80
    mov bx, 0x1000
    mov es, bx
    mov bx, 0x00
    int 0x13
    jc disk_error
    ret

disk_error:
    mov si, disk_error_msg
    call print_string
    jmp $

switch_to_protected:
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp 0x08:init_pm

[bits 32]
init_pm:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

gdt_start:
    dd 0x0
    dd 0x0
gdt_code:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0
gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

boot_msg db "Cloud-OS Bootloader v1.0", 0x0d, 0x0a, 0
disk_error_msg db "Disk error!", 0x0d, 0x0a, 0

times 510 - ($ - $$) db 0
dw 0xaa55

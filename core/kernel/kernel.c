// core/kernel/kernel.c
// Real kernel for Cloud-OS

#include "kernel.h"
#include "memory/memory.h"
#include "process/scheduler.h"
#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "interrupts/idt.h"
#include "interrupts/isr.h"
#include "system/syscall.h"
#include "fs/vfs.h"
#include "security/encryption.h"

void kernel_main() {
    screen_init();
    screen_print("Cloud-OS Kernel v1.0\n");
    screen_print("========================\n");

    memory_init();
    screen_print("[+] Memory Manager initialized\n");

    idt_init();
    screen_print("[+] IDT initialized\n");

    isr_init();
    screen_print("[+] ISRs initialized\n");

    keyboard_init();
    screen_print("[+] Keyboard driver initialized\n");

    vfs_init();
    screen_print("[+] Virtual File System initialized\n");

    encryption_init();
    screen_print("[+] Encryption module initialized\n");

    scheduler_init();
    screen_print("[+] Scheduler initialized\n");

    syscall_init();
    screen_print("[+] System calls initialized\n");

    screen_print("\nCloud-OS Shell v1.0\n");
    screen_print("Type 'help' for commands\n");
    shell_start();

    while (1) {
        scheduler_run();
    }
}

int syscall_handler(int syscall_number, void* arg1, void* arg2, void* arg3) {
    switch (syscall_number) {
        case SYS_PRINT:
            screen_print((char*)arg1);
            return 0;
        case SYS_READ:
            return keyboard_read((char*)arg1);
        case SYS_MALLOC:
            return (int)malloc((size_t)arg1);
        case SYS_FREE:
            free((void*)arg1);
            return 0;
        case SYS_OPEN:
            return vfs_open((char*)arg1, (int)arg2);
        case SYS_READ_FILE:
            return vfs_read((int)arg1, (char*)arg2, (size_t)arg3);
        case SYS_WRITE_FILE:
            return vfs_write((int)arg1, (char*)arg2, (size_t)arg3);
        case SYS_CLOSE:
            return vfs_close((int)arg1);
        case SYS_EXIT:
            scheduler_exit();
            return 0;
        default:
            return -1;
    }
}

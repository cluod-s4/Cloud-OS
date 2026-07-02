// core/kernel/system/syscall.c
// Real system call implementation

#include "syscall.h"
#include "kernel.h"
#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "memory/memory.h"
#include "fs/vfs.h"
#include "process/scheduler.h"

void syscall_init() {
    screen_print("[+] System calls initialized\n");
}

int syscall(int number, void* arg1, void* arg2, void* arg3) {
    int result = -1;
    
    __asm__ volatile(
        "int $0x80"
        : "=a"(result)
        : "a"(number), "b"(arg1), "c"(arg2), "d"(arg3)
        : "memory"
    );
    
    return result;
}

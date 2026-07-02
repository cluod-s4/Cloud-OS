// core/kernel/kernel.h
#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define SYS_PRINT     1
#define SYS_READ      2
#define SYS_MALLOC    3
#define SYS_FREE      4
#define SYS_OPEN      5
#define SYS_READ_FILE 6
#define SYS_WRITE_FILE 7
#define SYS_CLOSE     8
#define SYS_EXIT      9

#define KERNEL_VERSION "1.0.0"

void kernel_main();
int syscall_handler(int syscall_number, void* arg1, void* arg2, void* arg3);

#endif

// core/kernel/system/syscall.h
#ifndef SYSCALL_H
#define SYSCALL_H

void syscall_init();
int syscall(int number, void* arg1, void* arg2, void* arg3);

#endif

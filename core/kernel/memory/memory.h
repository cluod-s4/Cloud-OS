// core/kernel/memory/memory.h
#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void memory_init();
void* malloc(size_t size);
void free(void* ptr);

#endif

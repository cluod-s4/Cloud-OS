// core/kernel/memory/heap.h
#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

void heap_init();
void* kmalloc(size_t size);
void kfree(void* ptr);
size_t kmalloc_usable_size(void* ptr);

#endif

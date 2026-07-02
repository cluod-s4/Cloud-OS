// core/kernel/memory/paging.h
#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>

void paging_init();
void* alloc_page();
void map_page(void* physical, void* virtual, uint32_t flags);
void unmap_page(void* virtual);
void* get_physical_address(void* virtual);

#endif

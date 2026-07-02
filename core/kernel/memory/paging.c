// core/kernel/memory/paging.c
// Real paging implementation

#include "paging.h"
#include "drivers/screen.h"
#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE 4096
#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3FF)
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3FF)
#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_USER 0x4

static uint32_t* page_directory = NULL;
static uint32_t* page_tables[1024] = {0};
static uint32_t next_free_page = 0x1000000;

void paging_init() {
    // Allocate page directory
    page_directory = (uint32_t*)0x100000;
    
    // Initialize page directory
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002;
    }
    
    // Identity map first 4MB
    for (int i = 0; i < 1024; i++) {
        page_tables[0] = (uint32_t*)0x101000;
        page_tables[0][i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_WRITE;
    }
    
    page_directory[0] = (uint32_t)page_tables[0] | PAGE_PRESENT | PAGE_WRITE;
    
    // Enable paging
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0" : : "r"(cr0));
    
    screen_print("[+] Paging initialized\n");
}

void* alloc_page() {
    uint32_t page = next_free_page;
    next_free_page += PAGE_SIZE;
    return (void*)page;
}

void map_page(void* physical, void* virtual, uint32_t flags) {
    uint32_t pd_index = PAGE_DIRECTORY_INDEX((uint32_t)virtual);
    uint32_t pt_index = PAGE_TABLE_INDEX((uint32_t)virtual);
    
    if (!(page_directory[pd_index] & PAGE_PRESENT)) {
        page_tables[pd_index] = (uint32_t*)alloc_page();
        page_directory[pd_index] = (uint32_t)page_tables[pd_index] | PAGE_PRESENT | PAGE_WRITE;
    }
    
    page_tables[pd_index][pt_index] = (uint32_t)physical | flags;
}

void unmap_page(void* virtual) {
    uint32_t pd_index = PAGE_DIRECTORY_INDEX((uint32_t)virtual);
    uint32_t pt_index = PAGE_TABLE_INDEX((uint32_t)virtual);
    
    if (page_directory[pd_index] & PAGE_PRESENT) {
        page_tables[pd_index][pt_index] = 0x00000002;
    }
}

void* get_physical_address(void* virtual) {
    uint32_t pd_index = PAGE_DIRECTORY_INDEX((uint32_t)virtual);
    uint32_t pt_index = PAGE_TABLE_INDEX((uint32_t)virtual);
    
    if (!(page_directory[pd_index] & PAGE_PRESENT)) {
        return NULL;
    }
    
    uint32_t physical = page_tables[pd_index][pt_index] & ~0xFFF;
    physical += (uint32_t)virtual & 0xFFF;
    
    return (void*)physical;
}

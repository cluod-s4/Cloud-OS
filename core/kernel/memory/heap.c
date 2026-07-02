// core/kernel/memory/heap.c
// Real heap implementation

#include "heap.h"
#include "paging.h"
#include "drivers/screen.h"
#include <stdint.h>
#include <stddef.h>

#define HEAP_START 0x2000000
#define HEAP_SIZE 0x1000000

typedef struct heap_block {
    size_t size;
    int used;
    struct heap_block* next;
    struct heap_block* prev;
} heap_block_t;

static uint8_t* heap_start = (uint8_t*)HEAP_START;
static heap_block_t* free_list = NULL;

void heap_init() {
    free_list = (heap_block_t*)heap_start;
    free_list->size = HEAP_SIZE - sizeof(heap_block_t);
    free_list->used = 0;
    free_list->next = NULL;
    free_list->prev = NULL;
    
    screen_print("[+] Heap initialized\n");
}

void* kmalloc(size_t size) {
    size = (size + 7) & ~7;
    
    heap_block_t* current = free_list;
    
    while (current != NULL) {
        if (!current->used && current->size >= size) {
            if (current->size > size + sizeof(heap_block_t) + 8) {
                heap_block_t* new_block = (heap_block_t*)((uint8_t*)current + 
                                                         sizeof(heap_block_t) + size);
                new_block->size = current->size - size - sizeof(heap_block_t);
                new_block->used = 0;
                new_block->next = current->next;
                new_block->prev = current;
                
                if (current->next) {
                    current->next->prev = new_block;
                }
                
                current->size = size;
                current->next = new_block;
            }
            
            current->used = 1;
            return (void*)((uint8_t*)current + sizeof(heap_block_t));
        }
        
        current = current->next;
    }
    
    return NULL;
}

void kfree(void* ptr) {
    if (ptr == NULL) return;
    
    heap_block_t* block = (heap_block_t*)((uint8_t*)ptr - sizeof(heap_block_t));
    block->used = 0;
    
    // Merge with next block
    if (block->next && !block->next->used) {
        block->size += sizeof(heap_block_t) + block->next->size;
        block->next = block->next->next;
        if (block->next) {
            block->next->prev = block;
        }
    }
    
    // Merge with previous block
    if (block->prev && !block->prev->used) {
        block->prev->size += sizeof(heap_block_t) + block->size;
        block->prev->next = block->next;
        if (block->next) {
            block->next->prev = block->prev;
        }
    }
}

size_t kmalloc_usable_size(void* ptr) {
    if (ptr == NULL) return 0;
    
    heap_block_t* block = (heap_block_t*)((uint8_t*)ptr - sizeof(heap_block_t));
    return block->size;
}

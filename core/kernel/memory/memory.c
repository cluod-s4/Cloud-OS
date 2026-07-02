// core/kernel/memory/memory.c
// Real memory management

#include "memory.h"
#include "drivers/screen.h"

#define MEMORY_SIZE 0x1000000
#define BLOCK_SIZE 4096

typedef struct block {
    size_t size;
    int used;
    struct block* next;
} block_t;

static uint8_t* heap_start = (uint8_t*)0x1000000;
static block_t* free_list = NULL;

void memory_init() {
    free_list = (block_t*)heap_start;
    free_list->size = MEMORY_SIZE - sizeof(block_t);
    free_list->used = 0;
    free_list->next = NULL;
    screen_print("[+] Memory Manager initialized\n");
}

void* malloc(size_t size) {
    size = (size + 7) & ~7;
    
    block_t* current = free_list;
    block_t* prev = NULL;
    
    while (current != NULL) {
        if (!current->used && current->size >= size) {
            if (current->size > size + sizeof(block_t) + 8) {
                block_t* new_block = (block_t*)((uint8_t*)current + 
                                               sizeof(block_t) + size);
                new_block->size = current->size - size - sizeof(block_t);
                new_block->used = 0;
                new_block->next = current->next;
                
                current->size = size;
                current->next = new_block;
            }
            
            current->used = 1;
            return (void*)((uint8_t*)current + sizeof(block_t));
        }
        
        prev = current;
        current = current->next;
    }
    
    return NULL;
}

void free(void* ptr) {
    if (ptr == NULL) return;
    
    block_t* block = (block_t*)((uint8_t*)ptr - sizeof(block_t));
    block->used = 0;
    
    block_t* current = free_list;
    while (current != NULL && current->next != NULL) {
        if (!current->used && !current->next->used) {
            current->size += sizeof(block_t) + current->next->size;
            current->next = current->next->next;
        }
        current = current->next;
    }
}

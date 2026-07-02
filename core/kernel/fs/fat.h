// core/kernel/fs/fat.h
#ifndef FAT_H
#define FAT_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    int exists;
    uint16_t cluster;
    uint32_t size;
    uint8_t attributes;
} fat_entry_t;

int fat_init();
fat_entry_t fat_find_file(const char* filename);
int fat_read_file(const char* filename, uint8_t* buffer, uint32_t max_size);

#endif

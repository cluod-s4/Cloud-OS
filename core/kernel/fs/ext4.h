// core/kernel/fs/ext4.h
#ifndef EXT4_H
#define EXT4_H

#include <stdint.h>
#include <stddef.h>

int ext4_init();
int ext4_read_file(const char* filename, uint8_t* buffer, uint32_t max_size);

#endif

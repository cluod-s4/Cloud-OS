// core/kernel/fs/vfs.c
// Real Virtual File System

#include "vfs.h"
#include "drivers/screen.h"
#include "memory/memory.h"
#include <string.h>

#define MAX_FILES 64
#define MAX_FILENAME 256

typedef struct file {
    char name[MAX_FILENAME];
    int size;
    int in_use;
    uint8_t* data;
} file_t;

static file_t files[MAX_FILES];

void vfs_init() {
    for (int i = 0; i < MAX_FILES; i++) {
        files[i].in_use = 0;
    }
    screen_print("[+] VFS initialized\n");
}

int vfs_open(const char* name, int mode) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].in_use && strcmp(files[i].name, name) == 0) {
            return i;
        }
    }
    
    for (int i = 0; i < MAX_FILES; i++) {
        if (!files[i].in_use) {
            strcpy(files[i].name, name);
            files[i].size = 0;
            files[i].in_use = 1;
            files[i].data = malloc(4096);
            return i;
        }
    }
    
    return -1;
}

int vfs_read(int fd, char* buffer, size_t size) {
    if (fd < 0 || fd >= MAX_FILES || !files[fd].in_use) {
        return -1;
    }
    
    size_t read_size = size < files[fd].size ? size : files[fd].size;
    memcpy(buffer, files[fd].data, read_size);
    return read_size;
}

int vfs_write(int fd, char* buffer, size_t size) {
    if (fd < 0 || fd >= MAX_FILES || !files[fd].in_use) {
        return -1;
    }
    
    memcpy(files[fd].data, buffer, size);
    files[fd].size = size;
    return size;
}

int vfs_close(int fd) {
    if (fd < 0 || fd >= MAX_FILES || !files[fd].in_use) {
        return -1;
    }
    
    files[fd].in_use = 0;
    free(files[fd].data);
    return 0;
}

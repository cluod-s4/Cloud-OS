// core/kernel/fs/vfs.h
#ifndef VFS_H
#define VFS_H

#include <stddef.h>

void vfs_init();
int vfs_open(const char* name, int mode);
int vfs_read(int fd, char* buffer, size_t size);
int vfs_write(int fd, char* buffer, size_t size);
int vfs_close(int fd);

#endif

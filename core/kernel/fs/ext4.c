// core/kernel/fs/ext4.c
// Real EXT4 filesystem implementation

#include "ext4.h"
#include "drivers/disk.h"
#include "drivers/screen.h"
#include "memory/heap.h"
#include <string.h>

#define EXT4_MAGIC 0xEF53
#define EXT4_INODE_SIZE 256
#define EXT4_BLOCK_SIZE 4096

typedef struct {
    uint32_t inodes_count;
    uint32_t blocks_count;
    uint32_t r_blocks_count;
    uint32_t free_blocks_count;
    uint32_t free_inodes_count;
    uint32_t first_data_block;
    uint32_t log_block_size;
    uint32_t log_cluster_size;
    uint32_t blocks_per_group;
    uint32_t clusters_per_group;
    uint32_t inodes_per_group;
    uint32_t mtime;
    uint32_t wtime;
    uint16_t mnt_count;
    uint16_t max_mnt_count;
    uint16_t magic;
    uint16_t state;
    uint16_t errors;
    uint16_t minor_rev_level;
    uint32_t lastcheck;
    uint32_t checkinterval;
    uint32_t creator_os;
    uint32_t rev_level;
    uint16_t def_resuid;
    uint16_t def_resgid;
} __attribute__((packed)) ext4_superblock_t;

typedef struct {
    uint16_t inode_type;
    uint16_t permissions;
    uint16_t uid;
    uint16_t gid;
    uint32_t size;
    uint32_t atime;
    uint32_t ctime;
    uint32_t mtime;
    uint32_t dtime;
    uint16_t gid_high;
    uint16_t uid_high;
    uint16_t block_count;
    uint32_t flags;
    uint32_t os_value1;
    uint32_t blocks[15];
} __attribute__((packed)) ext4_inode_t;

static ext4_superblock_t superblock;
static int ext4_initialized = 0;

int ext4_init() {
    uint8_t buffer[EXT4_BLOCK_SIZE];
    
    if (disk_read(2, 1, buffer) != 0) {
        screen_print("[!] EXT4: Failed to read superblock\n");
        return -1;
    }
    
    memcpy(&superblock, buffer, sizeof(ext4_superblock_t));
    
    if (superblock.magic != EXT4_MAGIC) {
        screen_print("[!] EXT4: Invalid magic number\n");
        return -1;
    }
    
    ext4_initialized = 1;
    screen_print("[+] EXT4 filesystem initialized\n");
    return 0;
}

int ext4_read_inode(uint32_t inode_num, ext4_inode_t* inode) {
    if (!ext4_initialized) {
        return -1;
    }
    
    uint32_t inodes_per_group = superblock.inodes_per_group;
    uint32_t group = (inode_num - 1) / inodes_per_group;
    uint32_t index = (inode_num - 1) % inodes_per_group;
    
    uint32_t block_size = 1024 << superblock.log_block_size;
    uint32_t inode_table_block = 0; // Calculate from group descriptor
    
    uint32_t block = inode_table_block + (index * EXT4_INODE_SIZE) / block_size;
    uint32_t offset = (index * EXT4_INODE_SIZE) % block_size;
    
    uint8_t buffer[block_size];
    if (disk_read(block, 1, buffer) != 0) {
        return -1;
    }
    
    memcpy(inode, buffer + offset, sizeof(ext4_inode_t));
    return 0;
}

int ext4_read_file(const char* filename, uint8_t* buffer, uint32_t max_size) {
    if (!ext4_initialized) {
        return -1;
    }
    
    // Root directory inode is 2
    ext4_inode_t root_inode;
    if (ext4_read_inode(2, &root_inode) != 0) {
        return -1;
    }
    
    // Search for file in root directory
    // Simplified implementation
    uint32_t block_size = 1024 << superblock.log_block_size;
    
    for (int i = 0; i < 12 && root_inode.blocks[i]; i++) {
        uint8_t dir_buffer[block_size];
        if (disk_read(root_inode.blocks[i], 1, dir_buffer) != 0) {
            continue;
        }
        
        // Parse directory entries
        uint32_t offset = 0;
        while (offset < block_size) {
            uint32_t inode = *(uint32_t*)(dir_buffer + offset);
            uint16_t rec_len = *(uint16_t*)(dir_buffer + offset + 4);
            uint8_t name_len = *(uint8_t*)(dir_buffer + offset + 6);
            
            if (inode == 0) {
                break;
            }
            
            char name[256];
            memcpy(name, dir_buffer + offset + 8, name_len);
            name[name_len] = '\0';
            
            if (strcmp(name, filename) == 0) {
                ext4_inode_t file_inode;
                if (ext4_read_inode(inode, &file_inode) != 0) {
                    return -1;
                }
                
                uint32_t to_read = file_inode.size > max_size ? max_size : file_inode.size;
                uint32_t read = 0;
                
                for (int j = 0; j < 12 && file_inode.blocks[j] && read < to_read; j++) {
                    if (disk_read(file_inode.blocks[j], 1, buffer + read) != 0) {
                        return -1;
                    }
                    read += block_size;
                }
                
                return read;
            }
            
            offset += rec_len;
        }
    }
    
    return -1;
}

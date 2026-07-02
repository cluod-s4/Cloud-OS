// core/kernel/fs/fat.c
// Real FAT filesystem implementation

#include "fat.h"
#include "drivers/disk.h"
#include "drivers/screen.h"
#include "memory/heap.h"
#include <string.h>

#define FAT_SECTOR_SIZE 512
#define FAT_ENTRY_SIZE 2
#define FAT_FREE_ENTRY 0x0000
#define FAT_EOF_ENTRY 0xFFFF

typedef struct {
    uint8_t jump[3];
    char oem[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_count;
    uint16_t root_entries;
    uint16_t total_sectors;
    uint8_t media_descriptor;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_large;
} __attribute__((packed)) fat_boot_sector_t;

typedef struct {
    char filename[8];
    char extension[3];
    uint8_t attributes;
    uint8_t reserved;
    uint8_t create_time_tenths;
    uint16_t create_time;
    uint16_t create_date;
    uint16_t access_date;
    uint16_t cluster_high;
    uint16_t modify_time;
    uint16_t modify_date;
    uint16_t cluster_low;
    uint32_t file_size;
} __attribute__((packed)) fat_directory_entry_t;

static fat_boot_sector_t boot_sector;
static uint16_t* fat_table = NULL;
static uint32_t fat_table_size = 0;

int fat_init() {
    // Read boot sector
    uint8_t buffer[FAT_SECTOR_SIZE];
    if (disk_read(0, 1, buffer) != 0) {
        screen_print("[!] FAT: Failed to read boot sector\n");
        return -1;
    }
    
    memcpy(&boot_sector, buffer, sizeof(fat_boot_sector_t));
    
    // Validate FAT signature
    if (buffer[510] != 0x55 || buffer[511] != 0xAA) {
        screen_print("[!] FAT: Invalid boot sector signature\n");
        return -1;
    }
    
    // Read FAT table
    fat_table_size = boot_sector.sectors_per_fat * FAT_SECTOR_SIZE / FAT_ENTRY_SIZE;
    fat_table = kmalloc(boot_sector.sectors_per_fat * FAT_SECTOR_SIZE);
    if (fat_table == NULL) {
        screen_print("[!] FAT: Failed to allocate FAT table\n");
        return -1;
    }
    
    for (int i = 0; i < boot_sector.sectors_per_fat; i++) {
        uint32_t sector = boot_sector.reserved_sectors + i;
        if (disk_read(sector, 1, (uint8_t*)fat_table + i * FAT_SECTOR_SIZE) != 0) {
            screen_print("[!] FAT: Failed to read FAT table\n");
            kfree(fat_table);
            fat_table = NULL;
            return -1;
        }
    }
    
    screen_print("[+] FAT filesystem initialized\n");
    return 0;
}

fat_entry_t fat_find_file(const char* filename) {
    fat_entry_t result;
    result.exists = 0;
    result.cluster = 0;
    result.size = 0;
    
    // Read root directory
    uint32_t root_sector = boot_sector.reserved_sectors + 
                           boot_sector.fat_count * boot_sector.sectors_per_fat;
    uint32_t root_sectors = (boot_sector.root_entries * 32 + FAT_SECTOR_SIZE - 1) / FAT_SECTOR_SIZE;
    
    for (int i = 0; i < root_sectors; i++) {
        uint8_t buffer[FAT_SECTOR_SIZE];
        if (disk_read(root_sector + i, 1, buffer) != 0) {
            break;
        }
        
        for (int j = 0; j < FAT_SECTOR_SIZE / 32; j++) {
            fat_directory_entry_t* entry = (fat_directory_entry_t*)(buffer + j * 32);
            
            if (entry->filename[0] == 0x00) {
                break;
            }
            
            if (entry->filename[0] == 0xE5) {
                continue;
            }
            
            char entry_name[12];
            memcpy(entry_name, entry->filename, 8);
            entry_name[8] = '.';
            memcpy(entry_name + 9, entry->extension, 3);
            entry_name[12] = '\0';
            
            if (strcmp(entry_name, filename) == 0) {
                result.exists = 1;
                result.cluster = entry->cluster_low;
                result.size = entry->file_size;
                result.attributes = entry->attributes;
                return result;
            }
        }
    }
    
    return result;
}

int fat_read_file(const char* filename, uint8_t* buffer, uint32_t max_size) {
    fat_entry_t entry = fat_find_file(filename);
    
    if (!entry.exists) {
        screen_print("[!] FAT: File not found: ");
        screen_print(filename);
        screen_print("\n");
        return -1;
    }
    
    if (entry.size > max_size) {
        screen_print("[!] FAT: File too large\n");
        return -1;
    }
    
    uint32_t cluster = entry.cluster;
    uint32_t bytes_read = 0;
    uint32_t bytes_per_cluster = boot_sector.bytes_per_sector * boot_sector.sectors_per_cluster;
    
    while (cluster != FAT_EOF_ENTRY && bytes_read < entry.size) {
        uint32_t sector = boot_sector.reserved_sectors +
                         boot_sector.fat_count * boot_sector.sectors_per_fat +
                         boot_sector.root_entries * 32 / FAT_SECTOR_SIZE +
                         (cluster - 2) * boot_sector.sectors_per_cluster;
        
        uint32_t to_read = bytes_per_cluster;
        if (bytes_read + to_read > entry.size) {
            to_read = entry.size - bytes_read;
        }
        
        for (int i = 0; i < boot_sector.sectors_per_cluster && bytes_read < entry.size; i++) {
            if (disk_read(sector + i, 1, buffer + bytes_read) != 0) {
                return -1;
            }
            bytes_read += FAT_SECTOR_SIZE;
        }
        
        // Get next cluster
        cluster = fat_table[cluster];
    }
    
    return bytes_read;
}

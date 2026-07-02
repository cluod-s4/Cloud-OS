// core/kernel/drivers/disk.c
// Real disk driver

#include "disk.h"
#include "drivers/screen.h"
#include <stdint.h>

#define DISK_PORT 0x1F0
#define DISK_STATUS 0x1F7
#define DISK_COMMAND 0x1F7

void disk_init() {
    screen_print("[+] Disk driver initialized\n");
}

int disk_read(uint32_t sector, uint32_t count, uint8_t* buffer) {
    // Wait for disk ready
    while (!(inb(DISK_STATUS) & 0x40));
    
    // Send read command
    outb(0x1F6, 0xE0 | ((sector >> 24) & 0x0F));
    outb(0x1F2, count);
    outb(0x1F3, sector & 0xFF);
    outb(0x1F4, (sector >> 8) & 0xFF);
    outb(0x1F5, (sector >> 16) & 0xFF);
    outb(0x1F7, 0x20);
    
    // Read data
    for (int i = 0; i < count; i++) {
        while (!(inb(DISK_STATUS) & 0x08));
        for (int j = 0; j < 256; j++) {
            *((uint16_t*)buffer + j) = inw(DISK_PORT);
        }
        buffer += 512;
    }
    
    return 0;
}

int disk_write(uint32_t sector, uint32_t count, uint8_t* buffer) {
    // Wait for disk ready
    while (!(inb(DISK_STATUS) & 0x40));
    
    // Send write command
    outb(0x1F6, 0xE0 | ((sector >> 24) & 0x0F));
    outb(0x1F2, count);
    outb(0x1F3, sector & 0xFF);
    outb(0x1F4, (sector >> 8) & 0xFF);
    outb(0x1F5, (sector >> 16) & 0xFF);
    outb(0x1F7, 0x30);
    
    // Write data
    for (int i = 0; i < count; i++) {
        while (!(inb(DISK_STATUS) & 0x08));
        for (int j = 0; j < 256; j++) {
            outw(DISK_PORT, *((uint16_t*)buffer + j));
        }
        buffer += 512;
    }
    
    return 0;
}

uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile("inb %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

void outb(uint16_t port, uint8_t value) {
    __asm__ volatile("outb %%al, %%dx" : : "a"(value), "d"(port));
}

uint16_t inw(uint16_t port) {
    uint16_t result;
    __asm__ volatile("inw %%dx, %%ax" : "=a"(result) : "d"(port));
    return result;
}

void outw(uint16_t port, uint16_t value) {
    __asm__ volatile("outw %%ax, %%dx" : : "a"(value), "d"(port));
}

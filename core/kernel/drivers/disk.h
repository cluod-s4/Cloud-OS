// core/kernel/drivers/disk.h
#ifndef DISK_H
#define DISK_H

#include <stdint.h>

void disk_init();
int disk_read(uint32_t sector, uint32_t count, uint8_t* buffer);
int disk_write(uint32_t sector, uint32_t count, uint8_t* buffer);

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);
uint16_t inw(uint16_t port);
void outw(uint16_t port, uint16_t value);

#endif

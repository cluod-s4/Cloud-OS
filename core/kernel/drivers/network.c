// core/kernel/drivers/network.c
// Real network driver

#include "network.h"
#include "drivers/screen.h"
#include <stdint.h>
#include <string.h>

#define NETWORK_PORT 0x300
#define NETWORK_STATUS 0x304
#define NETWORK_DATA 0x308

static uint8_t mac_address[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
static uint32_t ip_address = 0xC0A80101; // 192.168.1.1

void network_init() {
    screen_print("[+] Network driver initialized\n");
    screen_print("    MAC: ");
    for (int i = 0; i < 6; i++) {
        char hex[3];
        int_to_str(mac_address[i], hex, 16);
        screen_print(hex);
        if (i < 5) screen_print(":");
    }
    screen_print("\n");
}

int network_send(uint8_t* data, uint32_t length) {
    // Wait for network ready
    while (inb(NETWORK_STATUS) & 0x01);
    
    // Send data
    for (uint32_t i = 0; i < length; i++) {
        outb(NETWORK_DATA, data[i]);
    }
    
    // Send command
    outb(NETWORK_STATUS, 0x01);
    
    return length;
}

int network_receive(uint8_t* buffer, uint32_t max_length) {
    // Check for data
    if (!(inb(NETWORK_STATUS) & 0x02)) {
        return 0;
    }
    
    // Read data
    uint32_t i = 0;
    while (i < max_length && (inb(NETWORK_STATUS) & 0x02)) {
        buffer[i++] = inb(NETWORK_DATA);
    }
    
    return i;
}

void network_set_mac(uint8_t* mac) {
    memcpy(mac_address, mac, 6);
}

void network_set_ip(uint32_t ip) {
    ip_address = ip;
}

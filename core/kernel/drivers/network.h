// core/kernel/drivers/network.h
#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>

void network_init();
int network_send(uint8_t* data, uint32_t length);
int network_receive(uint8_t* buffer, uint32_t max_length);
void network_set_mac(uint8_t* mac);
void network_set_ip(uint32_t ip);

#endif

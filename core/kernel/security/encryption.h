// core/kernel/security/encryption.h
#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <stdint.h>

void encryption_init();
void aes_encrypt_block(uint8_t* input, uint8_t* output, uint8_t* key);
void aes_decrypt_block(uint8_t* input, uint8_t* output, uint8_t* key);

#endif

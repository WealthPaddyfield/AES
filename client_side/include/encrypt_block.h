#pragma once
#include "aes_key_config.h"

void aes_encrypt_block(const unsigned char in[16], unsigned char out[16],
                       const unsigned char w[AES_WORDS][4]);

/*
void aes_decrypt_block(const unsigned char in[16], unsigned char out[16],
                       const unsigned char w[AES_WORDS][4]);
*/

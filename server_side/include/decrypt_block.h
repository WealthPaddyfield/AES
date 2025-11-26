#pragma once

#include "../include/aes_key_config.h"

void aes_decrypt_block(const unsigned char input[16], unsigned char output[16], unsigned char round_keys[AES_WORDS][4]);
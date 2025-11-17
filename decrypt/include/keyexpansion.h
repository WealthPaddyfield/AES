#pragma once

#include "aes_key_config.h"

void keyexpansion(unsigned char key[4][4], unsigned char w[AES_WORDS][4]);

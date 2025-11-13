#pragma once
#include "aes_key_config.h"

void get_roundkey(unsigned char roundkey[4][4], unsigned char w[44][4],
                  int round);

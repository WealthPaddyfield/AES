#pragma once
#include "aes_key_config.h"

int aes_encrypt_file(char *infile, char *outfile,
                     unsigned char w[AES_WORDS][4]);

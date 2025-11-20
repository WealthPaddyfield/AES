#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../include/addroundkey.h"
#include "../include/inv_shiftrows.h"
#include "../include/inv_subbytes.h"
#include "../include/sbox.h"
#include "../include/keyexpansion.h"
#include "../include/inv_mixcolumns.h"
#include "../include/getroundkey.h"
#include "../include/aes_key_config.h"
#include "../include/decrypt_block.h"

/*
unsigned char input[16] = {0x52, 0x6d, 0xe1, 0xdd, 0xcc, 0xab, 0xac, 0xb7,
                             0xea, 0x94, 0x4e, 0x42, 0x1b, 0x15, 0x85, 0xe6};
*/

//192bit
unsigned char input[16] = {0x8a, 0xb4, 0x90, 0x3b, 0xf3, 0x7a, 0x11, 0x35,
                             0xb6, 0xce, 0x1e, 0xc6, 0xe7, 0xb8, 0xb9, 0x70};

// 初期鍵
/*
unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                         0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
*/

unsigned char key[24] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                          0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
                         0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};

unsigned char output[16];

void print_block(const char *label, const unsigned char *data) {
  printf("%s: ", label);
  for (int i = 0; i < 16; i++) {
    printf("%02x ", data[i]);
  }
  printf("\n");
}

static unsigned char round_keys[AES_WORDS][4];

void aes_setkey(unsigned char *key) {
  unsigned char key_matrix[4][AES_NK];
  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      key_matrix[row][col] = key[col * 4 + row];
    }
  }
  keyexpansion(key_matrix, round_keys);
}

int main() {

  aes_setkey(key);
  aes_decrypt_block(input, output, round_keys);
  print_block("decrypted", output);

    return 0;
}
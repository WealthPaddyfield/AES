#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/PKCS#7.h"
#include "../include/addroundkey.h"
#include "../include/encrypt_block.h"
#include "../include/getroundkey.h"
#include "../include/keyexpansion.h"
#include "../include/mixcolumns.h"
#include "../include/sbox.h"
#include "../include/shiftrows.h"
#include "../include/subbytes.h"

unsigned char input[16] = {0xcb, 0xa9, 0xdc, 0x00, 0x3f, 0xc8, 0xfa, 0xdd,
                           0xb8, 0xc8, 0x0e, 0x30, 0x88, 0xd3, 0x56, 0x51};

// 初期鍵 128bit
unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                         0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

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
  for (int col = 0; col < AES_NK; col++) {
    for (int row = 0; row < 4; row++) {
      key_matrix[row][col] = key[col * 4 + row];
    }
  }
  keyexpansion(key_matrix, round_keys);
}

int main() {
  aes_setkey(key);
  aes_encrypt_block(input, output, round_keys);
  print_block("encrypted", output);
}

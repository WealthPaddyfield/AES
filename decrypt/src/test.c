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

// 暗号化されたデータ
unsigned char input[16] = {0x52, 0x6d, 0xe1, 0xdd, 0xcc, 0xab, 0xac, 0xb7,
                             0xea, 0x94, 0x4e, 0x42, 0x1b, 0x15, 0x85, 0xe6};

// 初期鍵
unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                         0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

void print_block(const char *label, const unsigned char *data) {
  printf("%s: ", label);
  for (int i = 0; i < 16; i++) {
    printf("%02x ", data[i]);
  }
  printf("\n");
}

static unsigned char round_keys[44][4];

void aes_setkey(unsigned char *key) {
  unsigned char key_matrix[4][4];
  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      key_matrix[row][col] = key[col * 4 + row];
    }
  }
  keyexpansion(key_matrix, round_keys);
}

int main() {

  unsigned char state[4][4];
  unsigned char round_key[4][4];
  unsigned char output[16];

  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      state[row][col] = input[col * 4 + row];
    }
  }

  aes_setkey(key);

  get_roundkey(round_key, round_keys, 10);
  addroundkey(state, round_key);

  for (int round = 9; round >= 1; --round) {

    inv_shiftrows(state);

    inv_subbytes(state);

    get_roundkey(round_key, round_keys, round);
    addroundkey(state, round_key);

    for (int c = 0; c < 4; c++) {
      inv_mixcolumns(state, c);
    }
  }

  inv_shiftrows(state);

  inv_subbytes(state);

  get_roundkey(round_key, round_keys, 0);
  addroundkey(state, round_key);

  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      output[col * 4 + row] = state[row][col];
    }
  }

  print_block("plain", output);

  return 0;
}
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/PKCS#7.h"
#include "../include/addroundkey.h"
#include "../include/getroundkey.h"
#include "../include/keyexpansion.h"
#include "../include/mixcolumns.h"
#include "../include/sbox.h"
#include "../include/shiftrows.h"
#include "../include/subbytes.h"

// 初期状態（例として16バイトのデータ）
unsigned char input[16] = {0xcb, 0xa9, 0xdc, 0x00, 0x3f, 0xc8, 0xfa, 0xdd,
                           0xb8, 0xc8, 0x0e, 0x30, 0x88, 0xd3, 0x56, 0x51};

// 初期鍵 128bit
unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                         0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

// 192bit
// unsigned char key[24] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
//                          0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
//                          0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};

// 256bit
//  unsigned char key[32] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
//                           0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
//                           0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
//                           0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

void print_state(unsigned char state[4][4]) {
  printf("State:\n");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("%02x ", state[i][j]);
    }
    printf("\n");
  }
}

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

  unsigned char state[4][4];
  unsigned char round_key[4][4];
  unsigned char output[16];

  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      state[row][col] = input[col * 4 + row];
    }
  }

  aes_setkey(key);

  get_roundkey(round_key, round_keys, 0);
  addroundkey(state, round_key);

  for (int round = 1; round < AES_NR; round++) {

    for (int k = 0; k < 4; k++) {
      for (int l = 0; l < 4; l++) {
        state[k][l] = subbyte(state[k][l]);
      }
    }

    shiftrows(state);

    mixcolumns(state);

    get_roundkey(round_key, round_keys, round);
    addroundkey(state, round_key);
  }

  for (int k = 0; k < 4; k++) {
    for (int l = 0; l < 4; l++) {
      state[k][l] = subbyte(state[k][l]);
    }
  }

  shiftrows(state);

  get_roundkey(round_key, round_keys, AES_NR);
  addroundkey(state, round_key);

  // 出力をoutputに展開
  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      output[col * 4 + row] = state[row][col];
    }
  }
  print_block("encrypted", output);

  return 0;
}

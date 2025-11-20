#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/addroundkey.h"
#include "../include/getroundkey.h"
// #include "../include/keyexpansion.h"
#include "../include/mixcolumns.h"
// #include "../include/sbox.h"
#include "../include/shiftrows.h"
#include "../include/subbytes.h"

void aes_encrypt_block(const unsigned char input[16], unsigned char output[16],
                       const unsigned char round_keys[AES_WORDS][4]) {

  unsigned char state[4][4];
  unsigned char round_key[4][4];

  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      state[row][col] = input[col * 4 + row];
    }
  }

  get_roundkey(round_key, (unsigned char (*)[4])round_keys, 0);
  addroundkey(state, round_key);

  for (int round = 1; round < AES_NR; round++) {

    for (int k = 0; k < 4; k++) {
      for (int l = 0; l < 4; l++) {
        state[k][l] = subbyte(state[k][l]);
      }
    }

    shiftrows(state);

    mixcolumns(state);

    get_roundkey(round_key, (unsigned char (*)[4])round_keys, round);
    addroundkey(state, round_key);
  }

  for (int k = 0; k < 4; k++) {
    for (int l = 0; l < 4; l++) {
      state[k][l] = subbyte(state[k][l]);
    }
  }

  shiftrows(state);

  get_roundkey(round_key, (unsigned char (*)[4])round_keys, AES_NR);
  addroundkey(state, round_key);

  // 出力をoutputに展開
  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      output[col * 4 + row] = state[row][col];
    }
  }
}

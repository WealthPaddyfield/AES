#include "../include/getroundkey.h"
#include "../include/aes_key_config.h"

void get_roundkey(unsigned char roundkey[4][4], unsigned char w[AES_WORDS][4],
                  int round) {
  for (int col = 0; col < AES_NB; col++) {
    for (int row = 0; row < 4; row++) {
      roundkey[row][col] = w[round * AES_NB + col][row];
    }
  }
}
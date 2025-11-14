#include "../include/keyexpansion.h"
#include "../include/aes_key_config.h"
#include "../include/sbox.h"
#include "../include/subbytes.h"

void function_g(unsigned char byte[4], int counter) {

  const unsigned char Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10,
                                  0x20, 0x40, 0x80, 0x1B, 0x36};

  unsigned char tmp;

  tmp = byte[0];
  byte[0] = byte[1];
  byte[1] = byte[2];
  byte[2] = byte[3];
  byte[3] = tmp;

  byte[0] = subbyte(byte[0]);
  byte[1] = subbyte(byte[1]);
  byte[2] = subbyte(byte[2]);
  byte[3] = subbyte(byte[3]);

  byte[0] ^= Rcon[counter];
}

void keyexpansion(unsigned char key[4][AES_NK], unsigned char w[AES_WORDS][4]) {

  int i, j;

  for (i = 0; i < AES_NK; ++i)
    for (j = 0; j < 4; ++j)
      w[i][j] = key[j][i];

  unsigned char temp[4];
  i = AES_NK;
  while (i < AES_WORDS) {
    for (j = 0; j < 4; ++j)
      temp[j] = w[i - 1][j];

    if (i % AES_NK == 0) {
      function_g(temp, (i / AES_NK) - 1);
    } else if (AES_NK == 8 && (i % AES_NK) == 4) {
      // AES-256 のみの追加処理（SubWord）
      for (j = 0; j < 4; ++j) {
        temp[j] = sbox[temp[j] >> 4][temp[j] & 0x0F];
      }
    }

    for (j = 0; j < 4; ++j)
      w[i][j] = w[i - AES_NK][j] ^ temp[j];
    ++i;
  }
}

/*
void keyexpansion(unsigned char key[4][4], unsigned char w[44][4]) {

  // 初期鍵をW[0]〜W[3]にコピー（列単位）
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      w[i][j] = key[j][i];
    }
  }

  unsigned char temp[4];
  int i = 4;
  while (i < 44) {
    for (int j = 0; j < 4; j++) {
      temp[j] = w[i - 1][j];
    }

    if (i % 4 == 0) {
      function_g(temp, (i / 4) - 1);
    }

    for (int j = 0; j < 4; j++) {
      w[i][j] = w[i - 4][j] ^ temp[j];
    }
    i++;
  }
}
*/

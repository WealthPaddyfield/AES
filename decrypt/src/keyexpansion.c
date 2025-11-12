#include "../include/keyexpansion.h"
#include "../include/sbox.h"
#include "../include/inv_subbytes.h"

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

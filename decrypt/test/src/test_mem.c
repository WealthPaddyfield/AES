#include "../include/aes_key_config.h"
#include "../include/decrypt_block.h"
#include "../include/encrypt_block.h" // encrypt_block, decrypt_block, setkey 等
#include "../include/keyexpansion.h"
#include <stdio.h>
#include <string.h>

unsigned char round_keys[AES_WORDS][4];

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
  unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                           0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  unsigned char plain[16] = {0}; /* 任意のテストデータ */
  unsigned char c[16], p[16];

  aes_setkey(key);
  aes_encrypt_block(plain, c, round_keys);
  aes_decrypt_block(c, p, round_keys);

  if (memcmp(plain, p, 16) == 0)
    printf("OK: encrypt->decrypt matched\n");
  else {
    printf("NG: mismatch\n");
    for (int i = 0; i < 16; i++)
      printf("%02x ", p[i]);
    printf("\n");
  }
  return 0;
}

#include <stdio.h>
#include <string.h>

unsigned char in_state[4][4] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

void to_binary(unsigned char n) {
  for (int i = 7; i >= 0; i--) {
    int bit = (n >> i) & 1; // iビット目を抽出
    printf("%x", bit);
  }
  printf("\n");
}

void put(unsigned char state[4][4]) {
  for (int k = 0; k < 4; k++) {
    for (int l = 0; l < 4; l++) {
      to_binary(state[k][l]);
    }
  }
}

int main() {
  put(in_state);
  // to_binary(0x01); // 出力: 00000101
  return 0;
}
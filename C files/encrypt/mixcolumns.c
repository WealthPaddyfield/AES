#include <stdint.h>
#include <stdio.h>

unsigned char in_state[4][4] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

void print_state(unsigned char state[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("%02X ", state[i][j]);
    }
    printf("\n");
  }
}

unsigned char gmul2(unsigned char x) {
  return (x << 1) ^ ((x & 0x80) ? 0x1b : 0x00);
}

unsigned char gmul3(unsigned char x) { return gmul2(x) ^ x; }

void mixcolumns(unsigned char state[4][4]) {
  unsigned char t[4]; // 一時保存用

  for (int c = 0; c < 4; c++) {
    unsigned char s0 = state[0][c];
    unsigned char s1 = state[1][c];
    unsigned char s2 = state[2][c];
    unsigned char s3 = state[3][c];

    t[0] = gmul2(s0) ^ gmul3(s1) ^ s2 ^ s3;
    t[1] = s0 ^ gmul2(s1) ^ gmul3(s2) ^ s3;
    t[2] = s0 ^ s1 ^ gmul2(s2) ^ gmul3(s3);
    t[3] = gmul3(s0) ^ s1 ^ s2 ^ gmul2(s3);

    state[0][c] = t[0];
    state[1][c] = t[1];
    state[2][c] = t[2];
    state[3][c] = t[3];
  }
}

// for test
int main() {
  mixcolumns(in_state);
  print_state(in_state);
}

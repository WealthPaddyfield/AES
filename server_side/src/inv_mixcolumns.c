#include "../include/inv_mixcolumns.h"

static unsigned char xtime(unsigned char x) {
  return (unsigned char)((x << 1) ^ ((x & 0x80) ? 0x1B : 0x00));
}

static unsigned char mul(unsigned char x, unsigned char y) {
  unsigned char r = 0;
  while (y) {
    if (y & 1)
      r ^= x;
    x = xtime(x);
    y >>= 1;
  }
  return r;
}

/* 修正: state と列 index を受け取ってその列を逆MixColumnsする */
void inv_mixcolumns(unsigned char state[4][4], int col) {
  unsigned char a0 = state[0][col];
  unsigned char a1 = state[1][col];
  unsigned char a2 = state[2][col];
  unsigned char a3 = state[3][col];

  unsigned char r0 =
      mul(a0, 0x0E) ^ mul(a1, 0x0B) ^ mul(a2, 0x0D) ^ mul(a3, 0x09);
  unsigned char r1 =
      mul(a0, 0x09) ^ mul(a1, 0x0E) ^ mul(a2, 0x0B) ^ mul(a3, 0x0D);
  unsigned char r2 =
      mul(a0, 0x0D) ^ mul(a1, 0x09) ^ mul(a2, 0x0E) ^ mul(a3, 0x0B);
  unsigned char r3 =
      mul(a0, 0x0B) ^ mul(a1, 0x0D) ^ mul(a2, 0x09) ^ mul(a3, 0x0E);

  state[0][col] = r0;
  state[1][col] = r1;
  state[2][col] = r2;
  state[3][col] = r3;
}
#include "../include/inv_subbytes.h"
#include "../include/sbox.h"

unsigned char subbyte(unsigned char in) {
  unsigned char upper = in >> 4;
  unsigned char lower = in & 0x0F;
  return sbox[upper][lower];
}

unsigned char inv_subbyte(unsigned char in) {
  unsigned char upper = in >> 4;
  unsigned char lower = in & 0x0F;
  return inv_sbox[upper][lower];
}

// for 4×4 state
void inv_subbytes(unsigned char state[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[i][j] = inv_subbyte(state[i][j]);
    }
  }
}

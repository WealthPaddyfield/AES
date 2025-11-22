#include "../include/subbytes.h"
#include "../include/sbox.h"

unsigned char subbyte(unsigned char in) {
  unsigned char upper = in >> 4;
  unsigned char lower = in & 0x0F;
  return sbox[upper][lower];
}

// for 4×4 state
void subbytes(unsigned char state[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[i][j] = subbyte(state[i][j]);
    }
  }
}

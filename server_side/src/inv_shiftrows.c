#include "../include/inv_shiftrows.h"

void inv_shiftrows(unsigned char state[4][4]) {
  unsigned char temp;

  // Shift row 1
  temp = state[1][0];
  state[1][0] = state[1][3];
  state[1][3] = state[1][2];
  state[1][2] = state[1][1];
  state[1][1] = temp;

  // Shift row 2
  temp = state[2][0];
  state[2][0] = state[2][2];
  state[2][2] = temp;
  temp = state[2][1];
  state[2][1] = state[2][3];
  state[2][3] = temp;

  // Shift row 3
  temp = state[3][0];
  state[3][0] = state[3][1];
  state[3][1] = state[3][2];
  state[3][2] = state[3][3];
  state[3][3] = temp;
}
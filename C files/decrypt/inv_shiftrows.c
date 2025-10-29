#include <stdio.h>
#include <string.h>

unsigned char state[4][4] = {0xA0, 0xA1, 0xA2, 0xA3, 0xB1, 0xB2, 0xB3, 0xB0,
                             0xC2, 0xC3, 0xC0, 0xC1, 0xD3, 0xD0, 0xD1, 0xD2};

void print_state(unsigned char state[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("%02X ", state[i][j]);
    }
    printf("\n");
  }
}

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

int main() {
  inv_shiftrows(state);
  print_state(state);
}

#include "../include/getroundkey.h"

void get_roundkey(unsigned char roundkey[4][4], unsigned char w[44][4], int round) {
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            roundkey[row][col] = w[round * 4 + col][row];
        }
    }
}
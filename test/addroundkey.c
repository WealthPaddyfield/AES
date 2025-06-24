#include <stdio.h>
#include <string.h>

//for test
unsigned char in_state [4] [4] = {
    0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F
};

unsigned char roundkey[4][4] ={
    0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F
};

unsigned char roundkey_two[4][4] ={
    0xFF, 0x01, 0xF2, 0x03,
    0x0D, 0x05, 0x16, 0x07,
    0x08, 0x25, 0x0A, 0x88,
    0x0C, 0x10, 0x9E, 0x00
};


void print_state(unsigned char state[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%02X ", state[i][j]);
        }
        printf("\n");
    }
}

void addroundkey(unsigned char state[4][4],unsigned char roundkey[4][4]){
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] ^= roundkey[i][j];
        }
    }
}

int main(){
    addroundkey(in_state,roundkey_two);
    print_state(in_state);
}
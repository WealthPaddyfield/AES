#include <stdio.h>
#include <stdint.h>

unsigned char in_state [4] [4] = {
    0x03, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F
};

void print_state(unsigned char state[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%02X ", state[i][j]);
        }
        printf("\n");
    }
}

void mixcolumns(unsigned char state[4][4]) {

    state[0][0] = (state[0][0] * 0x02) ^ (state[0][0] * 0x03) ^ state[0][0] ^ state[0][0];  
    state[1][0] = state[1][0] ^ (state[1][0] * 0x02) ^ (state[1][0] * 0x03) ^ state[1][0];
    state[2][0] = state[2][0] ^ state[2][0] ^ (state[2][0] * 0x02) ^ (state[2][0] * 0x03);
    state[3][0] = (state[3][0] * 0x03) ^ state[3][0] ^ state[3][0] ^ (state[3][0] * 0x02); 

    state[0][1] = (state[0][1] * 0x02) ^ (state[0][1] * 0x03) ^ state[0][1] ^ state[0][1]; 
    state[1][1] = state[1][1] ^ (state[1][1] * 0x02) ^ (state[1][1] * 0x03) ^ state[1][1];
    state[2][1] = state[2][1] ^ state[2][1] ^ (state[2][1] * 0x02) ^ (state[2][1] * 0x03);
    state[3][1] = (state[3][1] * 0x03) ^ state[3][1] ^ state[3][1] ^ (state[3][1] * 0x02); 

    state[0][2] = (state[0][2] * 0x02) ^ (state[0][2] * 0x03) ^ state[0][2] ^ state[0][1]; 
    state[1][2] = state[1][2] ^ (state[1][2] * 0x02) ^ (state[1][2] * 0x03) ^ state[1][2];
    state[2][2] = state[2][2] ^ state[2][2] ^ (state[2][2] * 0x02) ^ (state[2][2] * 0x03);
    state[3][2] = (state[3][2] * 0x03) ^ state[3][2] ^ state[3][2] ^ (state[3][2] * 0x02); 

    state[0][3] = (state[0][3] * 0x02) ^ (state[0][3] * 0x03) ^ state[0][3] ^ state[0][3]; 
    state[1][3] = state[1][3] ^ (state[1][3] * 0x02) ^ (state[1][3] * 0x03) ^ state[1][3];
    state[2][3] = state[2][3] ^ state[2][3] ^ (state[2][3] * 0x02) ^ (state[2][3] * 0x03);
    state[3][3] = (state[3][3] * 0x03) ^ state[3][3] ^ state[3][3] ^ (state[3][3] * 0x02); 
}

//for test
int main(){
    mixcolumns(in_state);
    print_state(in_state);
}
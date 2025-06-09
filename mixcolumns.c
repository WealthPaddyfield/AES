#include <stdio.h>
#include <stdint.h>

unsigned char in_state [4] [4] = {
    0x00, 0x01, 0x02, 0x03,
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

int mixcolumns(unsigned char state[4][4]) {
    unsigned char out_a[4][0],out_b[4][0],out_c[4][0],out_d[4][0];
    const unsigned char matrix[4][4] = {
       0x02,0x03,0x01,0x01,
       0x01,0x02,0x03,0x01,
       0x01,0x01,0x02,0x03,
       0x03,0x01,0x01,0x02
    };

    unsigned char a,b,c,d;
    unsigned char cal;

    for (int i,j = 0; i < 4; i++,j++){
        a = state[i][0] * matrix[i][0];
        b = state[i][0] * matrix[i][1];
        c = state[i][0] * matrix[i][2];
        d = state[i][0] * matrix[i][3];

        cal = a ^ b ^ c ^ d;
        cal = out_a[0][0];
    }

    return 0;    
}

//for test
int main(){
    unsigned char out = mixcolumns(in_state);
    print_state(out);
}
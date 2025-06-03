#include <stdio.h>
#include <stdint.h>

/*unsigned char in_state [4] [4] = {
    0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F
};*/

void print_state(unsigned char state[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%02X ", state[i][j]);
        }
        printf("\n");
    }
}

int mixcolumns(unsigned char state[4][4]) {
    unsigned char temp[4][4];
    const unsigned char matrix[4][4] = {
       0x02,0x03,0x01,0x01,
       0x01,0x02,0x03,0x01,
       0x01,0x01,0x02,0x03,
       0x03,0x01,0x01,0x02
    };

    

    return 0;    

}


int main(){
    mixcolumns(state);
    print_state(state);
}
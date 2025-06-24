#ifndef _H_mixcolumns_
#define _H_mixcolumns_

#include <stdio.h>
#include <stdint.h>

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

#endif //_H_mixcolumns_
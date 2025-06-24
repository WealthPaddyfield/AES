#ifndef _H_addroundkey_
#define _H_addroundkey_

#include <stdio.h>
#include <string.h>

void addroundkey(unsigned char state[4][4],unsigned char roundkey[4][4]){
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] ^= roundkey[i][j];
        }
    }
}

#endif //_H_addroundkey_
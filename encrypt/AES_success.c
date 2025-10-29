// Content from main.c
#include <stdint.h>
#include <stdio.h>

// 初期状態（例として16バイトのデータ）
/*
hnsigned char state[4][4] = {
    {0x32, 0x88, 0x31, 0xe0},
    {0x43, 0x5a, 0x31, 0x37},
    {0xf6, 0x30, 0x98, 0x07},
    {0xa8, 0x8d, 0xa2, 0x34}
};
*/

// 3243f6a8885a308d313198a2e0370734

// 初期鍵
/*
unsigned char key[4][4] = {
    {0x2b, 0x28, 0xab, 0x09},
    {0x7e, 0xae, 0xf7, 0xcf},
    {0x15, 0xd2, 0x15, 0x4f},
    {0x16, 0xa6, 0x88, 0x3c}
};
*/
static uint8_t input_save[16];

unsigned char subbytes(unsigned char in);
void subbytes_all(unsigned char state[4][4]) {
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      state[i][j] = subbytes(state[i][j]);
}

void print_state(unsigned char state[4][4]) {
  printf("State:\n");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("%02x ", state[i][j]);
    }
    printf("\n");
  }
}

void get_roundkey(unsigned char roundkey[4][4], unsigned char w[44][4],
                  int round) {
  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      roundkey[row][col] = w[round * 4 + col][row];
    }
  }
}

void keyexpantion(unsigned char key[4][4], unsigned char w[44][4]);

void addroundkey(unsigned char state[4][4], unsigned char roundkey[4][4]);

void shiftrows(unsigned char state[4][4]);

void mixcolumns(unsigned char state[4][4]);

/*
int main() {
    unsigned char w[44][4];
    unsigned char roundkey[4][4];

    keyexpantion(key, w);  // ラウンドキー生成

    printf("=== AES 128-bit 10 Rounds ===\n");
    printf("\nInitial State:\n");
    print_state(state);

    // Round 0: 初期 AddRoundKey
    get_roundkey(roundkey, w, 0);
    printf("\n[Round 0] AddRoundKey:\n");
    addroundkey(state, roundkey);
    print_state(state);

    // Rounds 1〜9
    for (int round = 1; round < 10; round++) {
        printf("\n[Round %d] SubBytes:\n", round);
        subbytes_all(state);
        print_state(state);

        printf("[Round %d] ShiftRows:\n", round);
        shiftrows(state);
        print_state(state);

        printf("[Round %d] MixColumns:\n", round);
        mixcolumns(state);
        print_state(state);

        get_roundkey(roundkey, w, round);
        printf("[Round %d] AddRoundKey:\n", round);
        addroundkey(state, roundkey);
        print_state(state);
    }

    // Round 10（最後のラウンドはMixColumnsなし）
    printf("\n[Round 10] SubBytes:\n");
    subbytes_all(state);
    print_state(state);

    printf("[Round 10] ShiftRows:\n");
    shiftrows(state);
    print_state(state);

    get_roundkey(roundkey, w, 10);
    printf("[Round 10] AddRoundKey:\n");
    addroundkey(state, roundkey);
    print_state(state);

    printf("\n=== AES Encryption Completed ===\n");

    return 0;
}
*/

// Content from addroundkey.c

// #include <stdio.h>
// #include <string.h>

// for test
/*
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
*/

/*
void addroundkey(unsigned char state[4][4],unsigned char roundkey[4][4]){
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] ^= roundkey[i][j];
        }
    }
}
*/

/*
void print_state(unsigned char state[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%02X ", state[i][j]);
        }
        printf("\n");
    }
}
*/

void addroundkey(unsigned char state[4][4], unsigned char roundkey[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[i][j] ^= roundkey[i][j];
    }
  }
}

/*
int main(){
    addroundkey(in_state,roundkey_two);
    print_state(in_state);
}
*/

// Content from keyexpantion.c
// #include "keyexpantion.h"
// #include "subbytes.h"

// #include <stdio.h>

const unsigned char sbox[][16] = {
    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b,
     0xfe, 0xd7, 0xab, 0x76},
    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf,
     0x9c, 0xa4, 0x72, 0xc0},
    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1,
     0x71, 0xd8, 0x31, 0x15},
    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2,
     0xeb, 0x27, 0xb2, 0x75},
    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3,
     0x29, 0xe3, 0x2f, 0x84},
    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39,
     0x4a, 0x4c, 0x58, 0xcf},
    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f,
     0x50, 0x3c, 0x9f, 0xa8},
    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21,
     0x10, 0xff, 0xf3, 0xd2},
    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d,
     0x64, 0x5d, 0x19, 0x73},
    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14,
     0xde, 0x5e, 0x0b, 0xdb},
    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62,
     0x91, 0x95, 0xe4, 0x79},
    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea,
     0x65, 0x7a, 0xae, 0x08},
    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f,
     0x4b, 0xbd, 0x8b, 0x8a},
    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9,
     0x86, 0xc1, 0x1d, 0x9e},
    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9,
     0xce, 0x55, 0x28, 0xdf},
    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f,
     0xb0, 0x54, 0xbb, 0x16}};

void function_g(unsigned char byte[4], int counter) {

  const unsigned char Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10,
                                  0x20, 0x40, 0x80, 0x1B, 0x36};

  unsigned char tmp;

  tmp = byte[0];
  byte[0] = byte[1];
  byte[1] = byte[2];
  byte[2] = byte[3];
  byte[3] = tmp;

  byte[0] = subbytes(byte[0]);
  byte[1] = subbytes(byte[1]);
  byte[2] = subbytes(byte[2]);
  byte[3] = subbytes(byte[3]);

  byte[0] ^= Rcon[counter];
}

/*void xor_calc(unsigned char first_word[4], unsigned char second_word[4],
              unsigned char result[4]) {

  result[0] = first_word[0] ^ second_word[0];
  result[1] = first_word[1] ^ second_word[1];
  result[2] = first_word[2] ^ second_word[2];
  result[3] = first_word[3] ^ second_word[3];
}*/

void keyexpantion(unsigned char key[4][4], unsigned char w[44][4]) {

  // 初期鍵をW[0]〜W[3]にコピー（列単位）
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      w[i][j] = key[j][i];
    }
  }

  unsigned char temp[4];
  int i = 4;
  while (i < 44) {
    for (int j = 0; j < 4; j++) {
      temp[j] = w[i - 1][j];
    }

    if (i % 4 == 0) {
      function_g(temp, (i / 4) - 1);
    }

    for (int j = 0; j < 4; j++) {
      w[i][j] = w[i - 4][j] ^ temp[j];
    }
    i++;
  }
}

/*  for (int round = 0; round < 11; round++) {
    printf("Round %d key:\n", round);
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        printf("%02X ", w[round * 4 + col][row]);
      }
      printf("\n");
    }
    printf("\n");
  }
}

int main() {
keyexpantion((unsigned char (*)[4])key);
return 0;
}
*/

// Content from mixcolumns.c

unsigned char gmul2(unsigned char x) {
  return (x << 1) ^ ((x & 0x80) ? 0x1b : 0x00);
}

unsigned char gmul3(unsigned char x) { return gmul2(x) ^ x; }

void mixcolumns(unsigned char state[4][4]) {
  unsigned char t[4]; // 一時保存用

  for (int c = 0; c < 4; c++) {
    unsigned char s0 = state[0][c];
    unsigned char s1 = state[1][c];
    unsigned char s2 = state[2][c];
    unsigned char s3 = state[3][c];

    t[0] = gmul2(s0) ^ gmul3(s1) ^ s2 ^ s3;
    t[1] = s0 ^ gmul2(s1) ^ gmul3(s2) ^ s3;
    t[2] = s0 ^ s1 ^ gmul2(s2) ^ gmul3(s3);
    t[3] = gmul3(s0) ^ s1 ^ s2 ^ gmul2(s3);

    state[0][c] = t[0];
    state[1][c] = t[1];
    state[2][c] = t[2];
    state[3][c] = t[3];
  }
}

// Content from shiftrows.c

/*
unsigned char state [4] [4] = {
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
*/

void shiftrows(unsigned char state[4][4]) {
  unsigned char temp;

  // Shift row 1
  temp = state[1][0];
  state[1][0] = state[1][1];
  state[1][1] = state[1][2];
  state[1][2] = state[1][3];
  state[1][3] = temp;

  // Shift row 2
  temp = state[2][0];
  state[2][0] = state[2][2];
  state[2][2] = temp;
  temp = state[2][1];
  state[2][1] = state[2][3];
  state[2][3] = temp;

  // Shift row 3
  temp = state[3][0];
  state[3][0] = state[3][3];
  state[3][3] = state[3][2];
  state[3][2] = state[3][1];
  state[3][1] = temp;
}

// Content from subbytes.c
// #include <stdio.h>
// #include <string.h>

// #include "subbytes.h"

extern const unsigned char sbox[][16];

unsigned char subbytes(unsigned char in) {
  unsigned char upper, lower;

  upper = in >> 4;
  // lower = (in << 4) >> 4;
  lower = 0x0F & in;
  // printf("%x\n",in);
  // printf("%x\n",upper);
  // printf("%x\n",lower);

  return sbox[upper][lower];
}

/*int main(void){

 printf("%x\n",subbytes(0b01010011));

}*/

// 7月19追記
static unsigned char round_keys[44][4];
// setkey: AES鍵拡張を行い、ラウンドキーを保存
/*
void AES128_ECB_indp_setkey(uint8_t *key) {
    keyexpantion((unsigned char (*)[4])key, round_keys);
}
*/

// 7月21変更
void AES128_ECB_indp_setkey(uint8_t *key) {
  unsigned char key_matrix[4][4];
  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      key_matrix[row][col] = key[col * 4 + row];
    }
  }
  keyexpantion(key_matrix, round_keys);
}

void AES128_ECB_indp_crypto(uint8_t *input) {
  unsigned char state[4][4];
  unsigned char roundkey[4][4];

  // 入力をstateに展開（列単位）
  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      state[row][col] = input[col * 4 + row];
    }
  }

  // 行優先に書き換え7/19(仕様通りじゃないけどうまくいった？)
  /*
  for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
      state[row][col] = input[row * 4 + col];  // row-major
      }
  }
  */

  // Round 0
  get_roundkey(roundkey, round_keys, 0);
  addroundkey(state, roundkey);

  // Round 1〜9
  for (int round = 1; round < 10; round++) {
    subbytes_all(state);
    shiftrows(state);
    mixcolumns(state);
    get_roundkey(roundkey, round_keys, round);
    addroundkey(state, roundkey);
  }

  // Round 10（MixColumnsなし）
  subbytes_all(state);
  shiftrows(state);
  get_roundkey(roundkey, round_keys, 10);
  addroundkey(state, roundkey);

  // 3行目のoutputをinputに変更7/21
  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      input[col * 4 + row] = state[row][col];
    }
  }
}

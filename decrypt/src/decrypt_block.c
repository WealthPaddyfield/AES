
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../include/addroundkey.h"
#include "../include/inv_shiftrows.h"
#include "../include/inv_subbytes.h"
#include "../include/sbox.h"
#include "../include/keyexpansion.h"
#include "../include/inv_mixcolumns.h"
#include "../include/getroundkey.h"
#include "../include/aes_key_config.h"
#include "../include/decrypt_block.h"

void aes_decrypt_block(const unsigned char input[16], unsigned char output[16], unsigned char round_keys[AES_WORDS][4])
{
    unsigned char state[4][4];
    unsigned char round_key[4][4];

    // 入力データを状態行列に変換
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            state[row][col] = input[col * 4 + row];
        }
    }

    // 最初のラウンドキーを追加
    get_roundkey(round_key, round_keys, AES_NR);
    addroundkey(state, round_key);

    // ラウンド処理
    for (int round = AES_NR - 1; round >= 1; --round)
    {
        inv_shiftrows(state);
        inv_subbytes(state);
        get_roundkey(round_key, round_keys, round);
        addroundkey(state, round_key);
        for (int c = 0; c < 4; c++)
        {
            inv_mixcolumns(state, c);
        }
    }

    // 最終ラウンド
    inv_shiftrows(state);
    inv_subbytes(state);
    get_roundkey(round_key, round_keys, 0);
    addroundkey(state, round_key);

    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            output[col * 4 + row] = state[row][col];
        }
    }
}
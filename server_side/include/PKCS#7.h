#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// パディング
unsigned char *pkcs7_pad(const unsigned char *data, size_t data_len,
                         size_t *out_len);

// パディング除去
ssize_t pkcs7_unpad(unsigned char *data, size_t data_len);

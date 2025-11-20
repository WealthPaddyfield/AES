#include <stdio.h>
#include <stdlib.h>

#include "../include/PKCS#7.h"
#include "../include/encrypt_block.h"
#include "../include/encrypt_file.h"

#ifndef ENCRYPT_CHUNK
#define ENCRYPT_CHUNK 4096
#endif

static void pad_last_block(const unsigned char *leftover, size_t leftover_len,
                           unsigned char out_block[16]) {
  const size_t B = 16;
  size_t L = leftover_len;
  if (L > B)
    L = B; // defensive
  if (L > 0)
    memcpy(out_block, leftover, L);
  unsigned char pad = (unsigned char)(B - L);
  for (size_t i = L; i < B; ++i)
    out_block[i] = pad;
}

int aes_encrypt_file(char *inpath, char *outpath,
                     unsigned char w[AES_WORDS][4]) {
  FILE *fin = NULL, *fout = NULL;
  unsigned char *buf = NULL;
  unsigned char leftover[16];
  size_t leftover_len = 0;

  fin = fopen(inpath, "rb");
  if (!fin) {
    perror("fopen input");
    return -1;
  }
  fout = fopen(outpath, "wb");
  if (!fout) {
    perror("fopen output");
    fclose(fin);
    return -1;
  }

  buf = malloc(ENCRYPT_CHUNK);
  if (!buf) {
    perror("malloc");
    fclose(fin);
    fclose(fout);
    return -1;
  }

  while (1) {
    size_t n = fread(buf, 1, ENCRYPT_CHUNK, fin);
    if (n == 0 && feof(fin))
      break;
    if (n == 0 && ferror(fin)) {
      perror("fread");
      free(buf);
      fclose(fin);
      fclose(fout);
      return -1;
    }

    size_t pos = 0;

    // if we have leftover from previous read, try to fill it
    if (leftover_len > 0) {
      size_t need = 16 - leftover_len;
      if (n < need) { // still not enough to make a block
        memcpy(leftover + leftover_len, buf, n);
        leftover_len += n;
        continue;
      } else {
        // fill and encrypt leftover block
        memcpy(leftover + leftover_len, buf, need);
        unsigned char outblk[16];
        aes_encrypt_block(leftover, outblk, w);
        if (fwrite(outblk, 1, 16, fout) != 16) {
          perror("fwrite");
          free(buf);
          fclose(fin);
          fclose(fout);
          return -1;
        }
        pos += need;
        leftover_len = 0;
      }
    }

    // encrypt all full 16-byte blocks in buf
    while (pos + 16 <= n) {
      unsigned char outblk[16];
      aes_encrypt_block(buf + pos, outblk, w);
      if (fwrite(outblk, 1, 16, fout) != 16) {
        perror("fwrite");
        free(buf);
        fclose(fin);
        fclose(fout);
        return -1;
      }
      pos += 16;
    }

    // store remainder as leftover
    leftover_len = n - pos;
    if (leftover_len > 0)
      memcpy(leftover, buf + pos, leftover_len);

    // if EOF reached, break and handle padding below
    if (n < ENCRYPT_CHUNK)
      break;
  }

  // finalize: produce padded last block (even if leftover_len == 0)
  unsigned char final_block[16];
  pad_last_block(leftover, leftover_len, final_block);
  unsigned char outblk[16];
  aes_encrypt_block(final_block, outblk, w);
  if (fwrite(outblk, 1, 16, fout) != 16) {
    perror("fwrite final");
    free(buf);
    fclose(fin);
    fclose(fout);
    return -1;
  }

  free(buf);
  fclose(fin);
  fclose(fout);

  return 0;
}

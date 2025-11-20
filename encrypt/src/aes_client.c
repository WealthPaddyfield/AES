#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/PKCS#7.h"
#include "../include/aes_key_config.h"
#include "../include/encrypt_block.h"
#include "../include/keyexpansion.h"

#define TOTAL_BYTES (4 * 4) // 16
#define BUFSIZE 1024

// unsigned char output[16];

//  鍵拡張
unsigned char round_keys[AES_WORDS][4];

void aes_setkey(unsigned char *key) {
  unsigned char key_matrix[4][AES_NK];
  for (int col = 0; col < AES_NK; col++) {
    for (int row = 0; row < 4; row++) {
      key_matrix[row][col] = key[col * 4 + row];
    }
  }
  keyexpansion(key_matrix, round_keys);
}

int main(int argc, char *argv[]) {
  int sockfd, nbytes;
  struct sockaddr_in servaddr;
  int fd;
  int f_siz;
  unsigned char *file_buf;
  const char *filename = argv[1];
  const char *ip_adress = argv[2];
  unsigned char input[16];
  FILE *file_fp;

  const unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae,
                                 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88,
                                 0x09, 0xcf, 0x4f, 0x3c};

  // file_fp = fopen(filename, "rb");
  // // バイナリをステートに変換しarg[1]のファイルを変更
  // size_t items_read = fread(input, 1, TOTAL_BYTES, file_fp);
  // fclose(file_fp);

  // AES暗号化
  // file_fp = fopen(filename, "rwb");
  // aes_setkey(key);
  // aes_encrypt_file(input, filename, round_keys);

  file_fp = fopen(filename, "rb");
  if (!file_fp) {
    perror("fopen");
    return 1;
  }

  size_t items_read = fread(input, 1, TOTAL_BYTES, file_fp);
  (void)items_read;
  fclose(file_fp);

  if ((fd = open(filename, O_RDONLY)) < 0) {
    perror("open");
    close(sockfd);
    exit(1);
  }

  f_siz = lseek(fd, 0, SEEK_END); // 送信したいファイルのサイズを取得（バイト）
  lseek(fd, 0, SEEK_SET);         // ファイルの読み書き位置を先頭に戻す

  printf("client: transmit file size:%d\n", f_siz);
  // fflush(stdout); exit(0);

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(10000);
  if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) < 0) {
    perror("inet_pton");
    exit(1);
  }

  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("connect");
    exit(1);
  }

  // 暗号化処理をここから
  // ファイルを読み込む
  file_buf = malloc(f_siz);
  if (!file_buf) {
    perror("malloc");
    close(fd);
    close(sockfd);
    return 1;
  }

  if (read(fd, file_buf, f_siz) != f_siz) {
    perror("read");
    free(file_buf);
    close(fd);
    close(sockfd);
    return 1;
  }

  // パディング　今回はちょうど１６バイトの倍数のデータであっても0x10(16)で埋めたブロックを一つケツにつけてます
  // 倍数でない場合(追加する場合)は追加する数のデータで埋める
  // １０バイトのデータなら0x06を10個
  size_t padded_len = 0;
  unsigned char *padded =
      pkcs7_pad((unsigned char *)file_buf, (size_t)f_siz, &padded_len);
  free(file_buf);
  if (!padded) {
    perror("pkcs7_pad");
    close(fd);
    close(sockfd);
    return 1;
  }

  // AESの本質的な処理
  aes_setkey((unsigned char *)key);
  unsigned char *ciphertext = malloc(padded_len);
  if (!ciphertext) {
    perror("malloc");
    free(padded);
    close(fd);
    close(sockfd);
    return 1;
  }

  for (size_t off = 0; off < padded_len; off += TOTAL_BYTES) {
    aes_encrypt_block(padded + off, ciphertext + off, round_keys);
  }

  free(padded);
  close(fd);

  // int n_send = (TOTAL_BYTES % 16) ? TOTAL_BYTES / 16 + 1 : TOTAL_BYTES / 16;
  int n_send = (int)(padded_len / TOTAL_BYTES);

  uint32_t netlen = htonl((uint32_t)padded_len);
  if (write(sockfd, &netlen, sizeof(netlen)) != sizeof(netlen)) {
    perror("write len");
    free(ciphertext);
    close(sockfd);
    return 1;
  }

  for (int i = 0; i < n_send; i++) {
    ssize_t w = write(sockfd, ciphertext + (i * TOTAL_BYTES), TOTAL_BYTES);
    if (w != TOTAL_BYTES) {
      perror("write block");
      free(ciphertext);
      close(sockfd);
      return 1;
    }
  }
  ssize_t sent = 0;

  // while (sent < (ssize_t)padded_len) {
  //   ssize_t w = write(sockfd, ciphertext + sent, padded_len - (size_t)sent);
  //   if (w <= 0) {
  //     perror("write data");
  //     free(ciphertext);
  //     close(sockfd);
  //     return 1;
  //   }
  //   sent += w;
  // }

  // write(sockfd, &n_send, sizeof(n_send));
  // // ファイルの内容がすべて読み出せるだけのメモリ領域を確保
  // // file_buf = malloc((TOTAL_BYTES%16)?TOTAL_BYTES / 16+1:TOTAL_BYTEAS/16);
  //
  // // file_bufにファイルからのデータを格納
  // read(fd, file_buf, n_send);
  //
  // // ファイルの内容を送信
  // // while(if nbytes = read(sockfd, file_buf,sizeof(file_buf)) < 0){
  // write(sockfd, file_buf, n_send);
  // //}

  free(ciphertext);
  close(sockfd);
  return 0;
}

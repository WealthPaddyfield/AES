#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "../include/PKCS#7.h"
#include "../include/aes_key_config.h"
#include "../include/decrypt_block.h"
#include "../include/keyexpansion.h"

#define BUFFER_SIZE 1024

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

int main(void) {

  int listenfd, connfd, nbytes;
  struct sockaddr_in servaddr;
  int output_fd;
  uint32_t net_file_size;
  uint32_t file_size;
  ssize_t r;
  unsigned char *cipher_buf = NULL;
  unsigned char *plain_buf = NULL;
  clock_t start, end;
  double cpu_time;

  const unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae,
                                 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88,
                                 0x09, 0xcf, 0x4f, 0x3c};

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(10000);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind");
    exit(1);
  }

  if (listen(listenfd, 5) < 0) {
    perror("listen");
    exit(1);
  }

  if ((connfd = accept(listenfd, (struct sockaddr *)NULL, NULL)) < 0) {
    perror("accept");
    exit(1);
  }

  // ファイルをオープンする
  output_fd = open("output.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
  if (output_fd < 0) {
    perror("open");
    close(connfd);
    close(listenfd);
    exit(1);
  }

  start = clock();

  r = read(connfd, &net_file_size, sizeof(net_file_size));
  if (r != sizeof(net_file_size)) {
    perror("read file size");
    close(output_fd);
    close(connfd);
    close(listenfd);
    exit(1);
  }
  file_size = ntohl(net_file_size);
  printf("serv receive file size: %u\n", file_size);

  cipher_buf = malloc(file_size);
  if (!cipher_buf) {
    perror("malloc cipher_buf");
    close(output_fd);
    close(connfd);
    close(listenfd);
    exit(1);
  }

  uint32_t received = 0;
  while (received < file_size) {
    r = read(connfd, cipher_buf + received, file_size - received);
    if (r < 0) {
      perror("read cipher");
      free(cipher_buf);
      close(output_fd);
      close(connfd);
      close(listenfd);
      exit(1);
    } else if (r == 0) {
      fprintf(stderr, "peer closed connection unexpectedly\n");
      free(cipher_buf);
      close(output_fd);
      close(connfd);
      close(listenfd);
      exit(1);
    }
    received += (uint32_t)r;
  }

  plain_buf = malloc(file_size);
  if (!plain_buf) {
    perror("malloc plain_buf");
    free(cipher_buf);
    close(output_fd);
    close(connfd);
    close(listenfd);
    exit(1);
  }

  // ブロック単位で復号
  size_t block_size = 16;
  if (file_size % block_size != 0) {
    fprintf(stderr, "暗号文がブロックサイズ(16)の倍数じゃない\n");
    free(cipher_buf);
    free(plain_buf);
    close(output_fd);
    close(connfd);
    close(listenfd);
    exit(1);
  }

  aes_setkey((unsigned char *)key);

  size_t n_blocks = file_size / block_size;
  for (size_t i = 0; i < n_blocks; ++i) {
    aes_decrypt_block(cipher_buf + i * block_size, plain_buf + i * block_size,
                      round_keys);
  }

  // パディングを戻す
  ssize_t plain_len = pkcs7_unpad(plain_buf, file_size);
  if (plain_len < 0) {
    fprintf(stderr, "pkcs7_unpad failed (invalid padding)\n");

    fprintf(stderr, "\n");
    free(cipher_buf);
    free(plain_buf);
    close(output_fd);
    close(connfd);
    close(listenfd);
    exit(1);
  }

  ssize_t wrote = 0;
  while (wrote < plain_len) {
    ssize_t w = write(output_fd, plain_buf + wrote, plain_len - (size_t)wrote);
    if (w < 0) {
      perror("write output");
      free(cipher_buf);
      free(plain_buf);
      close(output_fd);
      close(connfd);
      close(listenfd);
      exit(1);
    }
    wrote += w;
  }

  end = clock();
  cpu_time = ((double)(end - start) / CLOCKS_PER_SEC);
  printf("処理時間: %f 秒\n", cpu_time);

  free(cipher_buf);
  free(plain_buf);
  close(output_fd);
  close(connfd);
  close(listenfd);
  return 0;
}

/*
//
修正：ファイルサイズをネットワークバイトオーダーで受け取り、ホストオーダーに変換
nbytes = read(connfd, &net_file_size, sizeof(net_file_size));
if (nbytes != sizeof(net_file_size)) {
  perror("read file size");
  close(output_fd);
  close(connfd);
  exit(1);
}
file_size = ntohl(net_file_size);
printf("serv receive file size: %d\n", file_size);

// ファイル内容を受け取る
buf = malloc(file_size);
if (!buf) {
  perror("malloc");
  close(output_fd);
  close(connfd);
  exit(1);
}

nbytes = read(connfd, buf, file_size);
if (nbytes < 0) {
  perror("read data");
  free(buf);
  close(output_fd);
  close(connfd);
  exit(1);
}

// 書き込み
write(output_fd, buf, file_size);

// 計測終了
end = clock();
cpu_time = ((double)(end - start) / CLOCKS_PER_SEC);
printf("処理時間: %f 秒\n", cpu_time);

// ファイルを閉じる
free(buf);
close(output_fd);
close(connfd);
return 0;
}
*/

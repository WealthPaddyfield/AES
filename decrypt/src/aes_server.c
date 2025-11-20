#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(void) {
  int listenfd, connfd, nbytes;
  struct sockaddr_in servaddr;
  int output_fd;
  int file_size, receive_size;
  char *buf;
  clock_t start, end;
  double cpu_time;

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
    exit(0);
  }

  // 時間計測開始
  start = clock();

  //修正：ファイルサイズをネットワークバイトオーダーで受け取り、ホストオーダーに変換
  uint32_t net_file_size;
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

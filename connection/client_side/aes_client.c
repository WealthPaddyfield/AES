#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define TOTAL_BYTES (4 * 4) // 16
#define BUFSIZE 1024
int main(int argc, char *argv[]) {
  int sockfd, nbytes;
  struct sockaddr_in servaddr;
  int fd;
  int f_siz;
  char *file_buf;
  const char *filename = argv[1];
  const char *ip_adress = argv[2];
  unsigned char state[4][4];
  FILE *file_fp;
  file_fp = fopen(filename, "rb");
  // バイナリをステートに変換しarg[1]のファイルを変更
  size_t items_read = fread(state, 1, TOTAL_BYTES, file_fp);
  fclose(file_fp);

  if ((fd = open(filename, O_RDONLY)) < 0) {
    perror("open");
    close(sockfd);
    exit(1);
  }

  f_siz = lseek(fd, 0, SEEK_END); // 送信したいファイルのサイズを取得（バイト）
  lseek(fd, 0, SEEK_SET); // ファイルの読み書き位置を先頭に戻す

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

  // ファイルサイズを送信
  write(sockfd, &f_siz, sizeof(f_siz));

  // ファイルの内容がすべて読み出せるだけのメモリ領域を確保
  file_buf = malloc(f_siz);

  // file_bufにファイルからのデータを格納
  read(fd, file_buf, f_siz);

  // ファイルの内容を送信
  // while(if nbytes = read(sockfd, file_buf,sizeof(file_buf)) < 0){
  write(sockfd, file_buf, f_siz);
  //}

  close(fd);
  close(sockfd);

  free(file_buf);
  return 0;
}

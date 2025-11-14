#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]){
	int sockfd, nbytes;
	struct sockaddr_in servaddr;
	int fd;
    FILE *fp;
	int f_siz;
	char *file_buf;
	char *ip_adress=argv[2];  

	if ((fd = open(argv[1], O_RDONLY)) < 0) {
        perror("open");
        close(sockfd);
        exit(1);
    }
	f_siz = lseek(fd, 0, SEEK_END); //送信したいファイルのサイズを取得（バイト）
	lseek(fd, 0, SEEK_SET); //ファイルの読み書き位置を先頭に戻す

	printf("client: transmit file size:%d\n",f_siz);
	//fflush(stdout); exit(0);

	if ((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
		perror("socket");exit(1);
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(10000);
	if(inet_pton(AF_INET,ip_adress,&servaddr.sin_addr)<0){ //特定のIPアドレスに接続する
		perror("inet_pton");exit(1);
	}



	if (connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
		perror("connect");exit(1);
	}

	
	//ファイルサイズを送信
	write(sockfd, &f_siz, sizeof(f_siz));
	
	//ファイルの内容がすべて読み出せるだけのメモリ領域を確保
	file_buf = malloc(f_siz);
	

    //file_bufにファイルからのデータを格納
	read(fd, file_buf, f_siz);

	//ファイルの内容を送信
	//while(if nbytes = read(sockfd, file_buf,sizeof(file_buf)) < 0){
    	write(sockfd, file_buf, f_siz);
	//}
	
	close(fd);
	close(sockfd);

	free(file_buf);
	return 0;
}

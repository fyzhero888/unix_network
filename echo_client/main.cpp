#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int		sockfd;
	struct 	sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(10156);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr));

	exit(0);
}

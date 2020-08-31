#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>


#define SERV_PORT 10156

void str_echo(int sockfd) {
    ssize_t     n;
    char        buf[1024];

again:
    while((n = read(sockfd, buf, 1024)) > 0)
        write(sockfd, buf, n);

    if (n < 0 && errno == EINTR)    
        goto again;
    else if (n < 0)
        perror("[str_echo]");

    return ;
}

int main(int argc, char *argv[]) {
    
    int     listenfd, connfd;
    pid_t   childpid;
    socklen_t           chilen;
    struct sockaddr_in  chiaddr, servaddr; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    listen(listenfd, 5);

    for (;;) {

        chilen = sizeof(chiaddr);
        connfd = accept(listenfd, (struct sockaddr*)&servaddr, &chilen);

        if ((childpid = fork()) == 0) {
            // this is child process
            close(listenfd);

            // echo stuff
            str_echo(connfd);
            exit(0);
        }
    
        close(connfd);
    }

    close(listenfd);

    return 0;
}

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/un.h>

int main(){
    unlink("server.sock");
    // 创建套接字
    int lfd = socket(AF_UNIX, SOCK_STREAM, 0);

    // 绑定
    struct sockaddr_un saddr;
    saddr.sun_family = AF_UNIX;
    strcpy(saddr.sun_path, "server.sock");
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof saddr);
    if(ret == -1){
        perror("bind");
        exit(1);
    }

    // 监听
    ret = listen(lfd, 8);
    if(ret == -1){
        perror("listen");
        exit(2);
    }

    struct sockaddr_un cliaddr;
    socklen_t len = sizeof cliaddr;
    // 等待客户端连接
    int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);

    // 通信
    while(1)
    {
        char recvbuf[128];
        int len = recv(cfd, recvbuf, sizeof recvbuf, 0);
        if(len == -1){
            perror("read");
            exit(3);
        }else if(len == 0){
            printf("client closed...\n");
        }else if(len > 0){
            printf("client say: %s\n", recvbuf);
        }

        // 给客户端回复数据
        send(cfd, recvbuf, strlen(recvbuf) + 1, 0);
    }

    close(cfd);
    close(lfd);
}
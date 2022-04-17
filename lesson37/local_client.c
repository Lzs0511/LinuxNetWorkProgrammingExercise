#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/un.h>
#include<arpa/inet.h>

int main(){

    unlink("client.sock");
    // 创建套接字
    int fd = socket(PF_UNIX, SOCK_STREAM, 0);
    // 绑定
    struct sockaddr_un cliaddr;
    cliaddr.sun_family = AF_UNIX;
    strcpy(cliaddr.sun_path, "client.sock");
    int ret = bind(fd, (struct sockaddr *)&cliaddr, sizeof cliaddr);
    if(ret == -1){
        perror("bind");
        exit(5);
    }
    // 连接
    struct sockaddr_un saddr;
    saddr.sun_family = AF_UNIX;
    strcpy(saddr.sun_path, "server.sock");
    socklen_t len = sizeof saddr;
    ret = connect(fd, (struct sockaddr *)&saddr, len);
    if(ret == -1){
        perror("connect");
        exit(4);
    }
    // 通信

    int num = 0;
    while(1){
        char buf[128];
        sprintf(buf, "Hello Server, This is client %d", num++);
        int ret = send(fd, buf, strlen(buf) + 1, 0);
        if(ret == -1){
            perror("send");
            exit(3);
        }
        // 接受数据
        int len = recv(fd, buf, sizeof buf, 0);
        if(len == 0){
            printf("server closed...\n");
        }else if(len == -1){
            perror("recv");
            exit(2);
        }else if(len > 0)
        {
            printf("server say: %s\n", buf);
        }
        sleep(1);
    }
    close(fd);
}
#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>

int main()
{
    // 创建socket
    int fd = socket(PF_INET, SOCK_DGRAM, 0);

    // 设置socket选项
    int op = 1;
    int ret = setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &op, sizeof op);

    //  向客户端发送数据
    struct sockaddr_in cliaddr;
    ret = inet_pton(AF_INET, "172.17.255.255", &cliaddr.sin_addr.s_addr); 
    if(ret == -1){
        perror("inet_pton");
        exit(1);
    }
    cliaddr.sin_port = htons(9999);
    cliaddr.sin_family = AF_INET;
    int num = 0;
    while(1){
        char sendbuf[128];
        sprintf(sendbuf, "server send %d", num++);
        printf("server send %s\n", sendbuf);
        sendto(fd, sendbuf, strlen(sendbuf) + 1, 0, (struct sockaddr *)&cliaddr, sizeof cliaddr);
        sleep(1);
    }
    return 0;
}
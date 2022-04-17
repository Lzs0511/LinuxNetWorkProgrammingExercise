#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>


int main()
{
    // 创建套接字
    int fd = socket(PF_INET, SOCK_DGRAM, 0);

    // 绑定
    struct sockaddr_in saddr;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(8888);
    int ret = bind(fd, (struct sockaddr *)&saddr, sizeof saddr);
    if(ret == -1){
        perror("bind");
        exit(1);
    }
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof cliaddr;
    while(1)
    {
        // 接收数据
        char recvbuf[128];
        char ipbuf[16];
        recvfrom(fd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&cliaddr, &len);
        // 打印客户端信息
        printf("client IP:%s, port: %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ipbuf, sizeof cliaddr), ntohs(cliaddr.sin_port));

        // 打印接受到的数据
        printf("server read: %s\n", recvbuf);

        // 向客户端发送数据
        sendto(fd, recvbuf, strlen(recvbuf) + 1, 0, (struct sockaddr *)&cliaddr, len);

    }
}
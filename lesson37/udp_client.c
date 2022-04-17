#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{

    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd == -1)
    {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in saddr;
    inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);
    saddr.sin_port = htons(8888);
    saddr.sin_family = AF_INET;
    int num = 0;
    while (1)
    {
        char sendbuf[128];
        sprintf(sendbuf, "Hello Server, I am client, num is %d", num++);
        // 向服务器发送数据
        sendto(fd, sendbuf, strlen(sendbuf) + 1, 0, (struct sockaddr *)&saddr, sizeof saddr);

        // 接收数据
        recvfrom(fd, sendbuf, sizeof sendbuf, 0, NULL, NULL);
        printf("server say: %s\n", sendbuf);
        sleep(1);
    }
    return 0;
}
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    int fd = socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(9999);
    addr.sin_family = AF_INET;

    int ret = bind(fd, (struct sockaddr *)&addr, sizeof addr);
    if(ret == -1){
        perror("bind");
        exit(1);
    }

    while(1)
    {
        char recvbuf[128];
        recvfrom(fd, recvbuf, sizeof recvbuf, 0, NULL, NULL);
        printf("client recv: %s\n", recvbuf);
    }
    return 0;
}
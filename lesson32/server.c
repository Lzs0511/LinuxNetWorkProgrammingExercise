#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
int main()
{
    // 1. 创建套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1)
    {
        perror("socket");
        exit(1);
    }

    // 2. 套接字绑定ip地址和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
    addr.sin_port = htons(9999);
    int ret = bind(lfd, (struct sockaddr *)&addr, sizeof addr);
    if(ret == -1)
    {
        perror("bind");
        exit(2);
    }
    // 3. 监听

    ret = listen(lfd, 5);
    if(ret == -1)
    {
        perror("listen");
        exit(3);
    }
    struct sockaddr_in clientaddr;
    socklen_t addr_size = sizeof clientaddr;
    // 4. 接受连接
    int afd = accept(lfd, (struct sockaddr *)&clientaddr, &addr_size);

    // 5. 输出客户端的信息
    // 输出客户端的ip和端口
    char ipaddr[16];
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ipaddr, sizeof ipaddr);
    int clientport = ntohs(clientaddr.sin_port);
    printf("客户端的ip地址为:%s, 端口为:%d\n", ipaddr, clientport);

    // 6. 接受数据
    char buf[1024] = {0};
    int size = 0;
    while(1)
    {
        sleep(2);
        size = read(afd, buf, sizeof buf);
        if(size > 0)
        {
            printf("client receive: %s\n", buf);

            char *str = "Hello, Client! This is Server!";
            write(afd, str, strlen(str)); 
        }
        else if(size == 0)
        {
            printf("客户端断开连接...\n");
            break;
        }else if(size == -1 )
        {
            perror("read");
            exit(3);
        }
    }
    // 7. 发送数据
       
    // sleep(10);
    close(afd);
    close(lfd);

}

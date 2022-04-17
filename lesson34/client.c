#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>

int main()
{
    // 1. 创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in server_addr;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(10000);

    // 2. 连接服务器
    int ret = connect(fd, (struct sockaddr *)&server_addr, sizeof server_addr);
    if(ret == -1)
    {
        perror("connect");
        exit(2);
    }
    // 3. 发送数据和接受数据

    // 3. 发送数据
    


     // 4. 接受数据
    char buf[1024] = {0};
    char buf1[1024] = {0};
    int size = 0;
    int i = 0;
    while(1)
    {
        sprintf(buf1, "data: %d", i++);
        // char *str = "Hello, Server! This is Client!";
        write(fd, buf1, strlen(buf1) + 1);  
        
        // size = read(fd, buf, sizeof buf);
        // if(size > 0)
        // printf("client receive: %s\n", buf);
        // else if(size == 0)
        // {
        //     printf("服务器断开连接...\n");
        //     break;
        // }else if(size == -1)
        // {
        //     perror("read");
        //     exit(3);
        // }
        usleep(1000);
    }
    // if(size == -1)
    // {
    //     perror("read");
    //     exit(4);
    // }

    close(fd);     
    return 0;
}
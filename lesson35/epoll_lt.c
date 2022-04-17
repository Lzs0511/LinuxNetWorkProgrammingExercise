#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/epoll.h>
#include<sys/time.h>
#include<arpa/inet.h>
int main(){
    // 创建套接字
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    
    // bind
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
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

    // 创建epoll
    int epfd = epoll_create(100);
    if(epfd == -1){
        perror("epoll_create");
        exit(3);
    }
    // 将监听的文件描述符添加到kernel的epoll中
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);
    struct epoll_event epevs[1024];

    while(1){
        int ret = epoll_wait(epfd, epevs, 1024, -1);
        if(ret == -1){
            // 出错
            perror("epoll_wait");
            exit(4);
        }
        printf("ret == %d\n", ret);
        // 处理数据
        for(int i = 0; i < ret; i++){
            int currfd = epevs[i].data.fd;
            if(currfd == lfd){
                // 接受连接，并将文件描述符放到epoll中
                struct sockaddr_in cliaddr;
                socklen_t len = sizeof cliaddr;
                int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);
                epev.data.fd = cfd;
                epev.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            }else if(epevs[i].events & EPOLLIN){
                char buf[5] = {0};
                int rd = read(currfd, buf, sizeof buf);
                if(rd == -1){
                    perror("read");
                    exit(5);
                }else if(rd == 0){
                    // 客户端断开连接, 关闭文件描述符，并且在kernel中将文件描述符删除
                    printf("client closed...\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, currfd, NULL);
                    close(currfd);
                }else if(rd > 0){
                    // 读取数据
                    printf("read client = %s\n", buf);
                    write(currfd, buf, strlen(buf) + 1);
                }
            }
        }
    }
    close(epfd);
    close(lfd);
    return 0;
}
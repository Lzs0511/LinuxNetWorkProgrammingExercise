#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/time.h>
#include<poll.h>
#include<arpa/inet.h>
#include<string.h>
int main(){
    // 1. 创建socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    // 2. 绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1){
        perror("bind");
        exit(1);
    }
    // 3. 监听
    ret = listen(lfd, 128);
    if(ret == -1){
        perror("listen");
        exit(2);
    }

    struct pollfd fds[1024];
    // 初始化 pollfd
    for(int i = 0; i < 1024; i++){
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }
    fds[0].fd = lfd;

    int nfds = 0;
    while(1){
        
        // 使用poll进行IO多路复用
        ret = poll(fds, nfds+1, -1);
        
        if(ret == -1){
            // 出错
            perror("poll");
            exit(3);
        }else if(ret == 0){
            // 超时，在此代码中，不会超时
            continue;
        }else if(ret > 0){
            // 先判断是否有新的客户端连接进来
            
            if(fds[0].revents & POLLIN){
                // 接受新连接
                struct sockaddr_in caddr;
                socklen_t len = sizeof(caddr);
                int afd = accept(lfd, (struct sockaddr *)&caddr, &len);
                // 将新的文件描述符添加到fdset中
                for(int i = 1; i < 1024; i++){
                    if(fds[i].fd == -1){
                        fds[i].fd = afd;
                        fds[i].events = POLLIN;
                        break;
                    }
                }
                // 更新最大的文件描述符的索引
                nfds = nfds > afd ? nfds : afd;
            }

            // 判断监听文件描述符之后的文件
            for(int i = 1; i <= nfds; ++i){

                // 对应的客户端发送了数据
                if(fds[i].revents & POLLIN){
                    char buf[1024] = {0};
                
                    ret = read(fds[i].fd, buf, sizeof buf);
                
                    if(ret == -1){
                        perror("read");
                        exit(4);
                    }else if(read == 0){
                        // 客户端断开了连接
                        printf("client closed...\n");
                        close(fds[i].fd);
                        fds[i].fd = -1;
                        
                    }else if(read > 0){
                        printf("read client = %s\n", buf);
                        write(fds[i].fd, buf, strlen(buf) + 1);
                    }
                }
            }
            
        }
    }
    close(lfd);
}

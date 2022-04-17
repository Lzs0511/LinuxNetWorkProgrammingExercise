#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/select.h>
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

    // 设置fd_set
    fd_set fds, tmp;
    // 清空 fd_set并将监听的文件描述符添加到fdset中
    FD_ZERO(&fds);
    FD_SET(lfd, &fds);
    int max_fd = lfd;

    while(1){
        tmp = fds;
        // 使用select进行IO多路复用
        ret = select(max_fd + 1, &tmp, NULL, NULL, NULL);
        if(ret == -1){
            // 出错
            perror("select");
            exit(3);
        }else if(ret == 0){
            // 超时，在此代码中，不会超时
            continue;
        }else if(ret > 0){
            // 先判断是否有新的客户端连接进来
            int judge = FD_ISSET(lfd, &tmp);
            if(judge == 1){
                // 接受新连接
                struct sockaddr_in caddr;
                socklen_t len = sizeof(caddr);
                int afd = accept(lfd, (struct sockaddr *)&caddr, &len);
                // 将新的文件描述符添加到fdset中
                FD_SET(afd, &fds);
                max_fd = afd > max_fd ? afd : max_fd;
            }

            // 判断监听文件描述符之后的文件
            for(int i = lfd + 1; i < max_fd + 1; ++i){
                // 对应的客户端发送了数据
                if(FD_ISSET(i, &tmp)){
                    char buf[1024] = {};
                    ret = read(i, buf, sizeof buf);
                    if(ret == -1){
                        perror("read");
                        exit(4);
                    }else if(read == 0){
                        // 客户端断开了连接
                        printf("client closed...\n");
                        FD_CLR(i, &fds);
                        close(i);
                    }else if(read > 0){
                        printf("read client = %s\n", buf);
                        write(i, buf, strlen(buf) + 1);
                    }
                }
            }
            
        }
    }
    close(lfd);
}

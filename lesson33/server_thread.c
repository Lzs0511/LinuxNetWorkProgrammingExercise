#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<signal.h>
#include<sys/wait.h>
#include<errno.h>
#include<pthread.h>
struct Info
{
    pthread_t tid;
    struct sockaddr_in addr;
    int fd;
};
struct Info info[128];
void *work(void *args)
{
    struct Info *info_p = (struct Info *)args;
  
    // 打印客户端信息：
    char cli_ip[16];
    int cli_port = info_p->addr.sin_port;
    int clisize = sizeof(info_p->addr);
    inet_ntop(AF_INET, &info_p->addr.sin_addr.s_addr, cli_ip, clisize);
    printf("client ip is %s, port is %d, pid is %lu\n", cli_ip, cli_port, info_p->tid);
    // 接受数据
    char buf[1024];
    while(1)
    {
        int len = read(info_p->fd, buf, sizeof buf);
        if(len > 0)
        {
            printf("receive client data: %s\n", buf);
        }else if(len == -1)
        {
            perror("read");
            exit(4);
        }else
        {
            printf("client closed...\n");
            break;
        }
        // 返回客户端发来的信息到客户端
        write(info_p->fd, buf, strlen(buf)+1);

    }
    // 关闭文件
    close(info_p->fd);
    info_p->fd = -1;
    return NULL;
    
    
}
int main()
{

    // 1. 创建套接字
    int sfd = socket(PF_INET, SOCK_STREAM, 0);
    if(sfd == -1)
    {
        perror("socket");
        exit(1);
    }

    // 2. 套接字绑定端口
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;
    socklen_t size = sizeof saddr;
    int ret = bind(sfd, (struct sockaddr *)&saddr, size);
    if(ret == -1)
    {
        perror("bind");
        exit(2);
    }
    // 3. 监听
    ret = listen(sfd, 128);
    if(ret == -1)
    {
        perror("listen");
        exit(3);
    }

    // 4. 等待连接
    struct sockaddr_in cliaddr;
    socklen_t clisize = sizeof cliaddr;
    // 初始化info
    int info_len = sizeof(info) / sizeof(struct Info);
    for(int i = 0; i != info_len; ++i)
    {
        bzero(&info[i], sizeof(info[i]));
        info[i].fd = -1;
        info[i].tid = -1;
    }
    while(1)
    {
        struct Info *info_p;
        struct sockaddr_in cliaddr;
        
        
        for(int i = 0; i < info_len; ++i)
        {
            if(info[i].fd == -1)
            {
                info_p = &info[i];
                break;
            }
            if(i == info_len - 1)
            {
                sleep(1);
                i = -1;
            }
        }
        info_p->fd = accept(sfd, (struct sockaddr *)&info->addr, &clisize);
        if(info_p->fd == -1)
        {
            if(errno == EINTR)
            {
                continue;
            }
            perror("accept");
            exit(-1);
        }
        

        // info_p->fd = cfd;
        // info_p->addr = cliaddr;
        // memcpy(&info_p->addr, &cliaddr, clisize);
        pthread_create(&info_p->tid, NULL, work, info_p);
        
        // 回收线程资源
        pthread_detach(info_p->tid);
    }
    
    return 0;
}
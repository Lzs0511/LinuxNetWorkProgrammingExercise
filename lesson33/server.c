#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<signal.h>
#include<sys/wait.h>
#include<errno.h>
void recycleChild(int arg)
{
    while(1)
    {
        int ret = waitpid(-1, NULL, WNOHANG);
        if(ret == -1)
        {
            break;
        }else if(ret == 0)
        {
            break;
        }else if(ret > 0)
        {
            printf("子进程 %d 被回收了...\n", ret);
        }
    }
}
int main()
{
    // 注册信号捕捉
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = recycleChild;
    sigaction(SIGCHLD, &act, NULL);
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
    while(1)
    {
        int cfd = accept(sfd, (struct sockaddr *)&cliaddr, &clisize);
        if(cfd == -1)
        {
            if(errno == EINTR)
            {
                continue;
            }
            perror("accept");
            exit(-1);
        }
        // 创建子进程
        pid_t pid = fork();
        if(pid == 0)
        {
            // 子进程
            // 打印客户端信息：
            char cli_ip[16];
            int cli_port = cliaddr.sin_port;
            inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, cli_ip, clisize);
            printf("client ip is %s, port is %d, pid is %d\n", cli_ip, cli_port, getppid());
            // 接受数据
            char buf[1024];
            while(1)
            {
                int len = read(cfd, buf, sizeof buf);
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
                write(cfd, buf, strlen(buf));

            }
            // 关闭文件
            close(cfd);
        }
    }
    
    
}
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

void *callback(void *args)
{
    printf("child tid: %lu\n", pthread_self());
    pthread_exit(NULL);
}
int main()
{
    pthread_t tid;
    // 1.创建子线程
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if(ret != 0)
    {
        char* strerr = strerror(ret);
        printf("error1: %s\n", strerr);
        exit(1);
        }

    // 2. 分离子线程

    ret = pthread_detach(tid);    
    if(ret != 0)
    {
        char *strerr = strerror(ret);
        printf("error2: %s\n", strerr);
        exit(2);
    }

    // 3. 尝试连接子线程
    ret = pthread_join(tid, NULL);
    if(ret != 0)
    {
        char *strerr = strerror(ret);
        printf("error3: %s\n", strerr);
        exit(3);
    }

    pthread_exit(NULL);
    return 0;
}
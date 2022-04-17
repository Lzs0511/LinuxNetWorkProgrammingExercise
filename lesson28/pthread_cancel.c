#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

void *callback(void *args)
{
    printf("child tid: %lu\n", pthread_self());
    for(int i = 0; i < 50; i++)
        printf("child : %d\n", i);
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

    // 2. 取消子线程
    pthread_cancel(tid);

    // 3. 尝试连接子线程
    // ret = pthread_join(tid, NULL);
    // if(ret != 0)
    // {
    //     char *strerr = strerror(ret);
    //     printf("error3: %s\n", strerr);
    //     exit(3);
    // }

    for(int i = 0; i < 50; i++)
        printf("main : %d\n", i);

    pthread_exit(NULL);
    return 0;
}
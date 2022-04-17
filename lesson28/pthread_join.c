#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
int result = 10;
void *callback(void *args)
{
    printf("child thread starting...\n");
    printf("child thread id: %ld\n", pthread_self());
    
    pthread_exit(&result);
}
int main()
{
    pthread_t tid;

    // 创建子线程
    int ret = pthread_create(&tid, NULL, callback, NULL);   
    if(ret)
    {
        // char *errorStr = errstr()
        char *errstr = strerror(ret);
        printf("%s\n", errstr);
        exit(0);
    }

    for(int i = 0; i < 500; i++) printf("%d\n", i);
    printf("main thread id:%lu, child thread id: %lu\n", pthread_self(), tid);
    int *retval = NULL;
    // 回收子线程的资源
    ret = pthread_join(tid, (void**)&retval);
    if(ret)
    {
        char *strerr = strerror(ret);
        printf("%s\n", strerr);
        exit(1);
    }
    printf("子线程的资源已全部回收...\n");

    // 主线程退出
    pthread_exit(NULL);

    return 0;
}
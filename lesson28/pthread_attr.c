#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
void* callback(void* arg)
{
    printf("thread start...\n");
    printf("the arg is %d\n", *(int*)arg);

    return NULL;
}
int main()
{
    pthread_t tid;
    int num = 10;
    size_t stack_size = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_getstacksize(&attr, &stack_size);
    printf("stack size is:%lu\n", stack_size);
    int ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if(ret)
    {
        char *errstr = strerror(ret);
        printf("error1: %s\n", errstr);
        exit(1);
    }
    pthread_create(&tid, &attr, callback, &num);

    sleep(1);
    for(int i = 0; i < 5; ++i)
    {
        printf("%d\n", i);
    }

    return 0;
}

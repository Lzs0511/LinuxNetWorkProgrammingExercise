#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>

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
    pthread_create(&tid, NULL, callback, &num);

    sleep(1);
    for(int i = 0; i < 5; ++i)
    {
        printf("%d\n", i);
    }

    return 0;
}

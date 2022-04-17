#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
int num = 1000000;
// 创建读写锁变量
pthread_rwlock_t rwlock;
pthread_mutex_t mutex;
void *wlock(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(num == 0)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        // printf("+++write: tid: %lu, num: %d\n", pthread_self(), num--);
        num--;
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
}
void *rlock(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(num == 0)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        // printf("===read: tid: %lu, num: %d\n", pthread_self(), num);
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
}
int main()
{
    // 初始化读写锁
    pthread_mutex_init(&mutex, NULL);
    
    // 创建pthread_t 变量
    pthread_t wtid[3], rtid[5];

    // 创建写线程
    for(int i = 0; i != 3; ++i)
        pthread_create(&wtid[i], NULL, wlock, NULL);
    // 创建读线程
    for(int i = 0; i != 5; ++i)
        pthread_create(&rtid[i], NULL, rlock, NULL);

    // 设置线程份离
    for(int i = 0; i != 3; i++) pthread_detach(wtid[i]);
    for(int i = 0; i != 5; i++) pthread_detach(rtid[i]);

    // 销毁读写锁
    pthread_mutex_destroy(&mutex);

    // 退出主线程
    pthread_exit(NULL);
    
    return 0;
}
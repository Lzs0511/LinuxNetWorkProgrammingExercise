#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
// 声明一个互斥量
pthread_mutex_t mutex1;
int tickets = 1000;
void *sellticket(void *args)
{

    while(1)
    {
        //给临界区上锁
        pthread_mutex_lock(&mutex1);
        if(tickets > 0)
        {
            usleep(10000);
            printf("%lu线程正在售卖第%d张票\n", pthread_self(), tickets--);
        }else
        {
            // 给临界区解锁
            pthread_mutex_unlock(&mutex1);
            break;
        }
        // 给临界区解锁
        pthread_mutex_unlock(&mutex1);
    }
    pthread_exit(NULL);
}
int main()
{
    // 初始化互斥量
    pthread_mutex_init(&mutex1, NULL);
    // 创建三个pthread_t变量
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, sellticket, NULL);
    pthread_create(&tid2, NULL, sellticket, NULL);
    pthread_create(&tid3, NULL, sellticket, NULL);

    // 回收子线程的资源
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    // 释放互斥量
    pthread_mutex_destroy(&mutex1);
    //退出主线程
    pthread_exit(NULL);
    
    return 0;
}
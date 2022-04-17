#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
int tickets = 100;
void* sellticket(void *args)
{
    while(tickets > 0)
    {
        usleep(6000);
        printf("%lu线程正在售卖第%d张票\n", pthread_self(), tickets);
        tickets--;
    }
}
int main()
{
    // 创建三个pthread_t 变量
    pthread_t tid1, tid2, tid3;

    pthread_create(&tid1, NULL, sellticket, NULL);
    pthread_create(&tid2, NULL, sellticket, NULL);
    pthread_create(&tid3, NULL, sellticket, NULL);


    // 回收子线程的资源

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    
    // 退出主线程
    pthread_exit(NULL);

    return 0;
}
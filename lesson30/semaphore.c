#include<semaphore.h>
#include<unistd.h>
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
// 创建条件变量和互斥锁
// pthread_cond_t cond;
pthread_mutex_t mutex;
// 创建信号量
sem_t csem, psem;
struct Node
{
    int val;
    struct Node* next;
};
struct Node *head = NULL;
void *produce(void *arg)
{
    while(1)
    {
        sem_wait(&psem);
        // 生产新的节点
        pthread_mutex_lock(&mutex);
        struct Node *node = (struct Node*)malloc(sizeof(struct Node));
        node->val = rand() % 1000;
        printf("add a new node, val: %d, tid: %lu\n", node->val, pthread_self());
        node->next = head;
        head = node;
        // pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        sem_post(&csem);
        // usleep(100);
    }
}
void *custom(void *arg)
{
    while(1)
    {
        sem_wait(&csem);
        pthread_mutex_lock(&mutex);
        struct Node* tmp = head;
        head = head->next;      
        printf("del a node, val: %d, tid: %lu\n", tmp->val, pthread_self());
        free(tmp);
        pthread_mutex_unlock(&mutex);
        sem_post(&psem);
        
        
    }
}
int main()
{
    // 初始化条件变量和互斥锁
    // pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);
    // 初始化信号量
    // 初始化生产者的信号量
    sem_init(&psem, 0, 8);
    // 初始化消费者的信号量
    sem_init(&csem, 0, 0);
    // 创建线程id
    pthread_t ptid[5], ctid[5];

    // 创建线程
    for(int i = 0; i != 5; ++i)
    {
        pthread_create(&ptid[i], NULL, produce, NULL);
        pthread_create(&ctid[i], NULL, custom, NULL);
    }

    // 设置线程份离
    for(int i = 0; i < 5; i++)
    {
        pthread_detach(ptid[i]);
        pthread_detach(ctid[i]);
    }

    while(1);
    // 释放互斥锁和条件变量
    pthread_mutex_destroy(&mutex);
    // pthread_cond_destroy(&cond);
    // 退出主线程
    pthread_exit(NULL);
    return 0;
}

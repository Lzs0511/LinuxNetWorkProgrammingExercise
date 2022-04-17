#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
// 创建条件变量和互斥锁
pthread_cond_t cond;
pthread_mutex_t mutex;
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
        // 生产新的节点
        pthread_mutex_lock(&mutex);
        struct Node *node = (struct Node*)malloc(sizeof(struct Node));
        node->val = rand() % 1000;
        printf("add a new node, val: %d, tid: %lu\n", node->val, pthread_self());
        node->next = head;
        head = node;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
}
void *custom(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(head)
        {
            
            struct Node* tmp = head;
            head = head->next;      
            printf("del a node, val: %d, tid: %lu\n", tmp->val, pthread_self());
            free(tmp);
            pthread_mutex_unlock(&mutex);
        }else
        {
            pthread_cond_wait(&cond, &mutex);
            pthread_mutex_unlock(&mutex);
        }
        
    }
}
int main()
{
    // 初始化条件变量和互斥锁
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);
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
    pthread_cond_destroy(&cond);
    // 退出主线程
    pthread_exit(NULL);
    return 0;
}
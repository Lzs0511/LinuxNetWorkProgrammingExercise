#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
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
        struct Node *node = (struct Node*)malloc(sizeof(struct Node));
        node->val = rand() % 1000;
        printf("add a new node, val: %d, tid: %lu\n", node->val, pthread_self());
        node->next = head;
        head = node;
        usleep(100);
    }
}
void *custom(void *arg)
{
    while(1)
    {
        if(head)
        {
            struct Node* tmp = head;
            head = head->next;      
            printf("del a node, val: %d, tid: %lu\n", tmp->val, pthread_self());
            free(tmp);
        }
    }
}
int main()
{
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

    // 退出主线程
    pthread_exit(NULL);
    return 0;
}
#include<stdio.h>
#include<stdlib.h>
int main()
{
    int *ip;
    // 测试 calloc
    // ip = (int *)calloc(10, sizeof(int));
    // for(int i = 0; i < 10; i++)
    // {
    //     printf("%d\n", *ip++);
    // }

    // 测试 malloc
    ip = (int *)malloc(sizeof(int));
    printf("%d\n", *ip);
    return 0;   
}
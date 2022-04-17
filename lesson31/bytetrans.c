#include<stdio.h>
#include<arpa/inet.h>

int main()
{

    // 定义一个short 变量
    short x = 0x0102;
    x = htons(x);
    printf("htons :%x\n", x);

    char buf[4] = {192, 168, 1, 1};
    int y = *(int*)buf;
    y = htonl(y);
    unsigned char *p = (char*)&y;
    
    printf("htonl :%d.%d.%d.%d\n", *p, *(p+1), *(p+2), *(p+3));

    // 测试 ntohl 和 ntohs

    short x1 = 0x0102;
    x1 = ntohs(x);
    printf("ntonh :%x\n", x1);


    unsigned char buf1[4] = {1, 1, 192, 168};
    int p1 = *(int *)buf1;
    
    p1 = ntohl(p1);
    unsigned char *p2 = (char *)&p1;
    printf("ntohl :%d.%d.%d.%d\n", *p2, *(p2+1), *(p2+2), *(p2+3));
    return 0;
}
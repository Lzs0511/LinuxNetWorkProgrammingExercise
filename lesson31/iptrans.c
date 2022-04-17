#include<stdio.h>
#include<arpa/inet.h>

int main()
{

    unsigned num;
    // 点分十进制字符串转整型
    int ret = inet_pton(AF_INET, "192.168.1.1", &num);
    
    const unsigned char *p = (const unsigned char *)&num;
    printf("%d.%d.%d.%d\n", *p, *(p+1), *(p+2), *(p+3));

    // 整型转点分十进制
    char str[16] = "";
    const char *ip = inet_ntop(AF_INET, &num, str, sizeof str);
    printf("str: %s\n", str);
    printf("ip: %s\n", ip);
    return 0;
}
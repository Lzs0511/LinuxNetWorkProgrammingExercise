#include<iostream>
using namespace std;

int main()
{
    typedef union ts
    {
        short value;
        char buf[sizeof value];    
    }test;
    test t1;
    ts t2;
    t1.value = 0x0102;
    if(t1.buf[0] == 0x01 && t1.buf[1] == 0x02)
    {
        cout << "大端字节序" << endl;
    }else 
    {
        cout << "小端字节序" << endl;
    }

    return 0;
}
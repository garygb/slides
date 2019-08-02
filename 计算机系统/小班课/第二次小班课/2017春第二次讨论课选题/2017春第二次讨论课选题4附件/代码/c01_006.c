#include <stdio.h>
int main()
{
 
    union check
    {
        int i;
        char ch;
    }c;
    c.i=1;//union中的元素共占内存空间，长度由其中长度最大的那个元素决定，所有的元素从低位对齐。

    if(c.ch==1) printf("小端环境\n");
	else printf("大端环境\n");

return 0;
}

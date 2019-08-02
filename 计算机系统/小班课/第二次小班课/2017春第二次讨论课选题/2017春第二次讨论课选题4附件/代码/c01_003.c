#include <stdio.h>
int main()
{
char a[6]={'H','N','U','e','r','\0'};
char b[6]={72,78,85,101,114,0};
int c[2]={(72<<24)+(78<<16)+(85<<8)+101,114<<24};
printf("%d,%d\n", sizeof(char),sizeof(int));
printf("%s\n",a);
printf("%s\n",b);
printf("%s\n",c);

return 0;
}

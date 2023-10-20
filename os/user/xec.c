#include <timeros/types.h>
#include <timeros/syscall.h>
#include <timeros/string.h>

int main()
{
    while (1)
    {
       // printf("exec!\n");
       char a = getchar();
       printf("%x",a);
    }
    return 0;
}
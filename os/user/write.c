#include <timeros/types.h>
#include <timeros/syscall.h>
#include <timeros/string.h>
int main()
{
    int pid = sys_fork();
    while (1)
    {
        printf("pid:%d\n",pid);
        // char data = getchar();
        //printf("write:%d\n",4);
    }
    return 0;
}

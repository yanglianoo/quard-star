#include <timeros/types.h>
#include <timeros/syscall.h>
#include <timeros/string.h>
int main()
{
    // 在父进程中创建子进程
    int pid = sys_fork();
    while (1)
    {
       printf("pid:%d\n",pid);
    }
    
    return 0;
}

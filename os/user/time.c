#include <timeros/types.h>
#include <timeros/syscall.h>
#include <timeros/string.h>
int main()
{
    // 在父进程中创建子进程
    int pid = sys_fork();

    
    while (1)
    {
        if(pid > 0)
        {
            // 父进程执行的逻辑
            printf("father\n");
        }
        else if(pid == 0)
        {
            // 子进程执行的逻辑
            printf("child\n");
        }
        else // pid == -1
        {
            // 创建子进程失败了
        }
    }
    
    return 0;
}

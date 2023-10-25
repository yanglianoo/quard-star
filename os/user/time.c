#include <timeros/types.h>
#include <timeros/syscall.h>
#include <timeros/string.h>
int main()
{
    // // 在父进程中创建子进程
     int pid = sys_fork();
     printf("pid:%d\n",pid);
    if(pid==0)
    {
         sys_exec("xec");   
    }
    else{
        while (1)
        {
            printf("time!!\n");
        }
        
    }

    
    
    // while (1)
    // {
    //     // printf("pid:%d\n",pid);
    //     if(pid > 0)
    //     {
    //         // 父进程执行的逻辑
    //       // printf("father:%d\n",pid);
    //     }
    //     else if(pid == 0)
    //     {
    //         // 子进程执行的逻辑
    //        //printf("child:%d\n",pid);
    //     }
    //     else // pid == -1
    //     {
    //         // 创建子进程失败了
    //     }
    // }
   // sys_exec("xec");
    return 0;
}

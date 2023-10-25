#include <timeros/types.h>
#include <timeros/syscall.h>
#include <timeros/string.h>

int main()
{
   if(sys_fork() == 0)
   {
        sys_exec("user_shell");
   }
   else
   {
    while (1)
    {
        
    }
    
   }
    return 0;
}

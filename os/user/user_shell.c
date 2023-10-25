#include <timeros/types.h>
#include <timeros/syscall.h>
#include <timeros/string.h>

#define LF 0x0a     
#define CR 0x0d   //enter
#define DL 0x7f   
#define BS 0x08   // backspace
#define BUFFER_SIZE 1024
int main()
{
    printf("Timer os user shell\n");
    printf(">> ");
    char line[BUFFER_SIZE];
    while (1)
    {
        // char c = getchar();
        // switch (c)
        // {
        // case CR:
        //     printf("\n");
        //     if(strlen(line) > 0)
        //     {
        //         line[strlen(line)] = '\0';
        //         int pid = sys_fork();
        //         printf("pid:%d\n",pid);
        //         if(pid==0)
        //         {
        //             sys_exec(line);
        //         }
        //     }
        //     break;
        // case BS:
        //     if (strlen(line) > 0) 
        //     {
        //         printf("\b \b");
        //         line[strlen(line) - 1] = '\0';
        //     }
        //     break;
        // default:
        //     printf("%c",c);
        //     strncat(line,(char*)&c,1);
        //     break;
        // }


    }
    
    return 0;
}

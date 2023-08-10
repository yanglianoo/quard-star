#include "os.h"


void os_main()
{
   printf("hello timer os!\n");
   
   trap_init();

   task_init();

   timer_init();
   
   run_first_task();
}
#include <timeros/os.h>

extern void frame_allocator_test();
void os_main()
{
   printf("hello timer os!\n");
   frame_allocator_test();
   while (1)
   {
      /* code */
   }
   
   // trap_init();

   // task_init();

   // timer_init();
   
   // run_first_task();
}
#include <timeros/os.h>
#include <timeros/assert.h>
extern void frame_allocator_test();
void os_main()
{
   printk("hello timer os!\n");
   // frame_allocator_test();
   // while (1)
   // {
   //    /* code */
   // }
   trap_init();

   task_init();

   timer_init();
   
   run_first_task();
}
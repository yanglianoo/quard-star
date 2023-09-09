#include <timeros/os.h>
#include <timeros/assert.h>
#include <timeros/loader.h>
void os_main()
{
   printk("hello timer os!\n");

   // 内存分配器初始化
   frame_alloctor_init();
   
   get_app_data(2);
   //初始化内存
   kvminit();

   //映射内核
   kvminithart();

   //trap初始化
   trap_init();



   while (1)
   {
      /* code */
   }
   
   // task_init();

   // timer_init();

   
   // run_first_task();

}
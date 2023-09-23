#include <timeros/os.h>

void os_main()
{
   printk("hello timer os!\n");

   // 内存分配器初始化
   frame_alloctor_init();
   
   //初始化内存
   kvminit();

   load_app(0);
   app_init(0);
   load_app(1);
   app_init(1);
   //映射内核
   kvminithart();

   //trap初始化
   set_kernel_trap_entry();

   timer_init();

   run_first_task();

}
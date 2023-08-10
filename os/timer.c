#include "os.h"
#define CLOCK_FREQ 10000000 
#define TICKS_PER_SEC 100
static uint64_t timebase = 1000000;


/* 设置下次时钟中断的cnt值 */
void set_next_trigger()
{
    sbi_set_timer(r_mtime() + CLOCK_FREQ / TICKS_PER_SEC);
}

/* 开启S模式下的时钟中断 */
void timer_init()
{
   reg_t sie = r_sie();
   sie |= SIE_STIE;
   w_sie(sie);
   set_next_trigger();
}


/* 以us为单位返回时间 */
uint64_t get_time_us()
{
    r_mtime() / (CLOCK_FREQ / TICKS_PER_SEC);
}
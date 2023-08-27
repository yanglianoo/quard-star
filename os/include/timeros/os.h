#ifndef __OS_H__
#define __OS_H__

#include <stddef.h>
#include <stdarg.h>
#include "types.h"
#include "context.h"
#include "riscv.h"
#include "task.h"

/* printf.c */
extern int  printf(const char* s, ...);
extern void panic(char *s);
extern void sbi_console_putchar(int ch);

/* trap.c */
extern void trap_init();

/* sbi.c */

/* kerneltrap.S*/
extern void __alltraps(void);
extern void __restore(TrapContext *next);

/* syscall */
uint64_t __SYSCALL(size_t syscall_id, reg_t arg1, reg_t arg2, reg_t arg3);
#define __NR_write 64
#define __NR_sched_yield 124
#define __NR_exit 93
#define __NR_gettimeofday 169

/* switch.S */
extern void __switch(TaskContext *current_task_cx_ptr,TaskContext* next_task_cx_ptr );

/* task.c */
extern void schedule();
extern void task_create(void (*task_entry)(void));
extern void run_first_task();

/* app.c */
extern void task_init();

/* timer.c */
void sbi_set_timer(uint64_t stime_value);
void timer_init();
void set_next_trigger();
extern uint64_t get_time_us();
#endif /* __OS_H__ */
#ifndef TOS_OS_H__
#define TOS_OS_H__

#include <stddef.h>
#include <stdarg.h>
#include "types.h"
#include "context.h"
#include "riscv.h"
#include "task.h"
#include "stdio.h"
#include "syscall.h"

/* trap.c */
extern void trap_init();

/* sbi.c */

/* kerneltrap.S*/
extern void __alltraps(void);
extern void __restore(TrapContext *next);


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
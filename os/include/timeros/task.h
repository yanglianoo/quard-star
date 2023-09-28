#ifndef TOS_TASK_H__
#define TOS_TASK_H__

#include <timeros/os.h>

#define MAX_TASKS 10

typedef enum TaskState
{
	UnInit, // 未初始化
    Ready, // 准备运行
    Running, // 正在运行
    Exited, // 已退出
}TaskState;

typedef struct TaskControlBlock
{
    TaskState task_state;       //任务状态
    int pid;                    // Process ID
    TaskControlBlock* parent;  //Parent process
    TaskContext task_context;   //任务上下文
    u64 trap_cx_ppn;            //Trap 上下文所在物理地址
    u64  base_size;             //应用数据大小
    u64  kstack;                //应用内核栈的虚拟地址
    u64  ustack;                //应用用户栈的虚拟地址
    u64  entry;                 //应用程序入口地址
    PageTable pagetable;        //应用页表所在物理页
}TaskControlBlock;

/* 映射用户程序内核栈 */
void proc_mapstacks(PageTable* kpgtbl);
/* 创建应用页表 */
TaskControlBlock*  task_create_pt(size_t app_id);
/* 初始化应用程序 */
void app_init(size_t app_id);
/* 获取当前执行应用程序trap上下文地址 */
u64 get_current_trap_cx();
/*返回当前执行的应用程序的satp token*/
u64 current_user_token();
/* 任务调度*/
void schedule();
/* 启动第一个任务*/
void run_first_task();
#endif




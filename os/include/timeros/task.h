#ifndef TOS_TASK_H__
#define TOS_TASK_H__

#include "os.h"
typedef enum TaskState
{
	UnInit, // 未初始化
    Ready, // 准备运行
    Running, // 正在运行
    Exited, // 已退出
}TaskState;

typedef struct TaskControlBlock
{
    TaskState task_state; 
    TaskContext task_context; 
}TaskControlBlock;



#endif




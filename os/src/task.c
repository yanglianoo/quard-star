#include <timeros/os.h>

#define USER_STACK_SIZE (4096 * 2)
#define KERNEL_STACK_SIZE (4096 * 2)

#define MAX_TASKS 10
static int _current = 0;
static int _top = 0;

uint8_t KernelStack[MAX_TASKS][KERNEL_STACK_SIZE];
uint8_t UserStack[MAX_TASKS][USER_STACK_SIZE];

struct TaskControlBlock tasks[MAX_TASKS];


struct TaskContext tcx_init(reg_t kstack_ptr) {
    struct TaskContext task_ctx;

    task_ctx.ra = trap_return;
    task_ctx.sp = kstack_ptr;
    task_ctx.s0 = 0;
    task_ctx.s1 = 0;
    task_ctx.s2 = 0;
    task_ctx.s3 = 0;
    task_ctx.s4 = 0;
    task_ctx.s5 = 0;
    task_ctx.s6 = 0;
    task_ctx.s7 = 0;
    task_ctx.s8 = 0;
    task_ctx.s9 = 0;
    task_ctx.s10 = 0;
    task_ctx.s11 = 0;

    return task_ctx;
}


/* 为每个应用程序映射内核栈,内核空间以及进行了映射 */
void proc_mapstacks(PageTable* kpgtbl)
{
  struct TaskControlBlock *p;
  
  for(p = tasks; p < &tasks[MAX_TASKS]; p++) {
    char *pa = (char*)phys_addr_from_phys_page_num(kalloc()).value;
    if(pa == 0)
      panic("kalloc");
    u64 va = KSTACK((int) (p - tasks));
    PageTable_map(kpgtbl, virt_addr_from_size_t(va + PAGE_SIZE), phys_addr_from_size_t((u64)pa), \
                  PAGE_SIZE, PTE_R | PTE_W);
    // 给应用内核栈赋值 
    p->kstack = va + 2 * PAGE_SIZE;
  }
}


/* 为每个应用程序分配一页内存用与存放trap，同时初始化任务上下文 */
void proc_trap(struct TaskControlBlock *p)
{
  // 为每个程序分配一页trap物理内存
  p->trap_cx_ppn = phys_addr_from_phys_page_num(kalloc()).value;
  // 初始化任务上下文全部为0
  memset(&p->task_context, 0 ,sizeof(p->task_context));
}

extern char trampoline[];
/* 为用户程序创建页表，映射跳板页和trap上下文页*/
void proc_pagetable(struct TaskControlBlock *p)
{
  // 创建一个空的用户的页表，分配一页内存
  PageTable pagetable;
  pagetable.root_ppn = kalloc();
  
  //映射跳板页
  PageTable_map(&pagetable,virt_addr_from_size_t(TRAMPOLINE),phys_addr_from_size_t((u64)trampoline),\
                PAGE_SIZE , PTE_R | PTE_X);
  //映射用户程序的trap页
  PageTable_map(&pagetable,virt_addr_from_size_t(TRAPFRAME),phys_addr_from_size_t(p->trap_cx_ppn), \
                PAGE_SIZE, PTE_R | PTE_W );
  p->pagetable = pagetable;
}

TaskControlBlock* task_create_pt(size_t app_id)
{
  if(_top < MAX_TASKS)
  {

    //为应用程序分配一页内存用与存放trap
    proc_trap(&tasks[app_id]);
    //为用户程序创建页表，映射跳板页和trap上下文页
    proc_pagetable(&tasks[app_id]); 
    _top++;
  }
  
  return &tasks[app_id];
}

extern u64 kernel_satp;
void app_init(size_t app_id)
{
    TrapContext* cx_ptr = tasks[app_id].trap_cx_ppn;
    reg_t sstatus = r_sstatus();
    // 设置 sstatus 寄存器第8位即SPP位为0 表示为U模式
    sstatus &= (0U << 8);
    w_sstatus(sstatus);
    // 设置程序入口地址
    cx_ptr->sepc = tasks[app_id].entry;
    // 
    cx_ptr->sstatus = sstatus; 
    // 设置用户栈虚拟地址
    cx_ptr->sp = tasks[app_id].ustack;
    // 设置内核页表token
    cx_ptr->kernel_satp = kernel_satp;
    // 设置内核栈虚拟地址
    cx_ptr->kernel_sp = tasks[app_id].kstack;
    // 设置内核trap_handler的地址
    cx_ptr->trap_handler = (u64)trap_handler;

    /* 构造每个任务任务控制块中的任务上下文，设置 ra 寄存器为 trap_return 的入口地址*/
    tasks[app_id].task_context = tcx_init((reg_t)cx_ptr);
    // 初始化 TaskStatus 字段为 Ready
    tasks[app_id].task_state = Ready;
}

/* 返回当前执行的应用程序的trap上下文的地址 */
u64 get_current_trap_cx()
{
  return tasks[_current].trap_cx_ppn;
}
/* 返回当前执行的应用程序的satp token*/
u64 current_user_token()
{
   return MAKE_SATP(tasks[_current].pagetable.root_ppn.value);
}
void schedule()
{
	if (_top <= 0) {
		panic("Num of task should be greater than zero!\n");
		return;
	}
    
    /* 轮转调度 */
    int next = _current + 1;
    next = next % _top;

    if(tasks[next].task_state == Ready)
    {
        struct TaskContext *current_task_cx_ptr = &(tasks[_current].task_context);
        struct TaskContext *next_task_cx_ptr = &(tasks[next].task_context);
        tasks[next].task_state = Running;
        tasks[_current].task_state = Ready;
        _current = next;
        __switch(current_task_cx_ptr,next_task_cx_ptr);
    }
    
}

void run_first_task()
{
    tasks[0].task_state = Running;
    struct TaskContext *next_task_cx_ptr = &(tasks[0].task_context);
    struct TaskContext _unused ;
    __switch(&_unused,next_task_cx_ptr);
    panic("unreachable in run_first_task!");
}
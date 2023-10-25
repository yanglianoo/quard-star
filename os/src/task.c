#include <timeros/os.h>

static int _current = 0;
static int _top = 0;

int nextpid = 1;


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

void procinit()
{
  struct TaskControlBlock *p;
  for(p = tasks; p < &tasks[MAX_TASKS]; p++)
  {
    p->task_state = UnInit;
  }
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
    PageTable_map(kpgtbl, virt_addr_from_size_t(va ), phys_addr_from_size_t((u64)pa), \
                  PAGE_SIZE, PTE_R | PTE_W);
    // 给应用内核栈赋值 
    p->kstack = va +  PAGE_SIZE;
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


void proc_ustack(struct TaskControlBlock *p)
{
      // 映射应用程序用户栈开始地址
    PhysPageNum ppn = kalloc();
    u64 paddr = phys_addr_from_phys_page_num(ppn).value;
    PageTable_map(&p->pagetable,virt_addr_from_size_t(p->ustack - PAGE_SIZE),phys_addr_from_size_t(paddr), \
                  PAGE_SIZE, PTE_R | PTE_W | PTE_U);
}

TaskControlBlock* task_create_pt(size_t app_id)
{
  if(_top < MAX_TASKS)
  {

    //为应用程序分配一页内存用与存放trap
    proc_trap(&tasks[app_id]);   // 
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
    tasks[app_id].task_context = tcx_init((reg_t)tasks[app_id].kstack);
    // 初始化 TaskStatus 字段为 Ready
    tasks[app_id].task_state = Ready;
    /* 分配pid值 */
    tasks[app_id].pid = allocpid();
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

struct TaskControlBlock* current_proc()
{
  return &tasks[_current];
}

int allocpid()
{
  int pid;
  pid = nextpid;
  nextpid = nextpid + 1;
  return pid;
}

struct TaskControlBlock* allocproc()
{
  struct TaskControlBlock* p;
  for(p = tasks; p < &tasks[MAX_TASKS]; p++)
  {
    if(p->task_state == UnInit)
    {
      goto found;
    }
  }
  return 0;

found:
      p->pid = allocpid();
      p->task_state = Ready;
      // 为每个应用程序分配一页内存用与存放trap，同时初始化任务上下文
      proc_trap(p);
      // 为用户程序创建页表，映射跳板页和trap上下文页
      proc_pagetable(p);
  return p;
}

int __sys_fork()
{
  struct TaskControlBlock* np;
  struct TaskControlBlock* p = current_proc();
  // Allocate process.
  if((np = allocproc()) == 0){
    return -1;
  }

  // 拷贝父进程的内存数据，根据页表查找物理页拷贝
  uvmcopy(&p->pagetable,&np->pagetable,p->base_size);

  // 拷贝父进程的trap页数据
  memcpy((void*)np->trap_cx_ppn,(void*)p->trap_cx_ppn,PAGE_SIZE);

  // 子进程返回值为0
  TrapContext* cx_ptr = np->trap_cx_ppn;
  cx_ptr->a0 = 0;
  // 复制TCB的信息
  np->entry = p->entry;
  np->base_size = p->base_size;
  np->parent = p;
  np->ustack = p->ustack;
  // 设置子进程返回地址和内核栈
  np->task_context.ra = trap_return;
  np->task_context.sp = np->kstack;

  _top++;
  return np->pid;
}



int exec(const char* name)
{

    AppMetadata metadata = get_app_data_by_name(name);
    if(metadata.id<0)
    {
      return -1;
    }
    //ELF 文件头
    elf64_ehdr_t *ehdr = metadata.start;
    elf_check(ehdr);

    struct TaskControlBlock* proc = current_proc();
    PageTable old_pagetable = proc->pagetable;
    u64 oldsz = proc->base_size;
    //重新分配页表
    proc_pagetable(proc);
    //加载程序段
    load_segment(ehdr,proc);
    // 映射应用程序用户栈开始地址
    proc_ustack(proc);

    TrapContext* cx_ptr = proc->trap_cx_ppn;
    cx_ptr->sepc = (u64)ehdr->e_entry;
    cx_ptr->sp = proc->ustack;
    reg_t sstatus = r_sstatus();
    // 设置 sstatus 寄存器第8位即SPP位为0 表示为U模式
    sstatus &= (0U << 8);
    w_sstatus(sstatus);
    cx_ptr->sstatus = sstatus; 
    // 设置内核页表token
    cx_ptr->kernel_satp = kernel_satp;
    // 设置内核栈虚拟地址
    cx_ptr->kernel_sp = proc->kstack;
    // 设置内核trap_handler的地址
    cx_ptr->trap_handler = (u64)trap_handler;

    proc_freepagetable(&old_pagetable,oldsz);

    return 0;
}


void freeproc(struct TaskControlBlock* p)
{
    proc_freepagetable(&p->pagetable, p->base_size);

    p->pagetable.root_ppn.value = 0;
    p->base_size = 0;
    p->parent =  0;
    p->ustack = 0;
    p->entry = 0;
    p->task_state = UnInit;
    p->exit_code = 0;
}

void children_proc_clear(struct TaskControlBlock *p)
{
  struct TaskControlBlock *children;
  for(p = tasks; p < &tasks[MAX_TASKS]; p++)
  {
    if(children->parent == p)
    {
      children->parent = &tasks[0];
    }
  }
}

void exit_current_and_run_next(u64 exit_code)
{
  /* 不能把0号进程干掉了 */
  struct TaskControlBlock* p = current_proc();
  if(p->pid == 0)
  {
    panic("init exiting");
  }

  p->exit_code = exit_code;
  p->task_state = Zombie;
  children_proc_clear(p);
  schedule();
}


int wait()
{
  struct TaskControlBlock *children;
  struct TaskControlBlock* p = current_proc();
  int pid,havekids;

  for(;;)
  {
    havekids = 0;
    for(children = tasks; children < &tasks[MAX_TASKS];children++)
    {
      if(children->parent == p)
      {
        havekids = 1;
        if(children->exit_code == Zombie)
        {

          pid = children->pid;
          freeproc(children);
          return pid;
        }
      }
    }
    // 如果此进程没有子进程，则返回 -1
    if(!havekids)
    {
      return -1;
    }
  }
}


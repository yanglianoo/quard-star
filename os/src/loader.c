#include <timeros/loader.h>
#include <timeros/address.h>
extern u64 _num_app[];
extern char _app_names[];
static char* app_names[MAX_TASKS];
// 获取加载的app数量
size_t get_num_app()
{
    return _num_app[0];
}

/* 根据app id加载app的数据*/
AppMetadata  get_app_data(size_t app_id)
{
    AppMetadata metadata;

    size_t num_app = get_num_app();

    metadata.start = _num_app[app_id];  // 获取app起始地址

    metadata.size = _num_app[app_id+1] - _num_app[app_id];    // 获取app结束地址  

    metadata.id = app_id;
    assert(app_id <= num_app);

    return metadata;
}

/* 根据app的名字返回app的id */
AppMetadata get_app_data_by_name(char* path)
{
    AppMetadata metadata;
    int app_num = get_num_app();
    for (size_t i = 0; i < app_num; i++)
    {
        if(strcmp(path,app_names[i])==0)
        {
           metadata =  get_app_data(i);
           printk("find app:%s\n",path);
           return metadata;
        }
    }
    printk("not exit!!\n");
}

void get_app_names()
{
    int app_num = get_num_app();
    printk("/**** APPS ****\n");
    for (size_t i = 0; i < app_num; i++)
    {
        if(i==0)
        {
            size_t len = strlen(_app_names);
            app_names[0] = _app_names;
        }
        else
        {
            size_t len = strlen(app_names[i-1]);
            app_names[i] = (char*)((u64)_app_names + i * len + 1);
        }

        printk("%s\n",app_names[i]);
        
    }
    printk("**************/\n");
    
}

static u8 flags_to_mmap_prot(u8 flags)
{
    return (flags & PF_R ? PTE_R : 0) | 
           (flags & PF_W ? PTE_W : 0) |
           (flags & PF_X ? PTE_X : 0);
}

void load_app(size_t app_id)

{
    //加载ELF文件
    AppMetadata metadata = get_app_data(app_id + 1);

    //ELF 文件头
    elf64_ehdr_t *ehdr = metadata.start;

    //判断 elf 文件的魔数
    assert(*(u32 *)ehdr==ELFMAG);
    
    //判断传入文件是否为 riscv64 的
    if (ehdr->e_machine != EM_RISCV || ehdr->e_ident[EI_CLASS] != ELFCLASS64)
    {
        panic("only riscv64 elf file is supported");
    }

    //记录APP程序的入口地址，为 main 函数地址
    u64 entry = (u64)ehdr->e_entry;
    //创建任务
    TaskControlBlock* proc = task_create_pt(app_id);
    //赋值任务的 entry
    proc->entry = entry;
    // Program Header 解析
    elf64_phdr_t *phdr;
    //遍历每一个逻辑段
    for (size_t i = 0; i < ehdr->e_phnum; i++)
    {
        //拿到每个Program Header的指针
        phdr =(u64) (ehdr->e_phoff + ehdr->e_phentsize * i + metadata.start);
        if(phdr->p_type == PT_LOAD)
        {
            // 获取映射内存段开始位置
            u64 start_va = phdr->p_vaddr;
            // 获取映射内存段结束位置
            proc->ustack = start_va + phdr->p_memsz;
            //  转换elf的可读，可写，可执行的 flags
            u8 map_perm = PTE_U | flags_to_mmap_prot(phdr->p_flags);
            // 获取映射内存大小,需要向上对齐
            u64 map_size = PGROUNDUP(phdr->p_memsz);
            for (size_t j = 0; j < map_size; j+= PAGE_SIZE)
            {
                // 分配物理内存，加载程序段，然后映射
                PhysPageNum ppn = kalloc();
                    //获取到分配的物理内存的地址
                u64 paddr = phys_addr_from_phys_page_num(ppn).value;
                memcpy(paddr, metadata.start + phdr->p_offset + j, PAGE_SIZE);
                    //内存逻辑段内存映射
                PageTable_map(&proc->pagetable,virt_addr_from_size_t(start_va + j), \
                                phys_addr_from_size_t(paddr), PAGE_SIZE , map_perm);
                
            }
        
            
        }
    }

    // 映射应用程序用户栈开始地址
    proc->ustack =  2 * PAGE_SIZE + PGROUNDUP(proc->ustack);
    PhysPageNum ppn = kalloc();
    u64 paddr = phys_addr_from_phys_page_num(ppn).value;
    PageTable_map(&proc->pagetable,virt_addr_from_size_t(proc->ustack - PAGE_SIZE),phys_addr_from_size_t(paddr), \
                  PAGE_SIZE, PTE_R | PTE_W | PTE_U);
    proc->base_size=proc->ustack;
}



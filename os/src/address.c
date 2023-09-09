#include <timeros/address.h>



PhysAddr phys_addr_from_size_t(uint64_t v) {
    PhysAddr addr;
    addr.value = v & ((1ULL << PA_WIDTH_SV39) - 1);
    return addr;
}

PhysPageNum phys_page_num_from_size_t(uint64_t v) {
    PhysPageNum pageNum;
    pageNum.value = v & ((1ULL << PPN_WIDTH_SV39) - 1);
    return pageNum;
}

uint64_t size_t_from_phys_addr(PhysAddr v) {
    return v.value;
}

uint64_t size_t_from_phys_page_num(PhysPageNum v) {
    return v.value;
}
/* 从物理页号转换为实际物理地址 */
PhysAddr phys_addr_from_phys_page_num(PhysPageNum ppn)
{
    PhysAddr addr;
    addr.value = ppn.value << PAGE_SIZE_BITS ;
    return addr;
}

VirtAddr virt_addr_from_size_t(uint64_t v) {
    VirtAddr addr;
    addr.value = v & ((1ULL << VA_WIDTH_SV39) - 1);
    return addr;
}

VirtPageNum virt_page_num_from_size_t(uint64_t v) {
    VirtPageNum pageNum;
    pageNum.value = v & ((1ULL << VPN_WIDTH_SV39) - 1);
    return pageNum;
}

uint64_t size_t_from_virt_addr(VirtAddr v) {
    if (v.value >= (1ULL << (VA_WIDTH_SV39 - 1))) {
        return v.value | ~((1ULL << VA_WIDTH_SV39) - 1);
    } else {
        return v.value;
    }
}

uint64_t size_t_from_virt_page_num(VirtPageNum v) {
    return v.value;
}

/* 物理地址向下取整 */
PhysPageNum floor_phys(PhysAddr phys_addr) {
    PhysPageNum phys_page_num;
    phys_page_num.value = phys_addr.value / PAGE_SIZE;
    return phys_page_num;
}

/* 物理地址向上取整 */
PhysPageNum ceil_phys(PhysAddr phys_addr) {
    PhysPageNum phys_page_num;
    phys_page_num.value = (phys_addr.value + PAGE_SIZE - 1) / PAGE_SIZE;
    return phys_page_num;
}

/* 虚拟地址向下取整 */
VirtPageNum floor_virts(VirtAddr virt_addr) {
    VirtPageNum virt_page_num;
    virt_page_num.value = virt_addr.value / PAGE_SIZE;
    return virt_page_num;
}

/* 把虚拟地址转换为虚拟页号 */
VirtPageNum virt_page_num_from_virt_addr(VirtAddr virt_addr)
{
    VirtPageNum vpn;
    vpn.value =  virt_addr.value / PAGE_SIZE;
    return vpn;
}




/* 新建一个页表项 */
PageTableEntry PageTableEntry_new(PhysPageNum ppn, uint8_t PTEFlags) {
    PageTableEntry entry;
    entry.bits = (ppn.value << 10) | PTEFlags;
    return entry;
}

/* 判断页表项是否为空 */
PageTableEntry PageTableEntry_empty() {
    PageTableEntry entry;
    entry.bits = 0;
    return entry;
}

/* 获取下级页表的物理页号 */
PhysPageNum PageTableEntry_ppn(PageTableEntry *entry) {
    PhysPageNum ppn;
    ppn.value = (entry->bits >> 10) & ((1ul << 44) - 1);
    return ppn;
}

/* 获取页表项的标志位 */
uint8_t PageTableEntry_flags(PageTableEntry *entry) {
    return entry->bits & 0xFF;
}

/* 判断页表项是否为空 */
bool PageTableEntry_is_valid(PageTableEntry *entry) {
    uint8_t entryFlags = PageTableEntry_flags(entry);
    return (entryFlags & PTE_V) != 0;
}

uint8_t* get_bytes_array(PhysPageNum ppn)
{
    // 先从物理页号转换为物理地址
    PhysAddr addr = phys_addr_from_phys_page_num(ppn);
    return (uint8_t*) addr.value;
}

PageTableEntry* get_pte_array(PhysPageNum ppn)
{
    // 先从物理页号转换为物理地址
    PhysAddr addr = phys_addr_from_phys_page_num(ppn);
    return (PageTableEntry*) addr.value;
}

// void* get_mut(PhysPageNum ppn) {

//     PhysAddr addr = phys_addr_from_phys_page_num(ppn);
//     return (void*)addr.value; // Assuming addr is a valid memory address
// }


typedef struct 
{
    uint64_t current;   //空闲内存的起始物理页号
    uint64_t  end;      //空闲内存的结束物理页号
    Stack recycled;     // 
}StackFrameAllocator;


void StackFrameAllocator_new(StackFrameAllocator* allocator) {
    allocator->current = 0;
    allocator->end = 0;
    initStack(&allocator->recycled);
}

void StackFrameAllocator_init(StackFrameAllocator *allocator, PhysPageNum l, PhysPageNum r) {
    allocator->current = l.value;
    allocator->end = r.value;
}

PhysPageNum StackFrameAllocator_alloc(StackFrameAllocator *allocator) {
    PhysPageNum ppn;
    if (allocator->recycled.top >= 0) {
        ppn.value = pop(&(allocator->recycled));
    } else {
        if (allocator->current == allocator->end) {
            ppn.value = 0; // Return 0 as None
        } else {
            ppn.value = allocator->current++;
        }
    }
    /* 清空此页内存 ： 注意不能覆盖内核代码区，分配的内存只能是未使用部分*/
    PhysAddr addr = phys_addr_from_phys_page_num(ppn);
    memset(addr.value,0,PAGE_SIZE);
    // uint8_t* ptr = get_bytes_array(ppn);
    // for (size_t i = 0; i < PAGE_SIZE; i++)
    // {
        
    //     printk("%d",ptr[i]);
    //    // ptr++;
    // }
    return ppn;
}

void StackFrameAllocator_dealloc(StackFrameAllocator *allocator, PhysPageNum ppn) {
    uint64_t ppnValue = ppn.value;
    // 检查回收的页面之前一定被分配出去过
    if (ppnValue >= allocator->current) {
        printk("Frame ppn=%lx has not been allocated!\n", ppnValue);
        return;
    }
    // 检查未在回收列表中
    if(allocator->recycled.top>=0)
    {
        for (size_t i = 0; i <= allocator->recycled.top; i++)
        {
            if(ppnValue ==allocator->recycled.data[i] )
            return;
        }
    }
    // 回收物理内存页号
    push(&(allocator->recycled), ppnValue);
}





StackFrameAllocator FrameAllocatorImpl;
extern char kernelend[];
void frame_alloctor_init()
{
    // 初始化时 kernelend 需向上取整
    StackFrameAllocator_new(&FrameAllocatorImpl);
    StackFrameAllocator_init(&FrameAllocatorImpl, \
            ceil_phys(phys_addr_from_size_t(kernelend)), \
            ceil_phys(phys_addr_from_size_t(PHYSTOP)));
    printk("Memoery start:%p\n",kernelend);
    printk("Memoery end:%p\n",PHYSTOP);
}




/* 拿到虚拟页号的三级索引，按照从高到低的顺序返回 */
void indexes(VirtPageNum vpn, size_t* result) 
{
    size_t idx[3];
    for (int i = 2; i >= 0; i--) {
        idx[i] = vpn.value & 0x1ff;   // 1_1111_1111 = 0x1ff
        vpn.value >>= 9;
       // printk("result:%d\n",idx[i]);
    }

    for (int i = 0; i < 3; i++) {
        result[i] = idx[i];
      //  printk("result:%d\n",result[i]);
    }
}





PageTableEntry* find_pte_create(PageTable* pt,VirtPageNum vpn)
{
    
    // 拿到虚拟页号的三级索引，保存到idx数组中
    size_t idx[3];
    indexes(vpn, idx); 
    //根节点
    PhysPageNum ppn = pt->root_ppn;
    //从根节点开始遍历，如果没有pte，就分配一页内存，然后创建一个
    for (int i = 0; i < 3; i++) 
    {
        //拿到具体的页表项
        PageTableEntry* pte =  &get_pte_array(ppn)[idx[i]];
            if (i == 2) {
                return pte;
            }
        //如果此项页表为空
            if (!PageTableEntry_is_valid(pte)) {
                //分配一页物理内存
                PhysPageNum frame =  StackFrameAllocator_alloc(&FrameAllocatorImpl);
               //新建一个页表项
               *pte =  PageTableEntry_new(frame,PTE_V);
               //压入栈中
               // push(&pt->frames,frame.value);
            }
        //取出进入下级页表的物理页号
        ppn = PageTableEntry_ppn(pte);
    }
}

PageTableEntry* find_pte(PageTable* pt, VirtPageNum vpn)
{
    // 拿到虚拟页号的三级索引，保存到idx数组中
    size_t* idx;
    indexes(vpn, idx); 
    //根节点
    PhysPageNum ppn = pt->root_ppn;
    //从根节点开始遍历，如果没有pte，就分配一页内存，然后创建一个
    for (int i = 0; i < 3; i++) 
    {
        //拿到具体的页表项
        PageTableEntry* pte =  &get_pte_array(ppn)[idx[i]];
            if (i == 2) {
                return pte;
            }
        //如果此项页表为空
            if (!PageTableEntry_is_valid(pte)) {
                return NULL;
            }
        //取出进入下级页表的物理页号
        ppn = PageTableEntry_ppn(pte);
    }
    
}

void PageTable_map(PageTable* pt,VirtAddr va, PhysAddr pa, u64 size ,uint8_t pteflgs)
{
    if(size == 0)
        panic("mappages: size");
    
    PhysPageNum ppn = floor_phys(pa);
    VirtPageNum vpn = floor_virts(va);
    u64 last = (va.value + size - 1) / PAGE_SIZE;
    for(;;)
    {
        PageTableEntry* pte = find_pte_create(pt,vpn);
        assert(!PageTableEntry_is_valid(pte));
        *pte = PageTableEntry_new(ppn,PTE_V | pteflgs);
         
        if( vpn.value == last )
            break;
        
        // 一页一页映射
        vpn.value+=1;
        ppn.value+=1;
    }
}

/* 取消映射的函数先不管 */
void PageTable_unmap(PageTable* pt, VirtPageNum vpn)
{
    PageTableEntry* pte = find_pte(pt,vpn);
    assert(!PageTableEntry_is_valid(pte));
    *pte = PageTableEntry_empty();
}

extern char etext[];

PageTable kvmmake(void)
{
    PageTable pt;
    PhysPageNum root_ppn =  StackFrameAllocator_alloc(&FrameAllocatorImpl);
    pt.root_ppn = root_ppn;
    printk("root_ppn:%p\n",phys_addr_from_phys_page_num(root_ppn));

    printk("etext:%p\n",(u64)etext);
    // map kernel text executable and read-only.
    PageTable_map(&pt,virt_addr_from_size_t(KERNBASE),phys_addr_from_size_t(KERNBASE), \
                    (u64)etext-KERNBASE , PTE_R | PTE_X ) ;
    printk("finish kernel text map!\n");
    // map kernel data and the physical RAM we'll make use of. 
    PageTable_map(&pt,virt_addr_from_size_t((u64)etext),phys_addr_from_size_t((u64)etext ), \
                    PHYSTOP - (u64)etext , PTE_R | PTE_W ) ;
    printk("finish kernel data and physical RAM map!\n");
    return pt;
}

PageTable kernel_pagetable;
void kvminit()
{
  kernel_pagetable = kvmmake();
}

#define SATP_SV39 (8L << 60)
#define MAKE_SATP(pagetable) (SATP_SV39 | (((u64)pagetable)))

void kvminithart()
{
  // wait for any previous writes to the page table memory to finish.
  sfence_vma();
  w_satp(MAKE_SATP(kernel_pagetable.root_ppn.value));
  // flush stale entries from the TLB.
  sfence_vma();
  reg_t satp = r_satp();
  printk("satp:%lx\n",satp);
}







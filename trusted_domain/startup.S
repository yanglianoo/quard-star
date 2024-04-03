#include "riscv_encoding.h"
#include "quard_star.h"
#include "riscv-reg.h"


	.section .init
	.globl _start
	.type _start,@function
_start:  #程序入口地址
	.cfi_startproc
	.cfi_undefined ra
.option push
.option norelax  #取消汇编器的"relaxation"优化
	la  gp, __global_pointer$   #设置全局指针
.option pop
	// Continue primary hart
	li   a1, PRIM_HART     #将核心号加载到a1寄存器中
	bne  a0, a1, secondary #比较核心号是否相等，如果不等，跳转到secondary处执行

  	csrw sie, 0     #sie寄存器用于控制中断，禁用所有中断
  	csrw sip, 0     #sip用于标识中断中断标志位，清楚所有的中断的标志位
	/* set to disable FPU */
	li t0, SSTATUS_FS
	csrc sstatus, t0
	li t0, SSTATUS_SUM // SUM in sstatus
	csrs sstatus, t0

	// Primary hart  设置主栈指针
	la sp, _stack_top
	csrw sscratch, sp  //将栈指针写入sscratch寄存器

	// Load data section
	la a0, _data_lma    //data数据在物理内存中的地址
	la a1, _data        //数据段起始地址
	la a2, _edata       //数据段结束地址
	bgeu a1, a2, 2f     //
1:   //将数据段的数据从物理内存复制到dram
	LOAD t0, (a0)
	STOR t0, (a1)
	addi a0, a0, REGSIZE
	addi a1, a1, REGSIZE
	bltu a1, a2, 1b       //b代表向上寻找最近的同名标签

2:
	// Clear bss section
	la a0, _bss
	la a1, _ebss
	bgeu a0, a1, 2f      //f代表向下寻找最近的同名标签
1:  //清空bss段
	STOR zero, (a0)
	addi a0, a0, REGSIZE
	bltu a0, a1, 1b    
2:

	// argc, argv, envp is 0
	li  a0, 0
	li  a1, 0
	li  a2, 0
	jal main
1:
	wfi
	j 1b

ecall_err:
	wfi
	j ecall_err
	
secondary:
	// TODO: Multicore is not supported
	wfi
	j secondary
	.cfi_endproc

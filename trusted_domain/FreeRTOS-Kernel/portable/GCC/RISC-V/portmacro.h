/*
 * FreeRTOS Kernel V10.4.3 LTS Patch 3
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 * 1 tab == 4 spaces!
 */


#ifndef PORTMACRO_H
#define PORTMACRO_H

#include "quart_star.h"
#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions. */
#if __riscv_xlen == 64
	#define portSTACK_TYPE			uint64_t
	#define portBASE_TYPE			int64_t
	#define portUBASE_TYPE			uint64_t
	#define portMAX_DELAY 			( TickType_t ) 0xffffffffffffffffUL
	#define portPOINTER_SIZE_TYPE 	uint64_t
#elif __riscv_xlen == 32
	#define portSTACK_TYPE	uint32_t
	#define portBASE_TYPE	int32_t
	#define portUBASE_TYPE	uint32_t
	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#else
	#error Assembler did not define __riscv_xlen
#endif

//数据类型定义，分为32位 和 64 位
typedef portSTACK_TYPE StackType_t;  
typedef portBASE_TYPE BaseType_t;    
typedef portUBASE_TYPE UBaseType_t;
typedef portUBASE_TYPE TickType_t;

/* Legacy type definitions. */
#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		double
#define portLONG		long
#define portSHORT		short

/* 32-bit tick type on a 32-bit architecture, so reads of the tick count do
not need to be guarded with a critical section. */
/*portTICK_TYPE_IS_ATOMIC这个宏定义的设定通常用于指示系统的tick计数类型是
否可以在不使用临界区（critical section）的情况下安全地读取。*/
#define portTICK_TYPE_IS_ATOMIC 1
/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define portSTACK_GROWTH			( -1 )  //定义了栈的增长方向
#define portTICK_PERIOD_MS			( ( TickType_t ) 1000 / configTICK_RATE_HZ ) //操作系统的tick周期（以毫秒为单位）
#ifdef __riscv64
	#error This is the RV32 port that has not yet been adapted for 64.
	#define portBYTE_ALIGNMENT			16
#else
	#define portBYTE_ALIGNMENT			16   //所有数据结构都应该按16字节对齐
#endif
/*-----------------------------------------------------------*/


/* Scheduler utilities. */
extern void vTaskSwitchContext( void ); //上下文调度函数，不同架构不同
/* 通过sbi触发自己核心的软中断 */
extern void sbi_send_ipi(const unsigned long *hart_mask);
//#define portYIELD() __asm volatile( "ecall" );  原本是在m态通过ecall触发软中断
#define portYIELD() sbi_send_ipi((const unsigned long *)(0x1<<PRIM_HART)) //向hart发送中断信号
#define portEND_SWITCHING_ISR( xSwitchRequired ) if( xSwitchRequired ) vTaskSwitchContext()
#define portYIELD_FROM_ISR( x ) portEND_SWITCHING_ISR( x )
/*-----------------------------------------------------------*/


/* Critical section management. */
#define portCRITICAL_NESTING_IN_TCB					1
extern void vTaskEnterCritical( void );   //进入临界区
extern void vTaskExitCritical( void );    //退出临界区


#define portSET_INTERRUPT_MASK_FROM_ISR() 0
#define portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedStatusValue ) ( void ) uxSavedStatusValue


/* S模式下SIE全局中断控制  bit[1] 位SIE位用于控制s态中断*/
#define portDISABLE_INTERRUPTS()	__asm volatile( "csrc sstatus, 2" ) //csr clear
#define portENABLE_INTERRUPTS()		__asm volatile( "csrs sstatus, 2" ) //csr set
#define portENTER_CRITICAL()	vTaskEnterCritical()
#define portEXIT_CRITICAL()		vTaskExitCritical()

/*-----------------------------------------------------------*/

/* Architecture specific optimisations. */
/*使用一个32位的整数作为bitmap来存储优先级*/
#ifndef configUSE_PORT_OPTIMISED_TASK_SELECTION
	#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
#endif

#if( configUSE_PORT_OPTIMISED_TASK_SELECTION == 1 )

	/* Check the configuration. */
	#if( configMAX_PRIORITIES > 32 ) //检查系统配置的最大优先级数configMAX_PRIORITIES）是否超过了32。
		#error configUSE_PORT_OPTIMISED_TASK_SELECTION can only be set to 1 when configMAX_PRIORITIES is less than or equal to 32.  It is very rare that a system requires more than 10 to 15 difference priorities as tasks that share a priority will time slice.
	#endif

	/* Store/clear the ready priorities in a bit map. */
	//portRECORD_READY_PRIORITY通过位或操作（|=）来设置指定优先级对应位为1，表示有任务在该优先级上就绪。
	//portRESET_READY_PRIORITY通过位与操作（&=~）来清除指定优先级对应的位，表示该优先级上没有就绪任务。
	#define portRECORD_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( uxPriority ) ) /
	#define portRESET_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( uxPriority ) )

	/*-----------------------------------------------------------*/
	//portGET_HIGHEST_PRIORITY：这个宏用于快速找到就绪优先级位图中最高优先级的就绪任务。
	//__builtin_clz 是一个编译器支持的计算前导0个数的内建函数
	#define portGET_HIGHEST_PRIORITY( uxTopPriority, uxReadyPriorities ) uxTopPriority = ( 31UL - __builtin_clz( uxReadyPriorities ) )

#endif /* configUSE_PORT_OPTIMISED_TASK_SELECTION */


/*-----------------------------------------------------------*/

/* Task function macros as described on the FreeRTOS.org WEB site.  These are
not necessary for to use this port.  They are defined so the common demo files
(which build with all the ports) will build. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )

/*-----------------------------------------------------------*/

#define portNOP() __asm volatile 	( " nop " ) //无操作指令

#define portINLINE	__inline

#ifndef portFORCE_INLINE   //强制内联
	#define portFORCE_INLINE inline __attribute__(( always_inline))
#endif

//内存屏障指令
#define portMEMORY_BARRIER() __asm volatile( "" ::: "memory" )
/*-----------------------------------------------------------*/


/* configCLINT_BASE_ADDRESS is a legacy definition that was replaced by the
configMTIME_BASE_ADDRESS and configMTIMECMP_BASE_ADDRESS definitions.  For
backward compatibility derive the newer definitions from the old if the old
definition is found. */
// #if defined( configCLINT_BASE_ADDRESS ) && !defined( configMTIME_BASE_ADDRESS ) && ( configCLINT_BASE_ADDRESS == 0 )
// 	/* Legacy case where configCLINT_BASE_ADDRESS was defined as 0 to indicate
// 	there was no CLINT.  Equivalent now is to set the MTIME and MTIMECMP
// 	addresses to 0. */
// 	#define configMTIME_BASE_ADDRESS 	( 0 )
// 	#define configMTIMECMP_BASE_ADDRESS ( 0 )
// #elif defined( configCLINT_BASE_ADDRESS ) && !defined( configMTIME_BASE_ADDRESS )
// 	/* Legacy case where configCLINT_BASE_ADDRESS was set to the base address of
// 	the CLINT.  Equivalent now is to derive the MTIME and MTIMECMP addresses
// 	from the CLINT address. */
// 	#define configMTIME_BASE_ADDRESS 	( ( configCLINT_BASE_ADDRESS ) + 0xBFF8UL )
// 	#define configMTIMECMP_BASE_ADDRESS ( ( configCLINT_BASE_ADDRESS ) + 0x4000UL )
// #elif !defined( configMTIME_BASE_ADDRESS ) || !defined( configMTIMECMP_BASE_ADDRESS )
// 	#error configMTIME_BASE_ADDRESS and configMTIMECMP_BASE_ADDRESS must be defined in FreeRTOSConfig.h.  Set them to zero if there is no MTIME (machine time) clock.  See https://www.FreeRTOS.org/Using-FreeRTOS-on-RISC-V.html
// #endif



#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */

#include <stdint.h>
#include "sbi.h"


struct sbiret sbi_ecall(int ext, int fid, unsigned long arg0,
			unsigned long arg1, unsigned long arg2,
			unsigned long arg3, unsigned long arg4,
			unsigned long arg5)
{
	struct sbiret ret;

    //使用GCC的扩展语法，用于将一个值存储到RISC-V架构中的寄存器a0中。
	register uintptr_t a0 asm ("a0") = (uintptr_t)(arg0);
	register uintptr_t a1 asm ("a1") = (uintptr_t)(arg1);
	register uintptr_t a2 asm ("a2") = (uintptr_t)(arg2);
	register uintptr_t a3 asm ("a3") = (uintptr_t)(arg3);
	register uintptr_t a4 asm ("a4") = (uintptr_t)(arg4);
	register uintptr_t a5 asm ("a5") = (uintptr_t)(arg5);
	register uintptr_t a6 asm ("a6") = (uintptr_t)(fid);
	register uintptr_t a7 asm ("a7") = (uintptr_t)(ext);
	asm volatile ("ecall"
		      : "+r" (a0), "+r" (a1)
		      : "r" (a2), "r" (a3), "r" (a4), "r" (a5), "r" (a6), "r" (a7)
		      : "memory");
	ret.error = a0;
	ret.value = a1;

	return ret;
}


/**
 * sbi_set_timer() - Program the timer for next timer event.
 * @stime_value: The value after which next timer event should fire.
 *
 * Return: None
 */
void sbi_set_timer(uint64_t stime_value)
{
#if __riscv_xlen == 32
	sbi_ecall(SBI_EXT_SET_TIMER, SBI_FID_SET_TIMER, stime_value,
		  stime_value >> 32, 0, 0, 0, 0);
#else
	sbi_ecall(SBI_EXT_SET_TIMER, SBI_FID_SET_TIMER, stime_value,
		  0, 0, 0, 0, 0);
#endif
}


/**
 * sbi_get_spec_version() - get current SBI specification version
 * 获取SBI规范版本 (FID #0)
 * Return: version id
 */
long sbi_get_spec_version(void)
{
	struct sbiret ret;

	ret = sbi_ecall(SBI_EXT_BASE, SBI_EXT_BASE_GET_SPEC_VERSION,
			0, 0, 0, 0, 0, 0);
	if (!ret.error)
		if (ret.value)
			return ret.value;

	return -1;
}

/**
 * sbi_get_impl_id() - get SBI implementation ID
 * 获取SBI规范版本 (FID #0)  基本拓展 EID = 0x10
 * Return: implementation ID
 */
int sbi_get_impl_id(void)
{
	struct sbiret ret;

	ret = sbi_ecall(SBI_EXT_BASE, SBI_EXT_BASE_GET_IMP_ID,
			0, 0, 0, 0, 0, 0);
	if (!ret.error)
		if (ret.value)
			return ret.value;

	return -1;
}


/**
 * sbi_probe_extension() - Check if an SBI extension ID is supported or not.
 * @extid: The extension ID to be probed.
 *  探测SBI扩展功能 (FID #3) 基本拓展 EID = 0x10
 * Return: Extension specific nonzero value f yes, -1 otherwise.
 */
int sbi_probe_extension(int extid)
{
	struct sbiret ret;

	ret = sbi_ecall(SBI_EXT_BASE, SBI_EXT_BASE_PROBE_EXT, extid,
			0, 0, 0, 0, 0);
	if (!ret.error)
		if (ret.value)
			return ret.value;

	return -1;
}


/**
 * sbi_console_putchar() - Writes given character to the console device.
 * @ch: The data to be written to the console.
 *
 * Return: None
 */
void sbi_console_putchar(int ch)
{
	sbi_ecall(SBI_EXT_0_1_CONSOLE_PUTCHAR, 0, ch, 0, 0, 0, 0, 0);
}

/**
 * sbi_console_getchar() - Reads a byte from console device.
 *
 * Returns the value read from console.
 */
int sbi_console_getchar(void)
{
	struct sbiret ret;

	ret = sbi_ecall(SBI_EXT_0_1_CONSOLE_GETCHAR, 0, 0, 0, 0, 0, 0, 0);

	return ret.error;
}


/**
 * sbi_clear_ipi() - Clear any pending IPIs for the calling hart.
 * 清除IPI (EID #0x03) 清除任何挂起的IPI（处理器核间中断）。
 * Return: None
 */
void sbi_clear_ipi(void)
{
	sbi_ecall(SBI_EXT_0_1_CLEAR_IPI, 0, 0, 0, 0, 0, 0, 0);
}

/**
 * sbi_shutdown() - Remove all the harts from executing supervisor code.
 * 系统关闭 (EID #0x08)
 * Return: None
 */
void sbi_shutdown(void)
{
	sbi_ecall(SBI_EXT_0_1_SHUTDOWN, 0, 0, 0, 0, 0, 0, 0);
}


/**
 * sbi_send_ipi() - Send an IPI to any hart.
 * @hart_mask: A cpu mask containing all the target harts.
 * 向 hart_mask 中定义的所有 hart 发送跨处理器中断。
 * Return: None
 */
void sbi_send_ipi(const unsigned long *hart_mask)
{
	sbi_ecall(SBI_EXT_SEND_IPI, SBI_FID_SEND_IPI, (unsigned long)hart_mask,
		  0, 0, 0, 0, 0);
}

/**
 * sbi_remote_fence_i() - Execute FENCE.I instruction on given remote harts.
 * @hart_mask: A cpu mask containing all the target harts.
 * 远程指示harts执行FENCE.I指令
 * Return: None
 */
void sbi_remote_fence_i(const unsigned long *hart_mask)
{
	sbi_ecall(SBI_EXT_REMOTE_FENCE_I, SBI_FID_REMOTE_FENCE_I,
		  (unsigned long)hart_mask, 0, 0, 0, 0, 0);
}

/**
 * sbi_remote_sfence_vma() - Execute SFENCE.VMA instructions on given remote
 *			     harts for the specified virtual address range.
 * @hart_mask: A cpu mask containing all the target harts.
 * @start: Start of the virtual address
 * @size: Total size of the virtual address range.
 * 指示远程hart执行一个或多个SFENCE.VMA指令，覆盖从start到size的虚拟地址范围内的地址。
 * Return: None
 */
void sbi_remote_sfence_vma(const unsigned long *hart_mask,
			   unsigned long start,
			   unsigned long size)
{
	sbi_ecall(SBI_EXT_REMOTE_SFENCE_VMA, SBI_FID_REMOTE_SFENCE_VMA,
		  (unsigned long)hart_mask, start, size, 0, 0, 0);
}


/**
 * sbi_remote_sfence_vma_asid() - Execute SFENCE.VMA instructions on given
 * remote harts for a virtual address range belonging to a specific ASID.
 *
 * @hart_mask: A cpu mask containing all the target harts.
 * @start: Start of the virtual address
 * @size: Total size of the virtual address range.
 * @asid: The value of address space identifier (ASID).
 * 指示远程hart执行一个或多个SFENCE.VMA指令，覆盖从start到size的虚拟地址范围内的地址。这仅涵盖给定的ASID。
 * Return: None
 */
void sbi_remote_sfence_vma_asid(const unsigned long *hart_mask,
				unsigned long start,
				unsigned long size,
				unsigned long asid)
{
	sbi_ecall(SBI_EXT_REMOTE_SFENCE_VMA_ASID,
		  SBI_FID_REMOTE_SFENCE_VMA_ASID,
		  (unsigned long)hart_mask, start, size, asid, 0, 0);
}








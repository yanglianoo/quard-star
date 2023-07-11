OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{ 
	ram (rxai!w) : ORIGIN = 0x80200000, LENGTH = 128M
}
SECTIONS
{
	.text : {
		*(.text .text.*)
	} >ram

	.rodata : {
		*(.rodata .rodata.*)
	} >ram

	.data : {
		. = ALIGN(4096);
		*(.sdata .sdata.*)
		*(.data .data.*)
		PROVIDE(_data_end = .);
	} >ram

	.bss :{
		*(.sbss .sbss.*)
		*(.bss .bss.*)
		*(COMMON)
	} >ram

}
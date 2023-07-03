
CROSS_COMPILE = riscv64-unknown-elf-
CFLAGS = -nostdlib -fno-builtin 

# riscv64-unknown-elf-gcc 工具链可以同时编译汇编和 C 代码
CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump

SRCS_ASM = \
	entry.S

SRCS_C = \
	sbi.c \
	main.c \

# 将源文件替换为 .o 文件
OBJS = $(SRCS_ASM:.S=.o)
OBJS += $(SRCS_C:.c=.o)


os.elf: ${OBJS}
	${CC} ${CFLAGS} -T os.ld  -o os.elf $^
	${OBJCOPY} -O binary os.elf os.bin

%.o : %.c
	${CC} ${CFLAGS} -c -o $@ $<

%.o : %.S
	${CC} ${CFLAGS} -c -o $@ $<


.PHONY : clean
clean:
	rm -rf *.o *.bin *.elf
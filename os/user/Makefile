CROSS_COMPILE = riscv64-unknown-elf-
CFLAGS = -nostdlib -fno-builtin -mcmodel=medany

CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump
INCLUDE:=-I../include

LIB = ../lib

write: write.c $(LIB)/*.c
	${CC} ${CFLAGS} $(INCLUDE) -T user.ld -o bin/write.bin $^

time: time.c 
	${CC} ${CFLAGS} $(INCLUDE) -T user.ld -o bin/time.bin $^
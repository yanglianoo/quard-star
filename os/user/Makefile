CROSS_COMPILE = riscv64-unknown-elf-
CFLAGS = -nostdlib -fno-builtin 

CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump
INCLUDE:=-I../include

LIB = ../lib

write: write.c $(LIB)/*.c
	${CC} ${CFLAGS} $(INCLUDE) -T user.ld -Wl,-Map=write.map -o bin/write $^

time: time.c 
	${CC} ${CFLAGS} $(INCLUDE) -T user.ld -Wl,-Map=time.map -o bin/time $^

objdump:
	${OBJDUMP} -d bin/time > time.txt
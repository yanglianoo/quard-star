CROSS_COMPILE = riscv64-unknown-elf-
CFLAGS = -nostdlib -fno-builtin -mcmodel=medany

CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump
INCLUDE:=-I../include

LIB = ../lib

all: time write xec

write: write.c $(LIB)/*.c
	${CC} ${CFLAGS} $(INCLUDE) -T user.ld -Wl,-Map=write.map -o bin/write $^

time: time.c $(LIB)/*.c
	${CC} ${CFLAGS} $(INCLUDE) -T user.ld -Wl,-Map=time.map -o bin/time $^

xec: xec.c $(LIB)/*.c
	${CC} ${CFLAGS} $(INCLUDE) -T user.ld -o bin/xec $^

debug: objdump_time objdump_write objdump_xec

objdump_time:
	${OBJDUMP} -d bin/time > time.txt
objdump_write:
	${OBJDUMP} -d bin/write > write.txt
objdump_xec:
	${OBJDUMP} -d bin/xec > xec.txt
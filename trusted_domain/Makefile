##########################################################################################################################
# trusted_domain GCC compiler Makefile
##########################################################################################################################


TARGET = trusted_fw

DEBUG = 1

OPT = -Og

# abspath 用于获取绝对路径
# override 用于重定义
PROJECTBASE = $(PWD)
override PROJECTBASE    := $(abspath $(PROJECTBASE))  
TOP_DIR = $(PROJECTBASE)


#######################################
# 编译工具
#######################################
CROSS_COMPILE = riscv-
CC        = $(CROSS_COMPILE)gcc
AS        = $(CROSS_COMPILE)gcc -x assembler-with-cpp
OBJCOPY   = $(CROSS_COMPILE)objcopy
OBJDUMP   = $(CROSS_COMPILE)objdump
AR        = $(CROSS_COMPILE)ar
SZ        = $(CROSS_COMPILE)size
LD        = $(CROSS_COMPILE)ld
HEX       = $(OBJCOPY) -O ihex
BIN       = $(OBJCOPY) -O binary -S
GDB       = $(CROSS_COMPILE)gdb
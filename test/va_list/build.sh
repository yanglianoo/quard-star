

gcc -O0 -m32 -fno-builtin -fno-pic -fno-pie  -fno-stack-protector  va_list.c -o i386.out
riscv64-unknown-elf-gcc -O0 -fno-builtin -fno-pic -fno-pie -fno-stack-protector va_list.c -o riscv64.out

objdump -D i386.out > i386_debug.txt
riscv64-unknown-elf-objdump -D riscv64.out > riscv64_debug.txt

#   qemu-i386 i386.out
#   qemu-riscv64 riscv64.out

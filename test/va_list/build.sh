gcc -O0 -m32   va_list.c -o i386.out
riscv64-unknown-elf-gcc -O0 va_list.c -o riscv64.out
riscv32-unknown-elf-gcc -O0   va_list.c -o riscv32.out

objdump -D i386.out > i386_debug.txt
riscv64-unknown-elf-objdump -D riscv64.out > riscv64_debug.txt
riscv32-unknown-elf-objdump -D riscv32.out > riscv32_debug.txt

#   qemu-i386 i386.out
#   qemu-riscv64 riscv64.out
#   qemu-riscv32 riscv32.out
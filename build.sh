# 获取当前脚本文件所在的目录
SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)

if [ ! -d "$SHELL_FOLDER/output" ]; then  
mkdir $SHELL_FOLDER/output
fi  

cd qemu-8.0.2
if [ ! -d "$SHELL_FOLDER/output/qemu" ]; then  
./configure --prefix=$SHELL_FOLDER/output/qemu  --target-list=riscv64-softmmu --enable-gtk  --enable-virtfs --disable-gio
fi  
make -j16$PROCESSORS
make install


# # 编译 lowlevelboot
CROSS_PREFIX=riscv64-unknown-elf
if [ ! -d "$SHELL_FOLDER/output/lowlevelboot" ]; then  
mkdir $SHELL_FOLDER/output/lowlevelboot
fi  
cd  $SHELL_FOLDER/boot
$CROSS_PREFIX-gcc -x assembler-with-cpp -c start.s -o $SHELL_FOLDER/output/lowlevelboot/start.o
$CROSS_PREFIX-gcc -nostartfiles -T./boot.lds -Wl,-Map=$SHELL_FOLDER/output/lowlevelboot/lowlevel_fw.map -Wl,--gc-sections $SHELL_FOLDER/output/lowlevelboot/start.o -o $SHELL_FOLDER/output/lowlevelboot/lowlevel_fw.elf
# 使用gnu工具生成原始的程序bin文件
$CROSS_PREFIX-objcopy -O binary -S $SHELL_FOLDER/output/lowlevelboot/lowlevel_fw.elf $SHELL_FOLDER/output/lowlevelboot/lowlevel_fw.bin
# 使用gnu工具生成反汇编文件，方便调试分析（当然我们这个代码太简单，不是很需要）
$CROSS_PREFIX-objdump --source --demangle --disassemble --reloc --wide $SHELL_FOLDER/output/lowlevelboot/lowlevel_fw.elf > $SHELL_FOLDER/output/lowlevelboot/lowlevel_fw.lst



#编译 opensbi
if [ ! -d "$SHELL_FOLDER/output/opensbi" ]; then  
mkdir $SHELL_FOLDER/output/opensbi
fi  
cd $SHELL_FOLDER/opensbi-1.2
make CROSS_COMPILE=$CROSS_PREFIX- PLATFORM=quard_star
cp -r $SHELL_FOLDER/opensbi-1.2/build/platform/quard_star/firmware/*.bin $SHELL_FOLDER/output/opensbi/

# 生成sbi.dtb
cd $SHELL_FOLDER/dts
dtc -I dts -O dtb -o $SHELL_FOLDER/output/opensbi/quard_star_sbi.dtb quard_star_sbi.dts


#编译trusted_domain
if [ ! -d "$SHELL_FOLDER/output/trusted_domain" ]; then  
mkdir $SHELL_FOLDER/output/trusted_domain
fi  
cd $SHELL_FOLDER/trusted_domain
$CROSS_PREFIX-gcc  -x assembler-with-cpp -c startup.s -o $SHELL_FOLDER/output/trusted_domain/startup.o
$CROSS_PREFIX-gcc  -nostartfiles -T./link.lds -Wl,-Map=$SHELL_FOLDER/output/trusted_domain/trusted_fw.map -Wl,--gc-sections $SHELL_FOLDER/output/trusted_domain/startup.o -o $SHELL_FOLDER/output/trusted_domain/trusted_fw.elf
$CROSS_PREFIX-objcopy -O binary -S $SHELL_FOLDER/output/trusted_domain/trusted_fw.elf $SHELL_FOLDER/output/trusted_domain/trusted_fw.bin
$CROSS_PREFIX-objdump --source --demangle --disassemble --reloc --wide $SHELL_FOLDER/output/trusted_domain/trusted_fw.elf > $SHELL_FOLDER/output/trusted_domain/trusted_fw.lst

# # 编译uboot
# if [ ! -d "$SHELL_FOLDER/output/uboot" ]; then  
# mkdir $SHELL_FOLDER/output/uboot
# fi  
# cd $SHELL_FOLDER/u-boot-2023.04
# make CROSS_COMPILE=riscv64-unknown-linux-gnu- qemu-riscv64_smode_defconfig
# make CROSS_COMPILE=riscv64-unknown-linux-gnu-  -j16
# cp $SHELL_FOLDER/u-boot-2023.04/u-boot $SHELL_FOLDER/output/uboot/u-boot.elf
# cp $SHELL_FOLDER/u-boot-2023.04/u-boot.map $SHELL_FOLDER/output/uboot/u-boot.map
# cp $SHELL_FOLDER/u-boot-2023.04/u-boot.bin $SHELL_FOLDER/output/uboot/u-boot.bin
# riscv64-unknown-linux-gnu-objdump --source --demangle --disassemble --reloc --wide $SHELL_FOLDER/output/uboot/u-boot.elf > $SHELL_FOLDER/output/uboot/u-boot.lst

# # 生成uboot.dtb
# cd $SHELL_FOLDER/dts
# dtc -I dts -O dtb -o $SHELL_FOLDER/output/uboot/quard_star_uboot.dtb quard_star_uboot.dts

# 编译os
if [ ! -d "$SHELL_FOLDER/output/os" ]; then  
mkdir $SHELL_FOLDER/output/os
fi
cd $SHELL_FOLDER/os
# 编译app加载模块
make build_app
./build.out
# 编译os
make
cp $SHELL_FOLDER/os/os.bin $SHELL_FOLDER/output/os/os.bin
make clean




# 合成firmware固件
if [ ! -d "$SHELL_FOLDER/output/fw" ]; then  
mkdir $SHELL_FOLDER/output/fw
fi  
cd $SHELL_FOLDER/output/fw
rm -rf fw.bin
# 填充 32K的0
dd of=fw.bin bs=1k count=32k if=/dev/zero   
# # 写入 lowlevel_fw.bin 偏移量地址为 0
dd of=fw.bin bs=1k conv=notrunc seek=0 if=$SHELL_FOLDER/output/lowlevelboot/lowlevel_fw.bin
# 写入 quard_star_sbi.dtb 地址偏移量为 512K，因此 fdt的地址偏移量为 0x80000
dd of=fw.bin bs=1k conv=notrunc seek=512 if=$SHELL_FOLDER/output/opensbi/quard_star_sbi.dtb
# 写入 uboot.dtb,地址偏移量为 1K*1K = 0x100000
dd of=fw.bin bs=1k conv=notrunc seek=1K if=$SHELL_FOLDER/output/uboot/quard_star_uboot.dtb
# 写入 fw_jump.bin 地址偏移量为 2K*1K= 0x200000，因此 fw_jump.bin的地址偏移量为  0x200000
dd of=fw.bin bs=1k conv=notrunc seek=2k if=$SHELL_FOLDER/output/opensbi/fw_jump.bin
# 写入 trusted_domain.bin,地址偏移量为 1K*4K = 0x400000，因此 trusted_domain.bin的地址偏移量为  0x400000
dd of=fw.bin bs=1k conv=notrunc seek=4K if=$SHELL_FOLDER/output/trusted_domain/trusted_fw.bin
# 写入 uboot.bin,地址偏移量为 1K*8K =  0x800000
#dd of=fw.bin bs=1k conv=notrunc seek=8K if=$SHELL_FOLDER/output/uboot/u-boot.bin
# 写入 os.bin,地址偏移量为 1K*8K =  0x800000
dd of=fw.bin bs=1k conv=notrunc seek=8K if=$SHELL_FOLDER/output/os/os.bin



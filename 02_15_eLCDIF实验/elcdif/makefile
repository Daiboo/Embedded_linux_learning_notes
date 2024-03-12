#定义变量，用于保存编译选项和头文件保存路径
header_file := -fno-builtin -I$(shell pwd)/include
libgcc_address := /usr/local/arm/arm-linux-guneabihf/lib/gcc/arm-linux-gnueabihf/4.9.4 

export header_file


all : start.o main.o device/device.o 
	arm-linux-gnueabihf-ld -Tbase.lds $^ -o base.elf -static -L  $(libgcc_address) -lgcc
	arm-linux-gnueabihf-objcopy -O binary -S -g base.elf base.bin

%.o : %.S
	arm-linux-gnueabihf-gcc -g -c $^ 
%.o : %.c
	arm-linux-gnueabihf-gcc $(header_file) -c $^ 	

#调用其他文件的makefile
device/device.o :
	make -C device all


#定义清理伪目标
.PHONY: clean
clean:
	make -C device clean
	-rm -f *.o *.elf *.bin 


	

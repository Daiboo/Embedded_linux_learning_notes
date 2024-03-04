KERNELDIR := /home/daiboo/codes/linux_kernel/nxp_4.1.15

CURRENT_PATH := $(shell pwd)
obj-m := asyncnoti.o

build:kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
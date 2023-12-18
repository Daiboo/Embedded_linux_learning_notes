target = ledc.bin

objs = start.o main.o

arm_none := arm-none-eabi
arm_linux := arm-linux-gnueabihf
cc := $(arm_none)

gcc = $(cc)-gcc
ld = $(cc)-ld
objcopy = $(cc)-objcopy
objdump = $(cc)-objdump

$(target): $(objs)
	$(ld) -Timx6u.lds  $^ -o ledc.elf
	$(objcopy) -O binary -S ledc.elf $@
	$(objdump) -D -m arm ledc.elf > ledc.dis

%.o:%.s
	$(gcc) -Wall -nostdlib -c  -O2 -o $@ $<
	
%.o:%.S
	$(gcc) -Wall -nostdlib -c  -O2 -o $@ $<
	
%.o:%.c
	$(gcc) -Wall -nostdlib -c  -O2 -o $@ $<

.PHONY: clean
clean:
	rm -f $(target) $(objs) *.elf *.dis


target = ledc.bin

objs = start.o main.o

arm_none := arm-none-eabi
arm_linux := arm-linux-gnueabihf
cc := $(arm_none)


$(target): $(objs)
	$(cc)-ld -Timx6u.lds  -o ledc.elf
	$(cc)-objcopy -O binary -S ledc.elf $@
	$(cc)-objdump -D -m arm ledc.elf > ledc.dis

%.o:%.s
	$(cc)-gcc -Wall -nostdlib -c  -O2 -o $@ $<
	
%.o:%.S
	$(cc)-gcc -Wall -nostdlib -c  -O2 -o $@ $<
	
%.o:%.c
	$(cc)-gcc -Wall -nostdlib -c  -O2 -o $@ $<

.PHONY: clean
clean:
	rm -f $(target) $(objs) *.elf *.dis


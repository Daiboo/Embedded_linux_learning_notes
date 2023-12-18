led.bin:led.s
	arm-none-eabi-gcc -g -c -o led.o led.s
	arm-none-eabi-ld -Ttext 0x0 -g led.o -o led_elf
	arm-none-eabi-objcopy -O binary -S led_elf led.bin
	arm-none-eabi-objdump -D -m arm led_elf > led.dis

.PHONY:clean
clean:
	rm -f led.dis led.bin led_elf led.o load.imx
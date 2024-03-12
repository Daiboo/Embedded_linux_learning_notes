#include<linux/types.h>
#include<linux/kernel.h>
#include<linux/delay.h>
#include<linux/ide.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/errno.h>
#include<linux/gpio.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/of.h>
#include<linux/of_address.h>
#include<linux/of_gpio.h>
#include<linux/semaphore.h>
#include<linux/time.h>
#include<linux/irq.h>
#include<linux/wait.h>
#include<linux/poll.h>
#include<linux/fs.h>
#include<linux/fcntl.h>
#include<asm/mach/map.h>


static int fd;  // 文件描述符

static void sigio_signal_function(int signum)
{
	int err = 0;
	unsigned int keyvalue = 0;
	err = read(fd, &keyvalue, sizeof(keyvalue));
	if(err < 0)
	{
		
	}
	else
	{
		printf("key value = %#X\r\n", keyvalue);
	}
}

int main(int argc, char *argv[])
{
	int flags = 0;
	char *filename;

	if(argc != 2)
	{
		printf("Error Usage!\r\n");
		return -1;
	}
	filename = argv[1];
	fd = open(filename, O_RDWR | O_NONBLOCK);
	if(fd < 0)
	{
		printf("Can't open file %s\r\n", filename);
		return -1;
	}

	// 设置sigio的处理函数
	signal(SIGIO, sigio_signal_function);

	fcntl(fd, F_SETOWN, getpid());  // 将当前进程的进程号告诉给内核
	flags = fcntl(fd, F_GETFL);  // 获取文件的打开方式
	fcntl(fd, F_SETFL, flags | FASYNC);  // 设置文件的打开方式,使能异步通知

	while(1)
	{
		sleep(2);
	}
	
	close(fd);
	return 0;
}

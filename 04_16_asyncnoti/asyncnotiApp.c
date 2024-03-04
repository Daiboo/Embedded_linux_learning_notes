#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include "poll.h"
#include "sys/select.h"
#include "signal.h"
#include "linux/ioctl.h"
#include "sys/time.h"

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

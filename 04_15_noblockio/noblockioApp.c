#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include "poll.h"
#include "sys/select.h"



int main(int argc, char *argv[])
{
	int fd, ret = 0;
	char *filename;
	struct pollfd fds;
	fd_set readfd;
	struct timeval timeout;
	unsigned char data;
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
#if 0
	// 构造结构体
	fds.fd = fd;
	fds.events = POLLIN;
	while(1)
	{
		ret = poll(&fds, 1, 500);
		if(ret)    // 数据有效
		{
			ret = read(fd, &data, sizeof(data));
			if(ret < 0)
			{
				// 读取错误
			}
			else
			{
				if(data)
				{
					printf("key value = %#X\r\n", data);
				}
			}
		}
		else if(ret == 0)  // 超时
		{
			
		}
		else if(ret < 0)  // 错误
		{
			
		}
	}
#endif
	
	while(1)
	{	
		FD_ZERO(&readfd);
		FD_SET(fd, &readfd);
		timeout.tv_sec = 0;
		timeout.tv_usec = 500000;  // 500ms
		ret = select(fd + 1, &readfd, NULL, NULL, &timeout);
		switch(ret)
		{
			case 0:
		
				break;
			case -1:
			
				break;
			default:
				if(FD_ISSET(fd, &readfd))
				{
					ret = read(fd, &data, sizeof(data));
					if(ret < 0)
					{
						// 读取错误
					}
					else
					{
						if(data)
						{
							printf("key value = %#X\r\n", data);
						}
					}
				}
				break;
		}
	}
		
	ret = close(fd);
	return ret;
}

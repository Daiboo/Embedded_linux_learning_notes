#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

/************************
author:daiboo
************************/
#define KEY0VALUE     0xF0     // 按键值
#define INVAKEY       0x00    // 无效的按键值

int main(int argc, char *argv[])
{   
    int fd, retvalue;
    char *filename;
    unsigned char keyvalue;

    if(argc != 2)
    {
        printf("Error usage!\r\n");
        return -1;
    }

    filename = argv[1];


    fd = open(filename, O_RDWR);
    if(fd < 0)
    {
        printf("file %s open failed!\r\n", filename);
        return -1;
    }

    // 循环读取按键值数据 
    while(1)
    {
        read(fd, &keyvalue, sizeof(keyvalue));
        if(keyvalue == KEY0VALUE)
        {
            printf("key0 pressed, value = %#x\r\n", keyvalue);
        }
    }

    retvalue = close(fd);
    if(retvalue < 0)
    {
        printf("file %s close failed!\r\n", filename);
        return -1;
    }

    return 0;
}






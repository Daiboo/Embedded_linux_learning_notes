#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

/************************
help: ./ledApp /dev/red_led 0 turn off red led
      ./ledApp /dev/red_led 1 turn on red led

author:daiboo
************************/

int main(int argc, char *argv[])
{   
    int fd, retvalue;
    char *filename;
    unsigned char databuf[1];

    if(argc != 3)
    {
        printf("Error usage!\r\n");
        printf("Usage: %s /dev/ledstat 0|1\r\n", argv[0]);
        return -1;
    }

    filename = argv[1];

    /* open red led driver */
    fd = open(filename, O_RDWR);
    if(fd < 0)
    {
        printf("file %s open failed!\r\n", filename);
        return -1;
    }

    databuf[0] = atoi(argv[2]);   /* on or off ?*/
    retvalue = write(fd, databuf, sizeof(databuf));
    if(retvalue < 0)
    {
        printf("write file %s failed!\r\n", filename);
        close(fd);
        return -1;
    }

    int cnt = 0;
    while(1)
    {
        sleep(5);
        cnt++;
        printf("App running times:%d\r\n", cnt);
        if(cnt > 5) break;
    }

    retvalue = close(fd);
    if(retvalue < 0)
    {
        printf("file %s close failed!\r\n", filename);
        return -1;
    }

    return 0;

}






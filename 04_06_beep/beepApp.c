#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

/************************
help: ./beepApp /dev/red_led 0 turn off beep
      ./beepApp /dev/red_led 1 turn on beep

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
        printf("Usage: %s /dev/beepstat 0|1\r\n", argv[0]);
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

    retvalue = close(fd);
    if(retvalue < 0)
    {
        printf("file %s close failed!\r\n", filename);
        return -1;
    }

    return 0;

}






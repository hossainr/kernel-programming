#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include "./../ioctl.h"

int main(){
    int ret, fd;
    char ioctl_val[8];
    int i;

    fd = open("/dev/ellisys1", O_RDWR);             // Open the device with read/write access

    if (fd < 0){
        perror("Failed to open the device...");
        return errno;
    }

    if (ioctl(fd, MYCHAR_IOCTL_GET_TSF1, ( char * )ioctl_val) == -1)
    {
        perror("MYCHAR_IOCTL_GET_TSF ioctl error");
    }
    else
    {
        for(i=0; i<4; i++)
            printf("ch[%d]=%x\n", i, ioctl_val[i]);
    }
    close(fd);
    return 0;
}

#ifndef ___IOCTL_H__
#define ___IOCTL_H__

#include <linux/ioctl.h>

#ifdef __KERNEL__

#include <linux/types.h>

#else  //USERSPACE section

#include <stdint.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif  //PAGE_SIZE

#endif  //end of USERSPACE section

//COMMON SECTION

#define MYCHAR_IOCTL_GET_TSF _IOR('V',1,char*)
#define MYCHAR_IOCTL_GET_CHANNEL _IOR('V',2,char*)

#define MYCHAR_IOCTL_GET_TSF1 _IOR('A',1,char*)
#define MYCHAR_IOCTL_GET_CHANNEL1 _IOR('A',2,char*)

#endif


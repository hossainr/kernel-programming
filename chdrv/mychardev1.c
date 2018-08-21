#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>        // Required for the copy to user function
#include <linux/mm.h>
#include "ioctl.h"

MODULE_LICENSE("GPL");            ///< The license type -- this affects available functionality

#define  CLASS_NAME  "ellisys1"        ///< The device class -- this is a character device driver
#define  DEVICE_NAME "ellisys1"    //< The device will appear at /dev/ellisys11ad using this value

static struct class*  mychardev_class  = NULL; //< The device-driver class struct pointer
static struct device* mychardev_device = NULL; //< The device-driver device struct pointer
static int    mychardev_major_number;

static void mychardev_mmap_open(struct vm_area_struct *vma) 
{
    printk(KERN_INFO "dual(2): %s\n",__func__);
}

static void mychardev_mmap_close(struct vm_area_struct *vma)
{
    printk(KERN_INFO "dual(2): %s\n",__func__);
}

static struct vm_operations_struct mmap_vm_ops = {
    .open = mychardev_mmap_open,
    .close = mychardev_mmap_close
};


static int mychardev_mmap(struct file *filp, struct vm_area_struct *vma) 
{
    printk(KERN_INFO "dual(2): %s\n",__func__);
    vma->vm_ops = &mmap_vm_ops;
    return 0;
}

static ssize_t mychardev_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    ssize_t ret=0;
    printk(KERN_INFO "dual(2): %s\n",__func__);
    return ret;
}

static ssize_t mychardev_write(struct file *f, const char __user *buf, size_t len,loff_t *off)
{
    ssize_t ret=0;
    printk(KERN_INFO "dual(2): %s\n",__func__);
    return ret;
}


static int mychardev_open(struct inode *i, struct file *filp)
{
    printk(KERN_INFO "dual(2): %s\n",__func__);
    return 0;
}

static int mychardev_close(struct inode *i, struct file *filp)
{
    printk(KERN_INFO "dual(2): %s\n",__func__);
    return 0;
}

static int mychardev_get_tsf(char __user *data)
{
    int ret;
    int tsf;

    tsf = 0x55;

    ret= copy_to_user(data, (void*)&tsf, sizeof(tsf));

    if(ret != 0)
    {
        return -EFAULT;
    }
    printk(KERN_INFO "dual(2): %s\n",__func__);
    return 0;
}

static int mychardev_get_chan(void __user *data)
{
    int ret;
    int chan = 1;

    ret= copy_to_user( data, (void*)&chan, sizeof(int));

    if(ret != 0)
    {
        return -EFAULT;
    }
    printk(KERN_INFO "dual(2): %s\n",__func__);
    return 0;
}

static long mychardev_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    char __user *buf = (char * )arg;
    switch (cmd)
    {
        case MYCHAR_IOCTL_GET_TSF1:
            {
                return mychardev_get_tsf(buf);
            }
        case MYCHAR_IOCTL_GET_CHANNEL1:
            return mychardev_get_chan(buf);
        default:
            printk("Unsupported ioctl %s \n", __func__);
            return -ENOIOCTLCMD;
    }
}

static struct file_operations mychardev_fops =
{
    .owner = THIS_MODULE,
    .open = mychardev_open,
    .release = mychardev_close,
    .read = mychardev_read,
    .write = mychardev_write,
    .unlocked_ioctl = mychardev_ioctl,
    .mmap = mychardev_mmap
};

int mychardev_create1(void)
{
    int dev_id;
    printk(KERN_INFO "+ dual(2): %s\n",__func__);

    mychardev_major_number = register_chrdev(0,DEVICE_NAME,&mychardev_fops);
    if(mychardev_major_number < 0)
    {
        printk("cannot obtain major number \n");
        return -EINVAL;
    }
    dev_id = MKDEV(mychardev_major_number, 0);

    mychardev_class = class_create(THIS_MODULE, CLASS_NAME);

    if (IS_ERR(mychardev_class)) {
        //unregister_chrdev(mychardev_major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(mychardev_class);
    }
    printk(KERN_INFO "11ad character driver: device class registered correctly\n");

    mychardev_device = device_create(mychardev_class, NULL, MKDEV(mychardev_major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(mychardev_device)) {
        class_destroy(mychardev_class);
        unregister_chrdev(mychardev_major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(mychardev_device);
    }
    /* Made it! device was initialized */
    printk(KERN_INFO "11ad character driver: device class created correctly\n"); 
    printk(KERN_INFO "- dual(2): %s\n",__func__);
    return 0;
}

void mychardev_destroy1(void)
{
    printk(KERN_INFO "+ dual(2): %s\n",__func__);
    device_destroy(mychardev_class, MKDEV(mychardev_major_number, 0));
    class_unregister(mychardev_class);
    class_destroy(mychardev_class);
    unregister_chrdev(mychardev_major_number, DEVICE_NAME);
    printk("dual (1): mychardev_destroy\n");
}


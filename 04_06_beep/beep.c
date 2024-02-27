#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>   
#include <linux/errno.h>
#include <linux/gpio.h>
#include <asm/mach/map.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/io.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/of_address.h>
#include <linux/uaccess.h>


/********************************************************
function: turn on the red led on the emfire board
red led io:GPIO1_4
        CCM:CCGR1

*********************************************************/
#define BEEP_CNT        1               /* device number */
#define BEEP_NAME      "beep"      /* device name */
#define BEEPOFF 0   /* ture off */
#define BEEPON 1    /* turn on */


/* beep device structure */
struct beep_dev{
    dev_t devid;                /* device number */
    struct cdev cdev;           /* cdev */
    struct class *class;        /* class */
    struct device *device;      /* device */
    int major;                   /* major number */
    int minor;                  /* minor number */
    struct device_node *nd;     /* device node */
    int beep_gpio;               /* led gpio number */
};

struct beep_dev beep;  /* led device */





static int beep_open(struct inode *inode, struct file *file)
{
    file->private_data = &beep;  /* set private data */
    return 0;
}



static ssize_t beep_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    return 0;
}



static ssize_t beep_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    int retvalue;
    unsigned char databuf[1];
    unsigned char beepstat;
    struct beep_dev *dev = file->private_data;

    retvalue = copy_from_user(databuf, buf, count);
    if(retvalue < 0)
    {
        printk("kernel write failed!\r\n");
        return -EFAULT;
    }

    beepstat = databuf[0];   /* get the status */

    if(beepstat == BEEPON)
        gpio_set_value(dev->beep_gpio, 1);   /* turn on beep */
    else if(beepstat == BEEPOFF)
        gpio_set_value(dev->beep_gpio, 0);   /* turn off beep */

    return 0;
}



static int beep_release(struct inode *inode, struct file *file)
{
    return 0;
}


/* device operations */
static struct file_operations beep_fops = {
    .owner = THIS_MODULE,
    .open = beep_open,
    .read = beep_read,
    .write = beep_write,
    .release = beep_release,
};



static int __init beep_init(void)
{
    int ret;

    /* set gpio that beep uses */
    /* 1.get device nd: beep */
    beep.nd = of_find_node_by_path("/beep");
    if(beep.nd == NULL)
    {
        printk("beep node not find!\r\n");
        return -EINVAL;
    }
    else
    {
        printk("beep node find!\r\n");
    }

    /* 2.get gpio number */
    beep.beep_gpio = of_get_named_gpio(beep.nd, "beep-gpio", 0);
    if(beep.beep_gpio < 0)
    {
        printk("can't get beep-gpio\r\n");
        return -EINVAL;
    }

    printk("beep-gpio num:%d\r\n", beep.beep_gpio);

    /* 3.seting gpio's mode is output,set the gpio ouput the high and default to turn off beep*/
    ret = gpio_direction_output(beep.beep_gpio, 0);
    if(ret < 0)
    {
        printk("can't set gpio!\r\n");
    }


    /* register beep device */
    /* 1. create dev num */
    if(beep.major)  /* static registration */
    {
        beep.devid = MKDEV(beep.major, 0);
        register_chrdev_region(beep.devid, BEEP_CNT, BEEP_NAME);  /* register device number */
    }
    else   /* dynamic registration */
    {
        alloc_chrdev_region(&beep.devid, 0, BEEP_CNT, BEEP_NAME);  /* allocate device number */
        beep.major = MAJOR(beep.devid);   /* get major number */
        beep.minor = MINOR(beep.devid);   /* get minor number */

    }
    printk("beep major=%d, minor=%d\r\n", beep.major, beep.minor);

    /* 2. initialize cdev */
    beep.cdev.owner = THIS_MODULE;
    cdev_init(&beep.cdev, &beep_fops);

    /* 3. add cdev */
    cdev_add(&beep.cdev, beep.devid, BEEP_CNT);

    /* 4. create class */
    beep.class = class_create(THIS_MODULE, BEEP_NAME);
    if(IS_ERR(beep.class))
    {
        return PTR_ERR(beep.class);
    }

    /* 5. create device file */
    beep.device = device_create(beep.class, NULL, beep.devid, NULL, BEEP_NAME);
    if(IS_ERR(beep.device))
    {
        return PTR_ERR(beep.device);
    }
    return 0;
}


/**
 * @brief LED driver exit function.
 *
 * This function is called when the LED driver is removed.
 */
static void __exit beep_exit(void)
{

    /* unregister led */
    cdev_del(&beep.cdev);  /* delete cdev */
    unregister_chrdev_region(beep.devid, BEEP_CNT);  /* unregister device number */

    device_destroy(beep.class, beep.devid);  /* delete device file */
    class_destroy(beep.class);

}


module_init(beep_init);
module_exit(beep_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("daiboo");
MODULE_DESCRIPTION("This is a simple char device driver for beep control");


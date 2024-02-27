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
#include <linux/semaphore.h>

/********************************************************
function: turn on the red led on the emfire board
red led io:GPIO1_4
        CCM:CCGR1

*********************************************************/
#define NEWCHRLED_CNT       1               /* device number */
#define NEWCHRLED_NAME      "gpioled"      /* device name */
#define LEDOFF 0   /* ture off */
#define LEDON 1    /* turn on */


/* gpioled device structure */
struct gpioled_dev{
    dev_t devid;                /* device number */
    struct cdev cdev;           /* cdev */
    struct class *class;        /* class */
    struct device *device;      /* device */
    int major;                   /* major number */
    int minor;                  /* minor number */
    struct device_node *nd;     /* device node */
    int led_gpio;               /* led gpio number */
    int dev_state;               /* driver state; if state = 0,driver is free,otherwise is busy */
    struct semaphore sem;      
};

struct gpioled_dev gpioled;  /* led device */



/**
 * @brief Open function for the LED driver.
 *
 * This function is called when the LED driver is opened.
 *
 * @param[in] inode Pointer to the inode structure.
 * @param[in] file Pointer to the file structure.
 * @return 0 on success, negative error code on failure.
 */
static int led_open(struct inode *inode, struct file *file)
{
    /* 信号量,进入休眠状态的进程可以被信号打断 */
    if(down_interruptible(&gpioled.sem))  /* 获取信号量 */
    {
        return -ERESTARTSYS;
    }
    
#if 0
    down(&gpioled.sem);  /* 获取信号量 ,不能被信号打断 */
#endif

    file->private_data = &gpioled;  /* set private data */
    return 0;
}


/**
 * @brief Reads data from the LED device.
 *
 * This function is called when a read operation is performed on the LED device file.
 * It reads data from the device and copies it to the user buffer.
 *
 * @param file  Pointer to the file structure.
 * @param buf   Pointer to the user buffer.
 * @param count Number of bytes to read.
 * @param ppos  Pointer to the file position.
 * @return      Number of bytes read, or 0 if successful.
 */
static ssize_t led_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    return 0;
}


/**
 * @brief Writes data to the LED device file.
 *
 * This function is called when data is written to the LED device file. It copies the data from the user space buffer to the kernel space buffer and then controls the LED based on the received data.
 *
 * @param file Pointer to the file structure.
 * @param buf Pointer to the user space buffer containing the data to be written.
 * @param count Number of bytes to be written.
 * @param ppos Pointer to the file position.
 * @return On success, returns 0. On failure, returns -EFAULT.
 */
static ssize_t led_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    int retvalue;
    unsigned char databuf[1];
    unsigned char ledstat;
    struct gpioled_dev *dev = file->private_data;

    retvalue = copy_from_user(databuf, buf, count);
    if(retvalue < 0)
    {
        printk("kernel write failed!\r\n");
        return -EFAULT;
    }

    ledstat = databuf[0];   /* get the status */

    if(ledstat == LEDON)
    {
        gpio_set_value(dev->led_gpio, 0);   /* turn on led */
    }
    else if(ledstat == LEDOFF)
    {
        gpio_set_value(dev->led_gpio, 1);   /* turn off led */
    }

    return 0;
}


/**
 * @brief Release function for the LED driver.
 *
 * This function is called when the LED driver is released.
 *
 * @param[in] inode Pointer to the inode structure.
 * @param[in] file Pointer to the file structure.
 * @return 0 on success.
 */
static int led_release(struct inode *inode, struct file *file)
{
    up(&gpioled.sem);  /* 释放信号量 */
    return 0;
}


/* device operations */
static struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .read = led_read,
    .write = led_write,
    .release = led_release,
};


/**
 * @brief LED driver initialization function.
 *
 * This function is called when the LED driver is installed.
 *
 * @return 0 on success, negative error code on failure.
 */
static int __init led_init(void)
{
    int ret;

    /* initialize the semaphore */
    sema_init(&gpioled.sem, 1);
    
    /* set gpio that led uses */
    /* 1.get device nd: gpioled */
    gpioled.nd = of_find_node_by_path("/gpioled");
    if(gpioled.nd == NULL)
    {
        printk("gpioled node not find!\r\n");
        return -EINVAL;
    }
    else
    {
        printk("gpioled node find!\r\n");
    }

    /* 2.get gpio number */
    gpioled.led_gpio = of_get_named_gpio(gpioled.nd, "led-gpio", 0);
    if(gpioled.led_gpio < 0)
    {
        printk("can't get led-gpio\r\n");
        return -EINVAL;
    }
    printk("led-gpio num:%d\r\n", gpioled.led_gpio);

    /* 3.seting gpio's mode is output,set the gpio ouput the high and default to turn off led*/
    ret = gpio_direction_output(gpioled.led_gpio, 1);
    if(ret < 0)
    {
        printk("can't set gpio!\r\n");
    }


    /* register gpioled device */
    /* 1. create dev num */
    if(gpioled.major)  /* static registration */
    {
        gpioled.devid = MKDEV(gpioled.major, 0);
        register_chrdev_region(gpioled.devid, NEWCHRLED_CNT, NEWCHRLED_NAME);  /* register device number */
    }
    else   /* dynamic registration */
    {
        alloc_chrdev_region(&gpioled.devid, 0, NEWCHRLED_CNT, NEWCHRLED_NAME);  /* allocate device number */
        gpioled.major = MAJOR(gpioled.devid);   /* get major number */
        gpioled.minor = MINOR(gpioled.devid);   /* get minor number */

    }
    printk("gpioled major=%d, minor=%d\r\n", gpioled.major, gpioled.minor);

    /* 2. initialize cdev */
    gpioled.cdev.owner = THIS_MODULE;
    cdev_init(&gpioled.cdev, &led_fops);

    /* 3. add cdev */
    cdev_add(&gpioled.cdev, gpioled.devid, NEWCHRLED_CNT);

    /* 4. create class */
    gpioled.class = class_create(THIS_MODULE, NEWCHRLED_NAME);
    if(IS_ERR(gpioled.class))
    {
        return PTR_ERR(gpioled.class);
    }

    /* 5. create device file */
    gpioled.device = device_create(gpioled.class, NULL, gpioled.devid, NULL, NEWCHRLED_NAME);
    if(IS_ERR(gpioled.device))
    {
        return PTR_ERR(gpioled.device);
    }
    return 0;
}


/**
 * @brief LED driver exit function.
 *
 * This function is called when the LED driver is removed.
 */
static void __exit led_exit(void)
{

    /* unregister led */
    cdev_del(&gpioled.cdev);  /* delete cdev */
    unregister_chrdev_region(gpioled.devid, NEWCHRLED_CNT);  /* unregister device number */

    device_destroy(gpioled.class, gpioled.devid);  /* delete device file */
    class_destroy(gpioled.class);

}


module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("daiboo");
MODULE_DESCRIPTION("This is a simple char device driver for led control");


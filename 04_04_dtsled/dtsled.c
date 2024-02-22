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
#include <linux/of_address.h>
#include <linux/uaccess.h>


/********************************************************
function: turn on the red led on the emfire board
red led io:GPIO1_4
        CCM:CCGR1

*********************************************************/
#define NEWCHRLED_CNT       1               /* device number */
#define NEWCHRLED_NAME      "dtsled"      /* device name */
#define LEDOFF 0   /* ture off */
#define LEDON 1    /* turn on */




// register virtual address
static void __iomem *CCM_CCGR1;
static void __iomem *SW_MUX_GPIO1_IO04;
static void __iomem *SW_PAD_GPIO1_IO04;
static void __iomem *GPIO1_DR;
static void __iomem *GPIO1_GDIR;


/* dtsled device structure */
struct dtsled_dev{
    dev_t devid;       /* device number */
    struct cdev cdev;  /* cdev */
    struct class *class;  /* class */
    struct device *device;  /* device */
    int major;  /* major number */
    int minor;  /* minor number */
    struct device_node *nd;  /* device node */
};

struct dtsled_dev dtsled;  /* led device */



/**
 * @brief Control the LED status.
 * 
 * @param status LED status, LEDON or LEDOFF.
 */
void led_switch(u8 status)
{
    u32 val = 0;
    if(status == LEDON)
    {
        val = readl(GPIO1_DR);
        val &= ~(1<<4);   // bit4 clear
        writel(val, GPIO1_DR);
    }
    else if(status == LEDOFF)
    {
        val = readl(GPIO1_DR);
        val |= (1<<4);  // bit4 set
        writel(val, GPIO1_DR);
    }
}



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
    file->private_data = &dtsled;  /* set private data */
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

    retvalue = copy_from_user(databuf, buf, count);
    if(retvalue < 0)
    {
        printk("kernel write failed!\r\n");
        return -EFAULT;
    }

    ledstat = databuf[0];   /* get the status */
    if(ledstat == LEDON)
    {
        led_switch(LEDON);  /* turn on the led */
    }
    else if(ledstat == LEDOFF)
    {
        led_switch(LEDOFF);  /* turn off the led */
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
    u32 val = 0;
    int ret;
    u32 regdata[14];
    const char *str;
    struct property *proper;

    /* get device-tree property */
    /* 1. get device node:fireredled */
    dtsled.nd = of_find_node_by_path("/fireredled");
    if(dtsled.nd == NULL)
    {
        printk("fireredled node not find!\r\n");
        return -EINVAL;
    }
    else
    {
        printk("fireredled node find!\r\n");
    }

    /* 2. get compatible property */
    proper = of_find_property(dtsled.nd, "compatible", NULL);
    if(proper == NULL)
    {
        printk("compatible property find failed!\r\n");
    }
    else
    {
        printk("compatible = %s\r\n", (char *)proper->value);
    }

    /* 3. get status of peoperty*/
    ret = of_property_read_string(dtsled.nd, "status", &str);
    if(ret < 0)
    {
        printk("status read failed!\r\n");
    }
    else
    {
        printk("status = %s\r\n", str);
    }

    /* 4. get reg property */
    ret = of_property_read_u32_array(dtsled.nd, "reg", regdata, 10);
    if(ret < 0)
    {
        printk("reg property read failed!\r\n");
    }
    else
    {
        u8 i;
        printk("reg data:\r\n");
        for(i=0; i<10; i++)
        {
            printk("%#X ", regdata[i]);
        }
        printk("\r\n");
    }

    /* get address */
    CCM_CCGR1 = of_iomap(dtsled.nd, 0);
    SW_MUX_GPIO1_IO04 = of_iomap(dtsled.nd, 1);
    SW_PAD_GPIO1_IO04 = of_iomap(dtsled.nd, 2);
    GPIO1_DR = of_iomap(dtsled.nd, 3);
    GPIO1_GDIR = of_iomap(dtsled.nd, 4);


    /* 2.enable GPIO1 clock */
    val = readl(CCM_CCGR1);
    val &= ~(3<<26);   /* clear CG13 */
    val |= (3<<26);    /* set CG13 */
    writel(val, CCM_CCGR1);

    /* 3.set iomux of gpio1_io4 */
    writel(5, SW_MUX_GPIO1_IO04);   /* set as gpio1_io04 */
    writel(0x10B0, SW_PAD_GPIO1_IO04);   /* set pad */

    /* 4. set output */
    val = readl(GPIO1_GDIR);
    val &= ~(1<<4);   /* clear bit4 */
    val |= (1<<4);    /* set bit4 */
    writel(val, GPIO1_GDIR);

    /* 5. default to turn off led*/
    val = readl(GPIO1_DR);
    val |= (1<<4);   /* set bit4 */
    writel(val, GPIO1_DR);

    /* register dtsled */
    /* 1. create dev num */
    if(dtsled.major)  /* static registration */
    {
        dtsled.devid = MKDEV(dtsled.major, 0);
        register_chrdev_region(dtsled.devid, NEWCHRLED_CNT, NEWCHRLED_NAME);  /* register device number */
    }
    else   /* dynamic registration */
    {
        alloc_chrdev_region(&dtsled.devid, 0, NEWCHRLED_CNT, NEWCHRLED_NAME);  /* allocate device number */
        dtsled.major = MAJOR(dtsled.devid);   /* get major number */
        dtsled.minor = MINOR(dtsled.devid);   /* get minor number */

    }
    printk("dtsled major=%d, minor=%d\r\n", dtsled.major, dtsled.minor);

    /* 2. initialize cdev */
    dtsled.cdev.owner = THIS_MODULE;
    cdev_init(&dtsled.cdev, &led_fops);

    /* 3. add cdev */
    cdev_add(&dtsled.cdev, dtsled.devid, NEWCHRLED_CNT);

    /* 4. create class */
    dtsled.class = class_create(THIS_MODULE, NEWCHRLED_NAME);
    if(IS_ERR(dtsled.class))
    {
        return PTR_ERR(dtsled.class);
    }

    /* 5. create device file */
    dtsled.device = device_create(dtsled.class, NULL, dtsled.devid, NULL, NEWCHRLED_NAME);
    if(IS_ERR(dtsled.device))
    {
        return PTR_ERR(dtsled.device);
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

    /* address unmap */
    iounmap(CCM_CCGR1);
    iounmap(SW_MUX_GPIO1_IO04);
    iounmap(SW_PAD_GPIO1_IO04);
    iounmap(GPIO1_DR);
    iounmap(GPIO1_GDIR);

    /* unregister led */
    cdev_del(&dtsled.cdev);  /* delete cdev */
    unregister_chrdev_region(dtsled.devid, NEWCHRLED_CNT);  /* unregister device number */

    device_destroy(dtsled.class, dtsled.devid);  /* delete device file */
    class_destroy(dtsled.class);

}


module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("daiboo");
MODULE_DESCRIPTION("This is a simple char device driver for led control");


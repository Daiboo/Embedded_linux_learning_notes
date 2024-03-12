#include<linux/types.h>
#include<linux/kernel.h>
#include<linux/delay.h>
#include<linux/ide.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/errno.h>
#include<linux/gpio.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/of.h>
#include<linux/of_address.h>
#include<linux/of_gpio.h>
#include<linux/semaphore.h>
#include<linux/time.h>
#include<linux/irq.h>
#include<linux/wait.h>
#include<linux/poll.h>
#include<linux/fs.h>
#include<linux/fcntl.h>
#include<linux/platform_device.h>
#include<asm/mach/map.h>
#include<asm/uaccess.h>
#include<asm/io.h>


// **************platform设备****************

// 寄存器地址
#define CCM_CCGR1_BASE             (0x020C406C)    /* enable gpio1 */
#define SW_MUX_GPIO1_IO04_BASE     (0x020E006C)    /* set gpio1_io04 as gpio alt5 */
#define SW_PAD_GPIO1_IO04_BASE     (0x020E02F8)    /* set gpio1_io04 pad */
#define GPIO1_DR_BASE              (0x0209C000)    /* gpio1 data register */ 
#define GPIO1_GDIR_BASE            (0x0209C004)    /* gpio1 direction register */
#define REGISTER_SIZE              4

// 释放platform设备模块
static void led_release(struct device *dev)
{
    printk("led_release\r\n");
}


// 设备资源,LED0所使用的所有寄存器

static struct resource led_resources[] = {
    [0] = {
        .start = CCM_CCGR1_BASE,
        .end = CCM_CCGR1_BASE + REGISTER_SIZE - 1,
        .name = "CCM_CCGR1",
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = SW_MUX_GPIO1_IO04_BASE,
        .end = SW_MUX_GPIO1_IO04_BASE + REGISTER_SIZE - 1,
        .name = "SW_MUX_GPIO1_IO04",
        .flags = IORESOURCE_MEM,
    },
    [2] = {
        .start = SW_PAD_GPIO1_IO04_BASE,
        .end = SW_PAD_GPIO1_IO04_BASE + REGISTER_SIZE - 1,
        .name = "SW_PAD_GPIO1_IO04",
        .flags = IORESOURCE_MEM,
    },
    [3] = {
        .start = GPIO1_DR_BASE,
        .end = GPIO1_DR_BASE + REGISTER_SIZE - 1,
        .name = "GPIO1_DR",
        .flags = IORESOURCE_MEM,
    },
    [4] = {
        .start = GPIO1_GDIR_BASE,
        .end = GPIO1_GDIR_BASE + REGISTER_SIZE - 1,
        .name = "GPIO1_GDIR",
        .flags = IORESOURCE_MEM,
    },
}


// platform设备结构体
static struct platform_device led_device = {
    .name = "imx6ul-led",
    .id = -1,
    .dev = {
        .release = led_release,
    },
    .num_resources = ARRAY_SIZE(led_resources),
    .resource = led_resources,
}


static int __init leddevice_init(void)
{
    return platform_device_register(&led_device);
}


static void __exit leddevice_exit(void)
{
    platform_device_unregister(&led_device);
}


module_init(leddevice_init);
module_exit(leddevice_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("daiboo");
MODULE_DESCRIPTION("LED device driver");
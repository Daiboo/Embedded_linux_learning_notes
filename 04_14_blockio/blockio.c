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
#include<linux/timer.h>
#include<asm/mach/map.h>
#include<asm/uaccess.h>
#include<asm/io.h>
#include<linux/irq.h>
#include<linux/of_irq.h>



#define IMX6UIRQ_CNT 1  // 设备号个数
#define IMX6UIRQ_NAME "blockio"  // 设备名
#define KEY0VALUE 0x01  // 按键值
#define INVAKEY 0xFF  // 无效的按键值
#define KEY_NUM 1    // 按键数量



// 中断IO描述结构体
struct irq_keydesc {
    int gpio;    // gpio
    int irqnum;   // 中断号
    unsigned char value;  // 按键对应的键值
    char name[10];   // 名字
    irqreturn_t (*handler)(int, void *);   // 中断处理函数 
};



// imx6uirq设备结构体
struct imx6uirq_dev{
    dev_t devid;   // 设备号
    struct cdev cdev;   // cdev  
    struct class *class;   // 类
    struct device *device;   // 设备
    int major;   // 主设备号
    int minor;   // 次设备号
    struct device_node *nd;   // 设备节点
    atomic_t keyvalue;   // 有效的按键值
    atomic_t releasekey;   // 标记是否完一次完成的按键
    struct timer_list timer;  // 定义一个定时器
    struct irq_keydesc irqkeydesc[KEY_NUM];  // 按键描述数组
    unsigned char curkeynum;   // 当前按键值

    wait_queue_head_t r_wait;  // 读等待队列
};


struct imx6uirq_dev imx6uirq;  // irq设备


// 按键0中断处理函数
static irqreturn_t key0_handler(int irq, void *dev_id)
{
    struct imx6uirq_dev *dev = (struct imx6uirq_dev *)dev_id;

    dev->curkeynum = 0;
    dev->timer.data = (volatile long)dev_id;
    mod_timer(&dev->timer, jiffies + msecs_to_jiffies(10));  // 10ms后启动定时器
    return IRQ_RETVAL(IRQ_HANDLED);
}


// 定时器服务函数,用于按键消抖,定时器到了以后,再次读取按键值,如果按键还是处于按下状态,就表示按键有效
void timer_function(unsigned long arg)
{
    unsigned char value;
    unsigned char num;
    struct irq_keydesc *keydesc;
    struct imx6uirq_dev *dev = (struct imx6uirq_dev *)arg;

    num = dev->curkeynum;
    keydesc = &dev->irqkeydesc[num];
    value = gpio_get_value(keydesc->gpio);  // 读取IO值
    if(value == 1)   // 按下按键
    {
        atomic_set(&dev->keyvalue, keydesc->value);
    }
    else
    {
        atomic_set(&dev->keyvalue, 0x80 | keydesc->value);  // 松开按键 0x81
        atomic_set(&dev->releasekey, 1);      // 标记松开按键
    }

    // 唤醒进程
    if(atomic_read(&dev->releasekey))  // 完成一次按键过程
    {
        wake_up_interruptible(&dev->r_wait);
    }

}


// 按键IO初始化
static int keyio_init(void)
{
    unsigned int i;

    int ret;

    imx6uirq.nd = of_find_node_by_path("/key");
    if(imx6uirq.nd == NULL)
    {
        printk("key node not find!\r\n");
        return -EINVAL;
    }

    // 提取GPIO
    for(i = 0; i < KEY_NUM; i++)
    {
        imx6uirq.irqkeydesc[i].gpio = of_get_named_gpio(imx6uirq.nd, "key-gpio", i);
        if(imx6uirq.irqkeydesc[i].gpio < 0)
        {
            printk("can't get key%d\r\n", i);
            return -EINVAL;
        
        }
    }

    // 初始化key所使用的IO,并且设置成中断模式
    for(i = 0; i < KEY_NUM; i++)
    {
        memset(imx6uirq.irqkeydesc[i].name, 0, sizeof(imx6uirq.irqkeydesc[i].name));
        sprintf(imx6uirq.irqkeydesc[i].name, "KEY%d", i);
        gpio_request(imx6uirq.irqkeydesc[i].gpio, imx6uirq.irqkeydesc[i].name);
        gpio_direction_input(imx6uirq.irqkeydesc[i].gpio);
        imx6uirq.irqkeydesc[i].irqnum = irq_of_parse_and_map(imx6uirq.nd, i);  // 获取中断号
#if 0
        imx6uirq.irqkeydesc[i].irqnum = gpio_to_irq(imx6uirq.irqkeydesc[i].gpio);
#endif
    printk("key%d:gpio=%d,irqnum=%d\r\n", i, imx6uirq.irqkeydesc[i].gpio, imx6uirq.irqkeydesc[i].irqnum);
    }

    // 申请中断
    imx6uirq.irqkeydesc[0].handler = key0_handler;
    imx6uirq.irqkeydesc[0].value = KEY0VALUE;

    for(i = 0; i < KEY_NUM; i++)
    {
        ret = request_irq(imx6uirq.irqkeydesc[i].irqnum, 
                          imx6uirq.irqkeydesc[i].handler, 
                          IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, 
                          imx6uirq.irqkeydesc[i].name, &imx6uirq);
        if(ret < 0)
        {
            printk("irq %d request failed!\r\n", imx6uirq.irqkeydesc[i].irqnum);
            return -EFAULT;
        }
    }

    // 创建定时器
    init_timer(&imx6uirq.timer);
    imx6uirq.timer.function = timer_function;

    // 初始化等待队列头
    init_waitqueue_head(&imx6uirq.r_wait);
    return 0;
}

// 打开设备
static int imx6uirq_open(struct inode *inode, struct file *filp)
{
    filp->private_data = &imx6uirq;  // 设置私有数据
    return 0;
}


// 读设备
static ssize_t imx6uirq_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    int ret = 0;
    unsigned char keyvalue = 0;
    unsigned char releasekey = 0;
    struct imx6uirq_dev *dev = (struct imx6uirq_dev *)filp->private_data;

#if 0
    // 加入等待队列,等待被唤醒,也就是有按键按下
    ret = wait_event_interruptible(dev->r_wait, atomic_read(&dev->releasekey));
    if(ret < 0)
    {
        goto wait_error;
    }
#endif

    // 定义一个等待队列
    DECLARE_WAITQUEUE(wait, current);  // 定义一个等待队列项
    if(atomic_read(&dev->releasekey) == 0)  // 没有按键按下
    {
        add_wait_queue(&dev->r_wait, &wait);  // 添加到等待队列头
        __set_current_state(TASK_INTERRUPTIBLE);  // 设置当前任务为可中断的
        schedule();  // 调度其他任务
        if(signal_pending(current))  // 判断是否为是信号引起的唤醒
        {
            ret = -ERESTARTSYS;
            goto wait_error;
        }
        __set_current_state(TASK_RUNNING);  // 设置当前任务为运行态
        remove_wait_queue(&dev->r_wait, &wait);  // 从等待队列头移除
    }


    keyvalue = atomic_read(&dev->keyvalue);
    releasekey = atomic_read(&dev->releasekey);

    if(releasekey)   // 有按键按下
    {
        if(keyvalue & 0x80)   // 0x81 & 0x80 = 0x80
        {
            keyvalue &= ~0x80;  // 0x81 & ~0x80 = 0x01
            ret = copy_to_user(buf, &keyvalue, sizeof(keyvalue));
        }
        else
        {
            ret = -EINVAL;
        }
        atomic_set(&dev->releasekey, 0);  // 按下标志清零
    }
    else
    {
        return -EINVAL;
    }
    return 0;

wait_error:
    set_current_state(TASK_RUNNING);
    remove_wait_queue(&dev->r_wait, &wait);
    return ret;
}





// 设备操作函数 
static struct file_operations imx6uirq_fops = {
    .owner = THIS_MODULE,
    .open = imx6uirq_open,
    .read = imx6uirq_read,
};


static int __init imx6uirq_init(void)
{

    // 注册字符设备驱动 
    // 1.创建设备号
    if(imx6uirq.major){
        imx6uirq.devid = MKDEV(imx6uirq.major, 0);
        register_chrdev_region(imx6uirq.devid, IMX6UIRQ_CNT, IMX6UIRQ_NAME);
    }
    else{
        alloc_chrdev_region(&imx6uirq.devid, 0, IMX6UIRQ_CNT, IMX6UIRQ_NAME);
        imx6uirq.major = MAJOR(imx6uirq.devid);
        imx6uirq.minor = MINOR(imx6uirq.devid);
    }
    // 2. 初始化cdev
    imx6uirq.cdev.owner = THIS_MODULE;
    cdev_init(&imx6uirq.cdev, &imx6uirq_fops);

    // 3. 添加一个cdev
    cdev_add(&imx6uirq.cdev, imx6uirq.devid, IMX6UIRQ_CNT);

    // 4. 创建类
    imx6uirq.class = class_create(THIS_MODULE, IMX6UIRQ_NAME);
    if(IS_ERR(imx6uirq.class)){
        return PTR_ERR(imx6uirq.class);
    }

    // 5. 创建设备
    imx6uirq.device = device_create(imx6uirq.class, NULL, imx6uirq.devid, NULL, IMX6UIRQ_NAME);
    if(IS_ERR(imx6uirq.device)){
        return PTR_ERR(imx6uirq.device);
    }

    // 初始化按键
    atomic_set(&imx6uirq.keyvalue, 0);  // 初始化按键值
    atomic_set(&imx6uirq.releasekey, 0);  // 初始化按键标志
    keyio_init();  // 初始化按键

    return 0;
}

static void __exit imx6uirq_exit(void)
{
    unsigned int i;
    del_timer_sync(&imx6uirq.timer);  // 删除定时器
    // 释放中断
    for(i = 0; i < KEY_NUM; i++)
    {
        free_irq(imx6uirq.irqkeydesc[i].irqnum, &imx6uirq);
        gpio_free(imx6uirq.irqkeydesc[i].gpio);  // 释放GPIO
    }

    cdev_del(&imx6uirq.cdev);  // 注销cdev
    unregister_chrdev_region(imx6uirq.devid, IMX6UIRQ_CNT);  // 释放设备号
    device_destroy(imx6uirq.class, imx6uirq.devid);
    class_destroy(imx6uirq.class);
}


module_init(imx6uirq_init);
module_exit(imx6uirq_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("daiboo");





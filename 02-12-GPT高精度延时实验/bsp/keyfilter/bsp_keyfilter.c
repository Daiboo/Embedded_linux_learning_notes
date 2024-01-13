#include "bsp_keyfilter.h"
#include "bsp_gpio.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_beep.h"

/* 初始化keyfilter */
void keyfilter_init(void)
{
    gpio_pin_config_t key0_config;

    /* 初始化IO复用*/
    IOMUXC_SetPinMux(KEY0_IOMUXC, 0);
    /* 配置GPIO5_IO01的IO属性 */
    IOMUXC_SetPinConfig(KEY0_IOMUXC, KEY0_CONFIG);

    /* 配置GPIO5_IO01为输入 */
    key0_config.direction = kGPIO_DigitalInput;
    key0_config.interruptMode = kGPIO_IntRisingEdge;  // 上升沿触发
    gpio_init(KEY0_GPIO, KEY0_GPIO_PIN, &key0_config);

    GIC_EnableIRQ(KEY0_IRQN_TYPE);   /* 使能GIC中对应的中断 */
    /* 注册中断处理函数 */
    system_register_irqhandler(KEY0_IRQN_TYPE, (system_irq_handler_t)key0_irqhandler, NULL);
    gpio_enableInt(KEY0_GPIO, KEY0_GPIO_PIN); /* 使能引脚的中断功能 */

    /* 初始化定时器 */
    filtertimer_init(66000000 / 100);   // 定时周期10ms
}

/* 初始化定时器 */
void filtertimer_init(uint32_t value)
{
    /* 配置EPIT1_CR寄存器 */
    EPIT1->CR = 0;
    
    EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 24);

    EPIT1->LR = value;   /* 加载寄存器,相当于倒计数值 */

    EPIT1->CMPR = 0;    /* counter与0比较 */

    /* 初始化中断 */
    GIC_EnableIRQ(EPIT1_IRQn);  // 使能EPIT1_IRQn
    system_register_irqhandler(EPIT1_IRQn, (system_irq_handler_t)epit1_irqhandler, NULL); // 注册中断服务函数

}

/* 关闭EPIT1定时器 */
void filtertimer_stop(void)
{
    EPIT1->CR &= ~(1 << 0);
}

/* 重启EPIT1定时器 */
void filtertimer_restart(uint32_t value)
{
    EPIT1->CR &= ~(1 << 0); /* 关闭定时器 */
    EPIT1->LR = value;      /* 加载寄存器,相当于倒计数值 */
    EPIT1->CR |= 1 << 0;    /* 使能EPIT1 */
}

/* EPIT1定时器中断处理函数 */
void epit1_irqhandler(unsigned int gicciar, void *param)
{
    static unsigned char state = OFF;

    if(EPIT1->SR & (1 << 0))
    {
        /* 关闭定时器 */
        filtertimer_stop();

        if(gpio_pinread(KEY0_GPIO, KEY0_GPIO_PIN) == KEY0_VALUE)
        {
            state = !state;
            beep_switch(state);
        }
    }

    EPIT1->SR |= 1 << 0;    /* 清除中断标志位 */
}



/* 按键中断服务函数 */
void key0_irqhandler(unsigned int gicciar, void *param)
{
    /* 开启定时器 */
    filtertimer_restart(66000000 / 100);

    /* 清楚中断标志位 */
    gpio_clearIntflag(KEY0_GPIO, KEY0_GPIO_PIN);

}
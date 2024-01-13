#include "bsp_exti.h"
#include "bsp_gpio.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_beep.h"
/* 初始化外部中断,也就是gpio5_1 */
void exti_init(void)
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
}


/* KEY0中断处理函数 */
void key0_irqhandler(unsigned int gicciar, void *param)
{
    static unsigned char status = 0;

    if(gpio_pinread(KEY0_GPIO, KEY0_GPIO_PIN) == KEY0_VALUE)
    {
        status = !status;
        beep_switch(status);
    }

    /* 清楚中断标志位 */
    gpio_clearIntflag(KEY0_GPIO, KEY0_GPIO_PIN);

}
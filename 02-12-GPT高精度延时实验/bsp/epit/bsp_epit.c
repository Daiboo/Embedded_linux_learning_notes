#include "bsp_epit.h"
#include "bsp_int.h"
#include "bsp_led.h"

/* 初始化EPIT */
void epit_init(uint32_t frac, uint32_t value)
{
    if(frac > 4095)
    {
        frac = 4095;
    }

    /* 配置EPIT1_CR寄存器 */
    EPIT1->CR = 0;
    
    EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | (frac << 4) | (1 << 24);

    EPIT1->LR = value;   /* 加载寄存器,相当于倒计数值 */

    EPIT1->CMPR = 0;    /* counter与0比较 */

    /* 初始化中断 */
    GIC_EnableIRQ(EPIT1_IRQn);  // 使能EPIT1_IRQn
    system_register_irqhandler(EPIT1_IRQn, (system_irq_handler_t)epit1_irqhandler, NULL); // 注册中断服务函数
    
    EPIT1->CR |= 1 << 0;    /* 使能EPIT1 */
}


/* EPIT1中断服务函数 */
void epit1_irqhandler(void)
{
    static uint8_t state = 0;

    if(EPIT1->SR & (1 << 0))
    {
        state = !state;
        led_switch(led_green, state);
    }

    EPIT1->SR |= 1 << 0;    /* 清除中断标志位 */
}
#include "main.h"

/* 使能外设时钟 */
void clk_enable(void)
{
    CCM_CCGR1 = 0xFFFFFFFF;  // 使能所有GPIO时钟

}
/* 短延时 */
void delay_short(volatile unsigned int n)
{
    while(n--){}
}

/* 延时,一次循环大概是1ms, 在主频396MHZ时候*/
void delay(volatile unsigned int n)
{
    while(n--)
    {
        delay_short(0x7FF);
    }
}
/* 打开led */
void led_on(void)
{
    GPIO1_DR &= ~(1<<4);  // bit4清零
}

void led_off(void)
{
    GPIO1_DR |= (1<<4);  // bit4置1
}

/* 初始化led*/
void led_init(void)
{
    /* 1、初始化IO复用 */
    IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO04 = 0x5;

    /* 2、配置GPIO1_IO04的IO属性 */
    IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO04 = 0x10B0;

    /* 3、设置GPIO1_IO04为输出 */
    GPIO1_GDIR = 0x10;
 
    /* 4、设置GPIO1_IO04输出低电平 */
    GPIO1_DR = 0x0;

}


int main(void)
{   
    /* 使能时钟 */
    clk_enable();

    /* 初始化led */
    led_init();

    /* 设置led闪烁 */
    while(1)
    {
        led_on();
        delay(500);
        led_off();
        delay(500);
    }

    return 0;
}
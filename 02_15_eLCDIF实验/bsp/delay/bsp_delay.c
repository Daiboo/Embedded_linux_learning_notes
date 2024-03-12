#include "bsp_delay.h"
#include "bsp_int.h"
#include "bsp_led.h"

/* 延时初始化函数 */
void delay_init(void)
{
    /* CR寄存器清零 */
    GPT1->CR = 0;

    /* 软复位 */
    GPT1->CR |= 1 << 15;
    while((GPT1->CR >> 15) & 0x01);

    /* 
    * 设置GPT1时钟源为ipg_clk=66MHz,restart 模式
    * 默认计数器从0开始，一直加到0xFFFF FFFF
    */
    GPT1->CR |= (1 << 1) | (1 << 6);

    /* 设置分频 */
    GPT1->PR = 65;    // 66分频，1us加1

    /*
    * 1M的频率,计一个数就是1us,那么0xFFFFFFFF个数就是71.5分钟
    */
    GPT1->OCR[0] = 0xFFFFFFFF;  // 设置输出比较寄存器1的值

#if 0
    /* 设置GPT1的输出比较寄存器1的值 */
    GPT1->OCR[0] = 1000000 / 2;  // 设置中断周期为500ms

    /* 打开GPT1输出比较通道1中断 */
    GPT1->IR |= 1 << 0;

    /* GIC */
    GIC_EnableIRQ(GPT1_IRQn);

    /* 注册中断服务函数 */
    system_register_irqhandler(GPT1_IRQn, (system_irq_handler_t)gpt1_irqhandler, NULL);
#endif
    /* 使能GPT1 */
    GPT1->CR |= 1 << 0;
}
#if 0
/* gpt1输出比较中断服务函数 */
void gpt1_irqhandler(unsigned int gicciar, void *param)
{
    static unsigned char state = 0;
    if(GPT1->SR & (1 << 0))
    {
        state = !state;
        led_switch(led_green, state);
    }
    /* 清除中断标志位 */
    GPT1->SR |= 1 << 0;
}
#endif 

/* us延时 */
void delayus(uint32_t usdelay)
{
    uint32_t oldcnt,newcnt;
    uint32_t tcntvalue = 0;

    oldcnt = GPT1->CNT;

    while(1)
    {
        newcnt = GPT1->CNT;
        if(newcnt != oldcnt)
        {
            if(newcnt > oldcnt)
            {
                tcntvalue += newcnt - oldcnt;
            }
            else
            {
                tcntvalue += 0xFFFFFFFF - oldcnt + newcnt;
            }
            oldcnt = newcnt;
            if(tcntvalue >= usdelay)
            {
                break;
            }
        }
    }
}

/* 毫秒延时 */
void delayms(uint32_t msdelay)
{
    int i = 0;
    for(i = 0; i < msdelay; i++)
    {
        delayus(1000);
    }
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
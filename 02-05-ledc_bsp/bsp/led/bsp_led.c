#include "bsp_led.h"
/* 打开led */
void led_red_on(void)
{
    GPIO1->DR &= ~(1<<4);  // bit4清零
}

void led_red_off(void)
{
    GPIO1->DR |= (1<<4);  // bit4置1
}

void led_green_on(void)
{
    GPIO4->DR &= ~(1<<20);  // bit20清零
}

void led_green_off(void)
{
    GPIO4->DR |= (1<<20);  // bit20置1
}
void led_blue_on(void)
{
    GPIO4->DR &= ~(1<<19);  // bit19清零
}
void led_blue_off(void)
{
    GPIO4->DR |= (1<<19);  // bit19置1
}

/* 初始化led*/
void led_init(void)
{
    /* 1、初始化IO复用 */
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO04_GPIO1_IO04, 0);  // 复用为GPIO1_IO04
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO04_GPIO1_IO04, 0x10B1);  // 设置电气属性

    IOMUXC_SetPinMux(IOMUXC_CSI_HSYNC_GPIO4_IO20,0);   // 复用为GPIO4_IO20
    IOMUXC_SetPinConfig(IOMUXC_CSI_HSYNC_GPIO4_IO20,0x10B1);  // 设置电气属性

    IOMUXC_SetPinMux(IOMUXC_CSI_VSYNC_GPIO4_IO19,0);   // 复用为GPIO4_IO19
    IOMUXC_SetPinConfig(IOMUXC_CSI_VSYNC_GPIO4_IO19,0x10B1);  // 设置电气属性


    /* 3、设置GPIO1_IO04为输出 */
    GPIO1->GDIR |= (1 << 4);  // bit4设置为输出
    GPIO4->GDIR |= (1 << 20);  // bit20设置为输出
    GPIO4->GDIR |= (1 << 19);  // bit19设置为输出
 
    /* 4、设置GPIO1_IO04输出低电平 */
    GPIO1->DR |= (1 << 4);   // bit4设置为高电平
    GPIO4->DR |= (1 << 20);        // bit20设置为高电平
    GPIO4->DR |= (1 << 19);        // bit19设置为高电平

}

void led_switch(int led, int status)
{
    switch (led)
    {
        case led_red:
        {
            if (status == ON)
                led_red_on();
            else if (status == OFF)
                led_red_off();
            else
                led_red_off();
        }
        break;
    }

    
}
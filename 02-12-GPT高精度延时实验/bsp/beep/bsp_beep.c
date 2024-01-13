#include "bsp_beep.h"
#include "bsp_gpio.h"

void beep_init(void)
{
    gpio_pin_config_t beep_config;
    /* 初始化IO复用 */
    IOMUXC_SetPinMux(BEEP_CTL_PIN_MUX_IO, 0);  // 复用为GPIO1_IO19
    IOMUXC_SetPinConfig(BEEP_CTL_PIN_MUX_IO, 0x10B0);  // 设置电器属性

    /* 初始化GPIO */
    beep_config.direction = kGPIO_DigitalOutput;  // 输出
    beep_config.outputLogic = 0;  // 默认输出高电平
    gpio_init(BEEP_GPIO, BEEP_GPIO_PIN, &beep_config);


}

void beep_on(void)
{
    // BEEP_GPIO->DR &= ~(1 << BEEP_GPIO_PIN);  // 输出低电平，关闭蜂鸣器
    gpio_pinwrite(BEEP_GPIO, BEEP_GPIO_PIN, 0);
}

void beep_off(void)
{
    // BEEP_GPIO->DR |= (1 << BEEP_GPIO_PIN);  // 输出高电平，打开蜂鸣器
    gpio_pinwrite(BEEP_GPIO, BEEP_GPIO_PIN, 1);
}


void beep_switch(int status)
{
    if (status == ON) {
        // BEEP_GPIO->DR |= (1 << BEEP_GPIO_PIN);  // 输出高电平，打开蜂鸣器
        gpio_pinwrite(BEEP_GPIO, BEEP_GPIO_PIN, 1);
        
    } else if (status == OFF) {
        // BEEP_GPIO->DR &= ~(1 << BEEP_GPIO_PIN);  // 输出低电平，关闭蜂鸣器
        gpio_pinwrite(BEEP_GPIO, BEEP_GPIO_PIN, 0);
    }
}
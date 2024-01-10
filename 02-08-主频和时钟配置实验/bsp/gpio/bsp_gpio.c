#include "bsp_gpio.h"


/**
 * @brief Initializes a GPIO pin with the specified configuration.
 *
 * This function initializes a GPIO pin with the specified configuration. It sets the direction of the pin
 * as either input or output based on the configuration. If the pin is configured as an output, it also sets
 * the default output logic level.
 *
 * @param base The base address of the GPIO port.
 * @param pin The pin number to be initialized.
 * @param config Pointer to the configuration structure.
 */
void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config)
{
    if(config->direction == kGPIO_DigitalInput)
    {
        base->GDIR &= ~(1 << pin);   // 0: input
    }
    else
    {
        base->GDIR |= (1 << pin);    // 1: output
        gpio_pinwrite(base, pin, config->outputLogic);  // 设置默认输出电平
    }
}


/* 读取gpio pin值*/
/**
 * @brief Writes a value to a specific pin of a GPIO port.
 *
 * This function sets or clears a specific pin of a GPIO port based on the given value.
 *
 * @param base Pointer to the GPIO port base address.
 * @param pin The pin number to write to.
 * @param value The value to write to the pin (0 or 1).
 */
void gpio_pinwrite(GPIO_Type *base, int pin, int value)
{
    if(value == 0U)
    {
        base->DR &= ~(1 << pin);
    }
    else
    {
        base->DR |= (1 << pin);
    }
}

/* 读取指定io电平*/
/**
 * @brief 读取指定GPIO引脚的状态
 *
 * @param base GPIO模块的基地址
 * @param pin  要读取的引脚号
 * @return     引脚的状态，0表示低电平，1表示高电平
 */
int gpio_pinread(GPIO_Type *base, int pin)
{
    return (((base->DR) >> pin) & 0x1);
}
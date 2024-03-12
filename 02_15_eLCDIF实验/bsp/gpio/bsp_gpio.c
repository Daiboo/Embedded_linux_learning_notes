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
    gpio_intConfig(base, pin, config->interruptMode);  // 配置中断模式
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


/* 使能指定IO中断 */
void gpio_enableInt(GPIO_Type *base, int pin)
{
    base->IMR |= (1 << pin);
}

/* 禁止指定IO中断 */
void gpio_disableInt(GPIO_Type *base, int pin)
{
    base->IMR &= ~(1 << pin);
}

/* 清楚中断标志位 */
void gpio_clearIntflag(GPIO_Type *base, int pin)
{
    base->ISR |= (1 << pin);
}

/* gpio中断初始化函数 */
void gpio_intConfig(GPIO_Type *base, int pin, gpio_interrupt_mode_t pinInterruptMode)
{
    /* 1. 配置GPIO中断模式 */
    volatile uint32_t *icr;
    uint32_t icrShift;

    icrShift = pin;
    base->EDGE_SEL &= ~(1 << pin);  
    
    if(pin < 16)
    {
        icr = &(base->ICR1);
    }
    else
    {
        icr = &(base->ICR2);
        icrShift -= 16;
    }

    switch (pinInterruptMode)
    {
        case kGPIO_IntLowLevel:
            *icr &= ~(3 << (2 * icrShift));
            break;
        case kGPIO_IntHighLevel:
            *icr &= ~(3 << (2 * icrShift));
            *icr |= (1 << (2 * icrShift));
            break;
        case kGPIO_IntRisingEdge:
            *icr &= ~(3 << (2 * icrShift));
            *icr |= (2 << (2 * icrShift));
            break;
        case kGPIO_IntFallingEdge:
            *icr &= ~(3 << (2 * icrShift));
            *icr |= (3 << (2 * icrShift));
            break;
        case kGPIO_IntRisingOrFallingEdge:
            base->EDGE_SEL |= (1 << pin);
            break;
        default:
            break;
    }


    /* 2. 使能GPIO中断 */
    gpio_enableInt(base, pin);
}

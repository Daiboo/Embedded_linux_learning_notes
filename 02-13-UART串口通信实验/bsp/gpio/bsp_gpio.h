#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__
#include "imx6ul.h"

/* 描述中断触发类型 */
typedef enum _gpio_interrupt_mode
{
    kGPIO_NoIntmode = 0U,       /* 无中断功能 */
    kGPIO_IntLowLevel = 1U,     /* 低电平触发 */
    kGPIO_IntHighLevel = 2U,    /* 高电平触发 */
    kGPIO_IntRisingEdge = 3U,   /* 上升沿触发 */
    kGPIO_IntFallingEdge = 4U,  /* 下降沿触发 */
    kGPIO_IntRisingOrFallingEdge = 5U,   /* 上升沿和下降沿都触发 */
} gpio_interrupt_mode_t;


/* 枚举类结构体和gpio结构体 */
typedef enum _gpio_pin_direction
{
    kGPIO_DigitalInput = 0U,  /* 输入 */
    kGPIO_DigitalOutput = 1U, /* 输出 */
} gpio_pin_direction_t;

typedef struct _gpio_pin_config
{
    gpio_pin_direction_t direction; /* GPIO方向 */
    uint8_t outputLogic;            /* 输出电平 */
    gpio_interrupt_mode_t interruptMode;    /* 中断模式 */
} gpio_pin_config_t;

/* 函数声明 */
void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config);
int gpio_pinread(GPIO_Type *base, int pin);
void gpio_pinwrite(GPIO_Type *base, int pin, int value);

/* 使能指定IO中断 */
void gpio_enableInt(GPIO_Type *base, int pin);
/* 禁止指定IO中断 */
void gpio_disableInt(GPIO_Type *base, int pin);
/* 清楚中断标志位 */
void gpio_clearIntflag(GPIO_Type *base, int pin);
/* gpio中断初始化函数 */
void gpio_intConfig(GPIO_Type *base, int pin, gpio_interrupt_mode_t pinInterruptMode);

#endif // __BSP_GPIO_H__
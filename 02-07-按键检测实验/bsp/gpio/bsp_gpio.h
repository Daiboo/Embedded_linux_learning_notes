#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__
#include "imx6ul.h"

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
} gpio_pin_config_t;

/* 函数声明 */
void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config);
int gpio_pinread(GPIO_Type *base, int pin);
void gpio_pinwrite(GPIO_Type *base, int pin, int value);


#endif // __BSP_GPIO_H__
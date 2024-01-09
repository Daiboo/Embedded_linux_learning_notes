#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__
#include "imx6ul.h"

/**************************BEGIN CONFIG******************************/
#define KEY0_GPIO_PIN       (1U)
#define KEY0_GPIO           GPIO5
#define KEY0_IOMUXC         IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01

#define KEY0_CONFIG         (   IOMUXC_SW_PAD_CTL_PAD_SRE(0) |  \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(0) |  \
                                IOMUXC_SW_PAD_CTL_PAD_SPEED(1) | \
                                IOMUXC_SW_PAD_CTL_PAD_ODE(0) |    \
                                IOMUXC_SW_PAD_CTL_PAD_PKE(1) |  \
                                IOMUXC_SW_PAD_CTL_PAD_PUE(1) |  \
                                IOMUXC_SW_PAD_CTL_PAD_PUS(0) | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS(0))
                            
/* 按键值 */
// typedef enum
// {
//     KEY_NONE = 0,
//     KEY0_VALUE = 1,   // KEY0按下时,引脚是高电平
// }keyvalue;
#define KEY_NONE 0
#define KEY0_VALUE 1    // KEY0按下时,引脚是高电平



/**************************END CONFIG******************************/


/**
 * @brief Initializes the button module.
 */
void key_init(void);

/**
 * @brief Reads the state of the button.
 * @return The state of the button (0 or 1).
 */
int key_read(void);

/**
 * @brief 获取按键值
 * 
 * @return int 按键值
 */
int get_key_value(void);

#endif // __BSP_KEY_H__
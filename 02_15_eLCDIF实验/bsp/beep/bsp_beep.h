#ifndef __BSP_BEEP_H__
#define __BSP_BEEP_H__
#include "imx6ul.h"

#define     BEEP_CTL_PIN_MUX_IO   IOMUXC_UART1_RTS_B_GPIO1_IO19   // beep控制引脚复用
#define     BEEP_GPIO             GPIO1                          // beep控制引脚所在的GPIO
#define     BEEP_GPIO_PIN         (19U)                           // beep控制引脚编号

/**
 * @brief 初始化蜂鸣器模块
 */
void beep_init(void);

/**
 * @brief 打开蜂鸣器
 */
void beep_on(void);

/**
 * @brief 关闭蜂鸣器
 */
void beep_off(void);

/**
 * @brief 控制蜂鸣器开关状态
 * 
 * @param status 开关状态，OFF表示关闭，ON表示打开
 */
void beep_switch(int status);

#endif // __BSP_BEEP_H__
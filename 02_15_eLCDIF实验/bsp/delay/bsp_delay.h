#ifndef __BSP_DELAY_H__
#define __BSP_DELAY_H__

#include "imx6ul.h"
void delay_short(volatile unsigned int n);
void delay(volatile unsigned int n);
/* gpt1输出比较中断服务函数 */
void gpt1_irqhandler(unsigned int gicciar, void *param);
/* 延时初始化函数 */
void delay_init(void);
/* us延时 */
void delayus(uint32_t usdelay);
/* 毫秒延时 */
void delayms(uint32_t msdelay);

#endif // __BSP_DELAY_H__
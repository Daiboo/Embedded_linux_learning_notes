#ifndef __BSP_KEYFILTER_H__
#define __BSP_KEYFILTER_H__
#include "imx6ul.h"
/* 初始化keyfilter */
void keyfilter_init(void);
/* 初始化定时器 */
void filtertimer_init(uint32_t value);
/* EPIT1定时器中断处理函数 */
void epit1_irqhandler(unsigned int gicciar, void *param);
/* KEY0中断处理函数 */
void key0_irqhandler(unsigned int gicciar, void *param);
/* 重启EPIT1定时器 */
void filtertimer_restart(uint32_t value);
/* 关闭EPIT1定时器 */
void filtertimer_stop(void);

#endif // __BSP_KEYFILTER_H__
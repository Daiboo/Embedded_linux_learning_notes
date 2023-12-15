#ifndef __MAIN_H__
#define __MAIN_H__
/* 定义要使用的寄存器*/
// 时钟控制寄存器
#define CCM_CCGR0  *((volatile unsigned long *)0x020C4068)
#define CCM_CCGR1  *((volatile unsigned long *)0x020C406C)

// GPIO1_IO04复用功能选择寄存器
#define IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO04 *((volatile unsigned long *)0x020E006C)

// PAD属性设置寄存器
#define IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO04 *((volatile unsigned long *)0x020E02F8)

// GPIO方向设置寄存器
#define GPIO1_GDIR *((volatile unsigned long *)0x0209C004)

// GPIO数据输出寄存器
#define GPIO1_DR *((volatile unsigned long *)0x0209C000)




#endif // __MAIN_H__
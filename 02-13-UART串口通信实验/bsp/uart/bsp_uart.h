#ifndef __BSP_UART_H__
#define __BSP_UART_H__
#include "imx6ul.h"

/* 宏定义UART1 */
#define UART1_TX_MUX_IO          IOMUXC_UART1_TX_DATA_UART1_TX
#define UART1_RX_MUX_IO          IOMUXC_UART1_RX_DATA_UART1_RX

/* 宏定义IO属性 */
#define UART1_TX_IO_PAD_CONFIG  (   IOMUXC_SW_PAD_CTL_PAD_SRE(0) |  \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(1) |  \
                                IOMUXC_SW_PAD_CTL_PAD_SPEED(1) | \
                                IOMUXC_SW_PAD_CTL_PAD_ODE(0) |    \
                                IOMUXC_SW_PAD_CTL_PAD_PKE(1) |  \
                                IOMUXC_SW_PAD_CTL_PAD_PUE(0) |  \
                                IOMUXC_SW_PAD_CTL_PAD_PUS(3) | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS(0))          

#define UART1_RX_IO_PAD_CONFIG  (   IOMUXC_SW_PAD_CTL_PAD_SRE(0) |  \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(1) |  \
                                IOMUXC_SW_PAD_CTL_PAD_SPEED(1) | \
                                IOMUXC_SW_PAD_CTL_PAD_ODE(0) |    \
                                IOMUXC_SW_PAD_CTL_PAD_PKE(1) |  \
                                IOMUXC_SW_PAD_CTL_PAD_PUE(1) |  \
                                IOMUXC_SW_PAD_CTL_PAD_PUS(3) | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS(0))     
/* 初始化UART1,波特率为115200 */
void uart1_init(void);
/* UART1的io的初始化 */
void uart1_io_init(void);
/* 设置波特率 */
void uart_setbaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz);
/* 关闭UART1 */
void uart_disable(UART_Type *base);
/* 使能UART1 */
void uart_enable(UART_Type *base);
/* 软复位UART */
void uart_softreset(UART_Type *base);
/* 通过UART发送字符 */
void putc(unsigned char c);
/* 接收UART字符 */
unsigned char getc(void);
/* 通过串口发送一串字符 */
void puts(char *str);

#endif // __BSP_UART_H__
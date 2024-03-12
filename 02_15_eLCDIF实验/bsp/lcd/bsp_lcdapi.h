#ifndef __BSP_LCDAPI_H__
#define __BSP_LCDAPI_H__
#include "imx6ul.h"
#include "bsp_lcd.h"

/* 函数声明 */
void lcd_drawline(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void lcd_draw_rectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void lcd_draw_Circle(unsigned short x0,unsigned short y0,unsigned char r);
void lcd_showchar(unsigned     short x,unsigned short y,unsigned char num,unsigned char size, unsigned char mode);
unsigned int lcd_pow(unsigned char m,unsigned char n);
void lcd_shownum(unsigned short x, unsigned short y, unsigned int num, unsigned char len,unsigned char size);
void lcd_showxnum(unsigned short x, unsigned short y, unsigned int num, unsigned char len, unsigned char size, unsigned char mode);
void lcd_show_string(unsigned short x,unsigned short y,
unsigned short width, unsigned short height, unsigned char size,char *p);



#endif // __BSP_LCDAPI_H__
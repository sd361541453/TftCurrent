/*
 * lcd.h
 *
 *  Created on: 2019-7-4
 *      Author: Jason
 */

#ifndef LCD_H_
#define LCD_H_

typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned int u32;


////IO连接
//sbit LCD_CS     =P0^0;     //片选
//sbit LCD_DC     =P0^2;	  //数据/命令切换
//sbit LCD_SDI    =P1^5;	  //数据
//sbit LCD_SCK    =P1^7;	  //时钟
//sbit LCD_REST   =P0^1;	  //复位

//#define LCD_SDI 20
//#define LCD_SCK 30
//#define LCD_SDO 19
////#define LCD_CS 21
////#define LCD_CS 2
//#define LCD_CS_REAL 21
//
//#define LCD_REST 29
//#define LCD_DC 31



#define LCD_SDI 31
#define LCD_SCK 27
#define LCD_SDO 2
#define LCD_CS 25
//#define LCD_CS 16
#define LCD_CS_REAL 25

#define LCD_REST 4
#define LCD_DC 26



#include "nrf_delay.h"
#define delay_ms nrf_delay_ms


/*************** high performer GPIO operation ****************/

/**
 * high performer GPIO operation - faster
 *
 * different:
 * 	clear/set:0.88us/0.13us
 * 	toggle: 1.30us/0.31us
 *
 * 	52832:
 * 	clear/set:	17ns
 * 	toggle: 	60ns
 */
#define GPIO_TOGGLE(PIN)	((NRF_GPIO->OUT) ^= (1 << (PIN)))
#define GPIO_HIGH(PIN)	((NRF_GPIO->OUTSET) = (1 << (PIN)))
#define GPIO_LOW(PIN)	((NRF_GPIO->OUTCLR) = (1 << (PIN)))
#define GPIO_READ(PIN)	((NRF_GPIO->IN >> PIN) & 0x01)


/*************** high performer GPIO operation ****************/





//SPI显示屏接口
//LCD_RST
#define SPILCD_RST_HIGH  GPIO_HIGH(LCD_REST)
#define SPILCD_RST_LOW GPIO_LOW(LCD_REST)
//LCD_RS//dc
#define SPILCD_RS_HIGH  GPIO_HIGH(LCD_DC)
#define SPILCD_RS_LOW  GPIO_LOW(LCD_DC)
//LCD_CS
//#define SPILCD_CS_HIGH GPIO_HIGH(LCD_CS)
//#define SPILCD_CS_LOW  GPIO_LOW(LCD_CS)
#define SPILCD_CS_HIGH
#define SPILCD_CS_LOW






//LCD重要参数集
typedef struct {
	u16 width; //LCD 宽度
	u16 height; //LCD 高度
	u16 id; //LCD ID
	u8 wramcmd; //开始写gram指令
	u8 setxcmd; //设置x坐标指令
	u8 setycmd; //设置y坐标指令
} _lcd_dev;

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色
extern u16  POINT_COLOR;//默认红色
extern u16  BACK_COLOR; //背景颜色.默认为白色


//LCD地址结构体
typedef struct {
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;

//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A10作为数据命令区分线
//注意设置时STM32内部会右移一位对其! 111110=0X3E
#define LCD_BASE        ((u32)(0x60000000 | 0x0007FFFE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

void LCD_Init(void);													   	//初始化
void LCD_DisplayOn(void);													//开显示
void LCD_DisplayOff(void);													//关显示
void LCD_Clear(u16 Color);	 												//清屏
void LCD_SetCursor(u16 Xpos, u16 Ypos);										//设置光标
void LCD_DrawPoint(u16 x,u16 y);											//画点
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);								//快速画点
void Draw_Circle(u16 x0,u16 y0,u8 r);										//画圆
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);							//画线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   				//画矩形
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);		   				//填充单色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//填充指定颜色
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//显示 数字
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//显示一个字符串,12/16字体

void showimage(u16 x,u16 y); //显示40*40图片
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
void LCD_WriteRAM_Prepare(void);
void LCD_WR_DATA8(u8 da);   //写8位数据

void showhanzi16(unsigned int x,unsigned int y,unsigned char index);//16*16汉字
void showhanzi32(unsigned int x,unsigned int y,unsigned char index);//32*32汉字

















#endif /* LCD_H_ */

#ifndef __LCD1602_H
#define	__LCD1602_H

#include "nrf52.h"

#define E				       20	//设备使能 1
#define RW			       19	//读1 / 写0选择
#define RS			       18
 

void Pin_Init(void);
void LCD_Wcom(unsigned char com);
void LCD_Wdat(unsigned char dat);
void LCD1602_Init(void);


#endif


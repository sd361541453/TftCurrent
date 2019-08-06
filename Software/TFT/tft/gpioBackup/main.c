/**
 * Copyright (c) 2014 - 2018, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 *
 * @defgroup blinky_example_main main.c
 * @{
 * @ingroup blinky_example
 * @brief Blinky Example Application main file.
 *
 * This file contains the source code for a sample application to blink LEDs.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
//#include "boards.h"
#include "nrf_gpio.h"


#include "lcd.h"




#define LED 7





void SPI_WriteByte(u8 data){

	u8 bitdata = data;
	if(bitdata & 0x80){
		GPIO_HIGH(LCD_SDI);
	}else{
		GPIO_LOW(LCD_SDI);
	}
	GPIO_HIGH(LCD_SCK);
	GPIO_LOW(LCD_SCK);
	if(bitdata & 0x40){
		GPIO_HIGH(LCD_SDI);
	}else{
		GPIO_LOW(LCD_SDI);
	}
	GPIO_HIGH(LCD_SCK);
	GPIO_LOW(LCD_SCK);
	if(bitdata & 0x20){
		GPIO_HIGH(LCD_SDI);
	}else{
		GPIO_LOW(LCD_SDI);
	}
	GPIO_HIGH(LCD_SCK);
	GPIO_LOW(LCD_SCK);
	if(bitdata & 0x10){
		GPIO_HIGH(LCD_SDI);
	}else{
		GPIO_LOW(LCD_SDI);
	}
	GPIO_HIGH(LCD_SCK);
	GPIO_LOW(LCD_SCK);
	if(bitdata & 0x08){
		GPIO_HIGH(LCD_SDI);
	}else{
		GPIO_LOW(LCD_SDI);
	}
	GPIO_HIGH(LCD_SCK);
	GPIO_LOW(LCD_SCK);
	if(bitdata & 0x04){
		GPIO_HIGH(LCD_SDI);
	}else{
		GPIO_LOW(LCD_SDI);
	}
	GPIO_HIGH(LCD_SCK);
	GPIO_LOW(LCD_SCK);
	if(bitdata & 0x02){
		GPIO_HIGH(LCD_SDI);
	}else{
		GPIO_LOW(LCD_SDI);
	}
	GPIO_HIGH(LCD_SCK);
	GPIO_LOW(LCD_SCK);
	if(bitdata & 0x01){
		GPIO_HIGH(LCD_SDI);
	}else{
		GPIO_LOW(LCD_SDI);
	}
	GPIO_HIGH(LCD_SCK);
	GPIO_LOW(LCD_SCK);
}





void portInit(){

	nrf_gpio_cfg_output(LED);


	nrf_gpio_cfg_output(LCD_CS);
	nrf_gpio_cfg_output(LCD_DC);
	nrf_gpio_cfg_output(LCD_SDI);
	nrf_gpio_cfg_output(LCD_SCK);
	nrf_gpio_cfg_output(LCD_REST);

}

void xianshi(){//显示信息

	BACK_COLOR = WHITE;
	POINT_COLOR = RED;
	//显示32*32汉字
	showhanzi32(0, 0, 0); //淘
	showhanzi32(40, 0, 1); //晶
	showhanzi32(80, 0, 2); //驰
	//显示16*16汉字
	showhanzi16(0, 35, 0); //专
	showhanzi16(20, 35, 1); //注
	showhanzi16(40, 35, 2); //显
	showhanzi16(60, 35, 3); //示
	showhanzi16(80, 35, 4); //方
	showhanzi16(100, 35, 5); //案
	LCD_ShowString(0, 55, 200, 16, 16, "2.8 TFT SPI 240*320");
}
void showqq() {
	u16 x, y;
	x = 0;
	y = 75;
	while (y < lcddev.height - 39) {
		x = 0;
		while (x < lcddev.width - 39) {
			showimage(x, y);
			x += 40;
		}
		y += 40;
	}
}


/**
 * @brief Function for application main entry.
 */
int main(void) {


	portInit();


	LCD_Init();
 	POINT_COLOR=RED;//设置字体为红色
	xianshi();	   //显示信息
	showqq();	   //显示QQ

	for(;;) {
		GPIO_TOGGLE(LED);
		 nrf_delay_ms(200);

	}

}









































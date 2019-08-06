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




#define LED1 10
#define LED2 11


//#define SOC_SPI






#ifndef SOC_SPI


//#define JASON_DELAY	asm("nop");asm("nop");asm("nop");
#define JASON_DELAY

void SPI_WriteByte(u8 data){

	u8 bitdata = data;
	if(bitdata & 0x80){
		GPIO_HIGH(LCD_SDI);
	}else{
		GPIO_LOW(LCD_SDI);
	}
	GPIO_HIGH(LCD_SCK);
	JASON_DELAY;
	GPIO_LOW(LCD_SCK);
	if(bitdata & 0x40){
		GPIO_HIGH(LCD_SDI);
	}else{
		GPIO_LOW(LCD_SDI);
	}
	GPIO_HIGH(LCD_SCK);
	JASON_DELAY;
	GPIO_LOW(LCD_SCK);
	if(bitdata & 0x20){
		GPIO_HIGH(LCD_SDI);
	}else{
		GPIO_LOW(LCD_SDI);
	}
	GPIO_HIGH(LCD_SCK);
	JASON_DELAY;
	GPIO_LOW(LCD_SCK);
	if(bitdata & 0x10){
		GPIO_HIGH(LCD_SDI);
	}else{
		GPIO_LOW(LCD_SDI);
	}
	GPIO_HIGH(LCD_SCK);
	JASON_DELAY;
	GPIO_LOW(LCD_SCK);
	if(bitdata & 0x08){
		GPIO_HIGH(LCD_SDI);
	}else{
		GPIO_LOW(LCD_SDI);
	}
	GPIO_HIGH(LCD_SCK);
	JASON_DELAY;
	GPIO_LOW(LCD_SCK);
	if(bitdata & 0x04){
		GPIO_HIGH(LCD_SDI);
	}else{
		GPIO_LOW(LCD_SDI);
	}
	GPIO_HIGH(LCD_SCK);
	JASON_DELAY;
	GPIO_LOW(LCD_SCK);
	if(bitdata & 0x02){
		GPIO_HIGH(LCD_SDI);
	}else{
		GPIO_LOW(LCD_SDI);
	}
	GPIO_HIGH(LCD_SCK);
	JASON_DELAY;
	GPIO_LOW(LCD_SCK);
	if(bitdata & 0x01){
		GPIO_HIGH(LCD_SDI);
	}else{
		GPIO_LOW(LCD_SDI);
	}
	GPIO_HIGH(LCD_SCK);
	JASON_DELAY;
	GPIO_LOW(LCD_SCK);

}



#endif


#ifdef SOC_SPI

#include "app_util_platform.h"
#include "nrf_drv_spi.h"

#define SPI_INSTANCE  0 /**< SPI instance index. */
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE); /**< SPI instance. */
static volatile bool spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */

#define JASON_SPI_CONFIG                           \
{                                                            \
    .sck_pin      = LCD_SCK,                \
    .mosi_pin     = LCD_SDI,                \
    .miso_pin     = LCD_SDO,                \
    .ss_pin       = LCD_CS,                \
    .irq_priority = SPI_DEFAULT_CONFIG_IRQ_PRIORITY,         \
    .orc          = 0xFF,                                    \
    .frequency    = NRF_DRV_SPI_FREQ_125K,                     \
    .mode         = NRF_DRV_SPI_MODE_0,                      \
    .bit_order    = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST,         \
}

#define TEST_STRING "Nordic"
static uint8_t       m_tx_buf[] = TEST_STRING;           /**< TX buffer. */
static uint8_t       m_rx_buf[sizeof(TEST_STRING) + 1];    /**< RX buffer. */
static const uint8_t m_length = sizeof(m_tx_buf);        /**< Transfer length. */

#define MAX_LENTH	1024
uint8_t txBufTrue[MAX_LENTH];
uint8_t txBufFalse[MAX_LENTH];
uint8_t rxBuf[MAX_LENTH];
uint8_t txBufTrueLen;
uint8_t txBufFalseLen;
uint8_t txBufChoose;



/**
 * @brief SPI user event handler.
 * @param event
 */
void spi_event_handler(nrf_drv_spi_evt_t const * p_event, void * p_context) {
	spi_xfer_done = true;
}

void SpiInit(){

    nrf_drv_spi_config_t spi_config = JASON_SPI_CONFIG;
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler, NULL));
    spi_xfer_done = true;
    txBufChoose = false;
    txBufFalseLen = 0;
    txBufTrueLen = 0;
}

void SPI_flush(){

	while (!spi_xfer_done) {
		__WFE();
	}
    spi_xfer_done = false;

	if (txBufChoose) {
		nrf_drv_spi_transfer(&spi, txBufTrue, txBufTrueLen, rxBuf, 0);
		txBufFalseLen = 0;
		txBufChoose = false;
	} else {
		nrf_drv_spi_transfer(&spi, txBufFalse, txBufFalseLen, rxBuf, 0);
		txBufTrueLen = 0;
		txBufChoose = true;
	}
}

void SPI_WriteByte2(u8 data){

//	 nrf_drv_spi_transfer(&spi, m_tx_buf, m_length, m_rx_buf, m_length);

	if(txBufChoose){
		txBufTrue[txBufTrueLen] = data;
		txBufTrueLen++;
	}else{
		txBufFalse[txBufFalseLen] = data;
		txBufFalseLen++;
	}
	SPI_flush();
}


void SPI_WriteByte(u8 data){
	while (!spi_xfer_done) {
		__WFE();
	}
    spi_xfer_done = false;
	txBufTrue[0] = data;
	nrf_drv_spi_transfer(&spi, txBufTrue, 1, rxBuf, 0);
}



#endif


void portInit(){

	nrf_gpio_cfg_output(LED1);
	nrf_gpio_cfg_output(LED2);


//	nrf_gpio_cfg_output(LCD_CS);
	nrf_gpio_cfg_output(LCD_DC);
	nrf_gpio_cfg_output(LCD_SDI);
	nrf_gpio_cfg_output(LCD_SCK);
	nrf_gpio_cfg_output(LCD_REST);

	GPIO_HIGH(LCD_SDI);
	GPIO_LOW(LCD_SDI);
	GPIO_HIGH(LCD_CS);


	nrf_gpio_cfg_output(LCD_CS_REAL);
	GPIO_HIGH(LCD_CS_REAL);
	nrf_delay_ms(2);
	GPIO_LOW(LCD_CS_REAL);


}

void xianshi(){//显示信息

	BACK_COLOR = BLACK;
	POINT_COLOR = WHITE;
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

#ifdef SOC_SPI
	SpiInit();

#endif

	LCD_Init();

	showhanzi16(0, 35, 0); //专

	xianshi();	   //显示信息
	showqq();	   //显示QQ

	for(;;) {
		GPIO_TOGGLE(LED2);
		GPIO_TOGGLE(LED1);
		nrf_delay_ms(200);

////		nrf_gpio_pin_set(31);
//		nrf_gpio_pin_toggle(31);

	}

}









































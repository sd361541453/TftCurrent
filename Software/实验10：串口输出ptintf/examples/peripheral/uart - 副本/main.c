
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "app_uart.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "bsp.h"
#include "LCD1602.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_timer.h"

#define ERR_LED		6
#define PASS_LED	7
#define KEY_LEFT	8
#define KEY_UP		9
#define KEY_DOWN	10
#define KEY_RIGHT	11

/******************************************************************* Key ********************************************************************************/
unsigned char keyCnt;
unsigned char keyBuff;
unsigned char interKey;

void keyScan(void) {

	unsigned char kb = 0;

	if (nrf_gpio_pin_read(KEY_DOWN) == 0) {
		kb |= 1;
	}
	if (nrf_gpio_pin_read(KEY_UP) == 0) {
		kb |= 2;
	}
	if (nrf_gpio_pin_read(KEY_LEFT) == 0) {
		kb |= 4;
	}
	if (nrf_gpio_pin_read(KEY_RIGHT) == 0) {
		kb |= 8;
	}

	if (keyBuff == kb) {
		keyCnt++;
		if (keyCnt == 10) {
			interKey = kb;
		}
		if (keyCnt >= 200) {
			keyCnt -= 10;
		}
	} else {
		keyCnt = 0;
	}

	keyBuff = kb;
}


void keyInit(void) {
	nrf_gpio_cfg_input(KEY_UP, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(KEY_DOWN, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(KEY_LEFT, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(KEY_RIGHT, NRF_GPIO_PIN_PULLUP);
}

/******************************************************************* Key ********************************************************************************/

/******************************************************************* Display ********************************************************************************/

void displayNum(char * line1, char * line2) {


#define LCD_DELAY	nrf_delay_us(400)

	LCD_Wcom(0x01); //清屏
	LCD_DELAY;

	LCD_Wcom(0X80); //字符显示位置
	LCD_DELAY;

	for(;;){
		if(*line1 == 0){
			break;
		}
		LCD_Wdat(*line1);
		LCD_DELAY;
		line1++;
	}


	LCD_Wcom(0XC0); //显示位置
	LCD_DELAY;

	for(;;){
		if(*line2 == 0){
			break;
		}
		LCD_Wdat(*line2);
		LCD_DELAY;
		line2++;
	}

}

char displayString[20];
char* displayNumToString(unsigned int ua){
	unsigned char num = 0;
	for(num = 0;;){
		if(ua >= 1000000){
			ua -= 1000000;
			num++;
		}else{
			displayString[0] = num + '0';
			break;
		}
	}
	for(num = 0;;){
		if(ua >= 100000){
			ua -= 100000;
			num++;
		}else{
			displayString[1] = num + '0';
			break;
		}
	}
	for(num = 0;;){
		if(ua >= 10000){
			ua -= 10000;
			num++;
		}else{
			displayString[2] = num + '0';
			break;
		}
	}
	for(num = 0;;){
		if(ua >= 1000){
			ua -= 1000;
			num++;
		}else{
			displayString[3] = num + '0';
			break;
		}
	}
	for(num = 0;;){
		if(ua >= 100){
			ua -= 100;
			num++;
		}else{
			displayString[4] = num + '0';
			break;
		}
	}
	for(num = 0;;){
		if(ua >= 10){
			ua -= 10;
			num++;
		}else{
			displayString[5] = num + '0';
			break;
		}
	}

	displayString[6] = ua + '0';
	displayString[7] = 'u';
	displayString[8] = 'a';
	displayString[9] = 0;

	return displayString;
}

void displayCurrent(unsigned char num, unsigned int ua){
	char * numString = displayNumToString(ua);
	switch(num){
	case 1:
		displayNum("1.", numString);
		break;
	case 2:
		displayNum("2.", numString);
		break;
	case 3:
		displayNum("3.", numString);
		break;
	case 4:
		displayNum("4.", numString);
		break;
	case 5:
		displayNum("5.", numString);
		break;
	default:
		displayNum("Error","Error");
		break;
	}
}

void dispClear(){
	LCD_Wcom(0x01); //清屏
}


/******************************************************************* Display ********************************************************************************/





/******************************************* ADC ******************************************/

unsigned int adBattery, adCurrent200ms;
unsigned int adCnt200ms = 0;
#define AD_CNT_200_MAX	200
#define AD_BUFF_MAX	150
unsigned int adBuff[AD_BUFF_MAX];
unsigned int adCnt = 0;


#define SAMPLES_IN_BUFFER 3
static nrf_saadc_value_t m_buffer_pool[SAMPLES_IN_BUFFER];

//saacd配置
void saadc_callback(nrf_drv_saadc_evt_t const * p_event) {
	int i; //保存SAADC采样数据计算的实际电压值

	if (p_event->type == NRF_DRV_SAADC_EVT_DONE) {
		ret_code_t err_code;
		err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLES_IN_BUFFER);
		APP_ERROR_CHECK (err_code );

		for (i = 0; i < SAMPLES_IN_BUFFER; i++) {

			switch (i) {
			case 0: //ADOUT
				adCurrent200ms += p_event ->data.done.p_buffer[i];

				adCnt200ms++;
				if(adCnt200ms >= AD_CNT_200_MAX){
					adCnt200ms = 0;
					adBuff[adCnt] = adCurrent200ms;
					adCurrent200ms = 0;

					adCnt++;
					if(adCnt >= AD_BUFF_MAX){
						adCnt = 0;
					}

				}

				break;

			case 2: //POUT
				adBattery = p_event ->data.done.p_buffer[i];
				break;
			default:
				break;

			}

		}
	}

}

void saadc_init(void) {
	ret_code_t err_code;
	//adc通道配置
	nrf_saadc_channel_config_t P_channe_0_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE (NRF_SAADC_INPUT_AIN0 ); //ADOUT

	nrf_saadc_channel_config_t P_channe_1_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE (NRF_SAADC_INPUT_AIN1 ); //OPOUT

	nrf_saadc_channel_config_t P_channe_3_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE (NRF_SAADC_INPUT_AIN3 ); //POUT

	err_code = nrf_drv_saadc_init(NULL, saadc_callback);
	APP_ERROR_CHECK (err_code );
	//adc通道初始化

	err_code = nrf_drv_saadc_channel_init(0, &P_channe_0_config);
	err_code = nrf_drv_saadc_channel_init(1, &P_channe_1_config);
	err_code = nrf_drv_saadc_channel_init(3, &P_channe_3_config);
	APP_ERROR_CHECK (err_code );

	err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool, SAMPLES_IN_BUFFER);
	APP_ERROR_CHECK(err_code );
}

void adcScan(){

	nrf_drv_saadc_sample();

}


/******************************************* ADC ******************************************/



/******************************************* Led ******************************************/
//LED初始化
void ledInit(void) {
	nrf_gpio_cfg_output(ERR_LED);
	nrf_gpio_cfg_output(PASS_LED);

	nrf_gpio_pin_clear(ERR_LED);
	nrf_gpio_pin_clear(PASS_LED);
}

/******************************************* Led ******************************************/


/******************************************* Timer ******************************************/
unsigned char ms1;
const nrf_drv_timer_t TIMER_key = NRF_DRV_TIMER_INSTANCE(0); //定时器0
const nrf_drv_timer_t TIMER_adc = NRF_DRV_TIMER_INSTANCE(1); //定时器1


void onMs1TimerHandler(nrf_timer_event_t event_type, void* p_context) {//定时器中断

	switch (event_type) {
	case NRF_TIMER_EVENT_COMPARE0:

		ms1 = 1;

		break;

	default:
		//Do nothing.
		break;
	}
}

void ms1TimerInit(void) {
	uint32_t keytime_ms = 1; //定时器比较事件的时间

	uint32_t keytime_ticks;

	uint32_t err_code = NRF_SUCCESS;

	//配置定时器相关事件-key
	nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
	err_code = nrf_drv_timer_init(&TIMER_key, &timer_cfg, onMs1TimerHandler);
	APP_ERROR_CHECK(err_code);

	keytime_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_key, keytime_ms); //设置比较寄存器的时间值

	//触发定时器比较
	nrf_drv_timer_extended_compare(&TIMER_key, NRF_TIMER_CC_CHANNEL0, keytime_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

//	nrf_drv_timer_extended_compare(&TIMER_key, NRF_TIMER_CC_CHANNEL1, keytime_ticks, NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK, true);

	//使能定时器
	nrf_drv_timer_enable(&TIMER_key);
}

/******************************************* Timer ******************************************/



/******************************************* Time ******************************************/

unsigned char inter200ms = 0, inter500ms = 0, interSec = 0, interMin = 0;
unsigned short ms200Cnt = 0, ms500Cnt = 0, secCnt = 0, minCnt = 0;
void timeScan(){

	inter200ms = 0;
	inter500ms = 0;
	interSec = 0;
	interMin = 0;

	ms200Cnt++;
	if (ms200Cnt >= 200) {
		ms200Cnt = 0;
		inter200ms = 1;

		secCnt++;
		if (secCnt >= 5) {
			secCnt = 0;
			interSec = 1;
			minCnt++;
			if (minCnt >= 60) {
				minCnt = 0;
				interMin = 1;
			}
		}

	}

	ms500Cnt++;
	if (ms500Cnt >= 500) {
		ms500Cnt = 0;
		inter500ms = 1;
	}


}

/******************************************* Time ******************************************/

unsigned char show;
void Sys(){

	if(interKey){
		interKey = 0;
		show = !show;
	}


	if(interSec){

		if(show){
			displayCurrent(1,adCurrent200ms);
		}else{
			displayCurrent(2,adCnt);
		}

	}


}




int main(void) {

//	nrf_delay_ms(10);
	ledInit();
	keyInit();
	saadc_init();
	LCD1602_Init();
//	nrf_delay_ms(100);
	ms1TimerInit();

	while (1) {

		if(ms1){
			ms1 = 0;

			timeScan();
			keyScan();
			adcScan();
			timeScan();

			Sys();

		}
	}




}























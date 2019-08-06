/******************** (C) COPYRIGHT 2019 青风电子 ********************
 * 文件名  ：main
 * 出品论坛 ：www.qfv8.com        
 * 实验平台：青云nRF52xx蓝牙开发板
 * 描述    ：串口输出
 * 作者    ：青风
 * 店铺    ：qfv5.taobao.com
**********************************************************************/
 

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

#define SAMPLES_IN_BUFFER 3
static nrf_saadc_value_t m_buffer_pool[SAMPLES_IN_BUFFER];
const nrf_drv_timer_t TIMER_key = NRF_DRV_TIMER_INSTANCE(0);	//定时器0
const nrf_drv_timer_t TIMER_adc = NRF_DRV_TIMER_INSTANCE(1);	//定时器1 


#define ERR_LED		6
#define PASS_LED	7
#define Left_KEY	8
#define UP_KEY		9
#define DOWN_KEY	10
#define Right_KEY	11

int  i_val,key_count=1,unit=1,ad_count=0;

char AD_Current[]="1.Current_";
char dis_200ms[]="200ms";
char dis_600ms[]="600ms";
char dis_2s[]="2s";
char dis_3s[]="3s";
char dis_30s[]="30s";

char power[]="2.Buttery_Power";
float  AD_Sum[30000];
//float  AD_30sSum[10];
int Rve_TimeVal[6]={200,600,2000,3000,10000,30000};
float f_ad_val,Rve_AD_val=0, Rve_OP_val=0, Rve_Pow_val=0,AD_val=0,Pow_val=0,AD_30s_val=0;





void dis_select(void)
{
	if(nrf_gpio_pin_read (UP_KEY)==0)
	{
		nrf_delay_ms (10);
		if(nrf_gpio_pin_read (UP_KEY)==0)
		{
			key_count--;
			LCD_Wcom(0x01);				//清屏
			nrf_delay_ms(1);
		}
		while(nrf_gpio_pin_read (UP_KEY)==0);
	}
	
	
	if(nrf_gpio_pin_read (DOWN_KEY)==0)
	{
		nrf_delay_ms (10);
		if(nrf_gpio_pin_read (DOWN_KEY)==0)
		{
			key_count++;
			LCD_Wcom(0x01);				//清屏
			nrf_delay_ms(1);
		}
		while(nrf_gpio_pin_read (DOWN_KEY)==0);
	}
	if(key_count>2)
	{
		key_count=1;
	}
	
	if(key_count<1)
	{
		key_count=2;
	}
}



void units_select(void)
{
		if(nrf_gpio_pin_read (Left_KEY)==0)
		{
			nrf_delay_ms (10);
			if(nrf_gpio_pin_read (Left_KEY)==0)
			{
				unit--;
				LCD_Wcom(0x01);				//清屏
				nrf_delay_ms(1);
				if(unit<1)
				{
					unit=5;
				}
			}
			while(nrf_gpio_pin_read (Left_KEY)==0);
		}
	if(nrf_gpio_pin_read (Right_KEY)==0)
	{
		nrf_delay_ms (10);
		if(nrf_gpio_pin_read (Right_KEY)==0)
		{
			unit++;
			LCD_Wcom(0x01);				//清屏
			nrf_delay_ms(1);
			if(unit>5)
		{
			unit=1;
		}
		}
		while(nrf_gpio_pin_read (Right_KEY)==0);
	}
	
}
void AD_display(float Rve_AD)
{
		float val,Kua=3510;
		int  ge,shi,bai,i,ad_p_val;
		
		val=Rve_AD;		

				////////////////////////////uA////////////////////////
				
		f_ad_val=(val-6.05)*Kua;	//调0	原始：6.4
		ad_p_val =f_ad_val;
					
		bai=ad_p_val/1000%10+48;
		shi=ad_p_val/100%10+48;
		ge=ad_p_val/10%10+48;
				if((bai-48)>=6)				//OK或NG LED显示
				{
					nrf_gpio_pin_set(ERR_LED);				
					nrf_gpio_pin_clear(PASS_LED);
				}
				else
				{
					nrf_gpio_pin_set(PASS_LED);
					nrf_gpio_pin_clear(ERR_LED);
				}
				
				if(key_count==1)
				{
					LCD_Wcom (0X80);		//字符显示位置
					nrf_delay_ms(10);

					
					for(i=0;i<10;i++)
					{
						LCD_Wdat (AD_Current[i]);
						nrf_delay_ms(1);
					}
					
					switch(unit)
					{
						case 1:
							for(i=0;i<5;i++)
							{
								LCD_Wdat (dis_200ms[i]);
								nrf_delay_ms(1);
							}
							break ;
						case 2:
							for(i=0;i<5;i++)
							{
								LCD_Wdat (dis_600ms[i]);
								nrf_delay_ms(1);
							}
							break ;
						case 3:
							for(i=0;i<2;i++)
							{
								LCD_Wdat (dis_2s[i]);
								nrf_delay_ms(1);
							}
							break ;
						case 4:
							for(i=0;i<2;i++)
							{
								LCD_Wdat (dis_3s[i]);
								nrf_delay_ms(1);
							}
							break ;
						case 5:
							for(i=0;i<3;i++)
							{
								LCD_Wdat (dis_30s[i]);
								nrf_delay_ms(1);
							}
							break ;
							
						default :
							break;
							
					}
						
					LCD_Wcom (0XC0);		//显示位置
					nrf_delay_ms(10);
					
					if(bai>48)
						{
							LCD_Wdat (bai);
							nrf_delay_ms(10);	
						}
						else
						{
							LCD_Wdat ('0');
							nrf_delay_ms(10);
						}

					LCD_Wdat ('.');
					nrf_delay_ms(10);
									
					if(shi>48)
						{
							LCD_Wdat (shi);
							nrf_delay_ms(10);
						}
						else
						{
							LCD_Wdat ('0');
							nrf_delay_ms(10);
						}
						
						if(ge>48)
						{
							LCD_Wdat (ge);
							nrf_delay_ms(10);
						}
						else
						{
							LCD_Wdat ('0');
							nrf_delay_ms(10);
						}
					
					LCD_Wdat ('0');
					nrf_delay_ms(10);
					
					LCD_Wdat ('u');
					nrf_delay_ms(10);
					
					LCD_Wdat ('A');
					nrf_delay_ms(10);
				}
			}
		

void Pow_display(float Rve_AD)
{
	float f_p_val;
	int  ge,shi,bai,i,i_pow_val;  //保存SAADC采样数据计算的实际电压值
	f_p_val=Rve_AD;
	f_p_val=f_p_val* 3.6 /1024;
				
					i_pow_val=f_p_val*100*2;
					bai=i_pow_val/100%10+48;
					shi=i_pow_val/10%10+48;
					ge=i_pow_val/1%10+48;
				
					if(key_count==2)
					{
						LCD_Wcom (0X80);	//显示位置
						nrf_delay_ms(10);
							for(i=0;i<15;i++)
						{
							LCD_Wdat (power[i]);
							nrf_delay_ms(1);
						}
							
						LCD_Wcom (0XC0);	//显示位置
						nrf_delay_ms(10);
					
						LCD_Wdat (bai);
						nrf_delay_ms(10);
						
						LCD_Wdat ('.');
						nrf_delay_ms(10);
						
						LCD_Wdat (shi);
						nrf_delay_ms(10);

						LCD_Wdat (ge);
						nrf_delay_ms(10);

						LCD_Wdat ('V');
						nrf_delay_ms(10);
					}
}

//saacd配置
void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
	int  i;  //保存SAADC采样数据计算的实际电压值
		
	if(p_event->type ==NRF_DRV_SAADC_EVT_DONE )
	{
		ret_code_t err_code;
		err_code =
		nrf_drv_saadc_buffer_convert (p_event->data.done .p_buffer ,SAMPLES_IN_BUFFER);
		APP_ERROR_CHECK (err_code );
		

		for(i=0;i<SAMPLES_IN_BUFFER;i++)
		{
			i_val=p_event ->data .done .p_buffer [i];

			switch(i)
			{
				case 0:		//ADOUT
					AD_val=i_val+AD_val;
					break;
				
				case 2: 	//POUT
					Pow_val=i_val+Pow_val;
					break;
				default :
					break;
					
			}
			
		}
	}	
		
		//LCD_Wcom(0x01);				//清屏
		//nrf_delay_ms(500);
}


void saadc_init(void)
{
	ret_code_t err_code;
	//adc通道配置
	nrf_saadc_channel_config_t P_channe_0_config=
	NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE (NRF_SAADC_INPUT_AIN0 );	//ADOUT
	
	nrf_saadc_channel_config_t P_channe_1_config=
	NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE (NRF_SAADC_INPUT_AIN1 );	//OPOUT
	
	nrf_saadc_channel_config_t P_channe_3_config=
	NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE (NRF_SAADC_INPUT_AIN3 );	//POUT
	
	err_code =nrf_drv_saadc_init (NULL,saadc_callback);
	APP_ERROR_CHECK (err_code );
	//adc通道初始化
	
	err_code = nrf_drv_saadc_channel_init(0,&P_channe_0_config);
	err_code = nrf_drv_saadc_channel_init(1,&P_channe_1_config);
	err_code = nrf_drv_saadc_channel_init(3,&P_channe_3_config);
	APP_ERROR_CHECK (err_code );
	
	err_code =nrf_drv_saadc_buffer_convert (m_buffer_pool ,SAMPLES_IN_BUFFER );
	APP_ERROR_CHECK(err_code );
}



//LED初始化
void led_init(void)
{
	nrf_gpio_cfg_output (ERR_LED);
	nrf_gpio_cfg_output (PASS_LED);
	
	nrf_gpio_pin_clear(ERR_LED);
	nrf_gpio_pin_clear(PASS_LED);
}
void key_init(void)
{
	nrf_gpio_cfg_input (UP_KEY,NRF_GPIO_PIN_PULLUP );
	nrf_gpio_cfg_input (DOWN_KEY,NRF_GPIO_PIN_PULLUP );
	nrf_gpio_cfg_input (Left_KEY,NRF_GPIO_PIN_PULLUP );
	nrf_gpio_cfg_input (Right_KEY,NRF_GPIO_PIN_PULLUP );
}




void Current_200ms(void)
{
	int i=0;
	for(i=0;i<200;i++)
	{
		Rve_AD_val=AD_Sum[i]+Rve_AD_val;
	}
	Rve_AD_val=Rve_AD_val/200;
}

void Current_600ms(void)
{
	int i;
	Rve_AD_val=0;
	for(i=0;i<600;i++)
		{
			Rve_AD_val=AD_Sum[i]+Rve_AD_val;
		}
	Rve_AD_val=Rve_AD_val/600;
}

void Current_2s(void)
{
	int i;
	Rve_AD_val=0;
	for(i=0;i<2000;i++)
		{
			Rve_AD_val=AD_Sum[i]+Rve_AD_val;
		}
	
	Rve_AD_val=Rve_AD_val/2000;

}

void Current_3s(void)
{
	int i;
	Rve_AD_val=0;
	for(i=0;i<3000;i++)
		{
			Rve_AD_val=AD_Sum[i]+Rve_AD_val;
		}
	
	Rve_AD_val=Rve_AD_val/3000;
	AD_30s_val=Rve_AD_val;
}

void Current_30s(void)
{

		Rve_AD_val=Rve_AD_val/10;
	
}


void timer_led_event_handler(nrf_timer_event_t event_type, void* p_context)	//定时器中断
{
	int i;
	 switch (event_type)
	 {
		 case NRF_TIMER_EVENT_COMPARE0:
			 

			 nrf_drv_saadc_sample ();
			for(i=2999;i>0;i--)				//移位
			{
				AD_Sum[i]=AD_Sum[i-1];
			}

			AD_Sum[0]=AD_val;	
			Rve_Pow_val=Pow_val;
			AD_val=0;
			Pow_val=0;
///////////////////////////////////		
			ad_count++;				//采集次数
			
			if(ad_count==3000)
			{
				Current_3s();
				for(i=9;i>0;i--)		//移位
				{
					AD_30sSum[i]=AD_30sSum[i-1];
				}
				AD_30sSum[0]=AD_30s_val;
				
				for(i=0;i<10;i++)			//数组总和
				{
					Rve_AD_val=AD_30sSum[i]+Rve_AD_val;
				}
				
				ad_count=0;
			}
//////////////////////////////			
			
		   break;
		 case NRF_TIMER_EVENT_COMPARE1:
			 dis_select();
			 units_select();
		   break;

     default:
            //Do nothing.
       break;
	 }
}

void keytimer_init(void)
{
		uint32_t keytime_ms = 1; //定时器比较事件的时间
	
    uint32_t keytime_ticks;

    uint32_t err_code = NRF_SUCCESS;

	//配置定时器相关事件-key
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init(&TIMER_key, &timer_cfg, timer_led_event_handler);
    APP_ERROR_CHECK(err_code);

    keytime_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_key, keytime_ms);	//设置比较寄存器的时间值
	
	//触发定时器比较
    nrf_drv_timer_extended_compare(
    &TIMER_key, NRF_TIMER_CC_CHANNEL0, keytime_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
	
		nrf_drv_timer_extended_compare(
    &TIMER_key, NRF_TIMER_CC_CHANNEL1, keytime_ticks, NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK, true);
	

		//使能定时器
    nrf_drv_timer_enable(&TIMER_key);
}


int main(void)
{	
	//int i;
	nrf_delay_ms (10);
	led_init();
	key_init ();
	saadc_init();
	LCD1602_Init ();
	LCD_Wcom(0x01);							//清屏
	nrf_delay_ms(100);
	keytimer_init();
	while(1)
	{		

		switch(unit)							//选择时间平均值
		{
			case 1:
				Current_200ms();
				break ;
			case 2:
				Current_600ms();
				break ;
			case 3:
				Current_2s();
				break ;
			case 4:
				Current_3s();
				break ;
			case 5:
				Current_30s();
				break ;
		}
		nrf_delay_ms (100);
		AD_display(Rve_AD_val);
		Pow_display(Rve_Pow_val);

	}
	
}	
	



/*
file:key.c
for:all the botton which press the connect,loosen the disconnect,and vice versa
target:
version:v1.0
modify date:20160630
author:ztw
pay attention:用完了要清零
本程序不使用中断，在主函数中每隔reflash_time 就执行一次更新按键数据程序，
*/

#include "button.h"

/* USER CODE BEGIN EV */

BUTTON_CTRL_TypeDef button_1;

/* USER CODE END EV */


uint8_t read_button_1(void)
{
	return !HAL_GPIO_ReadPin(SET_KEY_GPIO_Port ,SET_KEY_Pin);
}



/* USER CODE BEGIN EFP */
void Button_1_init(void)
{
	button_1 .read_button = read_button_1;
	button_1.Is_press = NO ;
	button_1 .Is_click =NO;
	button_1.Is_support_continue_press= YES ;
	button_1.Is_support_click = NO ;

}



/* USER CODE END EFP */


//when using the button information,you must run this to updata the button information;
//该函数用在ISR中，如果10ms中断，则该按键侦测间隔为10ms
void BUTTON_scan(BUTTON_CTRL_TypeDef *button)
{
	static uint8_t dir=1;	
	static uint8_t continue_dir=1;	
	button -> state_now = button -> read_button();//获得当前按键状态

	//Interference prevention 预防抖动
	button ->time_count++;
	if(button ->state_pre ==button -> state_now)//如果上次读取的数值
	{
		button -> Is_press =button -> state_now;
	}
	else 
	{
		button ->state_pre = button -> state_now; 
	}
	

	if(button -> Is_support_continue_press)
	{
		button -> continue_press_time++;
	}			
	if(button -> Is_support_click)
	{
			if(dir){if(button -> Is_press)dir=0;}
			else {if(!(button -> Is_press)){dir=1;button ->Is_click =YES;}}


		
		
		if(button ->Is_support_continue_click)
		{
			if(continue_dir){if(button -> Is_click)continue_dir=0;}
			else {if(!(button -> Is_click)){continue_dir=1;button ->continue_click_count ++;}}
		}
	}
		
}



 





void BUTTON_1_ISR(void)
{
	BUTTON_scan(&button_1 );
}

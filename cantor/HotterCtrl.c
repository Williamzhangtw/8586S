/*            2016/07/16
*/
#include "../cantor/hotterctrl.h"
#include "../tool/msg_task.h"
#include "../bsp/tm1650.h"
#include "../tool/flash.h"
#include <string.h>
#include <math.h>


HOTTER_CTRL_Typedef solder1;

void solder1_init(void)
{
	solder1 .state =  POWERON   ;
	
}


void SOLDER_POWERON_operate(void)
{ 		
	Rotary_1_init();delaymsTask_CTRL(rotary_1_msg,ENABLE );
	Button_1_init();delaymsTask_CTRL(button_1_msg,ENABLE );
	Tm1650_1_init();delaymsTask_CTRL(tm1650_1_msg,ENABLE );
	FlshPara_Init();
	Hotter1321_init();
	delaymsTask_CTRL(hotter1321_adc_msg ,ENABLE );
	delaymsTask_CTRL(hotter1321_poweron_msg ,ENABLE );
	delaymsTask_CTRL(hotter1321_realTemp_msg ,ENABLE );	
	delaymsTask_CTRL(hotter1321_heated_count_msg ,ENABLE );
	delaymsTask_CTRL(hotter1321_hotter_state_msg ,ENABLE );
	delaymsTask_CTRL(hotterctrl_poweron_msg ,ENABLE );
	solder1 .state =IDLE ;
	tm1650_1.Is_num =NO;
	tm1650_1.dot_run_en =DISABLE ;	
	tm1650_1.blink_en  =DISABLE ;
	tm1650_1.bottom_dot_en =DISABLE ;
	tm1650_1 .word =(uint8_t*)CODE_Clean ;
}




void SOLDER_IDLE_operate(void)
{

	static uint8_t dir =0;
	if(hotter1321.Is_power_on)//solder switch checking
	{
		if (!dir)
		{
			tm1650_1.times_100ms=0;
			dir=~dir;
			tm1650_1.Is_num = YES ;
			tm1650_1.dot_run_en =DISABLE ;	
			tm1650_1.blink_en  =ENABLE ;
			tm1650_1.bottom_dot_en =DISABLE ;
			tm1650_1.num =hotter1321 .target_temperature ;
		}
		else
		{	
			
			if(tm1650_1.times_100ms>20)
			{
				tm1650_1.Is_num = YES ;
				tm1650_1.dot_run_en =DISABLE ;	
				tm1650_1.blink_en  =DISABLE ;
				tm1650_1.bottom_dot_en =DISABLE ;
				solder1 .state = TempCTRL  ;
				hotter1321 .work_state = notheating;//初始化H-E检查
				tm1650_1.times_100ms =0;
				dir =~dir;
			}
		}			
		
	}
	else
	{
		 
		if(HalTimeDelaySimple (&hal_100ms_flag ,1))
		{
			hotter1321 .heat_en(DISABLE );
			dir =0;
			tm1650_1 .word =(uint8_t*)OFF;
			tm1650_1.dot_run_en =DISABLE ;	
			tm1650_1.blink_en  = NO ;
			tm1650_1.Is_num = NO ;
			tm1650_1 .bottom_dot_en =NO;
			tm1650_1.times_100ms=0;
		}
	}
}



void SOLDER_TempCTRL_operate(void)
{
	static uint8_t dir = 1;//用于校准温度按键 
	
	static uint8_t dir_1 = 1;//用于H-E检查
	
	
	static uint8_t check_time = 0;

	
	 
	//显示
	
	
//	

	
	if(hotter1321 .work_state ==balance)
		tm1650_1.num = hotter1321.target_temperature;
	else tm1650_1.num = hotter1321.real_temperature;
// 	//加热控制-ADC超出范围

	if(hotter1321 .real_adc<hotter1321 .sensor_err_adc)
	{
		if( hotter1321 .real_temperature< hotter1321 .target_temperature)
		{
			hotter1321 .heat_en(ENABLE );
		 		
			tm1650_1.bottom_dot_en =ENABLE  ;//表示加热中
		}
		else
		{
			tm1650_1.bottom_dot_en =DISABLE  ;//表示不加热
			hotter1321 .heat_en(DISABLE );
		}
	}
	else 
	{
		solder1 .state = ALARM ;
		tm1650_1.Is_num = NO ;
    	tm1650_1.dot_run_en =DISABLE ;	
		tm1650_1.blink_en  =ENABLE ;
		hotter1321 .sensor_err =1;
	}
	//H-E检查
	if(dir_1)
	{
		if(	hotter1321 .work_state==heating)
		{
			hotter1321 .heated_time_count=0;
			
			hotter1321.minus_pre =  hotter1321.target_temperature -   hotter1321 .real_temperature;
			dir_1 = 0;
		}
	}
	else 
	{
		if(	hotter1321 .work_state!=heating)
		{
		    dir_1 = 1;
			 
		}
		else
		{
			if((hotter1321 .heated_time_count>10))
			{
				dir_1 = 1;
				hotter1321.minus_now = hotter1321.target_temperature-hotter1321 .real_temperature ;
				
				if((hotter1321.minus_pre -hotter1321.minus_now )<2)
				{
					if(hotter1321 .real_temperature <80)
					{		
						if(check_time++>12)
						{
							check_time =0;
							solder1 .state = ALARM ;
							hotter1321 .heat_en(DISABLE );
							hotter1321 .hotter_err=1;
							tm1650_1.Is_num = NO ;
							tm1650_1.dot_run_en =DISABLE ;	
							tm1650_1.blink_en  =ENABLE ;
						}
					}
					else
					{
							
						if(check_time++>1)
						{
							check_time =0;
						
							tm1650_1.blink_en  =ENABLE ;
						}							;
					}
						
						
				}
				else
				{
					tm1650_1.blink_en  =DISABLE   ;
					check_time = 0;
				}
			}	
		}
	}
			
	//调温按键检查	
	if(rotary_1.Spin_direction)
	{
		hotter1321 .heat_en(DISABLE );
		solder1 .state = TempSET ;
		tm1650_1.Is_num = YES  ;
    	tm1650_1.dot_run_en =DISABLE ;	
		tm1650_1.blink_en  =DISABLE ;
		tm1650_1.bottom_dot_en=DISABLE ;
		button_1 .times_10ms=0;
		

	}	
	//校温按键检查	
	if(dir)
	{
		if (button_1.Is_press == YES)
		{
			dir =0;button_1.times_10ms = 0;
		}
	}
	else
	{
		if (button_1.Is_press == YES)
		{
			if ((button_1.times_10ms>300)&&(hotter1321 .work_state ==balance))
			{
				
				
				hotter1321 .heat_en(DISABLE );
				tm1650_1.Is_num = NO  ;
				tm1650_1.word = (uint8_t*)CAL;
				tm1650_1.dot_run_en =ENABLE ;	
				tm1650_1.blink_en  =DISABLE ;
				tm1650_1.bottom_dot_en=DISABLE ;
				solder1 .state = TempADJUST ;
				button_1 .times_10ms =0;
				dir=1;
			}
		}
		else dir=1;
	
	}
}





void SOLDER_TempSET_operate(void)
{

	tm1650_1.num = hotter1321.target_temperature;
	/*
	如果左旋，温度做减法运算
	*/
	if(rotary_1.Spin_direction == Spin_left)
	{
		button_1 .times_10ms=0;
		rotary_1.Spin_direction = no_direction ;
		if(--hotter1321.target_temperature<=hotter1321 .Lmin)
			hotter1321.target_temperature=hotter1321 .Lmin;
	}
	/*
	如果右旋，温度做加法运算
	*/
	if(rotary_1.Spin_direction == Spin_right)
	{
		button_1 .times_10ms=0;
		rotary_1.Spin_direction = no_direction ;
		if(++hotter1321.target_temperature >hotter1321 .Lmax )
		{
			hotter1321.target_temperature =hotter1321 .Lmax ;
		}
	}
	/*
	如果确定，温度设定完成
	*/
	if(button_1 .Is_press == YES )
	{
		tm1650_1.blink_en  = NO ;
		tm1650_1 .bottom_dot_en =NO;
		tm1650_1.dot_run_en=NO;
		tm1650_1.Is_num = YES ;
		FlshPara_Save();
		solder1 .state = TempCTRL ;
	}
	
//	/*
//	如果持续未按并且持续时间超过xx秒，温度设定完成
//	*/
	
	if(button_1 .times_10ms>250)
	{
		FlshPara_Save();
		tm1650_1.blink_en  = NO ;
		tm1650_1 .bottom_dot_en =NO;
		tm1650_1.dot_run_en=NO;
		tm1650_1.Is_num = YES ;
		solder1 .state = TempCTRL ;
		
		
		
	}
	
	
 	
}




void SOLDER_TempADJUST_operate(void)
{
	static uint8_t dir = 0;
	static int16_t number=0;
	Hotter_flash__TypeDef hotter_flash;
	if(!dir)
	{
		if (button_1 .times_10ms>100)
		{ 
			memcpy(&hotter_flash,(Hotter_flash__TypeDef *)PARA_START_ADDR,sizeof (Hotter_flash__TypeDef)); 
			number = hotter_flash.adjust_temperature;		
			tm1650_1.Is_num = YES ;
			tm1650_1.dot_run_en =DISABLE ;	
			tm1650_1.blink_en  =ENABLE ;
			tm1650_1.bottom_dot_en =DISABLE ;
			dir =~dir;
		}
	}
	else
	{
		tm1650_1.num =  number  ;
		/*
		如果左旋，温度做减法运算
		*/
		if(rotary_1.Spin_direction == Spin_left)
		{
			
			button_1 .times_10ms =0;
			rotary_1.Spin_direction = no_direction;
			--number;
			if(number<hotter1321 .Cmin) number  =  hotter1321 .Cmin;
		}
		
		/*
		如果右旋，温度做加法运算
		*/
		if(rotary_1.Spin_direction == Spin_right)
		{	button_1 .times_10ms =0;
			rotary_1.Spin_direction = no_direction;
			++number;
			if(number > hotter1321 .Cmax) number =  hotter1321 .Cmax;
			
		}
		
		
		/*
		如果确定，温度设定完成
		*/
		if(button_1 .Is_press == YES )
		{
			tm1650_1.dot_run_en =NO ;
			tm1650_1.bottom_dot_en =NO;
			tm1650_1.blink_en  =DISABLE ;
			tm1650_1.Is_num = YES ;
			hotter1321 .adjust_temperature =number;
			dir=~dir;
			FlshPara_Save();
			solder1 .state =TempCTRL ;
			
		}
			
	//	如果持续未按并且持续时间超过xx秒
		if(button_1 .times_10ms>500)
		{	
			tm1650_1.dot_run_en =NO ;
			tm1650_1.bottom_dot_en =NO;
			tm1650_1.blink_en  =DISABLE ;
			tm1650_1.Is_num = YES  ;
			dir=~dir;
			solder1 .state = TempCTRL ;
		}
	}
		
}




void SOLDER_ALARM_operate(void)
{
	if(hotter1321 .sensor_err )
	{
		hotter1321 .heat_en (DISABLE );
		tm1650_1.word = (uint8_t*)S_E;
		tm1650_1.Is_num = NO ;
		tm1650_1.blink_en = NO;
		tm1650_1.bottom_dot_en  = NO ;
		tm1650_1.dot_run_en  = NO;
		if(hotter1321 .real_adc <hotter1321 .sensor_err_adc )
		{
			solder1 .state =TempCTRL ;
			tm1650_1.Is_num = YES ;
			tm1650_1.blink_en = NO;
			hotter1321 .sensor_err =0 ;
		}
	}
	else
	{
		if(hotter1321 .hotter_err )
		{
			hotter1321 .heat_en (ENABLE );
			tm1650_1.bottom_dot_en= ENABLE ;
			tm1650_1.word =(uint8_t*) H_E;
			if(hotter1321 .real_temperature  >90)
			{
				solder1 .state =TempCTRL ;
				tm1650_1.Is_num = YES ;
				tm1650_1.blink_en = NO;
				hotter1321 .hotter_err =0;
			}
		}
		
	}
}





/*******************************************************************************
函数名: Status_Tran 
函数作用: 根据按键、温度判断等控制状态转换
输入参数: NULL
返回参数: NULL
*******************************************************************************/
void Solder_1_Transformation(void)//状态转换
{
		 

	switch (solder1 .state) 
	{
		case POWERON :
			SOLDER_POWERON_operate();
			break;	
		case IDLE:
			SOLDER_IDLE_operate();
			break;
		case TempCTRL:
			SOLDER_TempCTRL_operate();
			break;
		case TempSET:
			 SOLDER_TempSET_operate();
			break;
		case ALARM:
			SOLDER_ALARM_operate();
			break;	 
		case TempADJUST:
			SOLDER_TempADJUST_operate();
			break;

		default:
			break;
	}
 
	
}

void Solder_1_poweron_ISR(void)
{
	 if(!hotter1321 .Is_power_on)
	{
		solder1 .state =IDLE ;
	}
}






/******************************** END OF FILE *********************************/

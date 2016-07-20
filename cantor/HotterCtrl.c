/*            2016/07/18
*/
#include "../cantor/hotterctrl.h"
#include "../tool/msg_task.h"
#include "../bsp/tm1650.h"
#include "../tool/flash.h"
#include <string.h>
#include <math.h>


void TEMP_IDLE_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl -> hotter ->heat_en(DISABLE );//不加热
	hotterCtrl -> tm1650 ->Is_num = NO ;//显示文字
	hotterCtrl -> tm1650 ->word =(uint8_t*)OFF;//内容OFF
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//不跑灯
	hotterCtrl -> tm1650 ->blink_en  = NO ;//不闪
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//不显示加热点
	hotterCtrl ->  state = TEMP_IDLE  ;	
}


void TEMP_IDLE_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{	
	if(hotterCtrl->hotter->Is_power_on)//solder开关检查
	{
		hotterCtrl ->  state = TEMP_TARGET_SHOW_READY  ;
	}
}
void TEMP_TARGET_SHOW_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hal_100ms_flag =0;//用于后面程序计时用
	hotterCtrl -> hotter->heat_en(DISABLE );//不加热
	//显示
	hotterCtrl->tm1650->Is_num = YES ;//显示数字
	hotterCtrl->tm1650->num =hotterCtrl->hotter->target_temperature ;//内容：目标温度
	hotterCtrl->tm1650->dot_run_en =DISABLE ;	//不跑灯
	hotterCtrl->tm1650->blink_en  =ENABLE ;//闪烁
	hotterCtrl->tm1650->bottom_dot_en =DISABLE ;//不显示加热点
	//显示
	hotterCtrl->state = TEMP_TARGET_SHOW ;
}


void TEMP_TARGET_SHOW_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	//2S后进入CTRL
	if(hal_100ms_flag>20)
	{
		hotterCtrl->state = TEMP_CTRL_READY  ;
	}
}
void TEMP_CTRL_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl->hotter->sensor_err =0;
	hotterCtrl->hotter->hotter_err =0;
	hotterCtrl->hotter->heated_check_dir = 0;
	//显示
	hotterCtrl->tm1650->Is_num = YES ;//显示数字类型
	hotterCtrl->tm1650->dot_run_en =DISABLE ;	//不跑灯

	//显示
	hotterCtrl->state = TEMP_CTRL ;
}


void TEMP_CTRL_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	
	//显示内容
	if(hotterCtrl->hotter->work_state ==TEMP_BALANCE )
	{
		hotterCtrl->tm1650->num = hotterCtrl->hotter->target_temperature;
	}
	else 
	{
		hotterCtrl->tm1650->num =hotterCtrl-> hotter->real_temperature;
	}
	//显示内容
	
	
	
 	//加热控制
	if(hotterCtrl->hotter->real_adc	<	hotterCtrl->hotter->sensor_err_adc)
	{
		if( hotterCtrl->hotter->real_temperature	<	hotterCtrl->hotter->target_temperature)
		{
			hotterCtrl->hotter->heat_en(ENABLE );	
			hotterCtrl->tm1650->bottom_dot_en =ENABLE  ;//表示加热中
		}
		else
		{
			hotterCtrl->hotter ->heat_en(DISABLE );
			hotterCtrl->tm1650 ->bottom_dot_en =DISABLE  ;//表示不加热
		}
	}
	else //ADC超出范围 S-E
	{
		
		hotterCtrl->hotter->sensor_err =1;
		hotterCtrl->state = TEMP_ALARM_READY ; 
	}
	//加热控制	
	
	//温度上不去检查
	if(hotterCtrl->hotter->heated_check_dir == 0 )
	{
		if(	hotterCtrl->hotter->work_state==HEATTING )
		{
			hotterCtrl->hotter->temperature_pre = hotterCtrl->hotter->real_temperature;//记录当前实时温度
			hotterCtrl->hotter->heated_check_dir=1;	
			hotterCtrl->hotter->heated_times =0;
		}
		hotterCtrl->tm1650->blink_en  =DISABLE   ;
	}
	else 
	{
		if(	hotterCtrl->hotter->work_state!=HEATTING )
		{
		   hotterCtrl->hotter->heated_check_dir=0; 
		}
		else
		{
			if((hotterCtrl->hotter->temperature_pre -hotterCtrl->hotter->real_temperature )>100)
			{
				hotterCtrl->tm1650->blink_en  =ENABLE ;		
			}
			else
			{
				hotterCtrl->tm1650->blink_en  =DISABLE   ;
			}	
				
			if((hotterCtrl->hotter->real_temperature <90)&&(hotterCtrl->hotter->heated_times>10000)&&(__fabs (hotterCtrl->hotter->temperature_pre -hotterCtrl->hotter->real_temperature )<4))
			{		
				hotterCtrl-> state = TEMP_ALARM_READY ;
				hotterCtrl->hotter->hotter_err=1;
			}
		}	
		
	}
	//温度上不去检查
	
	//调温按键检查	
	if(hotterCtrl->rotary->Spin_direction)
	{ 
		hotterCtrl-> state = TEMP_TARGET_SET_READY ;	
	}
	//调温按键检查	
	
	//校温按键检查	
	if ((hotterCtrl->button->continue_press_times>300)&&(hotterCtrl->hotter->work_state ==TEMP_BALANCE ))
	{
		hotterCtrl->state = TEMP_ADJUST_WARNING_READY  ;
	}
	//校温按键检查
}








void TEMP_TARGET_SET_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hal_100ms_flag =0;//用于后面程序计时用
	hotterCtrl -> hotter->heat_en(DISABLE );//不加热
	//显示
	hotterCtrl -> tm1650 ->Is_num = YES ;
	hotterCtrl -> tm1650 ->num =hotterCtrl ->hotter->target_temperature; 
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//不跑灯
	hotterCtrl -> tm1650 ->blink_en  =DISABLE  ;//不闪
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//不显示加热点
	hotterCtrl -> hotter ->heat_en(DISABLE );//不加热
	//显示
	hotterCtrl ->state = TEMP_TARGET_SET ;
}


void TEMP_TARGET_SET_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{

	
	//如果左旋，温度做减法运算
	if(hotterCtrl ->rotary->Spin_direction == Spin_left)
	{
	
		hotterCtrl ->rotary->Spin_direction = no_direction ;
		if(--hotterCtrl ->hotter->target_temperature<=hotterCtrl ->hotter ->Lmin)
		{
			hotterCtrl ->hotter->target_temperature=hotterCtrl ->hotter ->Lmin;
		}
		hal_100ms_flag =0;
	}
	/*
	如果右旋，温度做加法运算
	*/
	if(hotterCtrl ->rotary->Spin_direction == Spin_right)
	{

		hotterCtrl ->rotary->Spin_direction = no_direction ;
		if(++hotterCtrl ->hotter->target_temperature >hotterCtrl ->hotter->Lmax )
		{
			hotterCtrl ->hotter->target_temperature =hotterCtrl ->hotter->Lmax ;
		}
		hal_100ms_flag =0;
	}
	/*
	如果确定，温度设定完成
	*/
	if(hotterCtrl ->button->Is_press == YES )
	{
		FlshPara_Save();
		hotterCtrl ->state = TEMP_CTRL_READY ;
	}
	
//如果持续未按并且持续时间超过xx秒，温度设定完成
	if(hal_100ms_flag>25)
	{
		FlshPara_Save();
		hotterCtrl ->state = TEMP_CTRL_READY ;
	}
}



void TEMP_ADJUST_WARNING_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl -> hotter ->heat_en(DISABLE );//不加热
	//显示
	hotterCtrl -> tm1650 ->Is_num = NO ;
	hotterCtrl -> tm1650 ->word =(uint8_t*)CAL ; 
	hotterCtrl -> tm1650 ->dot_run_en =ENABLE  ;//不跑灯
	hotterCtrl -> tm1650 ->blink_en  = NO ;//不闪
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//不显示加热点
	//显示
	hotterCtrl ->state = TEMP_ADJUST_WARNING ;
}


void TEMP_ADJUST_WARNING_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	if (hal_100ms_flag >15)
	{ 		
		hotterCtrl ->state = TEMP_ADJUST_READY ;
	}
}




void TEMP_ADJUST_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	//显示
	hotterCtrl -> tm1650 ->Is_num = YES ;
	hotterCtrl -> tm1650 ->num =hotterCtrl ->hotter ->adjust_temperature ; 
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE   ;//不跑灯
	hotterCtrl -> tm1650 ->blink_en  = ENABLE ;//不闪
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//不显示加热点
	//显示
	hotterCtrl -> hotter ->heat_en(DISABLE );//不加热
	hotterCtrl ->hotter ->adjust_temperature =hotter_flash.adjust_temperature; 
	hal_100ms_flag =0;//用于后面程序计时用
	hotterCtrl -> state = TEMP_ADJUST ;
}
void TEMP_ADJUST_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	//如果左旋，温度做减法运算
	if(hotterCtrl -> rotary->Spin_direction == Spin_left)
	{
		hal_100ms_flag =0;//用于后面程序计时用
		hotterCtrl -> rotary->Spin_direction = no_direction;
		-- hotterCtrl ->hotter ->adjust_temperature ;
		if( hotterCtrl ->hotter ->adjust_temperature <hotterCtrl ->hotter ->Cmin) 
		{			
			hotterCtrl ->hotter ->adjust_temperature   =  hotterCtrl ->hotter ->Cmin;
		}
	}
	
	/*
	如果右旋，温度做加法运算
	*/
	if(hotterCtrl -> rotary->Spin_direction == Spin_right)
	{	
		hal_100ms_flag  =0;//用于后面程序计时用
		hotterCtrl -> rotary->Spin_direction = no_direction;
		++ hotterCtrl -> hotter ->adjust_temperature ;
		if( hotterCtrl -> hotter ->adjust_temperature  >hotterCtrl ->  hotter ->Cmax)  
		{
			hotterCtrl -> hotter ->adjust_temperature  =  hotterCtrl -> hotter ->Cmax;
		}
	}
	
	//如果确定，温度设定完成
	if(hotterCtrl -> button ->Is_press == YES )
	{
		FlshPara_Save();
		hotterCtrl -> state =TEMP_CTRL_READY ;
	}
		
//	如果持续未按并且持续时间超过xx秒
	if(hal_100ms_flag >50)
	{	
		hotterCtrl -> state = TEMP_CTRL_READY ;
	}	
}







void TEMP_ALARM_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{	
	//显示
	hotterCtrl -> tm1650 ->Is_num = NO ;
	hotterCtrl -> tm1650 ->dot_run_en =NO  ;	//不跑灯
	hotterCtrl -> tm1650 ->blink_en  = ENABLE ;//闪
	//显示
	
	hotterCtrl -> state = TEMP_ALARM ;
}
void TEMP_ALARM_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{

	if(hotterCtrl->hotter ->sensor_err )
	{
		hotterCtrl -> tm1650 ->word =(uint8_t *)S_E  ; 
	}
	else
	{
		hotterCtrl -> tm1650 ->word =(uint8_t *)H_E  ; 
	}

	
	if(hotterCtrl->hotter ->hotter_err )
	{
			
		hotterCtrl -> tm1650 ->bottom_dot_en =ENABLE ;//显示加热点
		hotterCtrl -> hotter ->heat_en(ENABLE );//加热
		if(hotterCtrl -> hotter->real_temperature  >90)
		{
			hotterCtrl ->  state =TEMP_CTRL_READY ;
		}
	}
	else
	{
		hotterCtrl -> tm1650 ->bottom_dot_en =DISABLE ;//不显示加热点
		hotterCtrl -> hotter ->heat_en(DISABLE );//不加热
		if(hotterCtrl -> hotter->real_adc <hotterCtrl -> hotter->sensor_err_adc )
		{
			hotterCtrl ->  state =TEMP_CTRL_READY ;
		}
	}
}





// 根据按键、温度判断等控制状态转换
void HotterCtrl(HOTTER_CTRL_Typedef *hotterCtrl) 
{
	switch (hotterCtrl->state) 
	{
		case TEMP_IDLE:
			TEMP_IDLE_operate(hotterCtrl);
			break;
		case TEMP_TARGET_SHOW :
			TEMP_TARGET_SHOW_operate(hotterCtrl);
			break ;
		case TEMP_CTRL:
			TEMP_CTRL_operate(hotterCtrl);
			break;
		case TEMP_TARGET_SET:
			 TEMP_TARGET_SET_operate(hotterCtrl);
			break;
		case TEMP_ALARM:
			TEMP_ALARM_operate(hotterCtrl);
			break;	 
	
		case TEMP_ADJUST_WARNING :
			TEMP_ADJUST_WARNING_operate(hotterCtrl);
			break ;
		case TEMP_ADJUST:
			TEMP_ADJUST_operate(hotterCtrl);
			break;
		
		case TEMP_IDLE_READY:
			TEMP_IDLE_READY_operate(hotterCtrl);
			break;
		case TEMP_TARGET_SHOW_READY :
			TEMP_TARGET_SHOW_READY_operate(hotterCtrl);
			break ;
		case TEMP_CTRL_READY:
			TEMP_CTRL_READY_operate(hotterCtrl);
			break;
		case TEMP_TARGET_SET_READY:
			 TEMP_TARGET_SET_READY_operate(hotterCtrl);
			break;
		case TEMP_ALARM_READY:
			TEMP_ALARM_READY_operate(hotterCtrl);
			break;	 
	
		case TEMP_ADJUST_WARNING_READY :
			TEMP_ADJUST_WARNING_READY_operate(hotterCtrl);
			break ;
		case TEMP_ADJUST_READY:
			TEMP_ADJUST_READY_operate(hotterCtrl);
			break;
		
		default:
			break;
	}
	if(!hotterCtrl->hotter->Is_power_on)
	{
		hotterCtrl->state =TEMP_IDLE_READY ;
	}
}



HOTTER_CTRL_Typedef solder1321;


void solder1321_init(void)	
{	
	solder1321 .state = TEMP_IDLE_READY; 
	solder1321 .button = 	&button_1 ;
	solder1321 .rotary =	&rotary_1 ;
	solder1321 .hotter =	&hotter1321 ;
	solder1321 .tm1650 =	&tm1650_1 ;
	
	Rotary_1_init();delaymsTask_CTRL(rotary_1_msg,ENABLE );
	Button_1_init();delaymsTask_CTRL(button_1_msg,ENABLE );
	Tm1650_1_init();delaymsTask_CTRL(tm1650_1_msg,ENABLE );
	
	Hotter1321_init();
 	FlshPara_Init();
	
	delaymsTask_CTRL(hotter1321_adc_msg ,ENABLE );
	delaymsTask_CTRL(hotter1321_poweron_msg ,ENABLE );
	delaymsTask_CTRL(hotter1321_realTemp_msg ,ENABLE );	
	delaymsTask_CTRL(hotter1321_heated_count_msg ,ENABLE );
	delaymsTask_CTRL(hotter1321_hotter_state_msg ,ENABLE );
	delaymsTask_CTRL(hotterctrl_poweron_msg ,ENABLE );

	solder1321.tm1650->Is_num =NO;
	solder1321.tm1650->dot_run_en =DISABLE ;	
	solder1321.tm1650->blink_en  =DISABLE ;
	solder1321.tm1650->bottom_dot_en =DISABLE ;
	solder1321.tm1650 ->word =(uint8_t*)CODE_Clean ;
}





/******************************** END OF FILE *********************************/

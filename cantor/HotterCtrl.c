/*            2016/07/18
*/
#include "../cantor/hotterctrl.h"
#include "../tool/msg_task.h"
#include "../bsp/tm1650.h"
#include "../tool/flash.h"
#include <string.h>
#include <math.h>
#include "../tool/adc_filter.h"

void TEMP_CHECK_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl -> hotter ->heat_en(DISABLE );//不加热
	hotterCtrl -> tm1650 ->Is_num = NO ;//显示文字
	hotterCtrl -> tm1650 ->word =(uint8_t*)CODE_Clean ;
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//不跑灯
	hotterCtrl -> tm1650 ->blink_en  = NO ;//不闪
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//不显示加热点
	hotterCtrl -> state = TEMP_CHECK  ;	
	hal_100ms_flag =0;
}


void TEMP_CHECK_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	if(hal_100ms_flag >3)
	{
		if(hotterCtrl->hotter->Is_power_on)
		{
			 hotterCtrl ->  state = TEMP_TARGET_SHOW_READY  ;
		}
		else 

		hotterCtrl->state =TEMP_IDLE_READY ;

	}

}





void TEMP_IDLE_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl -> hotter ->heat_en(DISABLE );//不加热
	hotterCtrl -> tm1650 ->Is_num = NO ;//显示文字
	hotterCtrl -> tm1650 ->word =(uint8_t*)OFF;//内容OFF
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//不跑灯
	hotterCtrl -> tm1650 ->blink_en  = NO ;//不闪
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//不显示加热点
	hotterCtrl -> state = TEMP_IDLE  ;	
}


void TEMP_IDLE_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	if(hotterCtrl->hotter->Is_power_on)
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
	hotterCtrl->tm1650->num = &hotterCtrl->hotter->target_temperature ;//内容：目标温度
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
	hotterCtrl->tm1650->blink_en  =DISABLE ;	// 
	//显示
	hotterCtrl->state = TEMP_CTRL ;
}


void TEMP_CTRL_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	
	//显示内容
	if(hotterCtrl->hotter->work_state ==TEMP_BALANCE )
	{
		hotterCtrl->tm1650->num = &hotterCtrl->hotter->target_temperature;
	}
	else 
	{
		hotterCtrl->tm1650->num =&hotterCtrl-> hotter->real_temperature;
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
	//归位检查
	if(hotterCtrl ->hotter ->Is_reset_position)
	{
		hotterCtrl->state = TEMP_RESET_POSITION_READY;
	}
 
}


void TEMP_RESET_POSITION_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl -> hotter->heat_en(DISABLE );//不加热
	//显示
	hotterCtrl -> tm1650 ->Is_num = YES ;
	hotterCtrl -> tm1650 ->num = &hotterCtrl ->hotter ->real_temperature ;
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//不跑灯
	hotterCtrl -> tm1650 ->blink_en  =DISABLE  ;//不闪
	hotterCtrl -> tm1650 ->bottom_dot_en =ENABLE;//不显示加热点
	//显示
	hotterCtrl ->state = TEMP_RESET_POSITION ;
}

void TEMP_RESET_POSITION_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	if(!hotterCtrl ->hotter ->Is_reset_position)
	{
		hotterCtrl ->state = TEMP_CTRL_READY;
	}
	if(hotterCtrl ->hotter->real_temperature<70)
	{
		//显示
		hotterCtrl -> tm1650 ->Is_num = NO ;
		hotterCtrl -> tm1650 ->word =(uint8_t *)SLP ;
		hotterCtrl -> tm1650 ->dot_run_en =ENABLE ;	//跑灯
		hotterCtrl -> tm1650 ->blink_en  =DISABLE  ;//不闪
		hotterCtrl -> tm1650 ->bottom_dot_en =NO;//不显示加热点
		//显示
	}
	
}



void TEMP_TARGET_SET_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hal_100ms_flag =0;//用于后面程序计时用
	hotterCtrl -> hotter->heat_en(DISABLE );//不加热
	//显示
	hotterCtrl -> tm1650 ->Is_num = YES ;
	hotterCtrl -> tm1650 ->num =&hotterCtrl ->hotter->target_temperature; 
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//不跑灯
	hotterCtrl -> tm1650 ->blink_en  =DISABLE  ;//不闪
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//不显示加热点
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
	hotterCtrl -> tm1650 ->dot_run_en =ENABLE  ;// 
	hotterCtrl -> tm1650 ->blink_en  = NO ;//不闪
	hotterCtrl -> tm1650 ->bottom_dot_en =NO ;//不显示加热点
	//显示
	hotterCtrl ->state = TEMP_ADJUST_WARNING ;
	
	hal_100ms_flag =0;
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
	hotterCtrl -> tm1650 ->num =&hotterCtrl ->hotter ->adjust_temperature ; 
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE   ;//不跑灯
	hotterCtrl -> tm1650 ->blink_en  = ENABLE ;// 闪
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//不显示加热点
	//显示
	hotterCtrl -> hotter ->heat_en(DISABLE );//不加热
//	hotterCtrl ->hotter ->adjust_temperature =hotter_flash.adjust_temperature; 
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
//		hotterCtrl ->hotter ->adjust_temperature =hotter_flash.adjust_temperature; 
		FlshPara_Init();
		hotterCtrl -> state =TEMP_CTRL_READY ;
	}
		
//	如果持续未按并且持续时间超过xx秒
	if(hal_100ms_flag >50)
	{	
//		hotterCtrl ->hotter ->adjust_temperature =hotter_flash.adjust_temperature; 
		FlshPara_Init();
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

void TEMP_PowerOn_ISR(HOTTER_CTRL_Typedef *hotterCtrl)
{
	if(!hotterCtrl->hotter->Is_power_on)
	{
		hotterCtrl->state =TEMP_IDLE_READY ;
	}
}



// 根据按键、温度判断等控制状态转换
void HotterCtrl(HOTTER_CTRL_Typedef *hotterCtrl) 
{
	switch (hotterCtrl->state) 
	{
		case TEMP_CHECK:
			TEMP_CHECK_operate(hotterCtrl);
			break;
		
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
		
		case TEMP_CHECK_READY:
			TEMP_CHECK_READY_operate(hotterCtrl);
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
	
}






#if 0
HOTTER_CTRL_Typedef solder1321;
void solder1321_init(void)	
{	
	
	
	
	solder1321 .state =  TEMP_CHECK_READY; 
	solder1321 .button = 	&button_1 ;
	solder1321 .rotary =	&rotary_1 ;
	solder1321 .hotter =	&hotter1321 ;
	solder1321 .tm1650 =	&tm1650_2 ;
	
	Rotary_1_init();TaskCtrl(&task_systick,rotary_1_msg,ENABLE );
	Button_1_init();TaskCtrl(&task_systick,button_1_msg,ENABLE );
	Tm1650_2_init();TaskCtrl(&task_systick,tm1650_2_msg,ENABLE );
	Avg_1_Init();
	Hotter1321_init();
 	FlshPara_Init();
	
	TaskCtrl(&task_systick,hotter1321_adc_msg ,ENABLE );
	TaskCtrl(&task_systick,hotter1321_poweron_msg ,ENABLE );
	TaskCtrl(&task_systick,hotter1321_realTemp_msg ,ENABLE );	

	
	TaskCtrl(&task_systick,hotter1321_hotter_state_msg ,ENABLE );
	TaskCtrl(&task_systick,hotterctrl_poweron_msg ,ENABLE );
	TaskCtrl(&task_systick,hotter1321_hotter_poewer_on_msg ,ENABLE );

}
void Solder1321_PowerOn_ISR(void)
{
	TEMP_PowerOn_ISR(&solder1321);
}


void Solder1321Ctrl(void)
{
	HotterCtrl(&solder1321);
}
#endif

HOTTER_CTRL_Typedef airK;
void airK_init(void)	
{	
	
	
	
	airK .state =  TEMP_CHECK_READY; 
	airK .button = 	&button_1 ;
	airK .rotary =	&rotary_1 ;
	airK .hotter =	&hotterK ;
	airK .tm1650 =	&tm1650_1 ;
	
	Rotary_1_init();TaskCtrl(&task_systick,rotary_1_msg,ENABLE );
	Button_1_init();TaskCtrl(&task_systick,button_1_msg,ENABLE );
	Tm1650_1_init();TaskCtrl(&task_systick,tm1650_1_msg,ENABLE );
	Avg_1_Init();
	HotterK_init();
 	FlshPara_Init();
	
	TaskCtrl(&task_systick,hotterK_adc_msg ,ENABLE );
	TaskCtrl(&task_systick,hotterK_poweron_msg ,ENABLE );
	TaskCtrl(&task_systick,hotterK_realTemp_msg ,ENABLE );	
	TaskCtrl(&task_systick,hotterK_hotter_state_msg ,ENABLE );
	TaskCtrl(&task_systick,airK_power_on_msg ,ENABLE );
	TaskCtrl(&task_systick,FAN_CTRL_MSG ,ENABLE );
	

}
void airK_PowerOn_ISR(void)
{
	TEMP_PowerOn_ISR(&airK );
}


void airKCtrl(void)
{
	HotterCtrl(&airK);
}
//void air_init(void)
void FanCtrl_ISR(void)
{
	if(airK.hotter->Is_reset_position)
	{
		if(airK .hotter->real_temperature <70)
		{
				HAL_GPIO_WritePin(AIR_ON_OFF_GPIO_Port,AIR_ON_OFF_Pin,GPIO_PIN_SET );
		}
	}
	else
	{
		if(airK .state == TEMP_CTRL)
		HAL_GPIO_WritePin(AIR_ON_OFF_GPIO_Port,AIR_ON_OFF_Pin,GPIO_PIN_RESET );
		if(airK .state == TEMP_IDLE )
		HAL_GPIO_WritePin(AIR_ON_OFF_GPIO_Port,AIR_ON_OFF_Pin,GPIO_PIN_SET );	
	}
	
}

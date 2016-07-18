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

	
	if(hotter1321.Is_power_on)//solder switch checking
	{
		solder1 .state = TempCTRL  ;

	}

	hotter1321 .heat_en(DISABLE );
	tm1650_1 .word =(uint8_t*)OFF;
	tm1650_1.dot_run_en =DISABLE ;	
	tm1650_1.blink_en  = NO ;
	tm1650_1.Is_num = NO ;
	tm1650_1 .bottom_dot_en =NO;
	tm1650_1.times_100ms=0;
}

void TempSetShow_operate(void )
{
	
	tm1650_1.times_100ms=0;
	tm1650_1.Is_num = YES ;
	tm1650_1.dot_run_en =DISABLE ;	
	tm1650_1.blink_en  =ENABLE ;
	tm1650_1.bottom_dot_en =DISABLE ;
	tm1650_1.num =hotter1321 .target_temperature ;
	if(tm1650_1.times_100ms>20)
	{
		tm1650_1.Is_num = YES ;
		tm1650_1.dot_run_en =DISABLE ;	
		tm1650_1.blink_en  =DISABLE ;
		tm1650_1.bottom_dot_en =DISABLE ;
		solder1 .state = TempCTRL  ;
		hotter1321 .work_state = notheating;//��ʼ��H-E���
		tm1650_1.times_100ms =0;
	}
	
}


void SOLDER_TempCTRL_operate(void)
{
 
	//��ʾ
	if(hotter1321 .work_state ==balance)
		tm1650_1.num = hotter1321.target_temperature;
	else tm1650_1.num = hotter1321.real_temperature;
// 	//���ȿ���-ADC������Χ
	if(hotter1321 .real_adc<hotter1321 .sensor_err_adc)
	{
		if( hotter1321 .real_temperature< hotter1321 .target_temperature)
		{
			hotter1321 .heat_en(ENABLE );
		 		
			tm1650_1.bottom_dot_en =ENABLE  ;//��ʾ������
		}
		else
		{
			tm1650_1.bottom_dot_en =DISABLE  ;//��ʾ������
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
	//�¶��ϲ�ȥ���
	if(!hotter1321 .heated_check_dir)
	{
		if(	hotter1321 .work_state==heating)
		{
			hotter1321 .heated_times=0;
			
			hotter1321.temperature_pre =   hotter1321 .real_temperature;
			hotter1321 .heated_check_dir=~hotter1321 .heated_check_dir;
		}
	}
	else 
	{
		if(	hotter1321 .work_state!=heating)
		{
		   hotter1321 .heated_check_dir=~hotter1321 .heated_check_dir;
			 
		}
		else
		{
			if((hotter1321.temperature_pre -hotter1321.real_temperature )>100)
			{

				tm1650_1.blink_en  =ENABLE ;		
			}
			else
			{
				tm1650_1.blink_en  =DISABLE   ;
			}	
				
			if((hotter1321 .real_temperature <80)&&(hotter1321 .heated_times>100)&&(__fabs (hotter1321 .real_temperature-hotter1321.temperature_pre)<4))
			{		
				solder1 .state = ALARM ;
				hotter1321 .hotter_err=1;
				tm1650_1.Is_num = NO ;
				tm1650_1.dot_run_en =DISABLE ;	
				tm1650_1.blink_en  =ENABLE ;
		
			}
		}	
		
	}
			
	//���°������	
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
	//У�°������	
	if ((button_1.continue_press_times>300)&&(hotter1321 .work_state ==balance))
	{
		
		hotter1321 .heat_en(DISABLE );
		tm1650_1.Is_num = NO  ;
		tm1650_1.word = (uint8_t*)CAL;
		tm1650_1.dot_run_en =ENABLE ;	
		tm1650_1.blink_en  =DISABLE ;
		tm1650_1.bottom_dot_en=DISABLE ;
		solder1 .state = TempADJUST ;
		button_1 .times_10ms =0;
	}
}






void SOLDER_TempSET_operate(void)
{

	tm1650_1.num = hotter1321.target_temperature;
	/*
	����������¶�����������
	*/
	if(rotary_1.Spin_direction == Spin_left)
	{
		button_1 .times_10ms=0;
		rotary_1.Spin_direction = no_direction ;
		if(--hotter1321.target_temperature<=hotter1321 .Lmin)
			hotter1321.target_temperature=hotter1321 .Lmin;
	}
	/*
	����������¶����ӷ�����
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
	���ȷ�����¶��趨���
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
//	�������δ�����ҳ���ʱ�䳬��xx�룬�¶��趨���
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


void TempAdjustEnterShow_operate(void)
{
	
	if (button_1 .times_10ms>100)
		{ 
			memcpy(&hotter_flash,(Hotter_flash__TypeDef *)PARA_START_ADDR,sizeof (Hotter_flash__TypeDef)); 		
			tm1650_1.Is_num = YES ;
			tm1650_1.dot_run_en =DISABLE ;	
			tm1650_1.blink_en  =ENABLE ;
			tm1650_1.bottom_dot_en =DISABLE ;
		}
}








void SOLDER_TempADJUST_operate(void)
{
	
	tm1650_1.num =  hotter1321 .adjust_temperature   ;
	/*
	����������¶�����������
	*/
	if(rotary_1.Spin_direction == Spin_left)
	{
		
		button_1 .times_10ms =0;
		rotary_1.Spin_direction = no_direction;
		-- hotter1321 .adjust_temperature ;
		if( hotter1321 .adjust_temperature <hotter1321 .Cmin)  hotter1321 .adjust_temperature   =  hotter1321 .Cmin;
	}
	
	/*
	����������¶����ӷ�����
	*/
	if(rotary_1.Spin_direction == Spin_right)
	{	button_1 .times_10ms =0;
		rotary_1.Spin_direction = no_direction;
		++ hotter1321 .adjust_temperature ;
		if( hotter1321 .adjust_temperature  > hotter1321 .Cmax)  hotter1321 .adjust_temperature  =  hotter1321 .Cmax;
		
	}
	
	/*
	���ȷ�����¶��趨���
	*/
	if(button_1 .Is_press == YES )
	{
		tm1650_1.dot_run_en =NO ;
		tm1650_1.bottom_dot_en =NO;
		tm1650_1.blink_en  =DISABLE ;
		tm1650_1.Is_num = YES ;
		hotter1321 .adjust_temperature = hotter1321 .adjust_temperature ;
		
		FlshPara_Save();
		solder1 .state =TempCTRL ;
		
	}
		
//	�������δ�����ҳ���ʱ�䳬��xx��
	if(button_1 .times_10ms>500)
	{	
		tm1650_1.dot_run_en =NO ;
		tm1650_1.bottom_dot_en =NO;
		tm1650_1.blink_en  =DISABLE ;
		tm1650_1.Is_num = YES  ;
	
		solder1 .state = TempCTRL ;
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
������: Status_Tran 
��������: ���ݰ������¶��жϵȿ���״̬ת��
�������: NULL
���ز���: NULL
*******************************************************************************/
void Solder_1_Transformation(void)//״̬ת��
{
		 

	switch (solder1 .state) 
	{
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

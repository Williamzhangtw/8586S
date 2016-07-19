/*            2016/07/18
*/
#include "../cantor/hotterctrl.h"
#include "../tool/msg_task.h"
#include "../bsp/tm1650.h"
#include "../tool/flash.h"
#include <string.h>
#include <math.h>









void Idle_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{	
	hotterCtrl -> tm1650 ->Is_num = NO ;
	hotterCtrl -> tm1650 ->word =(uint8_t*)OFF;//��ʾOFF
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//���ܵ�
	hotterCtrl -> tm1650 ->blink_en  = NO ;//����
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//����ʾ���ȵ�

	if(hotterCtrl->hotter->Is_power_on)//solder switch checking
	{
		hotterCtrl ->  state = TEMP_TARGET_SHOW  ;
		hotterCtrl -> tm1650 ->times_100ms=0;//���ں�������ʱ��	
	}
	hotterCtrl -> hotter ->heat_en(DISABLE );//������	
}












void TargetShow_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	//��ʾĿ���¶�
	hotterCtrl->tm1650->Is_num = YES ;
	hotterCtrl->tm1650->num =hotterCtrl->hotter->target_temperature ;
	
	hotterCtrl->tm1650->dot_run_en =DISABLE ;	//���ܵ�
	hotterCtrl->tm1650->blink_en  =ENABLE ;
	hotterCtrl->tm1650->bottom_dot_en =DISABLE ;
	
	//2S�����CTRL
	if(hotterCtrl->tm1650->times_100ms>20)
	{
		hotterCtrl->state = TEMP_CTRL  ;
		hotterCtrl->hotter->work_state = notheating;//��ʼ�������� �¶��ϲ�ȥ���
		hotterCtrl->tm1650->times_100ms =0;
	}
	
	hotterCtrl -> hotter->heat_en(DISABLE );//������
	
}


void Ctrl_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
 
	hotterCtrl->tm1650->Is_num = YES ;//��ʾ��������
	//��ʾ���ݿ���
	if(hotterCtrl->hotter->work_state ==balance)
	{
		hotterCtrl->tm1650->num = hotterCtrl->hotter->target_temperature;
	}
	else 
	{
		hotterCtrl->tm1650->num =hotterCtrl-> hotter->real_temperature;
	}
	hotterCtrl->tm1650->dot_run_en =DISABLE ;	//���ܵ�
	
	
 	//���ȿ���
	if(hotterCtrl->hotter->real_adc<hotterCtrl->hotter->sensor_err_adc)
	{
		if( hotterCtrl->hotter->real_temperature< hotterCtrl->hotter->target_temperature)
		{
			hotterCtrl->hotter->heat_en(ENABLE );	
			hotterCtrl->tm1650->bottom_dot_en =ENABLE  ;//��ʾ������
		}
		else
		{
			hotterCtrl->hotter ->heat_en(DISABLE );
			hotterCtrl->tm1650 ->bottom_dot_en =DISABLE  ;//��ʾ������
		}
	}
	else //ADC������Χ S-E
	{
		hotterCtrl->state = TEMP_ALARM ;
		hotterCtrl->hotter->sensor_err =1;
	}
	
	
	
	//�¶��ϲ�ȥ���
	if(hotterCtrl->hotter->heated_check_dir == 0 )
	{
		if(	hotterCtrl->hotter->work_state==heating)
		{
			hotterCtrl->hotter->heated_times=0;
			hotterCtrl->hotter->temperature_pre =   hotterCtrl->hotter->real_temperature;//��¼��ǰʵʱ�¶�
			hotterCtrl->hotter->heated_check_dir=1;
			
		}
		hotterCtrl->tm1650->blink_en  =DISABLE   ;
	}
	else 
	{
		if(	hotterCtrl->hotter->work_state!=heating)
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
				
			if((hotterCtrl->hotter->real_temperature <90)&&(hotterCtrl->hotter->heated_times>100)&&(__fabs (hotterCtrl->hotter->real_temperature-hotter1321.temperature_pre)<4))
			{		
				hotterCtrl-> state = TEMP_ALARM ;
				hotterCtrl->hotter->hotter_err=1;
			}
		}	
		
	}
			
	
	//���°������	
	if(hotterCtrl->rotary->Spin_direction)
	{
		 
		hotterCtrl-> state = TEMP_TARGET_SET ;
		hotterCtrl->button->times_10ms=0;
	}	
	//У�°������	
	if ((hotterCtrl->button->continue_press_times>300)&&(hotterCtrl->hotter->work_state ==balance))
	{
		
		hotterCtrl->state = TEMP_ADJUST_WARNING  ;
		hotterCtrl -> tm1650->times_100ms=0;//���ں�������ʱ��
	}
}







void TargetSet_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	//��ʾ
	hotterCtrl -> tm1650 ->Is_num = YES ;
	hotterCtrl -> tm1650 ->num =hotterCtrl ->hotter->target_temperature; 
	
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//���ܵ�
	hotterCtrl -> tm1650 ->blink_en  = NO ;//����
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//����ʾ���ȵ�
	hotterCtrl -> hotter ->heat_en(DISABLE );//������
	
	hotterCtrl -> hotter ->heat_en(DISABLE );//������	
	
	
	//����������¶�����������
	if(hotterCtrl ->rotary->Spin_direction == Spin_left)
	{
		hotterCtrl ->button->times_10ms=0;
		hotterCtrl ->rotary->Spin_direction = no_direction ;
		if(--hotterCtrl ->hotter->target_temperature<=hotterCtrl ->hotter ->Lmin)
		{
			hotterCtrl ->hotter->target_temperature=hotterCtrl ->hotter ->Lmin;
		}
	}
	/*
	����������¶����ӷ�����
	*/
	if(hotterCtrl ->rotary->Spin_direction == Spin_right)
	{
		hotterCtrl ->button ->times_10ms=0;
		hotterCtrl ->rotary->Spin_direction = no_direction ;
		if(++hotterCtrl ->hotter->target_temperature >hotterCtrl ->hotter->Lmax )
		{
			hotterCtrl ->hotter->target_temperature =hotterCtrl ->hotter->Lmax ;
		}
	}
	/*
	���ȷ�����¶��趨���
	*/
	if(hotterCtrl ->button->Is_press == YES )
	{
		FlshPara_Save();
		hotterCtrl ->state = TEMP_CTRL ;
	}
	
//�������δ�����ҳ���ʱ�䳬��xx�룬�¶��趨���
	if(hotterCtrl ->button->times_10ms>250)
	{
		FlshPara_Save();
		hotterCtrl ->state = TEMP_CTRL ;
	}
}


void AdjustWarning_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	//��ʾ
	hotterCtrl -> tm1650 ->Is_num = NO ;
	hotterCtrl -> tm1650 ->word =(uint8_t*)CAL ; 
	
	hotterCtrl -> tm1650 ->dot_run_en =ENABLE  ;	//���ܵ�
	hotterCtrl -> tm1650 ->blink_en  = NO ;//����
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//����ʾ���ȵ�
	
	hotterCtrl -> hotter ->heat_en(DISABLE );//������

	
	if (hotterCtrl->tm1650 ->times_100ms>15)
		{ 
			memcpy(&hotter_flash,(Hotter_flash__TypeDef *)PARA_START_ADDR,sizeof (Hotter_flash__TypeDef)); 		
			hotterCtrl ->state = TEMP_ADJUST ;
			hotterCtrl -> tm1650->times_100ms=0;//���ں�������ʱ��
			
				hotterCtrl -> tm1650 ->Is_num = YES ;
	hotterCtrl -> tm1650 ->dot_run_en =NO  ;	//���ܵ�
	hotterCtrl -> tm1650 ->blink_en  = ENABLE ;//��
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//����ʾ���ȵ�
	
	hotterCtrl -> hotter ->heat_en(DISABLE );//������
			
		}
}


void Adjust_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	//��ʾ
	
	hotterCtrl -> tm1650 ->num =hotterCtrl -> hotter ->adjust_temperature ; 


	//����������¶�����������
	if(hotterCtrl -> rotary->Spin_direction == Spin_left)
	{
		
		hotterCtrl -> tm1650 ->times_100ms =0;
		rotary_1.Spin_direction = no_direction;
		-- hotter1321 .adjust_temperature ;
		if( hotter1321 .adjust_temperature <hotter1321 .Cmin) 
		{			
			hotter1321 .adjust_temperature   =  hotter1321 .Cmin;
		}
	}
	
	/*
	����������¶����ӷ�����
	*/
	if(hotterCtrl -> rotary->Spin_direction == Spin_right)
	{	
		hotterCtrl -> tm1650 ->times_100ms =0;
		hotterCtrl -> rotary->Spin_direction = no_direction;
		++ hotterCtrl -> hotter ->adjust_temperature ;
		if( hotterCtrl -> hotter ->adjust_temperature  >hotterCtrl ->  hotter ->Cmax)  
		{
			hotterCtrl -> hotter ->adjust_temperature  =  hotterCtrl -> hotter ->Cmax;
		}
		
	}
	
	//���ȷ�����¶��趨���
	if(hotterCtrl -> button ->Is_press == YES )
	{
		
		FlshPara_Save();
		hotterCtrl -> state =TEMP_CTRL ;
		
	}
		
//	�������δ�����ҳ���ʱ�䳬��xx��
	if(hotterCtrl -> tm1650 ->times_100ms>50)
	{	
		hotterCtrl -> state = TEMP_CTRL ;
	}	
}





void Alarm_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	//��ʾ
	hotterCtrl -> tm1650 ->Is_num = NO ;
	if(hotterCtrl->hotter ->sensor_err )
	{
		hotterCtrl -> tm1650 ->word =(uint8_t *)S_E  ; 
	}
	else
	{
		hotterCtrl -> tm1650 ->word =(uint8_t *)H_E  ; 
	}
	
	hotterCtrl -> tm1650 ->dot_run_en =NO  ;	//���ܵ�
	hotterCtrl -> tm1650 ->blink_en  = ENABLE ;//��

	
	if(hotterCtrl->hotter ->hotter_err )
	{
			
		hotterCtrl -> tm1650 ->bottom_dot_en =ENABLE ;//��ʾ���ȵ�
		hotterCtrl -> hotter ->heat_en(ENABLE );//����
		if(hotter1321 .real_temperature  >90)
		{
			hotterCtrl ->  state =TEMP_CTRL ;
			hotterCtrl -> hotter ->hotter_err =0;
		}
	}
	else
	{
		hotterCtrl -> tm1650 ->bottom_dot_en =DISABLE ;//��ʾ���ȵ�
		hotterCtrl -> hotter ->heat_en(DISABLE );//����
		if(hotter1321 .real_adc <hotter1321 .sensor_err_adc )
		{
			hotterCtrl ->  state =TEMP_CTRL ;
			hotter1321 .sensor_err =0 ;
		}
	}
}





// ���ݰ������¶��жϵȿ���״̬ת��
void HotterCtrl(HOTTER_CTRL_Typedef *hotterCtrl) 
{
	switch (hotterCtrl->state) 
	{
		case TEMP_IDLE:
			Idle_operate(hotterCtrl);
			break;
		case TEMP_TARGET_SHOW :
			TargetShow_operate(hotterCtrl);
			break ;
		case TEMP_CTRL:
			Ctrl_operate(hotterCtrl);
			break;
		case TEMP_TARGET_SET:
			 TargetSet_operate(hotterCtrl);
			break;
		case TEMP_ALARM:
			Alarm_operate(hotterCtrl);
			break;	 
	
		case TEMP_ADJUST_WARNING :
			AdjustWarning_operate(hotterCtrl);
			break ;
		case TEMP_ADJUST:
			Adjust_operate(hotterCtrl);
			break;

		default:
			break;
	}
//	if(!hotterCtrl->hotter->Is_power_on)
//	{
//		solder1321 .state =TEMP_IDLE ;
//	}
}



HOTTER_CTRL_Typedef solder1321;


void solder1321_init(void)	
{	
	solder1321 .state = TEMP_IDLE ;
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

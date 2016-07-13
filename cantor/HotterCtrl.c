/*
Data:            2016/06/30
*/
#include "../cantor/hotterctrl.h"
#include "../tool/msg_task.h"
#include "../bsp/tm1650.h"
#include "../tool/flash.h"



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
	solder1 .state =IDLE   ;	
}




void SOLDER_IDLE_operate(void)
{
	hotter1321 .heat_en(DISABLE );
	tm1650_1.disp_type = word ;
	tm1650_1.word = OFF ;
	if(hotter1321.Is_power_on)//solder switch checking
	solder1 .state = TempCTRL  ;
}




void SOLDER_TempCTRL_operate(void)
{
	static uint8_t dir = 1;//����У׼�¶Ȱ��� 
	
	static uint8_t dir_1 = 1;//����H-E���
	static uint16_t temperature_pre ;
	//��ʾ
		
	if(hotter1321 .work_state ==3)
		tm1650_1.num = hotter1321.target_temperature;
	else tm1650_1.num = hotter1321.real_temperature;
// 	//���ȿ���-ADC������Χ

	if(hotter1321 .real_adc<hotter1321 .sensor_err_adc)
	{
		if( hotter1321 .real_temperature< hotter1321 .target_temperature)
		{
			hotter1321 .heat_en(ENABLE );
			
			tm1650_1.disp_type =dot ;//��ʾ������
		}
		else
		{
			tm1650_1.disp_type =num ;//��ʾ������
			hotter1321 .heat_en(DISABLE );
		}
	}
	else 
	{
		solder1 .state = ALARM ;
		hotter1321 .sensor_err =1;
	}
	//H-E���
	if(dir_1)
	{
		if(	hotter1321 .work_state==1)
		{
			hotter1321 .heated_time_count=0;
			temperature_pre = hotter1321 .real_temperature;
			dir_1 = 0;
		}
	}
	else 
	{
		if(	hotter1321 .work_state!=1)
		{
				dir_1 = 1;
		}
		else
		{
			if(hotter1321 .heated_time_count>100)
			{
				if ((__fabs (hotter1321 .real_temperature - temperature_pre)<2))
				{
					solder1 .state = ALARM ;
					hotter1321 .hotter_err=1;
					
				}
				dir_1 = 1;
			}	
		}
	}
			
	//���°������	
	if(rotary_1.Is_press == YES)
	{
		rotary_1.Is_press =NO;
		solder1 .state = TempSET ;	

	}	
	//У�°������	
	if(dir)
	{
		if (button_1.Is_press == YES)
		{
			dir =0;button_1.continue_press_time = NO;
		}
	}
	else
	{
		if (button_1.Is_press == YES)
		{
			if (button_1.continue_press_time>500)
			{
				solder1 .state = TempADJUST ;
				dir=1;
			}
		}
		else dir=1;
	
	}
}





void SOLDER_TempSET_operate(void)
{
	static uint8_t reset_flag = 0;
	tm1650_1.disp_type = num ;
	tm1650_1.num = hotter1321.target_temperature;
	/*
	����������¶�����������
	*/
	if(rotary_1.Spin_direction == Spin_left)
	{
		reset_flag =1;
		rotary_1.Spin_direction = no_direction ;
		if(--hotter1321.target_temperature<=hotter1321 .Lmin)
			hotter1321.target_temperature=hotter1321 .Lmin;
	}
	/*
	����������¶����ӷ�����
	*/
	if(rotary_1.Spin_direction == Spin_right)
	{
		reset_flag =1;
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
		FlshPara_Save();
		solder1 .state = TempCTRL ;
	}
	
//	/*
//	�������δ�����ҳ���ʱ�䳬��xx�룬�¶��趨���
//	*/
	
	if(HalTimeDelay(&hal_100ms_flag,30,&reset_flag))
	{	
		FlshPara_Save();
		solder1 .state = TempCTRL ;
	}		
}




void SOLDER_TempADJUST_operate(void)
{
	static uint8_t n = 0;
	static uint8_t dir =1;
	switch (n)
	{
		case 0:
 		tm1650_1.disp_type = word ;
	  tm1650_1.word = CAL;
		if(dir){button_1 .time_count =0;dir =0;}
		else {if (button_1 .time_count>100){hotter1321 .adjust_temperature = hotter1321 .target_temperature;
			n =1;dir=1;button_1 .time_count =0;}}
		break ;
		case 1:
     tm1650_1.disp_type = numrun ;
	   tm1650_1.num =  hotter1321.adjust_temperature ;

	/*
	����������¶�����������
	*/
	if(rotary_1.Spin_direction == Spin_left)
	{
		button_1 .time_count =0;
		rotary_1.Spin_direction = no_direction;
		++hotter1321.adjust_temperature;
		if(hotter1321.adjust_temperature > hotter1321 .Cmax) hotter1321.adjust_temperature  =  hotter1321 .Cmax;
	}
	
	/*
	����������¶����ӷ�����
	*/
	if(rotary_1.Spin_direction == Spin_right)
	{	button_1 .time_count =0;
		rotary_1.Spin_direction = no_direction;
		--hotter1321.adjust_temperature;
		if(hotter1321.adjust_temperature < hotter1321 .Cmin) hotter1321.adjust_temperature  =  hotter1321 .Cmin;
		
	}
	
	
	/*
	���ȷ�����¶��趨���
	*/
	if(button_1 .Is_press == YES )
	{
		n =0;
		hotter1321.Bs= hotter1321.Bs + hotter1321.adjust_temperature - hotter1321.real_temperature;
		FlshPara_Save();
		solder1 .state =TempCTRL ;
	}
		
//	�������δ�����ҳ���ʱ�䳬��xx��
	if(button_1 .time_count>300)
	{	 n =0;
		solder1 .state = TempCTRL ;
	}		
			break;
		default :
			break ;
	}
}




void SOLDER_ALARM_operate(void)
{
	tm1650_1.disp_type = word ;
	
	
	if(hotter1321 .sensor_err )
	{
		tm1650_1.word = S_E;
		if(hotter1321 .real_adc <hotter1321 .sensor_err_adc )
		{
			solder1 .state =TempCTRL ;
			hotter1321 .sensor_err =0 ;
		}
	}
	else
	{
		if(hotter1321 .hotter_err )
		{
			tm1650_1.word = H_E;
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

/*2016-7-15
*/
#include "../tool/msg_task.h"

//功能函数 接口interface//
#include "../bsp/tm1650.h"
#include "../cantor/hotterctrl.h"
#include "../bsp/hotter.h"

void TaskISR(Task_TypeDef * task)//轮询方式进行触发
{
	task ->msg_temp =task ->msg ;
	for(uint8_t i=0;i<task->id_num;i++) //遍睨所有消息结构体
	{
		if(task ->msg_temp->Is_enable == YES)
		{
			if(task ->msg_temp->time_temp  <= 1)//起到原函数中的 延时功能
			{
				task ->msg_temp->Is_ready = YES;
				task ->msg_temp->time_temp = task ->msg_temp->time ;
			}
			task ->msg_temp->time_temp --;
		}
		task ->msg_temp++;
	}
}

void TaskCtrl(Task_TypeDef * task,uint16_t id,uint8_t en)
{
	task ->msg_temp =task ->msg ;
	
	for(uint8_t i=0;i<task->id_num ;i++)
	{
		if(task ->msg_temp->id   == id)
		{
			task ->msg_temp->Is_enable =en;
		}
		
		task ->msg_temp++;
	}
	
}


void TaskProcess( Task_TypeDef * task)
{
	
	task ->msg_temp = task ->msg;
	for(uint8_t i=0; i< task->id_num  ;i++) 
	{
		
		if(task ->msg_temp  ->Is_ready ==YES)
		{	
			 
			task ->msg_temp->Is_ready = NO;	
			if (task ->msg_temp->funproc!=NULL)
			{
				task ->msg_temp->funproc();					
			}
		}
		task ->msg_temp++;
	}
}



Task_TypeDef task_systick;
Msg_TypeDef msg_systick[] =      //消息，消息处理函数
{	
	//id							Is_enable	Is_ready	time	(*funproc)
	{tm1650_1_msg, 					NO,		  	NO,			100,	Tm1650_1_show_ISR,0},	
	{tm1650_2_msg, 					NO,		  	NO,			100,	Tm1650_2_show_ISR,0},
	{button_1_msg , 				NO,			NO,			10,		BUTTON_1_ISR,0},	
	{rotary_1_msg, 					NO,			NO,			1,		Rotary_1_scan_ISR,0} ,
	
//	{hotter1321_adc_msg,			NO,			NO,			2,		Filter_hotter1321_adc_ISR,0},
//	{hotter1321_poweron_msg,		NO,			NO,			50,		Hotter1321_power_on_scan_ISR,0},
//	{hotter1321_realTemp_msg,		NO,			NO,			100,	Hotter1321_realTemp_ISR,0},
//	{hotter1321_hotter_state_msg,	NO,     	NO,         50,     Hotter1321WorkingState_ISR,0},
//	{hotter1321_hotter_power_on_msg,NO,		NO,			100,	Solder1321_PowerOn_ISR},
	
	{FAN_CTRL_MSG ,					NO,			NO,			100,	FanCtrl_ISR,0},
	{hotterK_adc_msg,				NO,			NO,			1,		Filter_hotterK_adc_ISR,0},
	{hotterK_poweron_msg,			NO,			NO,			50,		HotterK_power_on_scan_ISR,0},
	{hotterK_realTemp_msg,			NO,			NO,			1,	HotterK_realTemp_ISR,0},
	{hotterK_hotter_state_msg,		NO,     	NO,         50,     HotterKWorkingState_ISR,0},
	{airK_power_on_msg,	NO,			NO,			100,	airK_PowerOn_ISR},
};



void TaskSystickInit(void)
{
	task_systick.msg = msg_systick;
	task_systick.id_num =  ArrayNum(msg_systick);
}

void TaskSystickProcess(void)
{
	TaskProcess(&task_systick);
}




Task_TypeDef task_main;




Msg_TypeDef msg_main[] =      //消息，消息处理函数
{	
	//id				Is_enable		Is_ready	time			(*funproc)
//	{SOLDER1321_MSG , 	YES,			NO,			1,				Solder1321Ctrl,0},
	{AIRK_MSG , 		YES,			NO,			1,				airKCtrl,0},

	{TASK_SYSTICK_MSG, 	YES,		  	NO,			1,			TaskSystickProcess,0},	

};
void TaskMainkInit(void)
{
	task_main.msg = msg_main;
	task_main.id_num = ArrayNum(msg_main);
}

uint16_t 	hal_1ms_flag 	=	0;
uint16_t  	hal_10ms_flag	= 	0;
uint16_t  	hal_100ms_flag 	= 	0;
uint16_t  	hal_1s_flag 	= 	0;


void HAL_SYSTICK_Callback()
{
	static	uint8_t 	StaticFlag_1ms		=0;
	static	uint8_t 	StaticFlag_10ms		=0;
	static	uint8_t 	StaticFlag_100ms	=0;
	
	TaskISR(&task_systick);
	hotter1321 .heated_times ++;
	StaticFlag_1ms++; 
	
 	hal_1ms_flag ++;
  if (StaticFlag_1ms == 10) 
  { 
    StaticFlag_1ms =0;
    StaticFlag_10ms ++;  
  /* USER CODE  10ms  */
    hal_10ms_flag ++;
		
  }
  
  if (StaticFlag_10ms == 10) 
  { 
   StaticFlag_10ms =0;
   StaticFlag_100ms ++; 
    /* USER CODE 100ms */    

    hal_100ms_flag ++;		
   
  }
  
  if (StaticFlag_100ms ==10)
  {
    StaticFlag_100ms =0;
    /* USER CODE 1s */
    hal_1s_flag ++;
  }
}


/*函数功能：延时 
//参数 *p 时基 count 时基计数 总时间 是两者的乘积  data_reset 当改为置1时候，计时重新开始
//返回值：
*/
BOOL HalTimeDelay (uint8_t *p,uint16_t count,BOOL *data_reset)
{ 
  static uint16_t times_count =0;
  
  if(*p)   //倒计时
  { 
    *p =0;
  
    if (*data_reset)
    {
      *data_reset = 0;
     times_count=0; 
    }
    if(++times_count>count)//2s 后进入工作状态
    {
      times_count=0;
      return YES;
    }  
  }
  return NO;  
}

/*函数功能： 延迟，每间隔 *p 乘于 time_count时间 后返回1
//函数参数：*p 时基  time_count 计数
//返回值：YES 表示可以显示了！
*/
BOOL HalTimeDelaySimple (uint8_t *p,uint8_t time_count)
{
  static uint16_t count =1;
  if(*p)
  {
    *p = 0;
    if(++count > time_count )
    {
      count = 1;
      return YES;
    }
  }
  return NO;
}







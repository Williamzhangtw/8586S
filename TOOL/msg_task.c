/*2016-7-15
*/
#include "../tool/msg_task.h"

#define Msg_delayms_ID_Num 10	//消息总和


//功能函数 接口interface//
#include "../bsp/tm1650.h"
#include "../cantor/hotterctrl.h"
#include "../bsp/hotter.h"



Msg_Delayms Msg_delayms[Msg_delayms_ID_Num] =      //消息，消息处理函数
{	
	//id										Is_enable		Is_ready		time				(*funproc)
	button_1_msg , 								NO,					NO,			10,					BUTTON_1_ISR,
	tm1650_1_msg, 								NO,		  			NO,			100,				Tm1650_1_show_ISR,	
	rotary_1_msg, 								NO,					NO,			1,					Rotary_1_scan_ISR ,
	hotter1321_adc_msg,							NO,					NO,			2,					Filter_hotter1321_adc_ISR,
	hotter1321_poweron_msg,						NO,					NO,			50,					Hotter1321_power_on_scan_ISR,
	hotter1321_realTemp_msg,					NO,					NO,			100,				Hotter1321_realTemp_ISR,
	hotter1321_heated_count_msg ,				NO,    				NO,         100,        		Hotter1321_heated_time_count_ISR,
	hotter1321_hotter_state_msg,				NO,     			NO,         50,         		Hotter1321WorkingState_ISR,
	
};


//---------------------------------------------------
void Msg_delayms_Init(void)
{

	for(uint8_t i=0;i<Msg_delayms_ID_Num;i++) //消息队列清空
	{
		Msg_delayms[i].Is_ready  = NULL;

		
	}
}



void delaymsTask_ISR(void)//轮询方式进行触发
{

  static uint16_t temp[Msg_delayms_ID_Num]={0};
	for(uint8_t i=0;i<Msg_delayms_ID_Num;i++) //遍睨所有消息结构体
	{
		if(Msg_delayms[i].Is_enable == YES)
		{
			if(temp[i]-- <= 1)//起到原函数中的 延时功能
			{
				Msg_delayms[i].Is_ready = YES;
				temp[i] = Msg_delayms[i].time;
			}
		}
	}
}

void delaymsTask_CTRL(uint8_t id,uint8_t en)
{
	for(uint8_t i=0;i<Msg_delayms_ID_Num;i++)
	{
		if(Msg_delayms[i].id   == id)
		Msg_delayms[i].Is_enable =en;
	}
}

void Msg_delayms_process(void)
{
 
	for(uint8_t  id=0;id<Msg_delayms_ID_Num;id++) 
	{
		if(Msg_delayms[id].Is_ready ==YES)
		{
			if (Msg_delayms[id].funproc!=NULL)
			{
				Msg_delayms[id].funproc();	
				Msg_delayms[id].Is_ready = NO;
			}
		}
	}
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
	
	delaymsTask_ISR();
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







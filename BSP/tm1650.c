/*
modify:2016-7-16AM
attention: for a better vision,display will reflash with a Fixed frequency 
the reflash is update in the ISR.
*/
#include "tm1650.h"
const uint8_t CODE00[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};// 0-9
const uint8_t  CODE_Clean[4]={0x00,0x00,0x00};//
const uint8_t  CODE01[4]={0x40,0x40,0x40};//" - - - "
const uint8_t  S_E[4]={0x79,0x40,0x6D};//" S - E(0xF5) " 
const uint8_t  H_E[4]={0x79,0x40,0x76};//" H - E(0xF5) " 
const uint8_t  OFF[4]={0x71,0x71,0x3F};//" OFF " 
const uint8_t  CAL[4]={0x38,0x77,0x39};//" CAL " 
const uint8_t  MENU01[4]={0x06,0x3F,0x5C,};//"-01" 
const uint8_t  MENU02[4]={0x5B,0x3F,0x5C,};//"-02" 
const uint8_t  MENU03[4]={0x4F,0x3F,0x5C,};//"-03"
const uint8_t  MENU04[4]={0x66,0x3F,0x5C,};//"-04"
const uint8_t  MENU05[4]={0x6D,0x3F,0x5C,};//"-05"
const uint8_t  MENU06[4]={0x7D,0x3F,0x5C,};//"-06"
const uint8_t  MENU07[4]={0x07,0x3F,0x5C,};//"-07"
const uint8_t  MENU08[4]={0x7F,0x3F,0x5C,};//"-08"
const uint8_t  MENU09[4]={0x6F,0x3F,0x5C,};//"-09"
const uint8_t  MENU10[4]={0x3F,0x06,0x5C,};//"-10"
const uint8_t  _1_[4]={0x40,0x06,0x40};//"-1-"
const uint8_t  _2_[4]={0x40,0x5B,0x40};//"-2-"
const uint8_t _888[4] = {0x7F,0x7F,0x7F};//"888"
const uint8_t xianfu[4] = {0x09,0x09,0x09};//二二二
const uint8_t huxi[4] ={0x37,0x0E,0x01};//
const uint8_t hui[4] = {0x39,0x09,0x0f};//口



//************ START信号*******************************
void FD650_START(TM1650_STRUCT *tm1650)
{
	
	tm1650 ->clk(GPIO_PIN_SET);//tm1650 ->clk(GPIO_PIN_SET);;//时钟
	tm1650 ->dio(GPIO_PIN_SET);//tm1650 ->dio(GPIO_PIN_SET);;
	tm1650 ->dio(GPIO_PIN_RESET); ;
	tm1650 ->clk(GPIO_PIN_RESET); ;
}
//************ STOP信号********************************
void FD650_STOP(TM1650_STRUCT *tm1650)
{
	tm1650 ->clk(GPIO_PIN_SET); ;
	tm1650 ->dio(GPIO_PIN_RESET); ;
	tm1650 ->dio(GPIO_PIN_SET); ;
}
//************写1个字节给TM1650***************************
void FD650_write( TM1650_STRUCT *tm1650,uint8_t data1)
{
 	uint8_t i;
	for(i=0;i<8;i++)
	{
		if(data1&0x80)tm1650 ->dio(GPIO_PIN_SET); 
		else  tm1650 ->dio(GPIO_PIN_RESET); 		
		tm1650 ->clk(GPIO_PIN_SET); ;	  //上升沿
		data1<<=1;
		tm1650 ->clk(GPIO_PIN_RESET);  
	}
		tm1650 ->dio(GPIO_PIN_SET); ;
		tm1650 ->clk(GPIO_PIN_SET); ;
		tm1650 ->clk(GPIO_PIN_RESET);
}
//*************读按键**************************************
uint8_t FD650_read_button(TM1650_STRUCT *tm1650)
{
	uint8_t dat, i;
	tm1650 ->dio(GPIO_PIN_SET); 
	dat=0;
	for(i=0;i<8;i++)
	{
		tm1650 ->clk(GPIO_PIN_SET); 
		dat<<=1;  //左移一次
		 
		if(tm1650 ->read_dio())
		dat++; //如果为1，就+1变成1	
		tm1650 ->clk(GPIO_PIN_RESET); 		  //下降沿 
	}
	tm1650 ->dio(GPIO_PIN_SET);
	tm1650 ->clk(GPIO_PIN_SET); 
	tm1650 ->clk(GPIO_PIN_RESET); 
	return dat ;

} 
//***********读按键命令*************************************
uint8_t FD650_READ(TM1650_STRUCT *tm1650)
{
	uint8_t key;
	FD650_START(tm1650);
	FD650_write(tm1650,0x49);//读按键指令	
	key=FD650_read_button(tm1650);
	FD650_STOP(tm1650);
	return key;
} 
//***********发送命令信号***********************************
void FD650_send(TM1650_STRUCT *tm1650,uint8_t date1,uint8_t date2)
{
 	FD650_START(tm1650);
	FD650_write(tm1650,date1);
	FD650_write(tm1650,date2);
	FD650_STOP(tm1650);
}

void Tm1650_show_ISR(TM1650_STRUCT *tm1650)
{
	tm1650->times_100ms++;
	
	if(tm1650->blink_en && (tm1650->blink_dir ==0 ))
	{

		tm1650->word =(uint8_t *)CODE_Clean ;
		tm1650->disp_value[0] = *tm1650->word ++;
		tm1650->disp_value[1] = *tm1650->word ++;
		tm1650->disp_value[2] = *tm1650->word ;	
	}

	else
	{
		if(tm1650->Is_num ==YES)//整数显示
		{
			if(tm1650->num>=0)
			{		 
				tm1650->disp_value[0] = CODE00[ tm1650->num %10];
				tm1650->disp_value[1] = CODE00[  tm1650->num /10%10];
				tm1650->disp_value[2] =CODE00[  tm1650->num /100%10];
			}
			else//负数显示
			{
				
				tm1650->num = __fabs (tm1650->num);
				tm1650->disp_value[0] = CODE00[ tm1650->num %10];
				tm1650->disp_value[1]= CODE00[ tm1650->num /10%10];
				tm1650->disp_value[2] =0x40;
			}
		}
		else//文字显示
		{
			tm1650->word =  (uint8_t *)tm1650->word;
			tm1650->disp_value[0] = *tm1650->word ++;
			tm1650->disp_value[1] = *tm1650->word ++;
			tm1650->disp_value[2] = *tm1650->word ;	
		}
		
	}
	if(tm1650->times_100ms%3==0)
	{
		if(tm1650->blink_dir==0)
			tm1650->blink_dir=1;
		else
			tm1650->blink_dir=0;
	}
	
	

	if(tm1650->bottom_dot_en )
	{
		tm1650->disp_value[0] |=0x80;
	}
	else
	{
		if(tm1650->dot_run_en )
		{
			if(tm1650->times_100ms %2==0)
			{
				if((tm1650->dot_run_bit++)>2)
				tm1650->dot_run_bit=0;
			}
			tm1650->disp_value [tm1650->dot_run_bit]|=0x80;
		}
	}

		
	FD650_send(tm1650,0X68,tm1650->disp_value[0] ); //GID1
	FD650_send(tm1650,0X6A,tm1650->disp_value[1]);  //GID2
	FD650_send(tm1650,0X6C,tm1650->disp_value[2]);  //GID3
	
}









 






void tm1650_dio_1(GPIO_PinState en)
{
	HAL_GPIO_WritePin(TM1651_DIO_GPIO_Port,TM1651_DIO_Pin,en);
}


void tm1650_clk_1(GPIO_PinState en)
{
	HAL_GPIO_WritePin(TM1651_CLK_GPIO_Port,TM1651_CLK_Pin,en);
}


uint8_t tm1650_read_dio_1(void)
{
	HAL_GPIO_WritePin(TM1651_DIO_GPIO_Port,TM1651_DIO_Pin,GPIO_PIN_SET );
	return  HAL_GPIO_ReadPin(TM1651_DIO_GPIO_Port ,TM1651_DIO_Pin);
}



TM1650_STRUCT tm1650_1;

void Tm1650_1_init(void)
{
 
	tm1650_1.clk = tm1650_clk_1;
	tm1650_1 .dio = tm1650_dio_1;
	tm1650_1 .read_dio = tm1650_read_dio_1;	
	
	tm1650_1.times_100ms=0;
	tm1650_1 .blink_dir =0;
	tm1650_1 .dot_run_bit=0;
	
	//函数
	FD650_send(&tm1650_1,0x48,0x11);//设定8段显示，设定显示亮度0xab;a(1-7-0亮度依次递增)
}




/************Number Display************/
uint16_t  TM1650_BreathLightPWM(void)
{
  static uint16_t led0pwmval=0;
	static uint8_t dir=1;	 
	if(dir)led0pwmval++;

	else --led0pwmval;

	if(led0pwmval==4)
	{
		dir=0;
	}
	if(led0pwmval==0)dir=1;

	return   led0pwmval;
}






 

void Tm1650_1_show_ISR(void)
{
	Tm1650_show_ISR(&tm1650_1);
}











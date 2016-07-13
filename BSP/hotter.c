//version V1.0
//verify date:2016/6/28
#include "hotter.h"

/* USER CODE BEGIN Includes */
 #include "adc.h"
 #include "../tool/adc_filter.h"
/* USER CODE END Includes */
 
/* USER CODE BEGIN PV */

static uint16_t solder_adc_array[128]; 

/* USER CODE END PV */

 
/* USER CODE BEGIN EV */
HOTER_CTRL_TypeDef hotter1321 ;
/* USER CODE END EV */

/*
 get adc
*/

uint16_t  Get_hotter1321_adc (void)
{
  uint16_t adc;
	HAL_ADC_Start(&hadc) ;
	HAL_ADC_PollForConversion(&hadc,10);  
	adc = HAL_ADC_GetValue(&hadc);
  return adc;
}

void  Filter_hotter1321_adc_ISR (void)
{
	hotter1321.real_adc  = avg_filter (Get_hotter1321_adc(),solder_adc_array) ;
}

void  Hotter1321_realTemp_ISR (void)
{
	hotter1321.real_temperature   = hotter1321 .adc2temp  (hotter1321.real_adc) ;
}





/*
 heated
*/
void Heat_hotter1321(BOOL  en)
{
  if(en)// heated
  {
    HAL_GPIO_WritePin(SolderHOT_GPIO_Port,SolderHOT_Pin,GPIO_PIN_RESET);
  }
  else // not heated
  {
    HAL_GPIO_WritePin(SolderHOT_GPIO_Port,SolderHOT_Pin,GPIO_PIN_SET);
  }
}
uint16_t Hotter1321Adc2temp(uint16_t adc)
{
	uint16_t temp ;
  temp = adc *0.3 + 30;
  return  temp;
}

/*
ISR
*/
/*
大于20ms 执行一次即可。在此程序中选 100ms
也不用担心轮回后出现的相等，因为100ms内最多只能中断响应5次
para: Is_power_on,hz50_count
*/
void Hotter1321_power_on_scan_ISR(void)
{
	static uint8_t data_pre =0 ;
	
	
	
	
	if(hotter1321.hz50_count == data_pre)
	{	
		
		
		
		hotter1321.Is_power_on = NO;
	}
	else
	{
		hotter1321.Is_power_on = YES;
		data_pre = hotter1321.hz50_count;
	}
	
	
}

void Hotter1321_heated_time_count_ISR(void)
{
	hotter1321 .heated_time_count ++;
}


void 		Hotter1321WorkingState_ISR(void )
{
 #define temp_distinguish_dif 5 
  int16_t minus  ;
	minus =  hotter1321 .real_temperature -hotter1321 .target_temperature ;  
	switch(hotter1321 .work_state )
	{
		case 1: //加热
			if ( minus >0 )
			{
				 hotter1321 .work_state =2;
			
			}
			break ;

		case 2 : //过热

			if (  minus <=0 )
			{
				
				 hotter1321 .work_state =3;
			}
			break ;
			
		case 0: 	//停止加热
			if (  minus <=0 )
			{

				hotter1321 .work_state  = 3;
			}
			break ;
			
		case 3 ://平衡
			if (  minus > temp_distinguish_dif )
				hotter1321 .work_state  = 0 ; 
			if (  minus < -temp_distinguish_dif )
				hotter1321 .work_state  = 1 ; 	
			break;
		
		default :
			break ;
	}
}




/* USER CODE BEGIN EFP */

void Hotter1321_init(void)
{
	hotter1321.Lmax =480;
	hotter1321.Lmin =200,
	hotter1321.Cmin =100;
	hotter1321.Cmax =600;
	hotter1321.real_adc = 0;
	hotter1321.real_temperature = 25;
//	hotter1321.target_temperature = 300;
	hotter1321.adjust_temperature = 0;
	hotter1321.reset_position_time = 0;
	hotter1321.go_sleep_time = 0;
	hotter1321 .sensor_err_adc = 2300;
	hotter1321.power_off_time = 0;
	hotter1321.sensor_err =0;
	hotter1321.hotter_err =0;
	hotter1321.work_state =0;
	hotter1321.Hoter_alarm_update_flag = 0;
	hotter1321.Is_reset_position =0;
	hotter1321.Is_power_on =NO;
	hotter1321.Is_supply_eletric =0;
	hotter1321.heat_en  = Heat_hotter1321;

	hotter1321 .adc2temp =Hotter1321Adc2temp;
	hotter1321 .adc_reflash_ISR = Filter_hotter1321_adc_ISR;
	hotter1321 .power_on_scan_ISR =Hotter1321_power_on_scan_ISR;
}

/* USER CODE END EFP */







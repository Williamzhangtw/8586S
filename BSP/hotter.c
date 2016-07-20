//verify date:2016/7/16pm
#include "hotter.h"

/* USER CODE BEGIN Includes */
 #include "adc.h"
 #include "../tool/adc_filter.h"
 #include "../tool/flash.h"


void Hotter_heated_time_count_ISR(HOTER_CTRL_TypeDef * hotter)
{
//	hotter->heated_times ++;
}



void HotterWorkingState_ISR(HOTER_CTRL_TypeDef * hotter)
{

	hotter->minus_now =  hotter->real_temperature -hotter->target_temperature ;  
	switch(hotter->work_state )
	{
		case HEATTING : //加热
			if ( hotter->minus_now  >=0 )
			{
				 hotter->work_state =OVER_HEAT ;
			
			}
			break ;

		case OVER_HEAT  : //过热

			if (  hotter->minus_now  <0 )
			{
				
				 hotter->work_state =TEMP_BALANCE ;
			}
			break ;
			
		case NOT_HEAT : 	//停止加热
			if (  hotter->minus_now  <=0 )
			{

				hotter->work_state  = TEMP_BALANCE ;
			}
			break ;
			
			
		case TEMP_BALANCE  ://平衡
			if (  hotter->minus_now  > hotter->temp_distinguish_dif)
				hotter->work_state  = NOT_HEAT  ; 
			if (  hotter->minus_now  < -hotter->temp_distinguish_dif )
				hotter->work_state  = HEATTING  ; 	
			break;
		
		default :
			break ;
	}
}





//hotter1321
static uint16_t solder_adc_array[128]; 
HOTER_CTRL_TypeDef hotter1321 ;
/*heated
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

/* get adc
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
	hotter1321.real_temperature   = hotter1321.real_adc *hotter1321.Ks+hotter1321 .Bs-hotter_flash.adjust_temperature ; 
}



void Hotter1321_init(void)
{

	hotter1321 .Ks = 0.37;
	hotter1321 .Bs =-220;
	hotter1321.Lmax =480;
	hotter1321.Lmin =0,
	hotter1321.Cmin =-99;
	hotter1321.Cmax =99;
	hotter1321.real_adc = 0;
	hotter1321.real_temperature = 25;
//	hotter1321.target_temperature = 300;
//	hotter1321.adjust_temperature = 0;
	hotter1321.hz50_count =0;
	hotter1321 .sensor_err_adc = 2300;
//	hotter1321 .heated_times =0;
	
	hotter1321.sensor_err =0;
	hotter1321.hotter_err =0;
	hotter1321.work_state =NOT_HEAT ;

	hotter1321.Is_power_on =NO;
	hotter1321 .minus_pre =0;
	hotter1321 .minus_now =0;
	hotter1321 .temp_distinguish_dif =5;
	hotter1321.heat_en  = Heat_hotter1321;

//	hotter1321 .adc2temp =Hotter1321Adc2temp;
//	hotter1321 .adc_reflash_ISR = Filter_hotter1321_adc_ISR;
//	hotter1321 .power_on_scan_ISR =Hotter1321_power_on_scan_ISR;


//开机执行函数
	hotter1321.heat_en (DISABLE );
}

void Hotter1321WorkingState_ISR(void )
{
	HotterWorkingState_ISR(&hotter1321 );
}

void Hotter1321_heated_time_count_ISR(void)
{
	Hotter_heated_time_count_ISR(&hotter1321 );
}

/*ISR 大于20ms 执行一次即可。在此程序中选 100ms也不用担心轮回后出现的相等，因为100ms内最多只能中断响应5次
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



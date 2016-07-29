//verify date:2016/7/28pm
#include "hotter.h"

/* USER CODE BEGIN Includes */
 #include "adc.h"
 #include "../tool/adc_filter.h"
 #include "../tool/flash.h"





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







void HotterRealTemp_ISR(HOTER_CTRL_TypeDef * hotter)
{
		hotter->real_temperature   = hotter->real_adc *hotter->Ks+hotter->Bs- hotter ->adjust_temperature ; 
}
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






void Hotter1321_realTemp_ISR (void)
{
	HotterRealTemp_ISR(&hotter1321);
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


uint16_t  Get_hotter1321_adc (void)
{
	uint16_t adc;
	HAL_ADC_Start(&hadc) ;
	HAL_ADC_PollForConversion(&hadc,10);  
	adc = HAL_ADC_GetValue(&hadc);
	return adc;
}
void Filter_hotter1321_adc_ISR (void)
{
	hotter1321.real_adc  = AvgFilter (&avg_1 , Get_hotter1321_adc());
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


HOTER_CTRL_TypeDef hotterK ;
/*heated
*/
void Heat_hotterK(BOOL  en)
{
  if(en)// heated
  {
    HAL_GPIO_WritePin(AirHOT_GPIO_Port,AirHOT_Pin,GPIO_PIN_RESET);
  }
  else // not heated
  {
    HAL_GPIO_WritePin(AirHOT_GPIO_Port,AirHOT_Pin,GPIO_PIN_SET);
  }
}

/* get adc
*/






void HotterK_realTemp_ISR (void)
{
	HotterRealTemp_ISR(&hotterK);
}


void HotterK_init(void)
{

	hotterK .Ks = 0.253;
	hotterK .Bs =40;
	hotterK.Lmax =480;
	hotterK.Lmin =0,
	hotterK.Cmin =-99;
	hotterK.Cmax =99;
	hotterK.real_adc = 0;
	hotterK.real_temperature = 25;
	
	hotterK.target_temperature = 300;
	hotterK.adjust_temperature = 0;
	
	hotterK.hz50_count =0;
	hotterK .sensor_err_adc = 2300;
//	hotter1321 .heated_times =0;
	
	hotterK.sensor_err =0;
	hotterK.hotter_err =0;
	hotterK.work_state =NOT_HEAT ;
	hotterK.Is_reset_position = 0;
	hotterK.Is_power_on =NO;
	hotterK .minus_pre =0;
	hotterK .minus_now =0;
	hotterK .temp_distinguish_dif =5;
	hotterK.heat_en  = Heat_hotterK;

//	hotter1321 .adc2temp =Hotter1321Adc2temp;
//	hotter1321 .adc_reflash_ISR = Filter_hotter1321_adc_ISR;
//	hotter1321 .power_on_scan_ISR =Hotter1321_power_on_scan_ISR;


//开机执行函数
	hotterK.heat_en (DISABLE );
}

void HotterKWorkingState_ISR(void )
{
	HotterWorkingState_ISR(&hotterK );
}


uint16_t  Get_hotterK_adc (void)
{
	uint16_t adc;
	HAL_ADC_Start(&hadc) ;
	HAL_ADC_PollForConversion(&hadc,10);  
	adc = HAL_ADC_GetValue(&hadc);
	return adc;
}
void Filter_hotterK_adc_ISR (void)
{
	hotterK.real_adc  = AvgFilter (&avg_1 , Get_hotterK_adc());
}

/*ISR 大于20ms 执行一次即可。在此程序中选 100ms也不用担心轮回后出现的相等，因为100ms内最多只能中断响应5次
*/
void HotterK_power_on_scan_ISR(void)
{
	static uint8_t data_pre =0 ;
	if(hotterK.hz50_count == data_pre)
	{		
		hotterK.Is_power_on = NO;
	}
	else
	{
		hotterK.Is_power_on = YES;
		data_pre = hotterK.hz50_count;
	}
}





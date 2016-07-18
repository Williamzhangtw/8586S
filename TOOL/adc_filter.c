//2016-7-15

#include "adc_filter.h"
 
// #define Num  128 //采集80个数 去平均，然后传个filterjj  10个数去中值

uint16_t avg_filter( uint16_t adc_value,uint16_t *array)
{
  uint32_t avg_adc= 0;
  static uint16_t count =0;
   
  if ( count++ <128)
  {    
    *(array+count)  = adc_value ;
  }
  else 
  {
    count = 0 ;
  }

	
  for (uint8_t i=0; i<128; i++)
  {
    avg_adc += *(array+i );
  }
  avg_adc >>=   7;
  return avg_adc;  
}


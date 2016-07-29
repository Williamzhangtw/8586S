//2016-7-28

#include "adc_filter.h"
 
// #define Num  128 //采集80个数 去平均，然后传个filterjj  10个数去中值




uint16_t AvgFilter(AVG_FILTER_Struct *avg,uint16_t value)
{
   
  uint32_t sum =0;
	avg->count++ ;
  if ( avg->count >= avg->num)
    avg->count = 0 ;
 
  *(avg->array+avg->count)  =  value ;
	

   for (uint16_t i=0; i< avg->num; i++)
  {
    sum += *(avg->array+i);
  }
  avg-> f_value =  sum/avg->num;
  return  avg-> f_value;  
}



uint16_t avg_array_1[128];
AVG_FILTER_Struct avg_1;
void Avg_1_Init(void)
{
	avg_1 .num =128;
	avg_1.array = avg_array_1;
	avg_1 .count =0;
	avg_1 .f_value = 0;
}

#ifndef __ADC_FILTER_H
#define __ADC_FILTER_H
#include "stm32f0xx_hal.h"


//#define Num  128 //采集80个数 去平均，然后传个filterjj  10个数去中值

////变量


//extern uint8_t fan_dc_adc_array_index ;

////数组


//extern uint16_t fan_dc_adc_array[Num]; 



////函数
typedef struct 
{
	uint16_t num;
	uint16_t count;
	uint16_t *array;
	uint16_t f_value;
}
AVG_FILTER_Struct;
extern uint16_t avg_filter( uint16_t adc_original_value,uint16_t *array);

#endif

extern uint16_t AvgFilter(AVG_FILTER_Struct *avg,uint16_t value);
//extern uint16_t avg_array_1[128];
extern AVG_FILTER_Struct avg_1;
extern void Avg_1_Init(void);

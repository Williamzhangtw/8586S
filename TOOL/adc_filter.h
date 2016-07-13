#ifndef __ADC_FILTER_H
#define __ADC_FILTER_H
#include "stm32f0xx_hal.h"


//#define Num  128 //采集80个数 去平均，然后传个filterjj  10个数去中值

////变量


//extern uint8_t fan_dc_adc_array_index ;

////数组


//extern uint16_t fan_dc_adc_array[Num]; 



////函数
//extern uint16_t  filter(void);
extern uint16_t avg_filter( uint16_t adc_original_value,uint16_t *array);

#endif

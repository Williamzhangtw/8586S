
#include "stm32f0xx_hal.h"
#ifndef __ROTARY_H
#define __ROTARY_H
//Read_ROTARY_A 带中断
/********************GPIO**********************/
//#define Read_ROTARY_A  HAL_GPIO_ReadPin(ROTARY_A_GPIO_Port ,ROTARY_A_Pin)
//#define Read_ROTARY_B  HAL_GPIO_ReadPin(ROTARY_B_GPIO_Port ,ROTARY_B_Pin)




typedef enum {
	no_direction = 0x00 ,
	Spin_left = 0x01 ,
	Spin_right = 0x02 ,
}Spin_direction_enum;


typedef struct 
{
	uint8_t (*read_rotary_a)(void);
	uint8_t (*read_rotary_b)(void);
	uint8_t Spin_direction;
	uint8_t  elec_leval_a_pre;
	uint8_t  elec_leval_b_pre;
	uint8_t  elec_leval_a_now;
	uint8_t  elec_leval_b_now;
	uint8_t  temple;
	uint16_t times_1ms;
	uint8_t  N;
	
}R0TARY_CTRL_TypeDef;


extern void Rotary_1_init(void);
extern void Rotary_1_scan_ISR(void);
extern R0TARY_CTRL_TypeDef rotary_1;

#endif 

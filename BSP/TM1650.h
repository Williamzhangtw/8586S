#include "stm32f0xx_hal.h"
#ifndef __tm1650_H
#define __tm1650_H


//硬件描述
//TM1651_CLK:PULLUP;INPUT
//TM1651_DIO:PULLUP;INPUT
extern const uint8_t CODE00[10];
extern const uint8_t  CODE_Clean[4];
extern const uint8_t  CODE01[4];
extern const uint8_t  S_E[4];
extern const uint8_t  H_E[4];
extern const uint8_t  OFF[4];
extern const uint8_t  CAL[4];
extern const uint8_t  MENU01[4];
extern const uint8_t  MENU02[4];
extern const uint8_t  MENU03[4];
extern const uint8_t  MENU04[4];
extern const uint8_t  MENU05[4];
extern const uint8_t  MENU06[4];
extern const uint8_t  MENU07[4];
extern const uint8_t  MENU08[4];
extern const uint8_t  MENU09[4];
extern const uint8_t  MENU10[4];
extern const uint8_t  _1_[4];
extern const uint8_t  _2_[4];
extern const uint8_t _888[4] ;
extern const uint8_t xianfu[4] ;
extern const uint8_t CODE_Clean_Dot[4] ;





typedef struct 
{
	void (*clk)(GPIO_PinState);
	void (*dio)(GPIO_PinState);
	uint8_t (*read_dio)(void);

	uint8_t Is_num;
	int16_t num;
	uint8_t *word;
	
	uint8_t times_100ms;
	
	uint8_t dot_run_en;
	uint8_t bottom_dot_en;
	uint8_t blink_en;
	
	
	
	uint8_t disp_value[3];
	uint8_t blink_dir;
	uint8_t dot_run_bit;
	
	
}TM1650_STRUCT;









extern TM1650_STRUCT tm1650_1;
extern void Tm1650_1_show_ISR(void);
extern void Tm1650_1_init(void);

#endif



/***************************************************************
          CUBE application
0.<Sch>TM1651_DIO,must have a pull-up resistor(10K).   TM1651_CLK can use the mcu pull-up or  a pull-up resistor(10K).  the fact using mcu to pull-up doesn't work
1.<resources needed> 2GPIO 1st(TM1651_CLK)->OUTPUT With pull-up;2nd(TM1651_DIO)->OUTPUT With OD; 
  NOTE:
2.<CUBE SETTING> 
  PA3->①OUTPUT5:②With OD;
  PA4->①OUTPUT;②With pull-up;
3.<Add code> 
①add the code to the main.c what else needed;
#include "../drivers/bsp/tm1651.h"
②add the code below  to the mian.c 
 disp(0xc2,2);
 if use the key dective 
 put   
 tm1651_key_process();
 into the while {}

........The problem is working ......
attion:
IN the output with OD mode, If you need to read the volatage level,you need to do this first, 
 HAL_GPIO_WritePin(TM1651_DIO_Port ,TM1651_DIO_Pin,GPIO_PIN_SET) ;
*******************************************************************/
#include "stm32f0xx_hal.h"
#ifndef __tm1650_H
#define __tm1650_H


/********************定义控制端口**********************/
 //两个管脚
// 
//#define CLK_H HAL_GPIO_WritePin(TM1651_CLK_GPIO_Port,TM1651_CLK_Pin,GPIO_PIN_SET)
//#define CLK_L HAL_GPIO_WritePin(TM1651_CLK_GPIO_Port,TM1651_CLK_Pin,GPIO_PIN_RESET)

//#define DIO_H HAL_GPIO_WritePin(TM1651_DIO_GPIO_Port,TM1651_DIO_Pin,GPIO_PIN_SET)
//#define DIO_L HAL_GPIO_WritePin(TM1651_DIO_GPIO_Port,TM1651_DIO_Pin,GPIO_PIN_RESET)


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






typedef struct 
{
	void (*clk)(GPIO_PinState);
	void (*dio)(GPIO_PinState);
	void (*init)(void);
	uint8_t (*read_dio)(void);
	
	uint8_t disp_type;
	uint16_t num;
const	uint8_t *word;
	
}TM1650_STRUCT;


typedef enum
{
	 num,
	word,
	numblink,
	wordblink,
	numrun,
	dot,
	
	
}TM1650_DispType_ENUM;






extern TM1650_STRUCT tm1650_1;
extern void Tm1650_1_show_ISR(void);
extern void Tm1650_1_init(void);

#endif


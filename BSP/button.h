#include "stm32f0xx_hal.h"
#ifndef __BUTTON_H
#define __BUTTON_H

// GPIO PIN SETTING//
/*GPIO_PIN:key_SET_PIN   PIN_MODE:GPIO_INPUT WITH PULL UP*/


//#define KEY_SET_PIN		!HAL_GPIO_ReadPin(SET_KEY_GPIO_Port ,SET_KEY_Pin) //1表示按下，0表示松开



typedef struct 
{
	uint8_t (*read_button)(void);
	uint8_t state_pre;
	uint8_t state_now;
	uint8_t Is_press;
	uint8_t Is_click;
	uint8_t Is_support_continue_press;
	uint8_t Is_support_click;
	uint8_t Is_support_continue_click;
	uint16_t continue_press_time;//continue press time count, second per uint;
	uint16_t time_count;
	uint8_t continue_click_count;//
}BUTTON_CTRL_TypeDef;



//接口函数
/*
函数功能：按键扫描，更新按键的所有状态信息
函数使用示例：放在10ms的中断里面就实现了每个10ms更新按键数据结构体
*/
extern void BUTTON_1_ISR(void);

//初始化按键，让按键没有任何存储信息
extern void Button_1_init(void);

//使用全局按键变量，在使用中直接使用阅读里面的信息，因为所有的数据都在中断中被更新了。
extern BUTTON_CTRL_TypeDef button_1;



#endif

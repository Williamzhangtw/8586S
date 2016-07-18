#include "stm32f0xx_hal.h"
#ifndef __BUTTON_H
#define __BUTTON_H

//硬件描述
//  KEY_SET_PIN		:mcu上拉；输入模式



typedef struct 
{
	uint8_t (*read_button)(void);
	uint8_t state_pre;
	uint8_t state_now;
	uint8_t Is_press;
	uint8_t Is_click;
	uint8_t Is_support_click;
	uint8_t click_dir;
//	uint16_t continue_press_time;//
	uint16_t times_10ms;

}BUTTON_CTRL_TypeDef;







//使用全局按键变量，在使用中直接使用阅读里面的信息，因为所有的数据都在中断中被更新了。
extern BUTTON_CTRL_TypeDef button_1;
//初始化按键，让按键没有任何存储信息
extern void Button_1_init(void);
//接口函数
//函数功能：按键扫描，更新按键的所有状态信息
extern void BUTTON_1_ISR(void);

#endif

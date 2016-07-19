#include "stm32f0xx_hal.h"
#ifndef _HOTTERCTRL_H
#define _HOTTERCTRL_H

#include "../bsp/hotter.h"
#include "../bsp/button.h"
#include "../bsp/rotary.h"
#include "../bsp/tm1650.h"

typedef   enum 
{
    TEMP_IDLE,
	TEMP_TARGET_SHOW,
    TEMP_CTRL,
    TEMP_TARGET_SET,
    TEMP_ALARM,
	TEMP_ADJUST_WARNING,
    TEMP_ADJUST ,
} HOTTER_STATUS_ENUM;

typedef struct 
{
	HOTTER_STATUS_ENUM state;
	R0TARY_CTRL_TypeDef *rotary;
	BUTTON_CTRL_TypeDef *button;
	TM1650_STRUCT *tm1650;
	HOTER_CTRL_TypeDef *hotter;
}HOTTER_CTRL_Typedef;

 
extern HOTTER_CTRL_Typedef solder1321; 
extern void HotterCtrl(HOTTER_CTRL_Typedef *hotterCtrl) ;
extern void solder1321_init(void);
#endif

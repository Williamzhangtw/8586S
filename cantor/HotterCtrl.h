#include "stm32f0xx_hal.h"
#ifndef _HOTTERCTRL_H
#define _HOTTERCTRL_H

#include "../bsp/hotter.h"
#include "../bsp/button.h"
#include "../bsp/rotary.h"
#include "../bsp/tm1650.h"

typedef   enum 
{
	TEMP_CHECK_READY,
    TEMP_CHECK,
	
	TEMP_IDLE_READY,
    TEMP_IDLE,
	
	TEMP_TARGET_SHOW_READY,
	TEMP_TARGET_SHOW, 
	
	TEMP_CTRL_READY,
    TEMP_CTRL,
	
	TEMP_TARGET_SET_READY,
    TEMP_TARGET_SET,
	
	TEMP_ALARM_READY,
    TEMP_ALARM,
	
	TEMP_ADJUST_WARNING_READY,
	TEMP_ADJUST_WARNING,
	
	TEMP_ADJUST_READY ,
    TEMP_ADJUST ,
	
	TEMP_RESET_POSITION_READY,
	TEMP_RESET_POSITION,
	
} HOTTER_STATUS_ENUM;

typedef struct 
{
	HOTTER_STATUS_ENUM state;
	R0TARY_CTRL_TypeDef *rotary;
	BUTTON_CTRL_TypeDef *button;
	TM1650_STRUCT *tm1650;
	HOTER_CTRL_TypeDef *hotter;
}HOTTER_CTRL_Typedef;

extern void HotterCtrl(HOTTER_CTRL_Typedef *hotterCtrl) ; 
extern HOTTER_CTRL_Typedef solder1321; 

extern void solder1321_init(void);
extern void Solder1321Ctrl(void);
extern void Solder1321_PowerOn_ISR(void);


extern HOTTER_CTRL_Typedef airK;

extern void FanCtrl_ISR(void);
extern void airK_init(void);
extern void airKCtrl(void);
extern void airK_PowerOn_ISR(void);
#endif

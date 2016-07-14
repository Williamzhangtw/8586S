/*
Author :ztw
verify:2016/07/01   
*/
#include "stm32f0xx_hal.h"
#ifndef _HOTTERCTRL_H
#define _HOTTERCTRL_H

#include "../bsp/hotter.h"
#include "../bsp/button.h"
#include "../bsp/rotary.h"
#include "../bsp/tm1650.h"

typedef   enum 
{
    POWERON ,
    IDLE,
    TempCTRL,
	TargetTempShow,
    TempSET,
    ALARM,
    TempADJUST ,
} HOTTER_STATUS_ENUM;

typedef struct 
{
	HOTTER_STATUS_ENUM state;
	void(*state_trans)(void);
}HOTTER_CTRL_Typedef;

extern void Solder_1_Transformation(void);
extern void Solder_1_poweron_ISR(void);//100ms
extern void solder1_init(void);
#endif

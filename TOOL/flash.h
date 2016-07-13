#ifndef __FLASH_H
#define __FLASH_H
#include "stm32f0xx_hal.h"
#include "../bsp/hotter.h"

#define	PARA_START_ADDR   0x08003C00	//


//-------------------------------------------------


typedef struct 
{
	uint16_t Bs;
	uint16_t target_temperature;
	
}
Hotter_flash__TypeDef;













//±äÁ¿
extern uint16_t data_saved[10];

//º¯Êý
extern uint8_t APP_DataSaveIsError(uint32_t start_address, uint16_t *data,uint16_t data_num);

extern void APP_DataRead(uint32_t  address, uint16_t *data,uint16_t num);


extern void FlshPara_Save(void);
extern void FlshPara_Init(void);





#endif 

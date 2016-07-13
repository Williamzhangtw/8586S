/* 
用完了要清零
author:ztw
file name：rotary
version:ver1.1
verify:20160630
*/
#include "rotary.h"
/* USER CODE BEGIN Includes */
 
 
/* USER CODE END Includes */

uint8_t temple=0;

/* USER CODE BEGIN PV */




/* USER CODE END PV */



R0TARY_CTRL_TypeDef rotary_1 ;

uint8_t read_rotary_a(void)
{
	return HAL_GPIO_ReadPin(ROTARY_A_GPIO_Port ,ROTARY_A_Pin);
}

uint8_t read_rotary_b(void)
{
	return HAL_GPIO_ReadPin(ROTARY_B_GPIO_Port ,ROTARY_B_Pin);
}


/* USER CODE BEGIN EFP */
void Rotary_1_init(void)
{
	rotary_1.read_rotary_a = read_rotary_a;
	rotary_1.read_rotary_b = read_rotary_b;
	rotary_1.Is_enable = ENABLE;
	rotary_1.Is_press = NO;
	rotary_1 .Spin_direction = no_direction ;
}

/*
轮询法思路：
要确保1ms就能执行该函数一次
1.获得两脚的电平，与之前对比，出现不同视为
*/


void Rotary_scan(R0TARY_CTRL_TypeDef *rotary)
{
//	static uint8_t temple=0;
	static uint8_t  N=0;
	uint8_t elec_leval_a_now=0;
	uint8_t elec_leval_b_now=0;
	static uint8_t elec_leval_a_pre =1;
	static uint8_t elec_leval_b_pre=1;
	elec_leval_a_now= rotary->read_rotary_a() ;
	elec_leval_b_now= rotary->read_rotary_b();
		
	if((elec_leval_a_now!= elec_leval_a_pre)||(elec_leval_b_now!=elec_leval_b_pre))
	{
		switch(N)
		{
			case 0:rotary->Spin_direction =no_direction  ;rotary->Is_press = NO;if(elec_leval_a_now)temple|=0x02; temple|=elec_leval_b_now; elec_leval_a_pre = elec_leval_a_now ;elec_leval_b_pre = elec_leval_b_now ;N=1;break;
			case 1:temple <<=2;if(elec_leval_a_now)temple|=0x02; temple|=elec_leval_b_now; elec_leval_a_pre = elec_leval_a_now ;elec_leval_b_pre = elec_leval_b_now ;N=2;break;
			case 2:temple <<=2;if(elec_leval_a_now)temple|=0x02; temple|=elec_leval_b_now; elec_leval_a_pre = elec_leval_a_now ;elec_leval_b_pre = elec_leval_b_now ;N=3;break;
			case 3:temple <<=2;if(elec_leval_a_now)temple|=0x02; temple|=elec_leval_b_now; elec_leval_a_pre = elec_leval_a_now ;elec_leval_b_pre = elec_leval_b_now ;N=4;break;
			case 4: 
				switch(temple)
				{
					case 0Xd2:rotary->Spin_direction =Spin_left ;rotary->Is_press = YES;break;//210--
					case 0x4b:rotary->Spin_direction =Spin_left ;rotary->Is_press = YES;break;//75
					case 0x2d:rotary->Spin_direction =Spin_left ;rotary->Is_press = YES;break;//45--
					case 0xb4:rotary->Spin_direction =Spin_left ;rotary->Is_press = YES;break;//180--
					
					
					case 0xe1:rotary->Spin_direction =Spin_right ;rotary->Is_press = YES;break;//225--
					case 0x87:rotary->Spin_direction =Spin_right ;rotary->Is_press = YES;break;//135--
					case 0x1e:rotary->Spin_direction =Spin_right ;rotary->Is_press = YES;break;//30
					case 0x78:rotary->Spin_direction =Spin_right ;rotary->Is_press = YES;break;//120---
					default :rotary->Spin_direction =no_direction  ;rotary->Is_press = NO;break ;
				}
			temple = 0;N = 0;  break ;
			default :temple = 0;N = 0;  break ;
		}
	}
}

void Rotary_1_scan_ISR(void)
{
	Rotary_scan(&rotary_1);
}


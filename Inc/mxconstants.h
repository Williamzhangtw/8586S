/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MXCONSTANT_H
#define __MXCONSTANT_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

#include <stdint.h>
typedef   char I8 ;
typedef   int  I16 ;

typedef  uint8_t U8 ;
typedef  uint16_t  U16 ;
typedef  uint32_t  U32 ;

typedef  uint8_t BOOL;


#define TRUE  1
#define FALSE 0


#define DIR_UP    1
#define DIR_DOWN 0

//#define ENABLE  1
//#define DISABLE 0

#define YES  1
#define NO 0

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define SolderHOT_Pin GPIO_PIN_0
#define SolderHOT_GPIO_Port GPIOF
#define AirHOT_Pin GPIO_PIN_1
#define AirHOT_GPIO_Port GPIOF
#define Thermal_ADC_Pin GPIO_PIN_0
#define Thermal_ADC_GPIO_Port GPIOA
#define SET_KEY_Pin GPIO_PIN_2
#define SET_KEY_GPIO_Port GPIOA
#define TM1651_DIO_1_Pin GPIO_PIN_3
#define TM1651_DIO_1_GPIO_Port GPIOA
#define TM1651_CLK_1_Pin GPIO_PIN_4
#define TM1651_CLK_1_GPIO_Port GPIOA
#define ROTARY_A_Pin GPIO_PIN_5
#define ROTARY_A_GPIO_Port GPIOA
#define ROTARY_B_Pin GPIO_PIN_6
#define ROTARY_B_GPIO_Port GPIOA
#define AIR_ON_OFF_Pin GPIO_PIN_7
#define AIR_ON_OFF_GPIO_Port GPIOA
#define HZ_50_Pin GPIO_PIN_1
#define HZ_50_GPIO_Port GPIOB
#define TM1651_CLK_2_Pin GPIO_PIN_9
#define TM1651_CLK_2_GPIO_Port GPIOA
#define TM1651_DIO_2_Pin GPIO_PIN_10
#define TM1651_DIO_2_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

#define KEY_ENTER_Pin ROTARY_C_Pin
 
/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MXCONSTANT_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

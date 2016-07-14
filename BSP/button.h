#include "stm32f0xx_hal.h"
#ifndef __BUTTON_H
#define __BUTTON_H

// GPIO PIN SETTING//
/*GPIO_PIN:key_SET_PIN   PIN_MODE:GPIO_INPUT WITH PULL UP*/


//#define KEY_SET_PIN		!HAL_GPIO_ReadPin(SET_KEY_GPIO_Port ,SET_KEY_Pin) //1��ʾ���£�0��ʾ�ɿ�



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



//�ӿں���
/*
�������ܣ�����ɨ�裬���°���������״̬��Ϣ
����ʹ��ʾ��������10ms���ж������ʵ����ÿ��10ms���°������ݽṹ��
*/
extern void BUTTON_1_ISR(void);

//��ʼ���������ð���û���κδ洢��Ϣ
extern void Button_1_init(void);

//ʹ��ȫ�ְ�����������ʹ����ֱ��ʹ���Ķ��������Ϣ����Ϊ���е����ݶ����ж��б������ˡ�
extern BUTTON_CTRL_TypeDef button_1;



#endif

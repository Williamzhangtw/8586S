#include "stm32f0xx_hal.h"
#ifndef __BUTTON_H
#define __BUTTON_H

//Ӳ������
//  KEY_SET_PIN		:mcu����������ģʽ



typedef struct 
{
	uint8_t (*read_button)(void);//�����ܽ�
	uint8_t state_pre;//����֮ǰ��״̬����
	uint8_t state_now;//�������ڵ�״̬����
	
	uint8_t Is_press;//����ȷ��״̬���� YES��ʾ���£�NO��ʾû�а���
	
	uint8_t Is_support_click;//YES ��ʾ֧�ֵ����NO��ʾ��֧�ֵ��
	uint8_t Is_click;//YES��ʾ��������ˣ�NO��ʾû�е��
	uint8_t click_dir;
	
	uint16_t continue_press_times;
	uint8_t continue_press_dir;
	

//	uint16_t times_10ms;

}BUTTON_CTRL_TypeDef;







//ʹ��ȫ�ְ�����������ʹ����ֱ��ʹ���Ķ��������Ϣ����Ϊ���е����ݶ����ж��б������ˡ�
extern BUTTON_CTRL_TypeDef button_1;
//��ʼ���������ð���û���κδ洢��Ϣ
extern void Button_1_init(void);
//�ӿں���
//�������ܣ�����ɨ�裬���°���������״̬��Ϣ
extern void BUTTON_1_ISR(void);

#endif

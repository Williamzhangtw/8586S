#ifndef __MSG_TASK_API_H_
#define __MSG_TASK_API_H_


#include "stm32f0xx_hal.h"




#define  ArrayNum(a)     (sizeof(a)/sizeof(a[0]))
 

/**************delayms_task*****************************/


//1 ������Ϣ
typedef enum          //�����ﶨ��ϵͳ��Ϣ
{

	tm1650_1_msg =0,	
	button_1_msg,
	rotary_1_msg,   
	
	Status_Transformation_msg,
	
	hotter1321_adc_msg,
	hotter1321_poweron_msg,
	hotter1321_realTemp_msg,
	hotter1321_heated_count_msg,
	hotter1321_hotter_state_msg,
	
	hotterctrl_poweron_msg,
}MsgDelaymsID;


//2.���� ��Ϣ������
typedef struct  
{
	uint8_t	id;
	uint8_t    Is_enable;//���Ƹ���Ϣ�����Ƿ�ִ��
	uint8_t    Is_ready;//���Ƹ���Ϣ �Ƿ�ʱ�䵽�� ������ִ����
	uint32_t  time;	
//	void (*funproc)(void*);
	void (*funproc)(void);
}Msg_Delayms;


//��Ϣ�庯��
void Msg_delayms_process(void);
void delaymsTask_ISR(void);
void delaymsTask_CTRL(uint8_t id,uint8_t en);

void Msg_main_process(void);
void mainTask_ISR(void);








extern uint8_t 	hal_1ms_flag;
extern uint8_t  hal_10ms_flag ;
extern uint8_t  hal_100ms_flag ;
extern uint8_t  hal_1s_flag ;

extern void HAL_SYSTICK_Callback(void);
extern BOOL HalTimeDelay (uint8_t *p,uint16_t count,BOOL *data_reset);
extern BOOL HalTimeDelaySimple (uint8_t *p,uint8_t time_count);













#endif

#ifndef __MSG_TASK_API_H_
#define __MSG_TASK_API_H_

#include "stm32f0xx_hal.h"

#define  ArrayNum(a)     (sizeof(a)/sizeof(a[0]))

/**************delayms_task*****************************/

//1 ������Ϣ
typedef enum          //�����ﶨ��ϵͳ��Ϣ
{
	tm1650_1_msg =0,
	tm1650_2_msg,	
	button_1_msg,
	rotary_1_msg,   
	
	Status_Transformation_msg,
	


	
	hotter1321_adc_msg,
	hotter1321_poweron_msg,
	hotter1321_realTemp_msg,
	hotter1321_hotter_state_msg,
	//hotter1321_hotter_power_on_msg,
	
	hotterK_adc_msg,
	hotterK_poweron_msg,
	hotterK_realTemp_msg,
	hotterK_hotter_state_msg,
	
	airK_power_on_msg,
	
		
	FAN_CTRL_MSG,
	
	
}MsgDelaymsID;


////2.���� ��Ϣ������


typedef struct  
{
	uint8_t	id;
	uint8_t    Is_enable;//���Ƹ���Ϣ�����Ƿ�ִ��
	uint8_t    Is_ready;//���Ƹ���Ϣ �Ƿ�ʱ�䵽�� ������ִ����
	uint32_t  time;	
	void (*funproc)(void);
	uint32_t  time_temp;	
//	void (*funproc)(void*);
}Msg_TypeDef;



typedef struct  
{
	Msg_TypeDef *msg;
	Msg_TypeDef *msg_temp;
	uint8_t id_num;
}Task_TypeDef;



//��Ϣ�庯��
void Msg_delayms_process(void);
void TaskISR(Task_TypeDef * task);
extern void TaskCtrl(Task_TypeDef * task,uint16_t id,uint8_t en);
void TaskProcess( Task_TypeDef * task);

//��������Ϣ
typedef enum          //�����ﶨ��ϵͳ��Ϣ
{
	SOLDER1321_MSG,
	AIRK_MSG,
	TASK_SYSTICK_MSG,
}MsgMainID;
void TaskMainkInit(void);



extern Task_TypeDef task_systick ;
extern Task_TypeDef task_main;

void Msg_main_process(void);
void mainTask_ISR(void);
extern void TaskSystickInit(void);
extern void TaskSystickProcess(void);






extern	uint16_t	hal_1ms_flag;
extern 	uint16_t	hal_10ms_flag ;
extern 	uint16_t	hal_100ms_flag ;
extern 	uint16_t	hal_1s_flag ;

extern void HAL_SYSTICK_Callback(void);
extern BOOL HalTimeDelay (uint8_t *p,uint16_t count,BOOL *data_reset);
extern BOOL HalTimeDelaySimple (uint8_t *p,uint8_t time_count);













#endif

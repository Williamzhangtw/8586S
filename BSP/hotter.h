#include "stm32f0xx_hal.h"

#ifndef _HOTER1321_H
#define _HOTER1321_H
//Ӳ������
//SolderHOT_Pin //���ȹܽţ��͵�ƽ���ȣ��ߵ�ƽֹͣ����
//Solder_ADC_Pin  //�ӷ���˿ �ȵ�ż �����˷ŷŴ��������
//HZ_50_Pin //28V������ �������






 typedef struct 
 {
//	 float  Kl;  // pure, K value of linear equation of the sensing head in the air.
//	 int16_t Bl;//
	 float Ks;//with welding head,the  k value of linear equation of the sensing heed with the welding head outside.
	 int16_t Bs;
//	 int16_t Fmid;//magnification of circuit settings
	 int16_t Lmin;//minimum range
	 int16_t Lmax;//max range
	 int16_t Cmin;// adjust temperture min
	 int16_t Cmax;//adjust temperture max
	 int16_t real_adc;
	 int16_t real_temperature;
	 int16_t target_temperature;
	 int16_t adjust_temperature;
	 
	 
	 
	 
//	 uint16_t reset_position_time;//minute per uint 
//	 uint16_t go_sleep_time;
//	 uint16_t power_off_time;
	 uint8_t hz50_count;
	 uint16_t sensor_err_adc;
 	 uint16_t heated_times;
	 uint8_t heated_check_dir;
//	 uint8_t Hoter_alarm_update_flag;
	 uint8_t sensor_err;
	 uint8_t hotter_err;
	 uint8_t work_state;//heating,stop heating,cooling,keep constant temperature
	 
	 uint8_t Is_reset_position;
	 
	 uint8_t Is_power_on;//dev control switch on
//	 uint8_t Is_supply_eletric;// supply the power for heating or cooling
	int16_t temperature_pre;

	 int16_t minus_pre;//real_temperature -.target_temperature ;
	 int16_t minus_now;//real_temperature -.target_temperature ;
	 uint8_t temp_distinguish_dif;
	 
	 
	 
	 void (*heat_en)(uint8_t);//���Ⱥ���

	 
	 
//	 void (*adc_reflash_ISR)(void);//1msִ��һ��
//	 void (*power_on_scan_ISR)(void);//100msִ��һ��
//	 void (*position_reset_scan_ISR)(void);//100msִ��һ��
//	 uint16_t (*adc2temp)(uint16_t);
 }HOTER_CTRL_TypeDef;
 

 typedef enum 
 {
	HEATTING,
	OVER_HEAT,
	NOT_HEAT,
	TEMP_BALANCE,
 }
Hotter_state_enum;
 
 

//user//

extern 	HOTER_CTRL_TypeDef hotter1321;
 
 
extern 	void 	Hotter1321_init	(void); 
extern 	void	Filter_hotter1321_adc_ISR (void);
extern 	void	Hotter1321_power_on_scan_ISR(void); 
extern 	void	Hotter1321_realTemp_ISR (void);
extern 	void 	Hotter1321WorkingState_ISR(void );
 
extern HOTER_CTRL_TypeDef hotterK ;
extern 	void 	HotterK_init	(void); 
extern 	void	Filter_hotterK_adc_ISR (void);
extern 	void	HotterK_power_on_scan_ISR(void); 
extern 	void	HotterK_realTemp_ISR (void);
extern 	void 	HotterKWorkingState_ISR(void );
#endif

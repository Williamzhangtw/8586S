
#include "flash.h"
#include <string.h>
#include <math.h>
//ȫ�ֱ���

uint16_t data_saved[10]={0,0,0,0,0,0,0,0,0,0};

 const Hotter_flash__TypeDef hotter_flash_default ={20,300};



//function:������ 
//����ֵ �洢�Ƿ�������NO��ʾ û�г���yes ��ʾ�쳣
uint8_t APP_DataSaveIsError(uint32_t start_address, uint16_t *data,uint16_t data_num)
{
	
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint8_t return_value = 0;
  uint32_t page_error = 0;
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = start_address;
  EraseInitStruct.NbPages     =1; // FLASH_PAGE_SIZE;
	 if (HAL_FLASHEx_Erase(&EraseInitStruct, &page_error) != HAL_OK)
  {
		return_value = 1;
	}
	for(uint16_t  number_count = 0; number_count <data_num ;number_count ++)
	{

		HAL_FLASH_Program (FLASH_TYPEPROGRAM_HALFWORD, start_address+ (number_count<<1),*(data+number_count));
	}
	
	return return_value;
}

//function�� �����ݣ���һ����ַ��ȡN�����ݡ�
//������address ��ʾ ��ȡ���ĵ�ַ��*data��ʾ ��ŵĵ�ַ��num��ʾ��ŵĸ���
void APP_DataRead(uint32_t address, uint16_t *data,uint16_t num)
{

	for (uint16_t index= 0;index<num ;index++ ) 
	{
		*(data+index)  = *( uint16_t *)(address + (index<<1)  );
	}
	 
}




 


void FlshPara_Save(void)
{
 	FLASH_EraseInitTypeDef EraseInitStruct;

  uint32_t page_error = 0;
	uint32_t   size; 
  Hotter_flash__TypeDef hotter_flash={0};
	
	uint32_t *ptemp  = (uint32_t*)&hotter_flash;
  size = sizeof(ptemp);
	hotter_flash.target_temperature = hotter1321 .target_temperature; 
	hotter_flash.Bs = hotter1321 .Bs; 
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = PARA_START_ADDR;
	EraseInitStruct.NbPages     =1;//;  
	HAL_FLASHEx_Erase(&EraseInitStruct, &page_error);
 
	for(uint16_t  i = 0; i <size ;i +=4)
	{

		HAL_FLASH_Program (FLASH_TYPEPROGRAM_WORD, PARA_START_ADDR+ i,*ptemp++);
	}
 
 	HAL_FLASH_Lock();
}
//----------------------------------------------------
void FlshPara_Init(void)
{
	FLASH_EraseInitTypeDef EraseInitStruct;

  uint32_t page_error = 0;
	uint32_t   size;//size1,
	;	
  Hotter_flash__TypeDef hotter_flash;

	uint32_t *ptemp  = (uint32_t*)&hotter_flash;
  size = sizeof(ptemp);
 	memcpy(&hotter_flash,(HOTER_CTRL_TypeDef *)PARA_START_ADDR,size); 
//	
//	
	if (hotter_flash.Bs ==0xffff)
	{
		HAL_FLASH_Unlock();
		EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = PARA_START_ADDR;
    EraseInitStruct.NbPages     =1;  
	  HAL_FLASHEx_Erase(&EraseInitStruct, &page_error);
		memcpy(&hotter_flash,&hotter_flash_default,size); 
		 
		for(uint8_t  i = 0; i <size ;i+=4)
		{

			HAL_FLASH_Program (FLASH_TYPEPROGRAM_WORD, PARA_START_ADDR+ i,*ptemp++);
		}
		HAL_FLASH_Lock();
		memcpy(&hotter_flash,(HOTER_CTRL_TypeDef *)PARA_START_ADDR,size); 
	}
//	
	hotter1321 .Bs = hotter_flash.Bs;
	hotter1321 .target_temperature = hotter_flash.target_temperature   ;

}


/*-----------------------------------------------
		���������˿����ü�������
	˵��:	�Ѿ����˿����úͰ���ɨ������װ����
	        ���ݳ���ʵ����Ҫ������Ӧ�Ĳ������ɡ�
	ʱ��:	2013��7��22��
------------------------------------------------*/
#include "indkey.h"
#include "stm32f10x.h"
#include "delay.h"

void Key_IO_Init(void)	 //����IO����
{
  GPIO_InitTypeDef IO_Init;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);	
  IO_Init.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_2 | GPIO_Pin_3;				    
  IO_Init.GPIO_Mode = GPIO_Mode_IPU;  //PC5��2��3��������	
  GPIO_Init(GPIOC, &IO_Init);
}
/*-----------------------------------------------
	��������:	�����������
	��������:	�˿�����GPIOx,������GPIO_Pin
	������ֵ:	INT8U���� ����ֵ0,1
------------------------------------------------*/
unsigned char Key_Scan(GPIO_TypeDef* GPIOx,unsigned short int GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_PRESSED)  //���½�ͨ�͵�ƽ
	{
		delayms(3);
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_PRESSED)
		{
			while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_PRESSED);//�ȴ������ͷ�
			return KEY_PRESSED;
		}
		else
			return KEY_UNPRESS;
	}
	else
		return KEY_UNPRESS;
}

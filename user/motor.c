#include "motor.h"
#include "stm32f10x.h"
#include "stdint.h"
#include "pwm.h"
uint16_t LdcVal=4000,LDCValMax;
uint16_t LdcValTmp=0;

/*
		  ���� pwm1 <-> PB0 (Tim3_CH3)			���� pwm3 <-> PB8(Tim4_CH3)
		       pwm2 <-> PB1 (Tim3_CH4)               pwm4 <-> PB9(Tim4_CH4)		  
*/
/*
*********************************************************************************************************
*	�� �� ��: Run
*	����˵��: ǰ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Run(void)
{
	/*����*/
	GPIO_SetBits(GPIOC,GPIO_Pin_6);
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);
	/*����*/
	GPIO_SetBits(GPIOC,GPIO_Pin_8);
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);
}
/*
*********************************************************************************************************
*	�� �� ��: RunLeft
*	����˵��: ��ת����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RunLeft(void)
{
	/*����*/
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);
	/*����*/
	GPIO_SetBits(GPIOC,GPIO_Pin_8);
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);
}
/*
*********************************************************************************************************
*	�� �� ��: RunLeft
*	����˵��: ��ת����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RunRight(void)
{
	/*����*/
	GPIO_SetBits(GPIOC,GPIO_Pin_6);
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);
	/*����*/
	GPIO_ResetBits(GPIOC,GPIO_Pin_8);
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);
}


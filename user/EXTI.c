/*-------------------------------------------------------------------------
       �ⲿIO���ж����ó���
 ˵��: EXTI_Config��������IO��ģʽ,�����һЩע�������Ѿ���ע����˵����
	   NVIC_Config���������ж�����������ȼ���,ע�������Ѿ���ע��˵����
 ����: �жϷ������д��stm32f10x_it.c��
	   �жϷ�������������д,ֻ�ܰ������ĸ��ж��߾�д�ĸ��涨���жϺ����� 
       ������жϺ������뵽startup_stm32f10x_hd.s�ļ���62��Ѱ����Ӧ�����֡�
	   �ж�����stm32f10x.h�ļ���188�п����ҵ������ж����б�,������Ҫѡ��
 ʱ��: 2013��7��22��
--------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "EXTI.H"

void EXTI_Config(void)
{
	EXTI_InitTypeDef EXTI_IO_Init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);
	/* �����ж�IO����Ҫʹ������ʱ�� */
	NVIC_Config();  //�����ж��������ƼĴ���
	/* ���IO���жϲ���һ������,ֻ�����·ֿ����� */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5); //�����ж���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource2); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource3);

	EXTI_IO_Init.EXTI_Line = EXTI_Line5 | EXTI_Line2 | EXTI_Line3; 
	/* ����ж���ⲿ�ж�IO����Ҫ���ÿ���ʹ�� | �������ͳһ���� */
	EXTI_IO_Init.EXTI_Mode = EXTI_Mode_Interrupt;  //�ж�ģʽ
	EXTI_IO_Init.EXTI_Trigger = EXTI_Trigger_Falling;  //�½��ش����ж�
	EXTI_IO_Init.EXTI_LineCmd = ENABLE;		//ʹ���ж�
	EXTI_Init(&EXTI_IO_Init);	 //д������
}

static void NVIC_Config(void)
{
	/* ���Ҫ���ö���ж������Ĵ���ֻ�ֿܷ���������,����һ������ */
	NVIC_InitTypeDef NVICInit;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//��ռ��Ӧ���ȼ���1
	//����PC5�˿�
	/* �ж�����stm32f10x.h�ļ���188�п����ҵ������ж����б�,������Ҫѡ�� */
	NVICInit.NVIC_IRQChannel = EXTI9_5_IRQn;//0-4����IV,5-9����EXTI9_5_IRQn IV,10-15����EXTI15_10_IRQn IV	
	NVICInit.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0
    NVICInit.NVIC_IRQChannelSubPriority = 0;  		 //��Ӧ���ȼ�0
	NVICInit.NVIC_IRQChannelCmd = ENABLE;	         //ʹ���ж�
	NVIC_Init(&NVICInit);                            //д������
	//����PC2�˿�
	NVICInit.NVIC_IRQChannel = EXTI2_IRQn;
	NVICInit.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0
    NVICInit.NVIC_IRQChannelSubPriority = 0;         //��Ӧ���ȼ�0
	NVICInit.NVIC_IRQChannelCmd = ENABLE;	  		 //ʹ���ж�
	NVIC_Init(&NVICInit); 							 //д������
	//����PC3�˿�
	NVICInit.NVIC_IRQChannel = EXTI3_IRQn;
	NVICInit.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0
    NVICInit.NVIC_IRQChannelSubPriority = 0;  		 //��Ӧ���ȼ�0
	NVICInit.NVIC_IRQChannelCmd = ENABLE;	  	 	 //ʹ���ж�
	NVIC_Init(&NVICInit); 							 //д������
}

/*---------------------------------------
			PWM���ó���
----------------------------------------*/
#include "pwm.h"
#include "stm32f10x.h"
/*
*********************************************************************************************************
*	�� �� ��: GPIO_Motor                    
*	����˵��: ���������                        
*	��    ��: ��
*	�� �� ֵ: ��                             
*********************************************************************************************************
*/
void GPIO_Motor(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;           
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOC, &GPIO_InitStructure);       
}
/*
*********************************************************************************************************
*	�� �� ��: GPIO_TimPWM                       ��PB0 <-> TIM3_CH3      ��PB1 <-> TIM3_CH4  
*	����˵��: ������·PWM���Ƶ��                        
*	��    ��: ��
*	�� �� ֵ: ��                             
*********************************************************************************************************
*/
void GPIO_TimPWM(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE);
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;           
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);       
}
/*
*********************************************************************************************************
*	�� �� ��: pwm1
*	����˵��: TIM3���ã�����pwm�����Ƶ��
*	��    ��: pulse
*	�� �� ֵ: ��						
*********************************************************************************************************
*/
void pwm1(uint16_t pulse)
{
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
	 
	 TIM_TimeBaseStructure.TIM_Prescaler = 0;
	 TIM_TimeBaseStructure.TIM_Period = 7200-1;    
	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;					 
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;    //TIM���ϼ���ģʽ
	 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  
	 TIM_ARRPreloadConfig(TIM3, DISABLE);                           //��ֹARRԤװ�ػ�����,Ҳ���Բ�������

	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;              //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //�Ƚ����ʹ��
	 
	 TIM_OCInitStructure.TIM_Pulse =pulse;    
	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	 TIM_OC3Init(TIM3, &TIM_OCInitStructure);                 //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIM3
	 TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);        //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

	 TIM_CtrlPWMOutputs(TIM3,ENABLE);    //����TIM3��PWM���Ϊʹ��
	 TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIM3��ARR�ϵ�Ԥװ�ؼĴ��� 
	 TIM_Cmd(TIM3, ENABLE);              //ʹ��TIM3����
}
/*
*********************************************************************************************************
*	�� �� ��: pwm2
*	����˵��: TIM3���ã�����pwm�����Ƶ��
*	��    ��: pulse
*	�� �� ֵ: ��						
*********************************************************************************************************
*/
void pwm2(uint16_t pulse)
{
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 TIM_OCInitTypeDef  TIM_OCInitStructure;

	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
	 
	 TIM_TimeBaseStructure.TIM_Prescaler = 0;
	 TIM_TimeBaseStructure.TIM_Period = 7200-1;    
	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;					 
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;    //TIM���ϼ���ģʽ
	 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  
	 TIM_ARRPreloadConfig(TIM3, DISABLE);                           //��ֹARRԤװ�ػ�����,Ҳ���Բ�������

	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;              //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //�Ƚ����ʹ��
	 
	 TIM_OCInitStructure.TIM_Pulse =pulse;    
	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	 TIM_OC4Init(TIM3, &TIM_OCInitStructure);                 //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIM3
	 TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);        //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

	 TIM_CtrlPWMOutputs(TIM3,ENABLE);    //����TIM3��PWM���Ϊʹ��
	 TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIM3��ARR�ϵ�Ԥװ�ؼĴ��� 
	 TIM_Cmd(TIM3, ENABLE);              //ʹ��TIM3����
}
/*---------------------------------------
 ��������: ��ʱ��4ͨ��1����PWM��ʱ��
 ��������: ���8MHzʱ�Ӹ�TBCLK,LDC1000��Ҫ8MHzʱ�ӷ�������
----------------------------------------*/
void PWM_TIM4_CH1_Config(unsigned short int Auto_Reload,unsigned short int Clock_Div)
{	
	GPIO_InitTypeDef IO_Init;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	 //ʹ�ܶ�ʱ��4ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //ʹ��AFIOʱ��
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
 	
	IO_Init.GPIO_Pin = GPIO_Pin_6;				    
  	IO_Init.GPIO_Mode = GPIO_Mode_AF_PP;				 //PB6�����������
  	IO_Init.GPIO_Speed = GPIO_Speed_50MHz;	
  	GPIO_Init(GPIOB, &IO_Init);

	TIM_TimeBaseStructure.TIM_Prescaler =  Clock_Div; 			//ʱ��Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseStructure.TIM_Period = Auto_Reload;				//�Զ���װֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	    //ʱ�ӷ�Ƶ1
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode =  TIM_OCMode_PWM2;   //ѡ��ʱ��ģʽ:PWM2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);				 //����ָ���Ĳ�����ʼ������TIM3 OC2
	
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable); //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_SetCompare1(TIM4,4);
	TIM_Cmd(TIM4, ENABLE);
}




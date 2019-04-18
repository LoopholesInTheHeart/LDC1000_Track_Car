#include "duoji.h"
#include "stm32f10x.h"
#include "stdint.h"
/*
*********************************************************************************************************
*	�� �� ��: GPIO_Tim3CH2PWM
*	����˵��: ��ʱ��3ͨ��2��GPIO��ʼ�������ڲ���pwm�����ƶ��
*	��    ��: ��
*	�� �� ֵ: ��                            PA7 <-> TIM3_CH2
*********************************************************************************************************
*/
void GPIO_Tim3CH2PWM(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO , ENABLE);//
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;         //TIM_CH2  GPIOA_Pin_7���
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);        //������
}
/*
*********************************************************************************************************
*	�� �� ��: TIM3_CH2_Configuration
*	����˵��: TIM3���ã�����pwm�����Ƶ��
*	��    ��: pulse
*	�� �� ֵ: ��						Psc=100,Arr=14400-1
*********************************************************************************************************
*/
void TIM3_CH2_Configuration(uint16_t pulse)
{
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 TIM_OCInitTypeDef  TIM_OCInitStructure;

 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //ʹ��TIM3
 
//������Ҫ����50Hzռ�ձ�Ϊ50%��PWM����������ƶ��
//���÷�����
//ϵͳĬ��ʱ��Ϊ72MHz��Ԥ��Ƶ100�Σ��õ�TIM3����ʱ��Ϊ720000Hz
//��������Ϊ14400���ɵ�PWMƵ��Ϊ720000/14400=50Hz
//�������ÿɲο���������÷�����������!
 
 TIM_TimeBaseStructure.TIM_Prescaler = 100;
 TIM_TimeBaseStructure.TIM_Period = 14400-1;    
 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
                     
 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ

 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  
 TIM_ARRPreloadConfig(TIM3, DISABLE);  //��ֹARRԤװ�ػ�����,Ҳ���Բ�������

 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
 
 //ռ�ձ����÷�����
 //ռ�ձ�=(TIM_Pulse+1)/(TIM_Period+1)
 
 TIM_OCInitStructure.TIM_Pulse =pulse; //1600��Ӧ90�ȣ�1200��Ӧ45�� ��900��Ӧ0�� ��600��Ӧ-45�� ��300��Ӧ-90��   
 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;    //�������:TIM����Ƚϼ��Ե�
 TIM_OC2Init(TIM3, &TIM_OCInitStructure);          
 TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);           //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

 TIM_CtrlPWMOutputs(TIM3,ENABLE);      //����TIM3��PWM���Ϊʹ��
 TIM_ARRPreloadConfig(TIM3, ENABLE);   //ʹ��TIM3��ARR�ϵ�Ԥװ�ؼĴ��� 
 TIM_Cmd(TIM3, ENABLE);                //ʹ��TIM3����
}
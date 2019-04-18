/*
*********************************************************************************************************
*	                                 LDC1000ѭ��С��
*	
*     					�õ�3·PWM����TIM4_CH1_pwm���ڲ���8Mhzʱ�ӣ�
*	  					TIM3_CH3_pwm���ڿ���������
*	  					TIM3_CH4_pwm���ڿ����ҵ����
*
*********************************************************************************************************
*/
#include "stm32f10x.h"
#include "usart1.h"
#include "delay.h"
#include "stdio.h"
#include "ldc1000.h"
#include "pwm.h"
#include "motor.h"
#include "oled.h"
#include "timer.h"
/*----------------------------------------
	   ȫ�ֱ����뺯��������
----------------------------------------*/
extern uint8_t RegArray[REG_LEN];
uint16_t x,y,z;
int8_t  i = 0,n=0,m=0,h=0;
int reg21=0,reg22=0,reg2122=0;
char proximtyData[2];
int proximtyDataMIN;
int proximtyDataMAX;
int proximtyDataTEMP;

// float Q=0.0;//����·�̱���
int x1,x2,x3,x4,x5;

unsigned char SecBak;

uint16_t ReadValue(void);
void Ldc1000Test(void);
void Beep_GPIO_Config(void);
// void Lucheng(void);
/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: �������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int main(void)
{
	DelayInit();
	USART1_Config();
	PWM_TIM4_CH1_Config(8,0);	 //���8MHzʱ�Ӹ�TBCLK,LDC1000��Ҫ8MHzʱ�ӷ�������
	delayms(5);
	LDC1000_Init();
  	OLED_Init();			//��ʼ��OLED  
 	OLED_Clear() ; 
 	OLEDShowCHinese(); 	
// 	Encoder_Init();
 	TIM1_Int_Init(499,7199);  
	delayms(5);
	GPIO_TimPWM();
	GPIO_Motor();
 	Run();
 	while(1)
    {	
		if( SecBak!=Sec )
		{
			SecBak=Sec;
			OLED_ShowNum(77,3,Sec,2,16);
			OLED_ShowNum(48,3,Min,2,16);
			OLED_ShowNum(40,6,Q,5,16);
		}
		LdcValTmp=ReadValue();
		printf("ProximityData90=%d\n",LdcValTmp);
		if((8200<LdcValTmp)&&(LdcValTmp<15000))
		{
			pwm1(2100);
			pwm2(5250);
		}
		else if((3000<LdcValTmp)&&(LdcValTmp<8200))
		{
			pwm1(4700);
			pwm2(2100);
		}
		else if(LdcValTmp==8200)
		{
			pwm1(5000);
			pwm2(5000);
		}
		else if(LdcValTmp>=15000)
		{
			delayms(3);
			y=ReadValue();
			if(y<LdcValTmp)
			{    
				GPIO_SetBits(GPIOB,GPIO_Pin_8);
			}
			else
			{
				pwm1(4500);
				pwm2(4500);			
				GPIO_ResetBits(GPIOB,GPIO_Pin_8);
				delayms(200);
				GPIO_SetBits(GPIOB,GPIO_Pin_8);	 
			}
		}
	}			
}
/*
*********************************************************************************************************
*	�� �� ��: ReadValue
*	����˵��: Ldc��ֵ����
*	��    ��: ��
*	�� �� ֵ: reg2122
*********************************************************************************************************
*/
uint16_t ReadValue(void)
{
	SPI_LDC1000_ReadBytes(LDC1000_CMD_PROXLSB,&proximtyData[0],2);
    proximtyDataMAX = ((unsigned char) proximtyData[1]<<8) + proximtyData [0];//�������Ĵ����ж�����ֵ�ϲ�����ͬ
    proximtyDataMIN = proximtyDataMAX;
	for (i=0;i<100;i++)//��100��
    {
		SPI_LDC1000_ReadBytes(LDC1000_CMD_PROXLSB,&proximtyData[0],2);	
		proximtyDataTEMP = ((unsigned char)proximtyData[1]<<8) + proximtyData [0];
		if (proximtyDataTEMP < proximtyDataMIN)//С����С�ľͰ�ֵ������С��
			proximtyDataMIN = proximtyDataTEMP;
		if (proximtyDataTEMP > proximtyDataMAX)//�������ľͰ�ֵ��������
			proximtyDataMAX = proximtyDataTEMP;
		reg2122=reg2122+(int)(proximtyDataTEMP);
    }
	reg2122=reg2122/100;

	return reg2122;	
}


// void Lucheng(void)
// {

// 	x1=(int)Q/10000;
// 	
// 	x2=(int)Q%10000/1000;
// 	
// 	x3=(int)Q%1000/100;
// 	
// 	x4=(int)Q%100/10;
// 	
// 	x5=(int)Q%10;

// }






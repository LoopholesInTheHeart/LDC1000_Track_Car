#include "timer.h"
#include "stm32f10x.h"
#include "usart1.h"
#include "stdio.h"
uint16_t Sec=0,Min=0;

int mSecond=0;
int Q=0;
int Encoder_Left;

void TIM1_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM1��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM1�ж�,��������ж�
 
	//�ж����ȼ�NVIC����
  NVIC_InitStructure.NVIC_IRQChannel =  TIM1_UP_IRQn;;//ָ���ж�Դ
 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);            //ѡ����1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx					 
}



void Encoder_Init(void)
{
	
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
 TIM_ICInitTypeDef TIM_ICInitStructure; 
 GPIO_InitTypeDef GPIO_InitStructure;
 NVIC_InitTypeDef NVIC_InitStructure;
 
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  //ʱ��ʹ��
 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʱ��ʹ��
 
 GPIO_StructInit(&GPIO_InitStructure);
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1;   //����IO��
 GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;   // ����io��Ϊ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 
 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //��ռ���ȼ�2λ�����ȼ�2λ
 NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //TIM3ȫ���ж�
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;  //������ռ���ȼ�
 NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;    //���ô�ռ���ȼ�
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //�ж�ʹ��
 NVIC_Init(&NVIC_InitStructure);

 TIM_DeInit(TIM2);                                //������TIMx�Ĵ�������Ϊȱʡֵ
 TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);  //ÿһ��������ȱʡֵ����
 TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // Noprescaling  //Ԥ��Ƶ��
 TIM_TimeBaseStructure.TIM_Period =ENCODER_TIM_PERIOD-1;  // �趨�������Զ���װֵ
 TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;  //������ʱ�ӷָ�
 TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
 TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);     //��ʼ��TIMx��ʱ�������λ
 
 TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,
                            TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); //����TIMx�������,ʹ��TIM����ģʽ3
 TIM_ICStructInit(&TIM_ICInitStructure);                    //ÿһ��������ȱʡֵ����
 TIM_ICInitStructure.TIM_ICFilter = 10;  //ICx_FILTER;  //TIM_ICFilterѡ������Ƚ��˲���
 TIM_ICInit(TIM2,&TIM_ICInitStructure);  //��ʼ������TIMx
 
 // Clearall pending interrupts
 TIM_ClearFlag(TIM2, TIM_FLAG_Update);        //���TIMx�Ĵ������־λ
 TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);    //ʹ�ܻ���ʧ��ָ����TIM�ж�
  //Resetcounter
 TIM2->CNT = 0;
// ENC_Clear_Speed_Buffer();
 
 TIM_Cmd(TIM2, ENABLE);     //ʹ�ܻ���ʧ��TIMx����
}


/**************************************************************************
�������ܣ���ȡ�����������ݲ�������������ת��
��ڲ�������
����  ֵ����
**************************************************************************/
void readEncoder(void)
{
	 u16 Encoder_L;       //===���ұ��������������

// 		Encoder_R = TIM4 -> CNT;       //===��ȡ��������1����	
// 		TIM4-> CNT=0;                 //===����������  
	  Encoder_L= TIM2 -> CNT;        //===��ȡ��������2����	
	  TIM2 -> CNT=0;	               //===����������
	
// 	  Encoder_Left=Encoder_L; 
	
 		if(Encoder_L>32768)  Encoder_Left=Encoder_L-65000; 
	   else  Encoder_Left=Encoder_L;  
	  //=��������ԭ���ǣ���������0�������65000���¼��������������������ڿ��Ƴ�����ʹ��
// 	  if(Encoder_R>32768)  Encoder_Right=Encoder_R-65000; else  Encoder_Right=Encoder_R;
//    	Encoder_Left=-Encoder_Left;//����ȡ������Ϊ��ƽ��С���������������ת��180�Ȱ�װ�ģ�Ϊ�˱�֤ǰ������ʱ��ı��������ݷ���һ��
}


//��ʱ��1�жϷ������
void   TIM1_UP_IRQHandler(void)   //TIM1�ж�
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		
	{
		  	TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
// 		  	readEncoder(); 
// 				printf("����������� %d  \r\n",Encoder_Left);
		
		  mSecond++ ;	
		   if( mSecond<20 ) 			// �ж��Ƿ����1��
		     return ;
	      mSecond= 0;
		    Sec++ ;

	     if( Sec< 60 ) 				// �ж��Ƿ����60��
		   return ;
	     Sec= 0 ;
	     Min++ ;
	    if( Min< 60 ) 				// �ж��Ƿ����60��
		return ;
	    Min= 0 ;
		Q=0.076*(60*Min+Sec)*1000;
	
	}
}



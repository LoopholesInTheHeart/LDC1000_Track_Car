/*---------------------------------------------------------
		  ADC������������
	˵��: ��ʹ��IO��Ϊģ������,��������DMAͨ��,ע������һ��
	      DMAʹ�õ�ȫ�ֱ���short����,Ŀǰ���õ�����PC1��AD
		  ����,��Ӧ��ADת��ͨ��ΪADC123_IN11,�����ĸ�IO�ڶ�
		  Ӧ�ĸ�ADת��ͨ����ο�F10x�����ֲ�20ҳ���Ŷ��塣
	ʱ��: 2013��7��24�� BoX��д 
---------------------------------------------------------*/
#include "stm32f10x.h"
#include "adc.h"

unsigned short int ADC1_DMA_Value;

void ADC1_GPIO_Init(void)
{
	GPIO_InitTypeDef IO_Init;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC , ENABLE);
	//C1ģ������
	IO_Init.GPIO_Pin = GPIO_Pin_1;
	IO_Init.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC,&IO_Init);
}

/* #define ADC1_DR_Address ((unsigned int)0x40012400+0x4c) */
/* unsigned short int ADC1_DMA_Value; */

void ADC1_Mode_Config(void)
{
	//����DMA
	DMA_InitTypeDef DMA_csh;
	ADC_InitTypeDef ADC_csh;

	DMA_DeInit(DMA1_Channel1);  //DMA��λ,ͨ��1
	DMA_csh.DMA_PeripheralBaseAddr = ADC1_DR_Address;  //ADC1��ַ
	DMA_csh.DMA_MemoryBaseAddr = (unsigned int) &ADC1_DMA_Value;  //�ڴ��ַ
	/* ���ݴ�ADC1->DR�Ĵ���ͨ��DMAת�Ƶ��ڴ沢�ұ����ڱ���ADC_DMA_Value�� */
	DMA_csh.DMA_DIR = DMA_DIR_PeripheralSRC;  //����Ϊ����Դ
	DMA_csh.DMA_BufferSize = 1;  //�����С
	DMA_csh.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�̶�
	DMA_csh.DMA_MemoryInc = DMA_MemoryInc_Disable; //�ڴ��ַ�̶�
	DMA_csh.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //DMA�������ݴ�С ����
	DMA_csh.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //DMA�ڴ����ݴ�С ����
	DMA_csh.DMA_Mode = DMA_Mode_Circular;  //ѭ������
	DMA_csh.DMA_Priority = DMA_Priority_High;  //DMA���ȼ���
	DMA_csh.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1,&DMA_csh);  //д��DMA1���ò���
	DMA_Cmd(DMA1_Channel1,ENABLE);	   //ʹ��DMA1ͨ��1
	//����ADC
	ADC_csh.ADC_Mode = ADC_Mode_Independent;  //����ADCģʽ
    ADC_csh.ADC_ScanConvMode = DISABLE;   //�ر�ɨ��ģʽ
	ADC_csh.ADC_ContinuousConvMode = ENABLE;  //����ADת������
	ADC_csh.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //�ر��ⲿ����ת��
	ADC_csh.ADC_DataAlign = ADC_DataAlign_Right; //�����Ҷ���
	ADC_csh.ADC_NbrOfChannel = 1;  //Ҫת����ͨ����Ŀ1
	ADC_Init(ADC1,&ADC_csh);   //д��ADC1���ò���
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);  //PCLK2 = 72MHz 72DIV8 = 9MHz ,ADʱ�����14MHz
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,1,ADC_SampleTime_55Cycles5); //����ͨ������
               	     /* ADCx , ADCת��ͨ�� , ADCɨ��ת������ , ADC�������� */
	ADC_DMACmd(ADC1 , ENABLE); //ʹ��ADC1 DMA
	ADC_Cmd(ADC1 , ENABLE); //ʹ��ADC1
	ADC_ResetCalibration(ADC1);	  //��λУ׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1)); //�ȴ�У׼�Ĵ�����λ���
	ADC_StartCalibration(ADC1);	  //��ʼУ׼
	while(ADC_GetCalibrationStatus(ADC1)); //�ȴ�У׼���
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	 //ADC1�������ת������
}


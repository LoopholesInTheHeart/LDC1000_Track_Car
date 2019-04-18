#include "ldc1000.h"
#include "delay.h"
/*----------------------------------------
				ȫ�ֱ���
----------------------------------------*/
uint8_t RegArray[REG_LEN];
/*----------------------------------------
 				�궨��
----------------------------------------*/
#define CSB_0 	GPIOB->BRR  = GPIO_Pin_7
#define CSB_1 	GPIOB->BSRR = GPIO_Pin_7
/*----------------------------------------
 ��������: LDC1000��ʼ��
 ��������: ��
----------------------------------------*/
void LDC1000_Init(void)
{
	LDC1000_GPIO_Init();
	LDC1000_SPI_Init();
	
	  delayus(5);
    LDC1000_WriteReg(LDC1000_CMD_RPMAX,     0x16);
    LDC1000_WriteReg(LDC1000_CMD_RPMIN,     0x38);
	LDC1000_WriteReg(LDC1000_CMD_SENSORFREQ,  0x94);
	LDC1000_WriteReg(LDC1000_CMD_LDCCONFIG,   0x17);
	LDC1000_WriteReg(LDC1000_CMD_CLKCONFIG,   0x02);
	LDC1000_WriteReg(LDC1000_CMD_INTCONFIG,   0x02);

	LDC1000_WriteReg(LDC1000_CMD_THRESHILSB,  0x50);
	LDC1000_WriteReg(LDC1000_CMD_THRESHIMSB,  0x14);
	LDC1000_WriteReg(LDC1000_CMD_THRESLOLSB,  0xC0);
	LDC1000_WriteReg(LDC1000_CMD_THRESLOMSB,  0x12);

	LDC1000_WriteReg(LDC1000_CMD_PWRCONFIG,   0x01);	 	
}
/*----------------------------------------
 ��������: LDC1000 GPIO��ʼ��
 ��������: IO������ͼ

 	+5V 	<-> 	+5V
	DCLK	<-> 	PB6 <-> TIM4_CH1
	SCLK    <-> 	PA5 <-> SPI1_SCK
	SDI     <-> 	PA7 <-> SPI1_MOSI
	SDO     <-> 	PA6 <-> SPI1_MISO
	VIO		<-> 	3V3
	GND		<-> 	GND
	INT     <-> 	PC2 <-> GPIO
	CSB		<-> 	PB7	<-> GPIO
	GND		<->		GND
----------------------------------------*/
void LDC1000_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//��סʹ��AFIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC , ENABLE);
	
	//PB6->PWM_OUTPUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				    
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//PB6�����������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//A5->SCK  A6->MISO A7->MOSI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   //SPI�ø����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//PC2->INT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	//PB7->CSB
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
/*----------------------------------------
 ��������: SPI������ʼ��
 ��������: LDC1000��ʱ��Ƶ�ʱ�����4.2MHz
 		   �Ҳ��ܳ���5MHz,���ܵ���4MHz,����
		   ģ�鲻�ܹ�����ͨ��,����SPI������
		   ��Ƶϵ��ѡ��16��Ƶ.
----------------------------------------*/
void LDC1000_SPI_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);		//ʹ��APB2����ʱ��

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;			//SPI��
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//���ݸ�ʽ8λ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;				//ʱ�Ӽ��Ե�,�߶�����
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		    //ʱ����λ��1,2���ش���������
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_32; //������16��Ƶ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;		//MSB��ǰ,����
	SPI_InitStructure.SPI_CRCPolynomial = 7;				//CRC����ʽУ��,���п���
	SPI_Init(SPI1,&SPI_InitStructure);

	SPI_Cmd(SPI1,ENABLE);
}
/*----------------------------------------
 ��������: LDC1000д�Ĵ���
 ��������: - addr �Ĵ�����ַ
 		   - dat  ����
----------------------------------------*/
void LDC1000_WriteReg(int8_t addr, uint8_t dat)
{
	addr = addr & 0x7F;     		//MSBΪ0д�Ĵ���,Ϊ1���Ĵ���
	CSB_0;
	
	LDC1000_SendByte(addr);
	LDC1000_SendByte(dat);
// 	SPI_I2S_SendData(SPI1, addr);
// 	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
// 	SPI_I2S_SendData(SPI1, dat);
// 	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	CSB_1; 	
}
/*----------------------------------------
 ��������: LDC1000���Ĵ���
 ��������: - addr �Ĵ�����ַ
----------------------------------------*/
uint8_t LDC1000_ReadReg(int8_t addr)
{	
	uint8_t temp;
	addr = addr | 0x80;     		//MSBΪ1���Ĵ���,Ϊ0д�Ĵ���
	CSB_0;
// 	SPI_I2S_SendData(SPI1, addr);
// 	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
// 	SPI_I2S_SendData(SPI1, 0x00);
// 	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
// 	temp = (uint8_t)SPI_I2S_ReceiveData(SPI1);
	
	LDC1000_SendByte(addr);
	temp = LDC1000_SendByte(0x00);
	CSB_1;
	return temp; 	
}
/*----------------------------------------
 ��������: ��LDC1000����һ�ֽ�����
 ��������: Ҫ���͵�����dat
 �� �� ֵ: ����������������
----------------------------------------*/
uint8_t LDC1000_SendByte(uint8_t dat)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, dat);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return (uint8_t)SPI1->DR;
}
/*----------------------------------------
 ��������: ��ȡLDC1000һ���ֽ�
 ��������: ��
 �� �� ֵ: ������������һ�ֽ�����
----------------------------------------*/
uint8_t LDC1000_ReadByte(void)
{
	return 0x00;	
}
/*----------------------------------------
 ��������: ��ȡLDC1000 len���ֽ�
 ��������: - addr
 		   - pBuffer
		   - len
 ����ʾ��: LDC1000_ReadBytes(LDC1000_CMD_REVID,&RegArray[0],12);
----------------------------------------*/
void LDC1000_ReadBytes(int8_t addr, uint8_t* pBuffer, uint8_t len)
{
 	addr = addr | 0x80;		 //Read Data
	CSB_0;
	LDC1000_SendByte(addr);
	while(len > 0)
	{
	 	*pBuffer = LDC1000_ReadByte();
		pBuffer++;
		len--;	
	}
	CSB_1;
}

/********************************************************************************/
uint8_t SPI_LDC1000_ReadBytes(char ReadAddr,char* pBuffer,u8 NumByteToRead)//main()�����ж�ȡRpʱ���õ��������   
{ 
 	uint8_t i,readback;
	uint8_t txReadAddr;
	txReadAddr = ReadAddr | 0x80; 		  //��λ��1��ʾ��										    
	CSB_0;                            //ʹ������   
    SPIx_ReadWriteByte(txReadAddr);         //���Ͷ�ȡ����     
//    SPIx_ReadWriteByte((u8)txReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPIx_ReadWriteByte(0XFF);   //ѭ������  
    }
	CSB_1;                            //ȡ��Ƭѡ 
	return readback;    	      
}  
//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
uint8_t SPIx_ReadWriteByte(u8 TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����					    
}

/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��usart.c
 * ����    ����printf�����ض���USART1�������Ϳ�����printf��������Ƭ�������ݴ�
 *           ӡ��PC�ϵĳ����ն˻򴮿ڵ������֡�     
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2017-04-20
 * Ӳ������: TX->PA9;RX->PA10
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/	

//ͷ�ļ�
#include "usart.h"

u8 usart2Buf[64];
u8 usart2Len;
u8 usart1Buf[64];
u8 usart1Len;


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


 /**
  * @file   USART1_Config
  * @brief  USART1 GPIO ����,����ģʽ���á�9600-8-N-1
  * @param  ��
  * @retval ��
  */
void USART1_Config(void)
{	
    GPIO_InitTypeDef GPIO_InitStructure;	
    USART_InitTypeDef USART_InitStructure;  //���崮�ڳ�ʼ���ṹ��
    NVIC_InitTypeDef NVIC_InitStructure;
//	  USART_ClockInitTypeDef USART_ClockInitStruct1;//��ʼ��ʱ���ź�
    
	RCC_APB2PeriphClockCmd( USART_RCC | USART_GPIO_RCC,ENABLE);//1.����ʱ��ʹ�ܣ� RCC_APB2Periph_USART1���Լ�GPIOʹ�ܣ�RCC_APB2Periph_GPIOA��

    /*USART1_TX ->PA9*/			
    GPIO_InitStructure.GPIO_Pin = USART_TX;	       //2.ѡ�д���Ĭ������ܽ�         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // ����ܽ�9��ģʽ ����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;// �������������� 
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��		   
    /*USART1_RX ->PA10*/
    GPIO_InitStructure.GPIO_Pin = USART_RX;	        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // ����ܽ�10��ģʽ ����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);
	//GPIO�ĳ�ʼ�����
	//ʱ�ӳ�ʼ��
//	USART_ClockInitStruct1.USART_Clock=USART_Clock_Disable;
//	USART_ClockInitStruct1.USART_CPHA=USART_CPHA_2Edge;
//	USART_ClockInitStruct1.USART_CPOL=USART_CPOL_Low;
//	USART_ClockInitStruct1.USART_LastBit=USART_LastBit_Disable;
//	USART_ClockInit(USART1, &USART_ClockInitStruct1);
	//���ڵĳ�ʼ��
    /*����ͨѶ��������*/
    USART_InitStructure.USART_BaudRate = BaudRate; //������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
    USART_InitStructure.USART_Parity = USART_Parity_No;		//У��λ ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������

    USART_Init(USART, &USART_InitStructure);//���ڳ�ʼ��

		 NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//�ж����ȼ�Ϊ3
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQʹ��
		 NVIC_Init(&NVIC_InitStructure);	//��ʼ���ж�

	 USART_ClearFlag(USART,USART_FLAG_TC);
		USART_ITConfig(USART, USART_IT_RXNE, ENABLE);//�����жϽ���

    USART_Cmd(USART, ENABLE);//3.ʹ�ܴ���
		
}


void USART1_IRQHandler(void)//UART2�������ݺ���
	{
		uint8_t res1; 
				printf("U1�жϽ��պ�����ʼ\r\n");
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //
			{
				res1 =USART_ReceiveData(USART1); //��ȡUSART2�յ�������
				usart1Buf[usart1Len]=res1; //��¼�յ������ݵ��ڴ�

				usart1Len++; //�±��1
					if(usart1Len>8)
						usart1Len=0; 
			}
    if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET)//�������������ȶ�SR���ٶ�DR�Ĵ���
			{
        USART_ReceiveData(USART1);
        USART_ClearFlag(USART1,USART_FLAG_ORE);
			}
		USART_ClearFlag(USART1,USART_IT_RXNE); //��������ж�
					printf("U1�жϽ��պ���over\r\n");
	}

void myUSART_Sendbyte(USART_TypeDef* USARTx, uint16_t Data)//�����ڷ���һλ�ַ�
{printf("stepb1\r\n");
    while((USARTx->SR&0X40)==0);
		USARTx->DR = (Data & (uint16_t)0x01FF);
	printf("stepb2\r\n");
}

void myUSART_Sendarr(USART_TypeDef* USARTx, u8 a[] ,uint8_t len)//�����ڷ���һ�������ַ�
{
	uint8_t i=0;printf("stepa1\r\n");
	while(i < len )
		{ 
			myUSART_Sendbyte( USARTx, a[i]) ;
			i++;
		}
		printf("stepa2\r\n");
}



void USART1_SEND(void)//wang PC fasong
{
  printf("U1 fasong kaishi\r\n");
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//��־λ����
	{
		if(usart1Len >= 8)									//��������
	
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
 	myUSART_Sendarr(USART1,usart2Buf,8);
	 printf("U1 fasonghanshu jieshu\r\n");
	
}


void USART2_Config(void)
{	
       GPIO_InitTypeDef GPIO_InitStructure2;	
    USART_InitTypeDef USART_InitStructure2;  //���崮�ڳ�ʼ���ṹ��
    NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_APB1PeriphClockCmd(USART2_RCC ,ENABLE);//����ʱ��ʹ�ܣ� RCC_APB1Periph_USART2
  RCC_APB2PeriphClockCmd(USART2_GPIO_RCC,ENABLE);//GPIOʹ�ܣ�RCC_APB2Periph_GPIOA��
    /*USART1_TX ->PA2*/			
    GPIO_InitStructure2.GPIO_Pin = USART2_TX;	       //2.ѡ�д���Ĭ������ܽ�         
    GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_AF_PP;  // ����ܽ�2��ģʽ ����
    GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;// �������������� 
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure2);           //���ú������ѽṹ�����������г�ʼ��		   
    /*USART1_RX ->PA3*/
    GPIO_InitStructure2.GPIO_Pin = USART2_RX;	        
    GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // ����ܽ�3��ģʽ ����
    GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure2);
	//GPIO�ĳ�ʼ�����

    /*����ͨѶ��������*/
    USART_InitStructure2.USART_BaudRate = BaudRate; //������
    USART_InitStructure2.USART_WordLength = USART_WordLength_8b; //����λ8λ
    USART_InitStructure2.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
    USART_InitStructure2.USART_Parity = USART_Parity_No;		//У��λ ��
    USART_InitStructure2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
    USART_InitStructure2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������

    USART_Init(USART2, &USART_InitStructure2);//���ڳ�ʼ��

		 NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//�ж����ȼ�Ϊ3
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQʹ��
		 NVIC_Init(&NVIC_InitStructure);	//��ʼ���ж�


		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART2, ENABLE);//3.ʹ�ܴ���
		
	
}






void USART2_IRQHandler(void)//UART2�������ݺ���
	{
		uint8_t res; 
		printf("U2�жϽ��պ�����ʼ\r\n");
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //????? �
			{
				res =USART_ReceiveData(USART2); //��ȡUSART2�յ�������
				usart2Buf[usart2Len]=res; //��¼�յ������ݵ��ڴ�

				usart2Len++; //�±��1
					if(usart2Len>8)
						usart2Len=0; 
			}
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)//�������������ȶ�SR���ٶ�DR�Ĵ���
			{
        USART_ReceiveData(USART2);
        USART_ClearFlag(USART2,USART_FLAG_ORE);
			}
		USART_ClearFlag(USART2,USART_IT_RXNE); //��������ж�
				printf("U2�жϽ��պ�OVER\r\n");	
	}

	
	
	void USART2_SEND(void)//wang PC fasong
{
  printf("U2 fasong kaishi");
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	//��־λ����
	{
		if(usart2Len >= 8)									//��������
	
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
 	myUSART_Sendarr(USART2,usart1Buf,8);
	 printf("U1fasonghanshu jieshu");
	
}
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART, USART_FLAG_TC) == RESET)
  {}
  return ch;
}

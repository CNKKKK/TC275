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
//    NVIC_InitTypeDef NVIC_InitStructure;
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
		USART_ITConfig(USART1,USART_IT_TC|USART_IT_CTS|USART_IT_ERR|USART_IT_TXE|USART_IT_RXNE,DISABLE);//�ر����жϿ�

//		 NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//�ж����ȼ�Ϊ3
//		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
//		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQʹ��
//		 NVIC_Init(&NVIC_InitStructure);	//��ʼ���ж�


	 USART_ClearFlag(USART,USART_FLAG_TC);
//		USART_ITConfig(USART, USART_IT_RXNE, ENABLE);//�����ж�
//		USART_ITConfig(USART, USART_IT_TXE, ENABLE);		
    USART_Cmd(USART, ENABLE);//3.ʹ�ܴ���
		
		USART_Init(USART,&USART_InitStructure);
}

void USART2_Config(void)
{	
       GPIO_InitTypeDef GPIO_InitStructure2;	
    USART_InitTypeDef USART_InitStructure2;  //���崮�ڳ�ʼ���ṹ��
//    NVIC_InitTypeDef NVIC_InitStructure;
//	  USART_ClockInitTypeDef USART_ClockInitStruct1;//��ʼ��ʱ���ź�
    
	RCC_APB1PeriphClockCmd( USART_RCC | USART_GPIO_RCC,ENABLE);//1.����ʱ��ʹ�ܣ� RCC_APB2Periph_USART1���Լ�GPIOʹ�ܣ�RCC_APB2Periph_GPIOA��

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
	//ʱ�ӳ�ʼ��
//	USART_ClockInitStruct1.USART_Clock=USART_Clock_Disable;
//	USART_ClockInitStruct1.USART_CPHA=USART_CPHA_2Edge;
//	USART_ClockInitStruct1.USART_CPOL=USART_CPOL_Low;
//	USART_ClockInitStruct1.USART_LastBit=USART_LastBit_Disable;
//	USART_ClockInit(USART1, &USART_ClockInitStruct1);
	//���ڵĳ�ʼ��
    /*����ͨѶ��������*/
    USART_InitStructure2.USART_BaudRate = BaudRate; //������
    USART_InitStructure2.USART_WordLength = USART_WordLength_8b; //����λ8λ
    USART_InitStructure2.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
    USART_InitStructure2.USART_Parity = USART_Parity_No;		//У��λ ��
    USART_InitStructure2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
    USART_InitStructure2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������

    USART_Init(USART2, &USART_InitStructure2);//���ڳ�ʼ��
		USART_ITConfig(USART2,USART_IT_TC|USART_IT_CTS|USART_IT_ERR|USART_IT_TXE|USART_IT_RXNE,ENABLE);//�ر����жϿ�

//		 NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//�ж����ȼ�Ϊ3
//		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
//		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQʹ��
//		 NVIC_Init(&NVIC_InitStructure);	//��ʼ���ж�


	 USART_ClearFlag(USART2,USART_FLAG_TC);
//		USART_ITConfig(USART, USART_IT_RXNE, ENABLE);//�����ж�
//		USART_ITConfig(USART, USART_IT_TXE, ENABLE);		
    USART_Cmd(USART2, ENABLE);//3.ʹ�ܴ���
		
		USART_Init(USART2,&USART_InitStructure2);
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

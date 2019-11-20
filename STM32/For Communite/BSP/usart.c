

//ͷ�ļ�
#include "usart.h"
//#include "ringbuff.h"

u8 usart2Buf[35];
u8 usart2Len;
u8 usart1Buf[40];
u8 usart1Len=0;
u8 asd[7]={0x0b,0x03,0x02,0xff, 0xc2,0xe0,0x24 };

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
	void delay_ms(u32 i)
{
    u32 temp;
    SysTick->LOAD=9000*i;      //??????, 72MHZ?
    SysTick->CTRL=0X01;        //??,???????,???????
    SysTick->VAL=0;            //?????
    do
    {
        temp=SysTick->CTRL;       //????????
    }
    while((temp&0x01)&&(!(temp&(1<<16))));    //??????
    SysTick->CTRL=0;    //?????
    SysTick->VAL=0;        //?????
}
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
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQʹ��
		 NVIC_Init(&NVIC_InitStructure);	//��ʼ���ж�


		USART_ITConfig(USART, USART_IT_RXNE, ENABLE);//�����жϽ���
    USART_Cmd(USART, ENABLE);//3.ʹ�ܴ���
		
}


void USART1_IRQHandler(void)//UART1�������ݺ����ж�
	{
		uint8_t res; 

		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�ж϶�ȡ���ݼĴ����Ƿ�Ϊ��
			{
				res =USART_ReceiveData(USART1); //��ȡUSART2�յ�������
				usart1Buf[usart1Len]=res; //��¼�յ������ݵ��ڴ�
				usart1Len++; //�±��1
					if(usart1Len>=120)
						usart1Len=0; 
			}
    if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET)//�������������ȶ�SR���ٶ�DR�Ĵ���
			{
        USART_ReceiveData(USART1);
        USART_ClearFlag(USART1,USART_FLAG_ORE);
			}
		USART_ClearFlag(USART1,USART_IT_RXNE); //��������ж�

	}

void myUSART_Sendbyte(USART_TypeDef* USARTx, uint16_t Data)//�����ڷ���һλ�ַ�
{
    while((USARTx->SR&0X40)==0);

//		USARTx->DR = (Data & (uint16_t)0x01FF);
		USART_SendData(USARTx,Data);
//		printf("%x\n", Data);

}

void myUSART_Sendarr(USART_TypeDef* USARTx, u8 a[] ,uint8_t len)//�����ڷ���һ�������ַ�
{
	uint8_t i=0;
	while(i < len )
		{ 
			myUSART_Sendbyte( USARTx, a[i]) ;
			i++;
		}

}

	void USART1_SEND(void)//wang PC fasong
{
	//	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//���ͻ�������־Ϊ��

//for(i=0;i<7;i++)
//			{
//				if(usart1Buf[i*7]==0x0b)//ɸѡ����ַ��0b����Ϣ �浽�ڴ�׼������
//				{	}//��UASRT2�յ�������ͨ��UASRT1����ȥ,��7λΪ��λ����

//			}					
	myUSART_Sendarr(USART1,asd,7);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//���ͻ�������־Ϊ��
	

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
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQʹ��
		 NVIC_Init(&NVIC_InitStructure);	//��ʼ���ж�

		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART2, ENABLE);//3.ʹ�ܴ���	
}

void USART2_IRQHandler(void)//UART2�������ݺ����ж�
	{
		uint8_t res=0; 

		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //�ж϶�ȡ���ݼĴ����Ƿ�Ϊ��
			{
				res =USART_ReceiveData(USART2); //��ȡUSART2�յ�������
				usart2Buf[usart2Len]=res; //��¼�յ������ݵ��ڴ�	
			//	RingBuf_Write(res);
				usart2Len++; //�±��1
					if(usart2Len>=35)
					{
						usart2Len=0;
						USART_ClearFlag(USART2,USART_IT_RXNE); //��������ж�
					}
			}
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)//�������������ȶ�SR���ٶ�DR�Ĵ���
			{
        USART_ReceiveData(USART2);
        USART_ClearFlag(USART2,USART_FLAG_ORE);
			}
		USART_ClearFlag(USART2,USART_IT_RXNE); //��������ж�

	}

	void USART2_SEND(void)//wang PC fasong
	{int i;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);//���ͻ�������־Ϊ��
 		for(i=0;i<15;i++)
			{
				if(usart1Buf[i*8]==0x0b&&usart1Buf[i*8+3]==0x0c)//ɸѡ����ַ��0b����Ϣ �浽�ڴ�׼������
				{
					myUSART_Sendarr(USART2,&usart1Buf[8*i],8);
					delay_ms(10);
				}//��UASRT1�յ�������ͨ��UASRT2����ȥ,�԰�λΪ��λ����

			}
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//���ͻ�������־Ϊ��
		if(usart1Len >= 120)									//��������
			{
				usart1Len=0;
			}
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

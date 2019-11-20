

//ͷ�ļ�
#include "usart.h"
#include "ringbuff.h"

u8 usart1Buf[30];
u8 usart1Len;
u8 usart2Buf[30];
u8 usart2Len;
//char hexbuffer[5];//���ڴ��ʮ�����Ƶ��ٶ�ֵ
//u8 speedflag;
char posref[2];
char speedvalue[2];
extern float angle_pu;

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */



 /**
  * @file   USART1_Config
  * @brief  USART1 GPIO ����,����ģʽ���á�19600-8-N-1
  * @param  ��
  * @retval ��
  */
	
	void send_pos(void);
	void delay_ms(u32 i)
{
    u32 temp;
    SysTick->LOAD=9000*i;      //72MHZ?
    SysTick->CTRL=0X01;        
    SysTick->VAL=0;            
    do
    {
        temp=SysTick->CTRL;       
    }
    while((temp&0x01)&&(!(temp&(1<<16))));    
    SysTick->CTRL=0;    
    SysTick->VAL=0;        
}
/*���ζ��д洢����*/
		ringBuffer_t buffer = {0,0,{0}};//u2���ն��г�һ�����ζ��г�ʼ��Ϊ0
		ringBuffer_t buffer_A = {0,0,{0}};//U1���ն��γ�һ�����ζ��в���ʼ��Ϊ0


void RingBuf_Write(float data){//�򻺳���д��һ���ֽ�
		buffer.ringBuf[buffer.tailPosition]=data;//��β��׷��
	
		if(++buffer.tailPosition >= BUFFER_MAX)//β���ƫ��
			buffer.tailPosition = 0;//�������鳤�ȣ�������鳤�ȹ��㣬�γɻ�������
	//��β���ڵ�׷��ͷ���ڵ㣬���޸�ͷ�ڵ�ƫ��λ�ö�����������
		if(buffer.tailPosition == buffer.headPosition)
			if(++buffer.headPosition >= BUFFER_MAX)
			buffer.headPosition=0;
}

void RingBuf_Write_A(float data){//�򻺳���д��һ���ֽ�
	buffer_A.ringBuf[buffer_A.tailPosition]=data;//��β��׷��
	/*��ȡλ�òο�ֵ*/
	if(buffer_A.ringBuf[buffer_A.tailPosition]==0XFF||buffer_A.ringBuf[(buffer_A.tailPosition)-1]==0XFE)// posref1 posref2 FE FF �Ժ�������Ϊ��־λ
	{
		posref[0]=buffer_A.ringBuf[(buffer_A.tailPosition)-3];
		posref[1]=buffer_A.ringBuf[(buffer_A.tailPosition)-2];
	}
	/*��λ�òο�ֵ��ȡ����*/
		if(++buffer_A.tailPosition >= BUFFER_MAX)//β���ƫ��
		buffer_A.tailPosition = 0;//�������鳤�ȣ�������鳤�ȹ��㣬�γɻ�������
	//��β���ڵ�׷��ͷ���ڵ㣬���޸�ͷ�ڵ�ƫ��λ�ö�����������
	if(buffer_A.tailPosition == buffer_A.headPosition)
		if(++buffer_A.headPosition >= BUFFER_MAX)
			buffer_A.headPosition=0;
}



unsigned char RingBuf_Read( float* pData){//��ȡ������һ���ֽ�����
	if(buffer.headPosition == buffer.tailPosition)
		{//ͷβ��ӱ�ʾ������Ϊ��
		return 0;//��ȡʧ�� ����0
		}
	else{
		*pData = buffer.ringBuf[buffer.headPosition];//���������Ϊ����ȡͷ�ڵ㲢ƫ��ͷ�ڵ�
		if(++buffer.headPosition>=BUFFER_MAX)
			buffer.headPosition = 0;
		return 1;//��ȡ�ɹ�����1
	}
}

unsigned char RingBuf_Read_A( float* pData){//��ȡ������һ���ֽ�����
	if(buffer_A.headPosition == buffer_A.tailPosition)
		{//ͷβ��ӱ�ʾ������Ϊ��
		return 0;//��ȡʧ�� ����0
		}
	else{
		*pData = buffer_A.ringBuf[buffer_A.headPosition];//���������Ϊ����ȡͷ�ڵ㲢ƫ��ͷ�ڵ�
		if(++buffer_A.headPosition>=BUFFER_MAX)
			buffer_A.headPosition = 0;
		return 1;//��ȡ�ɹ�����1
	}
}
/*���ζ��ж��弰����*/


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



	void USART1_IRQHandler(void)//UART1????????
	{
		uint8_t res; 
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //?????????????
			{
				res =USART_ReceiveData(USART1); //??USART1?????
				usart1Buf[usart1Len++]=res; //??????????
				if(usart1Len>5)
					{
						if(usart1Buf[usart1Len-2]==0x10&&usart1Buf[usart1Len-1]==0x18)//����һ�ڶ�λ�Ƿ�Ϊ10 18 �����ǵĻ�����λ����Ϣ����λ��
							{ 
								send_pos();
								usart1Len=0;
							}
						if(usart1Buf[usart1Len-5]==0x06&&usart1Buf[usart1Len-4]==0x10&&usart1Buf[usart1Len-3]==0x07)//�����ٶ�дָ��06 10 07 Ȼ����ȡ�ٶ�ֵ
							{
									speedvalue[0]=usart1Buf[usart1Len-2];
									speedvalue[1]=usart1Buf[usart1Len-1];
								usart1Len=0;
							}
					}
   
					RingBuf_Write_A(res);
          if(usart1Len>40)
					usart1Len=0;
			}
    if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET)//??????,??SR,??DR???
   {
        USART_ReceiveData(USART1);
        USART_ClearFlag(USART1,USART_FLAG_ORE);
   }
		USART_ClearFlag(USART1,USART_IT_RXNE); //??????
	}

void myUSART_Sendbyte(USART_TypeDef* USARTx, uint16_t Data)//�����ڷ���һλ�ַ�
{
    while((USARTx->SR&0X40)==0);
		USART_SendData(USARTx,Data);
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

int crc_cal_value( char *data_value,unsigned char data_length)//CRCУ��λ����
{
		int i;
    int crc_value = 0xffff;
    while(data_length--)
			{
					crc_value ^= *data_value++;
					for(i=0;i<8;i++)
					{
						if(crc_value&0x0001)
							crc_value = (crc_value>>1)^0xa001;
						else
					crc_value = crc_value>>1;
					}
			}
		return(crc_value);
}

void send_pos(void)
{
	char pos[7];
	int i;

	{
			
		/*
		��λ����Ϣ��װ��modbusЭ�鷢�ͳ�ȥ
				myUSART_Sendbyte(USART1,0X0B 0X03);
		*/
				{
					pos[0]=0x0b;
					pos[1]=0x03;
					pos[2]=0x02;					
					pos[3]=((int)(angle_pu *100)/256);
					pos[4]=((int)(angle_pu *100)%256);

					pos[5]=crc_cal_value(pos,5);
					pos[6]=crc_cal_value(pos,5)>>8;
				}
				for(i=0;i<7;i++)
				{
									myUSART_Sendbyte(USART1,pos[i]);
				}
			
		}
	}
	void USART1_SEND_02(void)//wang PC fasong
{
	float ress;
		if(RingBuf_Read(&ress)==1)//�����ζ������������
		{
//			if(ress==0x0b)
//			send_pos();//����λ����Ϣ֡
		myUSART_Sendbyte(USART1,ress);
		}

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
		uint8_t res; 
//for( i=0;i<5;i++)
		{if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //�ж϶�ȡ���ݼĴ����Ƿ�Ϊ��
			{
				res =USART_ReceiveData(USART2); //��ȡUSART2�յ�������
				usart2Buf[usart2Len]=res; //��¼�յ������ݵ��ڴ�	

				RingBuf_Write(res);//���ζ�����д������

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
	{

		float ress;
//		for(i=0;i<15;i++){//���͵�ַΪ0b�ķ��ͳ�ȥ
//		if(usart1Buf[i*8]==0x0b)
//			myUSART_Sendarr(USART2,&usart1Buf[i*8],8);
//		//delay_ms(50);
//		}
		
		{
			if(RingBuf_Read_A(&ress)==1)
				{	
					myUSART_Sendbyte(USART2,ress);

				}
//				if(speedflag==1)
//				{		myUSART_Sendbyte(USART2,getspeedcommand(20));
//						speedflag=0;
//				}
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

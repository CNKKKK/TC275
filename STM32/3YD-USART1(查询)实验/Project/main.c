/********************  ********************************
 * �ļ���  ��main.c
 * ����    ��ͨ������1��ӡ����Լ�������ַ�������������ϢΪ��9600��N,8��1��  

**********************************************************************************/	

//ͷ�ļ�
#include "stm32f10x.h"
#include "usart.h"
#include "timer.h"
/**
  * @file   main
  * @brief  Main program.
  * @param  None
  * @retval None
  */





//uint16_t TIM_Count;	//��ʱ������
u8 RS232_RX_BUF[200];          //���ջ�����
u8 RS232_RX_CNT=0;                        //�������ݳ���
u8 RS232_TX_CNT=0;						//�������ݳ���
//���뼶����ʱ
//void TIM2_delay(u16 time)
//{
// 	TIM_Count = time;
//	while(TIM_Count); 
//}
unsigned int crc_cal_value(unsigned char *data_value,unsigned char data_length)//����CRC��ֵ
{
	int i;
    unsigned int crc_value = 0xffff;//CRC���Ϊ16���Ƶ�16λ
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

void myUSART_Sendbyte(USART_TypeDef* USARTx, uint16_t Data)//�����ڷ���һλ�ַ�
{
    while((USARTx->SR&0X40)==0); 
    USARTx->DR = (Data & (uint16_t)0x01FF);
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

void Send_ReadCommandtoInverter(void){		int i;
/*
	�������
	֡ͷ						3.5�ֽ�ʱ��
	ADR 						01H
	CMD������				03H
	�ٶ���ʼλ��λ	10H
	�ٶ���ʼλ��λ	07H
	�Ĵ���������λ	00H
	�Ĵ���������λ	02H
	CRC CHK��λ			56H
	CRC CHK��λ			CBH
	֡β						3.5�ַ�ʱ��
	*/	

	u8 RS232_TX_BUF[8]={0x01,0x03,0x10,0x07,0x00,0x02};//���ͽ����ٶ��źŵ�ָ��
	RS232_TX_BUF[6]=crc_cal_value(RS232_TX_BUF,8)&0x00ff;//CRC�Ȱѵ��ֽڼ��뵽֡��
	RS232_TX_BUF[7]=(crc_cal_value(RS232_TX_BUF,8)>>8)&0x00ff;//������ֽ�
	printf("Send_ReadCommandtoInverter start");
		for (i = 0; i<8; i++)
		printf("%d\n", RS232_TX_BUF[i]);
	TIM2_delay(3.5*(1000/BaudRate)*10*8);
	myUSART_Sendarr(USART2,RS232_TX_BUF,8);
	TIM2_delay(3.5*(1000/BaudRate)*10);
		printf("Send_ReadCommandtoInverter over");
}
void USART2_IRQHandler(void)
	{//USART2�Ľ�������
				printf("USART RECEIVE START");
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//�۲��Ƿ���յ�����
		{
			USART_ClearITPendingBit(USART2,USART_IT_RXNE);//�����־λ
			RS232_RX_BUF[RS232_RX_CNT++]=USART_ReceiveData(USART2);
          if( RS232_RX_CNT==8)
                RS232_RX_CNT=0;    
		}
						printf("USART RECEIVE over");
	}

void USART1_IRQHandler(void)
	{	printf("USART SEND START");
		//static u8 k;
		USART_ClearFlag(USART1,USART_FLAG_TC);//���USART1�ķ���
		if(USART_GetITStatus(USART1,USART_IT_RXNE)!=Bit_RESET)
			{
				USART_SendData(USART1,USART_ReceiveData(USART2));
						while(USART_GetFlagStatus(USART1,USART_IT_RXNE)!=RESET);
			}
			printf("USART SEND over");
	}

int main(void)
{
    USART1_Config();//����1�������ó�ʼ��
		USART2_Config();//����2�������ó�ʼ��
    while (1)
    {
	printf("Write Function");
			Send_ReadCommandtoInverter();//���ͽ�����Ϣ��ָ��
			USART2_IRQHandler();//USART2�����ٶ���Ϣ
			USART1_IRQHandler();//UASRT1��UASRT2�յ�����Ϣ����PC
    }
}









#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/






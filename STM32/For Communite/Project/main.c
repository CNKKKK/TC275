/********************  ********************************
 * 文件名  ：main.c
 * 描述    ：通过串口1打印输出自己输入的字符串；（串口信息为：9600，N,8，1）  

**********************************************************************************/	

//头文件
#include "stm32f10x.h"
#include "usart.h"

/**
  * @file   main
  * @brief  Main program.
  * @param  None
  * @retval None
  */





//uint16_t TIM_Count;	//定时器计数
u8 RS232_RX_BUF[200];          //接收缓冲区
u8 RS232_RX_CNT=0;                        //接收数据长度
u8 RS232_TX_CNT=0;						//发送数据长度



unsigned int crc_cal_value(unsigned char *data_value,unsigned char data_length)//计算CRC的值
{
	int i;
    unsigned int crc_value = 0xffff;//CRC输出为16进制的16位
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



//void Send_ReadCommandtoInverter(void){		int i;
///*
//	主机命令：
//	帧头						3.5字节时间
//	ADR 						01H
//	CMD读数据				03H
//	速度起始位高位	10H
//	速度起始位低位	07H
//	寄存器个数高位	00H
//	寄存器个数低位	02H
//	CRC CHK低位			56H
//	CRC CHK高位			CBH
//	帧尾						3.5字符时间
//	*/	

//	u8 RS232_TX_BUF[8]={0x01,0x03,0x10,0x07,0x00,0x02};//发送接收速度信号的指令
//	RS232_TX_BUF[6]=crc_cal_value(RS232_TX_BUF,8)&0x00ff;//CRC先把低字节加入到帧中
//	RS232_TX_BUF[7]=(crc_cal_value(RS232_TX_BUF,8)>>8)&0x00ff;//加入高字节
//	printf("Send_ReadCommandtoInverter start\r\n");
//		for (i = 0; i<8; i++)
//		printf("%x\n", RS232_TX_BUF[i]);
//	printf("shuzu zhenghe wancheng\\r\n");
//	delay_ms(4);//10Khz的计数频率，技计数到5000是500ms，在这里3.5ms
////	TIM3_Init(1000/BaudRate*10*3.5/10,7199);
//	printf("shuzu fasong kaishi\r\n");
//	myUSART_Sendarr(USART2,RS232_TX_BUF,8);
//	printf("shuzu fasong over\r\n");
//		printf("Send_ReadCommandtoInverter over\r\n");


//}


//void USART2_receive(void)
//	{//USART2的接收数据
//				printf("USART RECEIVE START");
//	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//观察是否接收到数据
//		{
//			USART_ClearITPendingBit(USART2,USART_IT_RXNE);//清除标志位
//			RS232_RX_BUF[RS232_RX_CNT++]=USART_ReceiveData(USART2);
//          if( RS232_RX_CNT==8)
//                RS232_RX_CNT=0;    
//		}
//						printf("USART RECEIVE over");
//	}

//void USART1_send(void)
//	{	printf("USART SEND START");
//		//static u8 k;
//		USART_ClearFlag(USART1,USART_FLAG_TC);//清除USART1的发送标志位
//		if(USART_GetITStatus(USART1,USART_IT_RXNE)!=Bit_RESET)
//			{
//				USART_SendData(USART1,usart2Buf);
//						while(USART_GetFlagStatus(USART1,USART_IT_RXNE)!=RESET);
//			}
//			printf("USART SEND over");
//	}





int main(void)
{	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	    USART1_Config();//串口1参数配置初始化
   	//	USART2_Config();//串口2参数配置初始化 


    while (1)
    {

			USART1_SEND();//UASRT1将UASRT2收到的信息传给PC	

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






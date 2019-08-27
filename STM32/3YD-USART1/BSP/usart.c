/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：usart.c
 * 描述    ：将printf函数重定向到USART1。这样就可以用printf函数将单片机的数据打
 *           印到PC上的超级终端或串口调试助手。     
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2017-04-20
 * 硬件连接: TX->PA9;RX->PA10
 * 调试方式：J-Link-OB
**********************************************************************************/	

//头文件
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
  * @brief  USART1 GPIO 配置,工作模式配置。9600-8-N-1
  * @param  无
  * @retval 无
  */
void USART1_Config(void)
{	
    GPIO_InitTypeDef GPIO_InitStructure;	
    USART_InitTypeDef USART_InitStructure;  //定义串口初始化结构体
   NVIC_InitTypeDef NVIC_InitStructure;
//	  USART_ClockInitTypeDef USART_ClockInitStruct1;//初始化时钟信号
    
	RCC_APB2PeriphClockCmd( USART_RCC | USART_GPIO_RCC,ENABLE);//1.串口时钟使能（ RCC_APB2Periph_USART1）以及GPIO使能（RCC_APB2Periph_GPIOA）

    /*USART1_TX ->PA9*/			
    GPIO_InitStructure.GPIO_Pin = USART_TX;	       //2.选中串口默认输出管脚         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // 定义管脚9的模式 推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;// 定义输出最大速率 
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);           //调用函数，把结构体参数输入进行初始化		   
    /*USART1_RX ->PA10*/
    GPIO_InitStructure.GPIO_Pin = USART_RX;	        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // 定义管脚10的模式 浮空
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);
	//GPIO的初始化完成
	//时钟初始化
//	USART_ClockInitStruct1.USART_Clock=USART_Clock_Disable;
//	USART_ClockInitStruct1.USART_CPHA=USART_CPHA_2Edge;
//	USART_ClockInitStruct1.USART_CPOL=USART_CPOL_Low;
//	USART_ClockInitStruct1.USART_LastBit=USART_LastBit_Disable;
//	USART_ClockInit(USART1, &USART_ClockInitStruct1);
	//串口的初始化
    /*串口通讯参数设置*/
    USART_InitStructure.USART_BaudRate = BaudRate; //波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位1位
    USART_InitStructure.USART_Parity = USART_Parity_No;		//校验位 无
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//使能接收和发送引脚

    USART_Init(USART, &USART_InitStructure);//串口初始化

		 NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//中断优先级为3
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ使能
		 NVIC_Init(&NVIC_InitStructure);	//初始化中断

		USART_ITConfig(USART, USART_IT_RXNE, ENABLE);//开启中断接收
    USART_Cmd(USART, ENABLE);//3.使能串口
		
}


void USART1_IRQHandler(void)//UART1接收数据函数中断
	{
		uint8_t res; 

		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //判断读取数据寄存器是否为空
			{
				res =USART_ReceiveData(USART1); //读取USART2收到的数据
				usart1Buf[usart1Len]=res; //记录收到的数据到内存

				usart1Len++; //下标加1
					if(usart1Len>=120)
						usart1Len=0; 
			}
    if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET)//如果发生溢出，先读SR，再读DR寄存器
			{
        USART_ReceiveData(USART1);
        USART_ClearFlag(USART1,USART_FLAG_ORE);
			}
		USART_ClearFlag(USART1,USART_IT_RXNE); //清除接收中断

	}

void myUSART_Sendbyte(USART_TypeDef* USARTx, uint16_t Data)//给串口发送一位字符
{
    while((USARTx->SR&0X40)==0);

//		USARTx->DR = (Data & (uint16_t)0x01FF);
		USART_SendData(USARTx,Data);
//		printf("%x\n", Data);

}

void myUSART_Sendarr(USART_TypeDef* USARTx, u8 a[] ,uint8_t len)//给串口发送一个数组字符
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
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//发送缓冲区标志为空
 			myUSART_Sendarr(USART1,usart2Buf,8);//把UASRT2收到的内容通过UASRT1发过去
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//发送完成标志
		if(usart2Len >= 120)									//长度限制
		{usart2Len=0;}

//	for (i = 0; i<8; i++)
//	{//printf("%x\n", usart2Buf[i]);
//		USART_SendData(USART1,usart2Buf[i]);}

	
}


void USART2_Config(void)
{	
       GPIO_InitTypeDef GPIO_InitStructure2;	
    USART_InitTypeDef USART_InitStructure2;  //定义串口初始化结构体
    NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_APB1PeriphClockCmd(USART2_RCC ,ENABLE);//串口时钟使能（ RCC_APB1Periph_USART2
  RCC_APB2PeriphClockCmd(USART2_GPIO_RCC,ENABLE);//GPIO使能（RCC_APB2Periph_GPIOA）
    /*USART1_TX ->PA2*/			
    GPIO_InitStructure2.GPIO_Pin = USART2_TX;	       //2.选中串口默认输出管脚         
    GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_AF_PP;  // 定义管脚2的模式 推挽
    GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;// 定义输出最大速率 
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure2);           //调用函数，把结构体参数输入进行初始化		   
    /*USART1_RX ->PA3*/
    GPIO_InitStructure2.GPIO_Pin = USART2_RX;	        
    GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // 定义管脚3的模式 浮空
    GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure2);
	//GPIO的初始化完成

    /*串口通讯参数设置*/
    USART_InitStructure2.USART_BaudRate = BaudRate; //波特率
    USART_InitStructure2.USART_WordLength = USART_WordLength_8b; //数据位8位
    USART_InitStructure2.USART_StopBits = USART_StopBits_1;	//停止位1位
    USART_InitStructure2.USART_Parity = USART_Parity_No;		//校验位 无
    USART_InitStructure2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
    USART_InitStructure2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//使能接收和发送引脚

    USART_Init(USART2, &USART_InitStructure2);//串口初始化

		 NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//中断优先级为3
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ使能
		 NVIC_Init(&NVIC_InitStructure);	//初始化中断


		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART2, ENABLE);//3.使能串口
		
	
}






void USART2_IRQHandler(void)//UART2接收数据函数中断
	{
		uint8_t res; 

		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //判断读取数据寄存器是否为空
			{
				res =USART_ReceiveData(USART2); //读取USART2收到的数据
				usart2Buf[usart2Len]=res; //记录收到的数据到内存

				usart2Len++; //下标加1
					if(usart2Len>=8)
						usart2Len=0; 
			}
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)//如果发生溢出，先读SR，再读DR寄存器
			{
        USART_ReceiveData(USART2);
        USART_ClearFlag(USART2,USART_FLAG_ORE);
			}
		USART_ClearFlag(USART2,USART_IT_RXNE); //清除接收中断

	}

	
	void USART2_SEND(void)//wang PC fasong
	{int i;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);//发送缓冲区标志为空
 		for(i=0;i<15;i++)
			{
				if(usart1Buf[i*8]==0x0b)
				{
					myUSART_Sendarr(USART2,&usart1Buf[8*i],8);
				}//把UASRT1收到的内容通过UASRT2发过去,以八位为单位发送
			}
				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//发送缓冲区标志为空
	if(usart1Len >= 120)									//长度限制
	{
	usart1Len=0;
	}
	//for (i = 0; i<8; i++)
	{//printf("%x\n", usart1Buf[i]);
	}
	
	 //printf("U1fasonghanshu jieshu\r\n");
	
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

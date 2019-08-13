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
//    NVIC_InitTypeDef NVIC_InitStructure;
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
		USART_ITConfig(USART1,USART_IT_TC|USART_IT_CTS|USART_IT_ERR|USART_IT_TXE|USART_IT_RXNE,DISABLE);//关闭了中断口

//		 NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//中断优先级为3
//		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ使能
//		 NVIC_Init(&NVIC_InitStructure);	//初始化中断


	 USART_ClearFlag(USART,USART_FLAG_TC);
//		USART_ITConfig(USART, USART_IT_RXNE, ENABLE);//开启中断
//		USART_ITConfig(USART, USART_IT_TXE, ENABLE);		
    USART_Cmd(USART, ENABLE);//3.使能串口
		
		USART_Init(USART,&USART_InitStructure);
}

void USART2_Config(void)
{	
       GPIO_InitTypeDef GPIO_InitStructure2;	
    USART_InitTypeDef USART_InitStructure2;  //定义串口初始化结构体
//    NVIC_InitTypeDef NVIC_InitStructure;
//	  USART_ClockInitTypeDef USART_ClockInitStruct1;//初始化时钟信号
    
	RCC_APB1PeriphClockCmd( USART_RCC | USART_GPIO_RCC,ENABLE);//1.串口时钟使能（ RCC_APB2Periph_USART1）以及GPIO使能（RCC_APB2Periph_GPIOA）

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
	//时钟初始化
//	USART_ClockInitStruct1.USART_Clock=USART_Clock_Disable;
//	USART_ClockInitStruct1.USART_CPHA=USART_CPHA_2Edge;
//	USART_ClockInitStruct1.USART_CPOL=USART_CPOL_Low;
//	USART_ClockInitStruct1.USART_LastBit=USART_LastBit_Disable;
//	USART_ClockInit(USART1, &USART_ClockInitStruct1);
	//串口的初始化
    /*串口通讯参数设置*/
    USART_InitStructure2.USART_BaudRate = BaudRate; //波特率
    USART_InitStructure2.USART_WordLength = USART_WordLength_8b; //数据位8位
    USART_InitStructure2.USART_StopBits = USART_StopBits_1;	//停止位1位
    USART_InitStructure2.USART_Parity = USART_Parity_No;		//校验位 无
    USART_InitStructure2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
    USART_InitStructure2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//使能接收和发送引脚

    USART_Init(USART2, &USART_InitStructure2);//串口初始化
		USART_ITConfig(USART2,USART_IT_TC|USART_IT_CTS|USART_IT_ERR|USART_IT_TXE|USART_IT_RXNE,ENABLE);//关闭了中断口

//		 NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//中断优先级为3
//		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ使能
//		 NVIC_Init(&NVIC_InitStructure);	//初始化中断


	 USART_ClearFlag(USART2,USART_FLAG_TC);
//		USART_ITConfig(USART, USART_IT_RXNE, ENABLE);//开启中断
//		USART_ITConfig(USART, USART_IT_TXE, ENABLE);		
    USART_Cmd(USART2, ENABLE);//3.使能串口
		
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

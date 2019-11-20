

//头文件
#include "usart.h"
#include "ringbuff.h"

u8 usart1Buf[30];
u8 usart1Len;
u8 usart2Buf[30];
u8 usart2Len;
//char hexbuffer[5];//用于存放十六进制的速度值
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
  * @brief  USART1 GPIO 配置,工作模式配置。19600-8-N-1
  * @param  无
  * @retval 无
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
/*环形队列存储数据*/
		ringBuffer_t buffer = {0,0,{0}};//u2接收端行程一个环形队列初始化为0
		ringBuffer_t buffer_A = {0,0,{0}};//U1接收端形成一个环形队列并初始化为0


void RingBuf_Write(float data){//向缓冲区写入一个字节
		buffer.ringBuf[buffer.tailPosition]=data;//从尾部追加
	
		if(++buffer.tailPosition >= BUFFER_MAX)//尾结点偏移
			buffer.tailPosition = 0;//大于数组长度，最大数组长度归零，形成环形数组
	//若尾部节点追到头部节点，则修改头节点偏移位置丢弃早期数据
		if(buffer.tailPosition == buffer.headPosition)
			if(++buffer.headPosition >= BUFFER_MAX)
			buffer.headPosition=0;
}

void RingBuf_Write_A(float data){//向缓冲区写入一个字节
	buffer_A.ringBuf[buffer_A.tailPosition]=data;//从尾部追加
	/*提取位置参考值*/
	if(buffer_A.ringBuf[buffer_A.tailPosition]==0XFF||buffer_A.ringBuf[(buffer_A.tailPosition)-1]==0XFE)// posref1 posref2 FE FF 以后面两个为标志位
	{
		posref[0]=buffer_A.ringBuf[(buffer_A.tailPosition)-3];
		posref[1]=buffer_A.ringBuf[(buffer_A.tailPosition)-2];
	}
	/*把位置参考值提取出来*/
		if(++buffer_A.tailPosition >= BUFFER_MAX)//尾结点偏移
		buffer_A.tailPosition = 0;//大于数组长度，最大数组长度归零，形成环形数组
	//若尾部节点追到头部节点，则修改头节点偏移位置丢弃早期数据
	if(buffer_A.tailPosition == buffer_A.headPosition)
		if(++buffer_A.headPosition >= BUFFER_MAX)
			buffer_A.headPosition=0;
}



unsigned char RingBuf_Read( float* pData){//读取缓存区一个字节数据
	if(buffer.headPosition == buffer.tailPosition)
		{//头尾相接表示缓冲区为空
		return 0;//读取失败 返回0
		}
	else{
		*pData = buffer.ringBuf[buffer.headPosition];//如果缓冲区为空则取头节点并偏移头节点
		if(++buffer.headPosition>=BUFFER_MAX)
			buffer.headPosition = 0;
		return 1;//读取成功返回1
	}
}

unsigned char RingBuf_Read_A( float* pData){//读取缓存区一个字节数据
	if(buffer_A.headPosition == buffer_A.tailPosition)
		{//头尾相接表示缓冲区为空
		return 0;//读取失败 返回0
		}
	else{
		*pData = buffer_A.ringBuf[buffer_A.headPosition];//如果缓冲区为空则取头节点并偏移头节点
		if(++buffer_A.headPosition>=BUFFER_MAX)
			buffer_A.headPosition = 0;
		return 1;//读取成功返回1
	}
}
/*环形队列定义及方法*/


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



	void USART1_IRQHandler(void)//UART1????????
	{
		uint8_t res; 
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //?????????????
			{
				res =USART_ReceiveData(USART1); //??USART1?????
				usart1Buf[usart1Len++]=res; //??????????
				if(usart1Len>5)
					{
						if(usart1Buf[usart1Len-2]==0x10&&usart1Buf[usart1Len-1]==0x18)//检测第一第二位是否为10 18 ，若是的话发送位置信息给上位机
							{ 
								send_pos();
								usart1Len=0;
							}
						if(usart1Buf[usart1Len-5]==0x06&&usart1Buf[usart1Len-4]==0x10&&usart1Buf[usart1Len-3]==0x07)//传入速度写指令06 10 07 然后提取速度值
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

void myUSART_Sendbyte(USART_TypeDef* USARTx, uint16_t Data)//给串口发送一位字符
{
    while((USARTx->SR&0X40)==0);
		USART_SendData(USARTx,Data);
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

int crc_cal_value( char *data_value,unsigned char data_length)//CRC校验位计算
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
		把位置信息包装成modbus协议发送出去
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
		if(RingBuf_Read(&ress)==1)//读环形队列里面的数据
		{
//			if(ress==0x0b)
//			send_pos();//发送位置信息帧
		myUSART_Sendbyte(USART1,ress);
		}

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
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ使能
		 NVIC_Init(&NVIC_InitStructure);	//初始化中断

		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART2, ENABLE);//3.使能串口	
}

void USART2_IRQHandler(void)//UART2接收数据函数中断
	{
		uint8_t res; 
//for( i=0;i<5;i++)
		{if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //判断读取数据寄存器是否为空
			{
				res =USART_ReceiveData(USART2); //读取USART2收到的数据
				usart2Buf[usart2Len]=res; //记录收到的数据到内存	

				RingBuf_Write(res);//向环形队列中写入数据

				USART_ClearFlag(USART2,USART_IT_RXNE); //清除接收中断
					}
			}
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)//如果发生溢出，先读SR，再读DR寄存器
			{
        USART_ReceiveData(USART2);
        USART_ClearFlag(USART2,USART_FLAG_ORE);
			}
		USART_ClearFlag(USART2,USART_IT_RXNE); //清除接收中断
		}
	

	void USART2_SEND(void)//wang PC fasong
	{

		float ress;
//		for(i=0;i<15;i++){//发送地址为0b的发送出去
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

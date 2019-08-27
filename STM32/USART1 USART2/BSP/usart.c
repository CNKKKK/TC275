/******************** (C) COPYRIGHT  Ô´µØ¹¤×÷ÊÒ ********************************
 * ÎÄ¼þÃû  £ºusart.c
 * ÃèÊö    £º½«printfº¯ÊýÖØ¶¨Ïòµ½USART1¡£ÕâÑù¾Í¿ÉÒÔÓÃprintfº¯Êý½«µ¥Æ¬»úµÄÊý¾Ý´ò
 *           Ó¡µ½PCÉÏµÄ³¬¼¶ÖÕ¶Ë»ò´®¿Úµ÷ÊÔÖúÊÖ¡£     
 * ×÷Õß    £ºzhuoyingxingyu
 * ÌÔ±¦    £ºÔ´µØ¹¤×÷ÊÒhttp://vcc-gnd.taobao.com/
 * ÂÛÌ³µØÖ·£º¼«¿ÍÔ°µØ-Ç¶ÈëÊ½¿ª·¢ÂÛÌ³http://vcc-gnd.com/
 * °æ±¾¸üÐÂ: 2017-04-20
 * Ó²¼þÁ¬½Ó: TX->PA9;RX->PA10
 * µ÷ÊÔ·½Ê½£ºJ-Link-OB
**********************************************************************************/	

//Í·ÎÄ¼þ
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
  * @brief  USART1 GPIO ÅäÖÃ,¹¤×÷Ä£Ê½ÅäÖÃ¡£9600-8-N-1
  * @param  ÎÞ
  * @retval ÎÞ
  */
void USART1_Config(void)
{	
    GPIO_InitTypeDef GPIO_InitStructure;	
    USART_InitTypeDef USART_InitStructure;  //¶¨Òå´®¿Ú³õÊ¼»¯½á¹¹Ìå
    NVIC_InitTypeDef NVIC_InitStructure;
//	  USART_ClockInitTypeDef USART_ClockInitStruct1;//³õÊ¼»¯Ê±ÖÓÐÅºÅ
    
	RCC_APB2PeriphClockCmd( USART_RCC | USART_GPIO_RCC,ENABLE);//1.´®¿ÚÊ±ÖÓÊ¹ÄÜ£¨ RCC_APB2Periph_USART1£©ÒÔ¼°GPIOÊ¹ÄÜ£¨RCC_APB2Periph_GPIOA£©

    /*USART1_TX ->PA9*/			
    GPIO_InitStructure.GPIO_Pin = USART_TX;	       //2.Ñ¡ÖÐ´®¿ÚÄ¬ÈÏÊä³ö¹Ü½Å         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // ¶¨Òå¹Ü½Å9µÄÄ£Ê½ ÍÆÍì
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;// ¶¨ÒåÊä³ö×î´óËÙÂÊ 
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);           //µ÷ÓÃº¯Êý£¬°Ñ½á¹¹Ìå²ÎÊýÊäÈë½øÐÐ³õÊ¼»¯		   
    /*USART1_RX ->PA10*/
    GPIO_InitStructure.GPIO_Pin = USART_RX;	        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // ¶¨Òå¹Ü½Å10µÄÄ£Ê½ ¸¡¿Õ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);
	//GPIOµÄ³õÊ¼»¯Íê³É
	//Ê±ÖÓ³õÊ¼»¯
//	USART_ClockInitStruct1.USART_Clock=USART_Clock_Disable;
//	USART_ClockInitStruct1.USART_CPHA=USART_CPHA_2Edge;
//	USART_ClockInitStruct1.USART_CPOL=USART_CPOL_Low;
//	USART_ClockInitStruct1.USART_LastBit=USART_LastBit_Disable;
//	USART_ClockInit(USART1, &USART_ClockInitStruct1);
	//´®¿ÚµÄ³õÊ¼»¯
    /*´®¿ÚÍ¨Ñ¶²ÎÊýÉèÖÃ*/
    USART_InitStructure.USART_BaudRate = BaudRate; //²¨ÌØÂÊ
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //Êý¾ÝÎ»8Î»
    USART_InitStructure.USART_StopBits = USART_StopBits_1;	//Í£Ö¹Î»1Î»
    USART_InitStructure.USART_Parity = USART_Parity_No;		//Ð£ÑéÎ» ÎÞ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÁ÷¿ØÖÆ
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//Ê¹ÄÜ½ÓÊÕºÍ·¢ËÍÒý½Å

    USART_Init(USART, &USART_InitStructure);//´®¿Ú³õÊ¼»¯

		 NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//ÖÐ¶ÏÓÅÏÈ¼¶Îª3
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//×ÓÓÅÏÈ¼¶2
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÊ¹ÄÜ
		 NVIC_Init(&NVIC_InitStructure);	//³õÊ¼»¯ÖÐ¶Ï

	 USART_ClearFlag(USART,USART_FLAG_TC);
		USART_ITConfig(USART, USART_IT_RXNE, ENABLE);//¿ªÆôÖÐ¶Ï½ÓÊÕ

    USART_Cmd(USART, ENABLE);//3.Ê¹ÄÜ´®¿Ú
		
}


void USART1_IRQHandler(void)//UART2½ÓÊÕÊý¾Ýº¯Êý
	{
		uint8_t res1; 
				printf("U1ÖÐ¶Ï½ÓÊÕº¯Êý¿ªÊ¼\r\n");
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //
			{
				res1 =USART_ReceiveData(USART1); //¶ÁÈ¡USART2ÊÕµ½µÄÊý¾Ý
				usart1Buf[usart1Len]=res1; //¼ÇÂ¼ÊÕµ½µÄÊý¾Ýµ½ÄÚ´æ

				usart1Len++; //ÏÂ±ê¼Ó1
					if(usart1Len>8)
						usart1Len=0; 
			}
    if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET)//Èç¹û·¢ÉúÒç³ö£¬ÏÈ¶ÁSR£¬ÔÙ¶ÁDR¼Ä´æÆ÷
			{
        USART_ReceiveData(USART1);
        USART_ClearFlag(USART1,USART_FLAG_ORE);
			}
		USART_ClearFlag(USART1,USART_IT_RXNE); //Çå³ý½ÓÊÕÖÐ¶Ï
					printf("U1ÖÐ¶Ï½ÓÊÕº¯Êýover\r\n");
	}

void myUSART_Sendbyte(USART_TypeDef* USARTx, uint16_t Data)//¸ø´®¿Ú·¢ËÍÒ»Î»×Ö·û
{printf("stepb1\r\n");
    while((USARTx->SR&0X40)==0);
		USARTx->DR = (Data & (uint16_t)0x01FF);
	printf("stepb2\r\n");
}

void myUSART_Sendarr(USART_TypeDef* USARTx, u8 a[] ,uint8_t len)//¸ø´®¿Ú·¢ËÍÒ»¸öÊý×é×Ö·û
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
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//±êÖ¾Î»ÇåÁã
	{
		if(usart1Len >= 8)									//³¤¶ÈÏÞÖÆ
	
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
 	myUSART_Sendarr(USART1,usart2Buf,8);
	 printf("U1 fasonghanshu jieshu\r\n");
	
}


void USART2_Config(void)
{	
       GPIO_InitTypeDef GPIO_InitStructure2;	
    USART_InitTypeDef USART_InitStructure2;  //¶¨Òå´®¿Ú³õÊ¼»¯½á¹¹Ìå
    NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_APB1PeriphClockCmd(USART2_RCC ,ENABLE);//´®¿ÚÊ±ÖÓÊ¹ÄÜ£¨ RCC_APB1Periph_USART2
  RCC_APB2PeriphClockCmd(USART2_GPIO_RCC,ENABLE);//GPIOÊ¹ÄÜ£¨RCC_APB2Periph_GPIOA£©
    /*USART1_TX ->PA2*/			
    GPIO_InitStructure2.GPIO_Pin = USART2_TX;	       //2.Ñ¡ÖÐ´®¿ÚÄ¬ÈÏÊä³ö¹Ü½Å         
    GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_AF_PP;  // ¶¨Òå¹Ü½Å2µÄÄ£Ê½ ÍÆÍì
    GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;// ¶¨ÒåÊä³ö×î´óËÙÂÊ 
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure2);           //µ÷ÓÃº¯Êý£¬°Ñ½á¹¹Ìå²ÎÊýÊäÈë½øÐÐ³õÊ¼»¯		   
    /*USART1_RX ->PA3*/
    GPIO_InitStructure2.GPIO_Pin = USART2_RX;	        
    GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // ¶¨Òå¹Ü½Å3µÄÄ£Ê½ ¸¡¿Õ
    GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure2);
	//GPIOµÄ³õÊ¼»¯Íê³É

    /*´®¿ÚÍ¨Ñ¶²ÎÊýÉèÖÃ*/
    USART_InitStructure2.USART_BaudRate = BaudRate; //²¨ÌØÂÊ
    USART_InitStructure2.USART_WordLength = USART_WordLength_8b; //Êý¾ÝÎ»8Î»
    USART_InitStructure2.USART_StopBits = USART_StopBits_1;	//Í£Ö¹Î»1Î»
    USART_InitStructure2.USART_Parity = USART_Parity_No;		//Ð£ÑéÎ» ÎÞ
    USART_InitStructure2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÁ÷¿ØÖÆ
    USART_InitStructure2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//Ê¹ÄÜ½ÓÊÕºÍ·¢ËÍÒý½Å

    USART_Init(USART2, &USART_InitStructure2);//´®¿Ú³õÊ¼»¯

		 NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//ÖÐ¶ÏÓÅÏÈ¼¶Îª3
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//×ÓÓÅÏÈ¼¶3
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÊ¹ÄÜ
		 NVIC_Init(&NVIC_InitStructure);	//³õÊ¼»¯ÖÐ¶Ï


		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//¿ªÆôÖÐ¶Ï
    USART_Cmd(USART2, ENABLE);//3.Ê¹ÄÜ´®¿Ú
		
	
}






void USART2_IRQHandler(void)//UART2½ÓÊÕÊý¾Ýº¯Êý
	{
		uint8_t res; 
		printf("U2ÖÐ¶Ï½ÓÊÕº¯Êý¿ªÊ¼\r\n");
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //?????  
			{
				res =USART_ReceiveData(USART2); //¶ÁÈ¡USART2ÊÕµ½µÄÊý¾Ý
				usart2Buf[usart2Len]=res; //¼ÇÂ¼ÊÕµ½µÄÊý¾Ýµ½ÄÚ´æ

				usart2Len++; //ÏÂ±ê¼Ó1
					if(usart2Len>8)
						usart2Len=0; 
			}
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)//Èç¹û·¢ÉúÒç³ö£¬ÏÈ¶ÁSR£¬ÔÙ¶ÁDR¼Ä´æÆ÷
			{
        USART_ReceiveData(USART2);
        USART_ClearFlag(USART2,USART_FLAG_ORE);
			}
		USART_ClearFlag(USART2,USART_IT_RXNE); //Çå³ý½ÓÊÕÖÐ¶Ï
				printf("U2ÖÐ¶Ï½ÓÊÕº¯OVER\r\n");	
	}

	
	
	void USART2_SEND(void)//wang PC fasong
{
  printf("U2 fasong kaishi");
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	//±êÖ¾Î»ÇåÁã
	{
		if(usart2Len >= 8)									//³¤¶ÈÏÞÖÆ
	
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

/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：main.c
 * 描述    ：通过串口1打印输出自己输入的字符串；（串口信息为：9600，N,8，1）  
 * 库版本  ：V3.50
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2017-04-20
 * 调试方式：J-Link-OB
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



void USART_R_T(char *str)
{	u8 uart1_get_data; //存储收到的数据
while(1)

{

if(USART_GetFlagStatus(USART1,USART_IT_RXNE)==SET)
{ 
uart1_get_data = USART_ReceiveData(USART1);
	USART_SendData(USART1,uart1_get_data);

}

}
}

int main(void)
{
    USART1_Config();//串口参数配置初始化
    while (1)
    {
      while (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
      USART_SendData(USART1,USART_ReceiveData(USART1));
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






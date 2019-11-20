/********************  ********************************
位置闭环的构建

**********************************************************************************/	

//头文件
#include "stm32f10x.h"
#include "usart.h"
#include "dac.h"
#include "time4.h"
#include "TMER.h"

/**
  * @file   main
  * @brief  Main program.
  * @param  None
  * @retval None
  */






//unsigned int crc_cal_value(unsigned char *data_value,unsigned char data_length)//计算CRC的值
//{
//	int i;
//    unsigned int crc_value = 0xffff;//CRC输出为16进制的16位
//    while(data_length--)
//    {
//        crc_value ^= *data_value++;
//        for(i=0;i<8;i++)
//				{
//    if(crc_value&0x0001)
//        crc_value = (crc_value>>1)^0xa001;
//		else
//    crc_value = crc_value>>1;
//				}
//		}			
//	return(crc_value);
//}
extern float Elecspeed;//转速值
extern float angle_pu;//角度值
int espd;//为了串口表示的速度值
int angle,oldangle;//为了串口表示的角度值
extern char posref[2];
float angle_gi;//设定的位置参考值
int i=0;
int main(void)
{	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		USART1_Config();//串口1参数配置初始化
		USART2_Config();//串口2参数配置初始化
//	TIM6_Configuration();//计时器初始化 为DAC的PA5口提供三角波for test
		DAC_configration();//DAC初始化
		Timer3_Init();  //定时器中断 计算位置等信息
		Timer4_Init();//正交编码器定时器中断

	while (1)
    {
		TIM4_getCount();//计算角速度，位置等信息
		espd=Elecspeed*1;//for test
		angle=angle_pu*100;//for test
//  Dac1_Set_Vol(100);/*0000-3300对应-10-10hz*///PA4口输出固定电平	for test
	Dac1_Set_Vol(PIfunction(4.125,8.5));//PI控制输出
//printf("电角度数值 = %d\n",posref[0]);//for test
//		printf("转速 = %d\n",PIfunction(4.125,8.5));
		USART2_SEND();//UASRT2将UASRT1收到的信息传给PC		
		USART1_SEND_02();//UASRT1将UASRT2收到的信息传给PC	
//		i++;
//		if(i%1000==0)
//			send_pos();//发送位置信息
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






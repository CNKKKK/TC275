/********************  ********************************
λ�ñջ��Ĺ���

**********************************************************************************/	

//ͷ�ļ�
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






//unsigned int crc_cal_value(unsigned char *data_value,unsigned char data_length)//����CRC��ֵ
//{
//	int i;
//    unsigned int crc_value = 0xffff;//CRC���Ϊ16���Ƶ�16λ
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
extern float Elecspeed;//ת��ֵ
extern float angle_pu;//�Ƕ�ֵ
int espd;//Ϊ�˴��ڱ�ʾ���ٶ�ֵ
int angle,oldangle;//Ϊ�˴��ڱ�ʾ�ĽǶ�ֵ
extern char posref[2];
float angle_gi;//�趨��λ�òο�ֵ
int i=0;
int main(void)
{	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		USART1_Config();//����1�������ó�ʼ��
		USART2_Config();//����2�������ó�ʼ��
//	TIM6_Configuration();//��ʱ����ʼ�� ΪDAC��PA5���ṩ���ǲ�for test
		DAC_configration();//DAC��ʼ��
		Timer3_Init();  //��ʱ���ж� ����λ�õ���Ϣ
		Timer4_Init();//������������ʱ���ж�

	while (1)
    {
		TIM4_getCount();//������ٶȣ�λ�õ���Ϣ
		espd=Elecspeed*1;//for test
		angle=angle_pu*100;//for test
//  Dac1_Set_Vol(100);/*0000-3300��Ӧ-10-10hz*///PA4������̶���ƽ	for test
	Dac1_Set_Vol(PIfunction(4.125,8.5));//PI�������
//printf("��Ƕ���ֵ = %d\n",posref[0]);//for test
//		printf("ת�� = %d\n",PIfunction(4.125,8.5));
		USART2_SEND();//UASRT2��UASRT1�յ�����Ϣ����PC		
		USART1_SEND_02();//UASRT1��UASRT2�յ�����Ϣ����PC	
//		i++;
//		if(i%1000==0)
//			send_pos();//����λ����Ϣ
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






#include "TMER.h"
#include"time4.h"
#include <stdlib.h>


u16 PRR=625;
float Elecspeed;
u16 Old_bias=0;
float temp;
float Old_angle_pu,angle_pu;
float ElecThetaChagTimeRatio=1000;
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��APB1��2��Ƶ,��APB1Ϊ36M
//arr:�Զ�����װֵ
//psc:ʱ��Ԥ��Ƶ��
//ʹ�ö�ʱ��3!
void Timer3_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 10-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ�������ֵ,������10Ϊ1ms
    TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz����Ƶ��  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM�����ؼ���
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
     
    TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
        TIM3, //TIM3
        TIM_IT_Update  |  //TIM �ж�Դ
        TIM_IT_Trigger,   //TIM �����ж�Դ
        ENABLE  //ʹ��
        );
     
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ��NVIC�Ĵ���

    TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
                             
}
//u16 Getangle_puValue(u16 angle_pu)
//{
//	return angle_pu;
//}
//u16 GetElecspeedValue(u16 Elecspeed)
//{
//	return Elecspeed;
//}

void TIM3_IRQHandler(void)   //TIM3�жϷ�����
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
        {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
     
		Old_angle_pu=angle_pu;														//��ȡ��ǰ�Ľ�λ��
		temp=TIM4_getCount()-Old_bias;
		if(temp <= 0)
		{temp = temp + (PRR*4*4);	}							//��ת

		angle_pu =  temp / (PRR*4); 									//��λ��
		if(abs(angle_pu-Old_angle_pu)>(0.5f))
		{
			if((angle_pu+(1.0f)-Old_angle_pu)<(0.5f))
				Elecspeed=ElecThetaChagTimeRatio*(angle_pu+(1.0f)-Old_angle_pu);
			else if((angle_pu-(1.0f)-Old_angle_pu)>-(0.5f))
				Elecspeed=ElecThetaChagTimeRatio*(angle_pu-(1.0f)-Old_angle_pu);
		}
		else
			Elecspeed=ElecThetaChagTimeRatio*(angle_pu-Old_angle_pu);
//					Getangle_puValue(angle_pu);
//					GetElecspeedValue(Elecspeed);

        //GPIO_WriteBit(GPIOD, GPIO_Pin_2, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_2)));

        }
}
/*
 temp = pQepData->qep_cnt - gHalHandle->qep_bias;
 if(temp < 0)

  temp = temp +  (ENCODER_PPR * 4 * 4);

 angle_pu =  temp / (ENCODER_PPR * 4);
	
	
	 gCtrlHandle->angle_pu = angle_pu;
	
	
	 if( MATH_abs(obj->ElecTheta - obj->OldElecTheta) > (0.5f) )
 {
  if( (obj->ElecTheta + (1.0f)- obj->OldElecTheta)<(0.5f) )
  {
   obj->ElecSpeed = ( obj->ElecThetaChagTimeRatio * (obj->ElecTheta +(1.0f)- obj->OldElecTheta) );
  }
  else if( (obj->ElecTheta - (1.0f) - obj->OldElecTheta)>(-0.5f) )
  {
   obj->ElecSpeed = ( obj->ElecThetaChagTimeRatio * (obj->ElecTheta -(1.0f)- obj->OldElecTheta) );
  }
 }
 else
 {
  obj->ElecSpeed = ( obj->ElecThetaChagTimeRatio * (obj->ElecTheta - obj->OldElecTheta) );
 }
 
 */

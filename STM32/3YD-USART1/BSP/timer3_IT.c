#include "TMER.h"
#include"time4.h"
#include <stdlib.h>


u16 PRR=625;
float Elecspeed;
u16 Old_bias=0;
float temp;
float Old_angle_pu,angle_pu;
float ElecThetaChagTimeRatio=1000;
//通用定时器中断初始化
//这里时钟选用APB1的2倍频,而APB1为36M
//arr:自动填重装值
//psc:时钟预分频数
//使用定时器3!
void Timer3_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 10-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期值,计数到10为1ms
    TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz计数频率  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM上升沿计数
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
     
    TIM_ITConfig(  //使能或者失能指定的TIM中断
        TIM3, //TIM3
        TIM_IT_Update  |  //TIM 中断源
        TIM_IT_Trigger,   //TIM 触发中断源
        ENABLE  //使能
        );
     
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //抢占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化NVIC寄存器

    TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
                             
}
//u16 Getangle_puValue(u16 angle_pu)
//{
//	return angle_pu;
//}
//u16 GetElecspeedValue(u16 Elecspeed)
//{
//	return Elecspeed;
//}

void TIM3_IRQHandler(void)   //TIM3中断服务函数
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
        {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
     
		Old_angle_pu=angle_pu;														//获取先前的角位置
		temp=TIM4_getCount()-Old_bias;
		if(temp <= 0)
		{temp = temp + (PRR*4*4);	}							//反转

		angle_pu =  temp / (PRR*4); 									//角位置
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

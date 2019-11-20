#include"time4.h"


u16	Parameter = 2500;			//输入编码器线数

u16 Count;




void Timer4_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;      
    //PB6 ch1  ,PB7 ch2

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能TIM4时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIO时钟
	
	GPIO_StructInit(&GPIO_InitStructure);//缺省GPIO配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  TIM_DeInit(TIM4);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);//缺省
    TIM_TimeBaseStructure.TIM_Period = Parameter*4;  //设定计数器重装值   TIMx_ARR = 359*4
    TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3时钟预分频
    TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//设置时钟分割 T_dts = T_ck_int    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 

		TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//使用编码模式3,上升下降都计数
    TIM_ICStructInit(&TIM_ICInitStructure);//将结构体的内容缺省录入
    TIM_ICInitStructure.TIM_ICFilter = 6;  //选择输入比较滤波器 
    TIM_ICInit(TIM4, &TIM_ICInitStructure);//将TIM_ICInitStructure中的指定参数初始化TIM4

//  TIM_ARRPreloadConfig(TIM4, ENABLE);//使能预装载
//    TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清楚TIM3的更新标志位
//    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//运行更新中断
    //Reset counter
    TIM4->CNT = 0;//

    TIM_Cmd(TIM4, ENABLE);   //启动TIM4定时器
		
		/*  用寄存器的方法配置TIM3
void TIM3_Mode_Config(void)
{
    ///TIM3 clock source enable 
    RCC->APB1ENR|=1<<1;       //TIM3时钟使能
    // Enable 1GPIOA, clock 
    RCC->APB2ENR|=1<<2;    //使能PORTA时钟

    // Configure PA.06,07 as encoder input 
    GPIOA->CRL&=0XF0FFFFFF;//PA6
    GPIOA->CRL|=0X04000000;//浮空输入
    GPIOA->CRL&=0X0FFFFFFF;//PA7
    GPIOA->CRL|=0X40000000;//浮空输入

    // Enable the TIM3 Update Interrupt 
    //?????????????????
    TIM3->DIER|=1<<0;   //允许更新中断              
    TIM3->DIER|=1<<6;   //允许触发中断

    TIM3_NVIC_Config();


    //Timer configuration in Encoder mode 
    TIM3->PSC = 0x0;//预分频器
    TIM3->ARR = 15-1;//设定计数器自动重装
    TIM3->CR1 &=~(3<<8);// 选择时钟分频:不分频
    TIM3->CR1 &=~(3<<5);// 选择计数模式:边沿对齐模式

    TIM3->CCMR1 |= 1<<0; //CC1S='01' IC1FP1映射到TI1
    TIM3->CCMR1 |= 1<<8; //CC2S='01' IC2FP2映射到TI2
    TIM3->CCER &= ~(1<<1);  //CC1P='0'  IC1FP1不反向,IC1FP1=TI1
    TIM3->CCER &= ~(1<<5);  //CC2P='0'  IC2FP2不反向,IC2FP2=TI2
    TIM3->CCMR1 |= 3<<4; // IC1F='1000' 输入捕获1滤波器
    TIM3->SMCR |= 3<<0;  //SMS='011' 所有输出输入在上升沿与下降沿有效
    TIM3->CNT = 0;
    TIM3->CR1 |= 0x01;    //CEN=1,使能定时器

}*/
}	

int TIM4_getCount(void)
{
		Count = TIM4->CNT;		
return Count;	//获取编码器当前数值	
		
//		printf("计数值= %d\n",Count);
//		printf("电角度数值 = %d\n",angle_pu);
//		printf("转速 = %d\n",Elecspeed);
		
}




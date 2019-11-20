#include"stm32f10x_gpio.h"
#include"stm32f10x_dac.h"
#include"stm32f10x_tim.h"

unsigned long lastTime;
double lastErr;
extern float angle_pu;
extern float angle_gi;
/*
DAC输出=Vref*(DOR/4095)
DAC两个通道可以配置使用；
相同触发源/不同触发源
同时触发/独立触发DAC_DualSoftwareTriggerCmd函数设置软件同时触发
使用波形发生器/不使用波形发生器
使用三角波发生器/使用噪声发生器/不使用波形发生器
设置相同的DAC_LFSRUnmask_TriangleAmplitude的值/设置不同的DAC_LFSRUnmask_TriangleAmplitude的值*/
void DAC_configration(){
		GPIO_InitTypeDef GPIO_InitStruct; //定义GPIO的结构
		DAC_InitTypeDef DAC_InitStruct;  //定义DAC结构并初始化
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	
	//配置参数
	/*一旦DACx通道，相应的GPIO引脚就会自动与DAC的模拟输出链接，为了避免寄生的干扰和额外的功耗，引脚A4/A5在之前设置成 模拟输入，
	因为STM32中没有模拟输出，所以虽然PA4 PA5是模拟输出信号，但也只能设置为GPIO_Mode_AIN*/
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_InitStruct);
		GPIO_SetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_5);//这两个引脚输入高，上拉起抗干扰的作用

	
	/*DAC 通道1  PA4 普通数模转换*/
	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;//关闭波形发生器
	DAC_InitStruct.DAC_Trigger = DAC_Trigger_None;//DAC_Trigger_Software/DAC_Trigger_Ext_IT9
	DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Disable;//输出缓存用来减少输出阻抗，无需外部运放即可直接驱动外部负载
	DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;//普通DAC转换设置为0即可
	DAC_Init(DAC_Channel_1,&DAC_InitStruct);

/*DAC 通道2  PA5 产生三角波*/
	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;//三角波发生器
	DAC_InitStruct.DAC_Trigger = DAC_Trigger_T6_TRGO;
	DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_2047;//内部的三角波计数器每一次触发之后累加1,该计数器的值与DAC_DHRx的数值相加,去掉溢出位后写入DAC_DORx寄存器,输出电压
	DAC_Init(DAC_Channel_2,&DAC_InitStruct);//三角波的最大值是由DAC_LFSRUnmask_TriangleAmplitude确定的,当计数器达到这个最大值,三角波计数器开始递减

//使能器件
//DAC_SetDualChannelData(DAC_Align_12b_R,4095,0);等价于DAC_SetChannel1Data(DAC_Align_12b_R, 4095); DAC_SetChannel2Data(DAC_Align_12b_R, 0); 
/*通道1 PA4使能*/
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式对齐DAC值  设置最大的值为4095,超过4096则溢出,DORx值为0
	DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC1
	
	/*通道2 PA5使能*/
		DAC_Cmd(DAC_Channel_2, ENABLE);  //使能DAC2
	DAC_SetChannel2Data(DAC_Align_12b_R, 0x100);  //12位右对齐数据格式对齐DAC值  

}

/*基本定时器*/
void TIM6_Configuration()
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
		//定时器使能时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); 
 
		//配置参数
		TIM_TimeBaseInitStruct.TIM_Period = 10 -1;
		TIM_TimeBaseInitStruct.TIM_Prescaler = 72;
		TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
		TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);//  TIMx->EGR.UG   
 
	
		/*TIM6,7可以输出3种类型TRGO信号
			#define TIM_TRGOSource_Reset               ((uint16_t)0x0000) //复位 UG
			#define TIM_TRGOSource_Enable              ((uint16_t)0x0010) //使能 CEN
			#define TIM_TRGOSource_Update              ((uint16_t)0x0020) //更新事件
		*/
		
		TIM_SelectOutputTrigger(TIM6,TIM_TRGOSource_Update);//输出触发TRGO信号  这里TRGO信号就是定时器溢出产生的更新信号
		
		//使能器件
		TIM_Cmd(TIM6,ENABLE);//CEN 位
}
//设置通道1输出电压
//vol:0~3300,代表0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	float temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值
}
u16 PIfunction(double kp,double ki)
{double et,etSum,PI;
		 et = angle_gi - angle_pu*100;  //反馈值于输入值之间的差值 比例环节
		 etSum =etSum+(et*ki);//累加得到积分
		 PI=kp * et + etSum;
	PI=PI+1650;//为了使控制范围为0-3300
	if(PI>=3300)//限幅
		PI=3300;
	else if(PI<=0)
		PI=0;	
		return	PI;
}

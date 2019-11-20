#include"stm32f10x_gpio.h"
#include"stm32f10x_dac.h"
#include"stm32f10x_tim.h"

unsigned long lastTime;
double lastErr;
extern float angle_pu;
extern float angle_gi;
/*
DAC���=Vref*(DOR/4095)
DAC����ͨ����������ʹ�ã�
��ͬ����Դ/��ͬ����Դ
ͬʱ����/��������DAC_DualSoftwareTriggerCmd�����������ͬʱ����
ʹ�ò��η�����/��ʹ�ò��η�����
ʹ�����ǲ�������/ʹ������������/��ʹ�ò��η�����
������ͬ��DAC_LFSRUnmask_TriangleAmplitude��ֵ/���ò�ͬ��DAC_LFSRUnmask_TriangleAmplitude��ֵ*/
void DAC_configration(){
		GPIO_InitTypeDef GPIO_InitStruct; //����GPIO�Ľṹ
		DAC_InitTypeDef DAC_InitStruct;  //����DAC�ṹ����ʼ��
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	
	//���ò���
	/*һ��DACxͨ������Ӧ��GPIO���žͻ��Զ���DAC��ģ��������ӣ�Ϊ�˱�������ĸ��źͶ���Ĺ��ģ�����A4/A5��֮ǰ���ó� ģ�����룬
	��ΪSTM32��û��ģ�������������ȻPA4 PA5��ģ������źţ���Ҳֻ������ΪGPIO_Mode_AIN*/
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_InitStruct);
		GPIO_SetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_5);//��������������ߣ������𿹸��ŵ�����

	
	/*DAC ͨ��1  PA4 ��ͨ��ģת��*/
	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;//�رղ��η�����
	DAC_InitStruct.DAC_Trigger = DAC_Trigger_None;//DAC_Trigger_Software/DAC_Trigger_Ext_IT9
	DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Disable;//�������������������迹�������ⲿ�˷ż���ֱ�������ⲿ����
	DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;//��ͨDACת������Ϊ0����
	DAC_Init(DAC_Channel_1,&DAC_InitStruct);

/*DAC ͨ��2  PA5 �������ǲ�*/
	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;//���ǲ�������
	DAC_InitStruct.DAC_Trigger = DAC_Trigger_T6_TRGO;
	DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_2047;//�ڲ������ǲ�������ÿһ�δ���֮���ۼ�1,�ü�������ֵ��DAC_DHRx����ֵ���,ȥ�����λ��д��DAC_DORx�Ĵ���,�����ѹ
	DAC_Init(DAC_Channel_2,&DAC_InitStruct);//���ǲ������ֵ����DAC_LFSRUnmask_TriangleAmplitudeȷ����,���������ﵽ������ֵ,���ǲ���������ʼ�ݼ�

//ʹ������
//DAC_SetDualChannelData(DAC_Align_12b_R,4095,0);�ȼ���DAC_SetChannel1Data(DAC_Align_12b_R, 4095); DAC_SetChannel2Data(DAC_Align_12b_R, 0); 
/*ͨ��1 PA4ʹ��*/
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ  ��������ֵΪ4095,����4096�����,DORxֵΪ0
	DAC_Cmd(DAC_Channel_1, ENABLE);  //ʹ��DAC1
	
	/*ͨ��2 PA5ʹ��*/
		DAC_Cmd(DAC_Channel_2, ENABLE);  //ʹ��DAC2
	DAC_SetChannel2Data(DAC_Align_12b_R, 0x100);  //12λ�Ҷ������ݸ�ʽ����DACֵ  

}

/*������ʱ��*/
void TIM6_Configuration()
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
		//��ʱ��ʹ��ʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); 
 
		//���ò���
		TIM_TimeBaseInitStruct.TIM_Period = 10 -1;
		TIM_TimeBaseInitStruct.TIM_Prescaler = 72;
		TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
		TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);//  TIMx->EGR.UG   
 
	
		/*TIM6,7�������3������TRGO�ź�
			#define TIM_TRGOSource_Reset               ((uint16_t)0x0000) //��λ UG
			#define TIM_TRGOSource_Enable              ((uint16_t)0x0010) //ʹ�� CEN
			#define TIM_TRGOSource_Update              ((uint16_t)0x0020) //�����¼�
		*/
		
		TIM_SelectOutputTrigger(TIM6,TIM_TRGOSource_Update);//�������TRGO�ź�  ����TRGO�źž��Ƕ�ʱ����������ĸ����ź�
		
		//ʹ������
		TIM_Cmd(TIM6,ENABLE);//CEN λ
}
//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	float temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
}
u16 PIfunction(double kp,double ki)
{double et,etSum,PI;
		 et = angle_gi - angle_pu*100;  //����ֵ������ֵ֮��Ĳ�ֵ ��������
		 etSum =etSum+(et*ki);//�ۼӵõ�����
		 PI=kp * et + etSum;
	PI=PI+1650;//Ϊ��ʹ���Ʒ�ΧΪ0-3300
	if(PI>=3300)//�޷�
		PI=3300;
	else if(PI<=0)
		PI=0;	
		return	PI;
}

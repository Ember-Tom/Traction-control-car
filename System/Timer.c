#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//St1:启用定时器2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	//St2:启用内部时钟
	TIM_InternalClockConfig(TIM3);
	
	//St3:配置时基单元		
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//1分频率
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period =100-1 ;					//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200-1;				//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3,TIM_IT_Update);								//清除一次标志位
	
	//St4:使能更新中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	//St5:配置NVIC
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);					//为中断分组
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	//St6:开启定时器2
	TIM_Cmd(TIM3,ENABLE);
}

////定时中断使用模板
//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)	//判断是否达到中断条件
//	{
//		
//		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);	//清除定时中断标志位
//	}
//}

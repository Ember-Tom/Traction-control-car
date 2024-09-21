#include "stm32f10x.h"                  // Device header

#define Buzzer		GPIO_Pin_8	//将PA8口赋给蜂鸣器

/**
  * 函    数：Buzzer初始化
  * 参    数：无
  * 返 回 值：无
  */

void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin  = Buzzer;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,Buzzer);
}

/**
  * 函    数：Buzzer报警
  * 参    数：无
  * 返 回 值：无
  */

void Buzzer_Ring(void)
{
	GPIO_ResetBits(GPIOA,Buzzer);
}

void Buzzer_Calm(void)
{
	GPIO_SetBits(GPIOA,Buzzer);
}

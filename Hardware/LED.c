#include "stm32f10x.h"                  // Device header

#define LED1 GPIO_Pin_9		//LED1启用PA9口
#define LED2 GPIO_Pin_10	//LED2启用PA10口

/**
  * 函    数：LED初始化
  * 参    数：无
  * 返 回 值：无
  */
void LED_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin  = LED1 | LED2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,LED1 | LED2);
}

void LED1_NO(void)//到达第一个停车点打开LED1
{
	GPIO_ResetBits(GPIOA,LED1);
}

void LED2_NO(void)//到达第二个停车点打开LED2
{
	GPIO_ResetBits(GPIOA,LED2);
}

void LED_OFF(void)//到达终点停车全部熄灭
{
	GPIO_SetBits(GPIOA,LED1 | LED2);
}

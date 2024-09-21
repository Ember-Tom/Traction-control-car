#include "stm32f10x.h"                  // Device header
#include "TCRT5000.h"
#include "Buzzer.h"
#include "Timer.h"
#include "OLED.h"

/******************************************************************	
*函 数 名：main
*函数功能：主函数
******************************************************************/
int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	TCRT_Init();		//循迹模块初始化
	Timer_Init();
	while (1)
	{
		TCRT_Adjust();
	}
}

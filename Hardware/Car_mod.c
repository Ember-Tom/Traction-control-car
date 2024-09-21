#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "OLED.h"
#include "LED.h"

void Car_Mod_Init(void)
{
	Motor_Init();
	LED_Init();
	OLED_Init();				//OLED初始化
}

void Car_Mod_Go(void)			//小车前进
{
	Motor_LEFT_SetSpeed(40);
	Motor_RIGHT_SetSpeed(40);
}

void Car_Mod_LTurn(void)		//小车左转
{
	Motor_LEFT_SetSpeed(0);
	Motor_RIGHT_SetSpeed(40);
}

void Car_Mod_Left_Angle_Turn(void)		//小车直角左转
{
	Motor_LEFT_SetSpeed(0);
	Motor_RIGHT_SetSpeed(60);
}

void Car_Mod_RTurn(void)		//小车右转
{
	Motor_LEFT_SetSpeed(40);
	Motor_RIGHT_SetSpeed(0);
}

void Car_Mod_Right_Angle_Turn(void)		//小车直角右转
{
	Motor_LEFT_SetSpeed(60);
	Motor_RIGHT_SetSpeed(0);
}

void Car_Mod_Park1(void)		//第一停车点
{	
	Motor_LEFT_SetSpeed(0);
	Motor_RIGHT_SetSpeed(0);
	LED1_NO();
}

void Car_Mod_Park2(void)		//第二停车点
{	
	Motor_LEFT_SetSpeed(0);
	Motor_RIGHT_SetSpeed(0);
	LED2_NO();
}

void Car_Mod_EndPark(void)		//最终停车
{	
	Motor_LEFT_SetSpeed(0);
	Motor_RIGHT_SetSpeed(0);
	LED_OFF();
}

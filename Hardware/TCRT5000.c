#include "stm32f10x.h"                  // Device header
#include "Car_mod.h"
#include "Buzzer.h"
#include "OLED.h"

#define TCRT_L1_PIN		GPIO_Pin_3//左1传感器对应的引脚
#define TCRT_L2_PIN		GPIO_Pin_4//左2传感器对应的引脚
#define TCRT_M0_PIN		GPIO_Pin_5//中0传感器对应的引脚
#define TCRT_R2_PIN		GPIO_Pin_6//右2传感器对应的引脚
#define TCRT_R1_PIN		GPIO_Pin_7//右1传感器对应的引脚

uint16_t TC_L1,TC_L2,TC_M0,TC_R2,TC_R1;

uint16_t flag_A=0;				  //循迹功能开启标志位
uint16_t flag_5=0;				  //停车点1标志位
uint16_t flag_10=0;				  //停车点2标志位
uint16_t flag_2_Park=0;			  //停车点2标记位
uint16_t flag_End=0;			  //终点停车标志位
uint16_t flag_End_Park=0;		  //终点停车标记位

void TCRT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//开启GPIO与AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	
	//将PB3与PB4释放为普通GPIO口
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	                       
	
	GPIO_InitStructure.GPIO_Pin = TCRT_L1_PIN | TCRT_L2_PIN | TCRT_M0_PIN | TCRT_R2_PIN | TCRT_R1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	Car_Mod_Init();
	Buzzer_Init();
	OLED_Init();
}
	
void TCRT_Adjust(void)
{	
	//读取循迹返回信号
	TC_L1 = GPIO_ReadInputDataBit(GPIOB,TCRT_L1_PIN);
	TC_L2 = GPIO_ReadInputDataBit(GPIOB,TCRT_L2_PIN);
	TC_M0 = GPIO_ReadInputDataBit(GPIOB,TCRT_M0_PIN);
	TC_R2 = GPIO_ReadInputDataBit(GPIOB,TCRT_R2_PIN);
	TC_R1 = GPIO_ReadInputDataBit(GPIOB,TCRT_R1_PIN);
	
	//循迹模式
	if(flag_A==0)
	{
		//正常行驶
		if((TC_L1==0 && TC_L2==0 && TC_M0==1 && TC_R2==0 && TC_R1==0)
		 ||(TC_L1==1 && TC_L2==0 && TC_R2==1 && TC_R1==0)
		 ||(TC_L1==0 && TC_L2==1 && TC_R2==0 && TC_R1==1))
		{
			Car_Mod_Go();
		}
		
		//左边有黑线修正
		if(TC_L1==0 && TC_L2==1 && TC_M0==0 && TC_R2==0 && TC_R1==0)
		{
			Car_Mod_LTurn();
		}
		
		//右边有黑线修正
		if(TC_L1==0 && TC_L2==0 && TC_M0==0 && TC_R2==1 && TC_R1==0)
		{
			Car_Mod_RTurn();
		}
		
	}
	
	//左停1点	
	if(flag_5==0)
	{

		if(TC_L1==1 && TC_L2==(0||1)  && TC_R1==0)
		{
			flag_5=1;
		}
	}
	
	//右停2点
	if(flag_10==0)
	{	
		if(TC_L1==0 && TC_R2==(0||1) && TC_R1==1)
		{
			flag_10=1;
		}
	}
		
	//左边直角弯	
	if(flag_5==2 && flag_10==2)
	{
		
		if(TC_L1==1 && TC_L2==1)
		{
			flag_A=1;
			Car_Mod_Left_Angle_Turn();
			flag_5=3;flag_10=3;
		}
	}	
	
	//终点停车	
	if(flag_5==3 && flag_10==3)
	{
		if(TC_L1==0 && TC_L2==1 && TC_M0==0 && TC_R2==0 && TC_R1==0)
		{
			flag_A=0;
		}
		
		if(flag_End==0)
		{
			if((TC_L2==1 && TC_R2==1)
			 ||(TC_L1==1 && TC_L2==0 && TC_R2==1 && TC_R1==0)
			 ||(TC_L1==0 && TC_L2==1 && TC_R2==0 && TC_R1==1))
			{
				flag_End=1;
			}
		}
	}
}

//定时中断使用
void TIM3_IRQHandler(void)							//中断1ms
{
	volatile static uint32_t Count1,Count2,Count3;
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)	//判断是否达到中断条件
	{

/******************************************************************	
*检测到一号停车信号时进入判断
******************************************************************/	
		
		if(flag_5==1)								
		{
			Count1++;
			if(Count1<10)
			{
				Buzzer_Ring();
			}
			if(10<=Count1)
			{
				Buzzer_Calm();
			}
			
			Car_Mod_Park1();						//执行停车功能
			flag_A=1;								//关闭循迹功能
			if(Count1>=500)							//限时5s
			{
				flag_A=0;							//出发
				Car_Mod_Go();						
				Count1=0;
				flag_5=2;
			}	
		}

/******************************************************************	
*检测到二号停车信号时进入判断
******************************************************************/
		
		if(flag_10==1)								
		{
			Count2++;			
			if(flag_2_Park>=2)						//到达指定停车点
			{
				if(Count2<10)
				{
					Buzzer_Ring();
				}
				if(10<=Count2)
				{
					Buzzer_Calm();
				}
				Car_Mod_Park2();					//执行停车功能
				flag_A=1;							//关闭循迹功能
				if(Count2>=1000)					//限时10s
				{
					flag_A=0;						//出发
					Car_Mod_Go();						
					Count2=0;
					flag_10=2;
					flag_2_Park=3;
				}
			}
			
			if(flag_2_Park<=1)						//判定右侧接触几个障碍线
			{
				Count2++;	
				flag_10=1;							//关闭右侧停车判定
				if(Count2>=100)						//150ms通过障碍
				{
					flag_2_Park++;					//记录判定数据
					flag_10=0;						//打开右侧停车检测
					Count2=0;						
				}
			}
		}
		
/******************************************************************	
*十字路口判断及停车
******************************************************************/
		
		if(flag_End==1)
		{
			Count3++;
			if(flag_End_Park<=2)					//判定经过几个十字路口
			{
				flag_End=1;							//关闭停车判定
				if(Count3>=70)						//150ms通过障碍
				{
					flag_End_Park++;				//记录判定数据
					flag_End=0;						//打开十字停车检测
					Count3=0;						
				}
			}
			
			if(flag_End_Park>=3)					//到达指定停车点
			{
				Car_Mod_EndPark();					//执行停车功能
				flag_A=1;							//关闭循迹功能
			}
		}
		
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);	//清除定时中断标志位
	}
}

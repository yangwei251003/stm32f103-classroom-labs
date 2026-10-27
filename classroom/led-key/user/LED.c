#include "stm32f10x.h"

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//打开GPIOA外设时钟

	GPIO_InitTypeDef GPIO_Initstructure;					//声明一个GPIO_InitTypeDef类型的结构体
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;		//GPIO模式：推挽输出
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0;				//引脚选择：0引脚
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO速率
	GPIO_Init(GPIOA, &GPIO_Initstructure);					//初始化GPIOA的0引脚
}

void LED_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);	//PA0引脚输出低电平，点亮LED
}

void LED_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_0);	//PA0引脚输出高电平，熄灭LED
}
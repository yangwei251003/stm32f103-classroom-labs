#include "stm32f10x.h"
#include "delay.h"

//LED初始化 PB5
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_Infrastructure;
	GPIO_Infrastructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Infrastructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Infrastructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_Infrastructure);
	
	//上电默认灯熄灭
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

//按键初始化 PB6 上拉输入
void KEY_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_Infrastructure;
	GPIO_Infrastructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	GPIO_Infrastructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Infrastructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_Infrastructure);
}

int main(void)
{
	Delay_Init();
	LED_Init();
	KEY_Init();
	
	while(1)
	{
		//检测PB6按键按下
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
		{
			Delay_ms(20); //消抖
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
			{
				//翻转PB5灯
				if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5)==1)
				{
					GPIO_ResetBits(GPIOB,GPIO_Pin_5);
				}
				else
				{
					GPIO_SetBits(GPIOB,GPIO_Pin_5);
				}
				while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)==0); //等待松开
			}
		}
	}
}
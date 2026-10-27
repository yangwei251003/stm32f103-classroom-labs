#include "stm32f10x.h"
#include "Delay.h"

//LED初始化 PB5 推挽输出
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_Infrastructure;
	GPIO_Infrastructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Infrastructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Infrastructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_Infrastructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_5); //上电灯灭
}

//按键初始化 PB6 上拉输入
void KEY_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_Infrastructure;
	GPIO_Infrastructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Infrastructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Infrastructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_Infrastructure);
}

int main(void)
{
	LED_Init();
	KEY_Init();
	
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
		{
			Delay_ms(20); //消抖
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
			{
				//翻转LED
				if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5))
				{
					GPIO_ResetBits(GPIOB, GPIO_Pin_5);
				}
				else
				{
					GPIO_SetBits(GPIOB, GPIO_Pin_5);
				}
				//等待按键松开
				while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0);
				Delay_ms(20);
			}
		}
	}
}
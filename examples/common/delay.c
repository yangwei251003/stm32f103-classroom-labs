#include "stm32f10x.h"
#include "delay.h"

static uint32_t fac_us = 0;
static uint32_t fac_ms = 0;

void Delay_Init(void)
{
    SystemCoreClockUpdate();
    fac_us = SystemCoreClock / 1000000;
    fac_ms = fac_us * 1000;
    /* Poll the full 24-bit core-clock counter without interrupts. Hardware timing remains unverified. */
    SysTick->CTRL = 0;
    SysTick->LOAD = 0x00ffffffu;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}

void Delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;
    ticks = nus * fac_us;
    told = SysTick->VAL;
    while(1)
    {
        tnow = SysTick->VAL;
        if(tnow != told)
        {
            if(tnow < told)
                tcnt += told - tnow;
            else
                tcnt += reload - tnow + told;
            told = tnow;
            if(tcnt >= ticks) break;
        }
    }
}

void Delay_ms(uint32_t nms)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;
    ticks = nms * fac_ms;
    told = SysTick->VAL;
    while(1)
    {
        tnow = SysTick->VAL;
        if(tnow != told)
        {
            if(tnow < told)
                tcnt += told - tnow;
            else
                tcnt += reload - tnow + told;
            told = tnow;
            if(tcnt >= ticks) break;
        }
    }
}
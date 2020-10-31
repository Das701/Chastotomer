#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


void HAL_TIM::Init()
{
#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR     *(volatile unsigned long *)0xE000EDFC

#ifndef GUI
    //разрешаем использовать счётчик
    SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; //-V2523
    //обнуляем значение счётного регистра
    DWT_CYCCNT = 0; //-V2523
    //запускаем счётчик
    DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk; //-V2523
#endif
}


uint HAL_TIM::TimeMS()
{
    return HAL_GetTick();
}


void HAL_TIM::DelayMS(uint timeMS)
{
    HAL_Delay(timeMS);
}


static __inline uint32_t delta(uint32_t t0, uint32_t t1)
{
    return (t1 - t0); 
}


void HAL_TIM::DelayUS(uint timeUS)
{
#ifdef GUI
    timeUS = timeUS;
#else
    uint32_t t0 = DWT->CYCCNT;
    uint32_t us_count_tic = timeUS * (SystemCoreClock / 1000000);
    while (delta(t0, DWT->CYCCNT) < us_count_tic) //-V712
    {
    };
#endif
}

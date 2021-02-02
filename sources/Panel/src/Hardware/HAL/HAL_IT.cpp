#include "Hardware/HAL/HAL.h"
#include "Utils/Debug.h"
#include <stm32f4xx_hal.h>


#ifdef __cplusplus
extern "C" {
#endif


void NMI_Handler(void)
{

}

void HardFault_Handler(void)
{
    const char *file = Debug::file[0];
    int line = Debug::line[0];

    while (1)
    {
        file = Debug::file[0];
        line = Debug::line[0];
        
        file = file;
        line = line;
    }
}

void MemManage_Handler(void)
{
    while (1)
    {
    }
}

void BusFault_Handler(void)
{
    while (1)
    {
    }
}

void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

void SVC_Handler(void)
{

}

void DebugMon_Handler(void)
{

}

void PendSV_Handler(void)
{

}

void SysTick_Handler(void)
{
    HAL_IncTick();
}


void OTG_FS_IRQHandler()
{
    HAL_PCD_IRQHandler((PCD_HandleTypeDef *)HAL_PCD::handle); //-V2533 //-V2571
}


#ifdef __cplusplus
}
#endif

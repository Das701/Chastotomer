#pragma once
#include "stm32f4xx_hal_def.h"


#define SYSTICK_CLKSOURCE_HCLK_DIV8    0x00000000U
#define SYSTICK_CLKSOURCE_HCLK         0x00000004U

#define NVIC_PRIORITYGROUP_0         0x00000007U /*!< 0 bits for pre-emption priority
                                                      4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         0x00000006U /*!< 1 bits for pre-emption priority
                                                      3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         0x00000005U /*!< 2 bits for pre-emption priority
                                                      2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         0x00000004U /*!< 3 bits for pre-emption priority
                                                      1 bits for subpriority */
#define NVIC_PRIORITYGROUP_4         0x00000003U /*!< 4 bits for pre-emption priority
                                                      0 bits for subpriority */


extern "C"
{
    void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);
    uint32_t HAL_SYSTICK_Config(uint32_t TicksNumb);
    void HAL_NVIC_EnableIRQ(IRQn_Type IRQn);
    void HAL_NVIC_DisableIRQ(IRQn_Type IRQn);
    void HAL_SYSTICK_IRQHandler(void);
    void HAL_SYSTICK_CLKSourceConfig(uint32_t CLKSource);
    void HAL_NVIC_SetPriorityGrouping(uint32_t PriorityGroup);
}

#include <stm32f4xx_hal.h>



uint32_t HAL_RCC_GetHCLKFreq(void)
{
    return 0;
}


HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef * /*RCC_ClkInitStruct*/, uint32_t /*FLatency*/)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef * /*RCC_OscInitStruct*/)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef * /*PeriphClkInit*/)
{
    return HAL_OK;
}

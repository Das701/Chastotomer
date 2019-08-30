#pragma once
#include "stm32f4xx_hal_conf.h"


HAL_StatusTypeDef HAL_Init(void);
uint32_t HAL_GetTick(void);
void HAL_Delay(uint32_t Delay);
void HAL_IncTick(void);

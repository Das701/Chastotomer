#pragma once
#include "stm32f4xx.h"
#include "Legacy/stm32_hal_legacy.h"


typedef enum
{
    HAL_OK = 0x00U,
    HAL_ERROR = 0x01U,
    HAL_BUSY = 0x02U,
    HAL_TIMEOUT = 0x03U
} HAL_StatusTypeDef;


typedef enum
{
    HAL_UNLOCKED = 0x00U,
    HAL_LOCKED = 0x01U
} HAL_LockTypeDef;


#define __HAL_LINKDMA(__HANDLE__, __PPP_DMA_FIELD__, __DMA_HANDLE__)

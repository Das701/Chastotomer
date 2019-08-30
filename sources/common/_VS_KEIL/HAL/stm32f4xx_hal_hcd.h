#pragma once
#include "stm32f4xx_ll_usb.h"


typedef struct
{
    int i;
} HCD_HandleTypeDef;

void HAL_HCD_IRQHandler(HCD_HandleTypeDef *hhcd);

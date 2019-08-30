#pragma once
#include <stm32f4xx_hal.h>
#include "stm32f4xx_ll_usb.h"


#define PCD_SPEED_FULL               2U
#define PCD_PHY_EMBEDDED             2U


typedef USB_OTG_GlobalTypeDef  PCD_TypeDef;
typedef USB_OTG_CfgTypeDef     PCD_InitTypeDef;


typedef struct
{
    int temp;

    PCD_TypeDef             *Instance;    /*!< Register base address              */
    PCD_InitTypeDef         Init;         /*!< PCD required parameters            */
} PCD_HandleTypeDef;


HAL_StatusTypeDef HAL_PCD_Init(PCD_HandleTypeDef *hpcd);
void HAL_PCD_IRQHandler(PCD_HandleTypeDef *hpcd);

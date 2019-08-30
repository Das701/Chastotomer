#pragma once


#include "stm32f4xx_ll_fmc.h"


typedef struct
{
    FMC_NORSRAM_TypeDef           *Instance;  /*!< Register base address                        */

    FMC_NORSRAM_EXTENDED_TypeDef  *Extended;  /*!< Extended mode register base address          */

    FMC_NORSRAM_InitTypeDef       Init;       /*!< SRAM device control configuration parameters */
//
//    HAL_LockTypeDef               Lock;       /*!< SRAM locking object                          */
//
//    __IO HAL_SRAM_StateTypeDef    State;      /*!< SRAM device access state                     */
//
//    DMA_HandleTypeDef             *hdma;      /*!< Pointer DMA handler                          */

}SRAM_HandleTypeDef;


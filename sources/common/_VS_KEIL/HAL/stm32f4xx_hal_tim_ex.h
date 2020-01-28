#pragma once
#include "stm32f4xx_hal_def.h"


typedef struct {
  uint32_t  MasterOutputTrigger;   /*!< Trigger output (TRGO) selection. 
                                      This parameter can be a value of @ref TIM_Master_Mode_Selection */

  uint32_t  MasterSlaveMode;       /*!< Master/slave mode selection. 
                                      This parameter can be a value of @ref TIM_Master_Slave_Mode */

} TIM_MasterConfigTypeDef;


HAL_StatusTypeDef HAL_TIMEx_MasterConfigSynchronization(TIM_HandleTypeDef* htim, TIM_MasterConfigTypeDef * sMasterConfig);

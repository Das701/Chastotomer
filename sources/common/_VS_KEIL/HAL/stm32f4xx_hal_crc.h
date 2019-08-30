#pragma once


typedef struct
{
    CRC_TypeDef                 *Instance;  /*!< Register base address   */

    HAL_LockTypeDef             Lock;       /*!< CRC locking object      */

//    __IO HAL_CRC_StateTypeDef   State;      /*!< CRC communication state */

}CRC_HandleTypeDef;


HAL_StatusTypeDef HAL_CRC_Init(CRC_HandleTypeDef *hcrc);
uint32_t HAL_CRC_Calculate(CRC_HandleTypeDef *hcrc, uint32_t *pBuffer, uint32_t BufferLength);

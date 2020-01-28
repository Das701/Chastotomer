#pragma once
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_flash_ex.h"


#define FLASH_FLAG_EOP      0   /*!< FLASH End of Operation flag               */
#define FLASH_FLAG_OPERR    0   /*!< FLASH operation Error flag                */
#define FLASH_FLAG_WRPERR   0   /*!< FLASH Write protected error flag          */
#define FLASH_FLAG_PGAERR   0   /*!< FLASH Programming Alignment error flag    */
#define FLASH_FLAG_PGPERR   0   /*!< FLASH Programming Parallelism error flag  */
#define FLASH_FLAG_PGSERR   0   /*!< FLASH Programming Sequence error flag     */


#define __HAL_FLASH_CLEAR_FLAG(x)


HAL_StatusTypeDef HAL_FLASH_Unlock(void);
HAL_StatusTypeDef HAL_FLASH_Lock(void);
HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data);

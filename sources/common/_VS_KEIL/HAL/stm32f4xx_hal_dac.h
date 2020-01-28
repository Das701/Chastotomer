#pragma once
#include <stm32f4xx_hal_adc.h>


#define DAC_TRIGGER_NONE            0x00000000U
#define DAC_OUTPUTBUFFER_ENABLE     0x00000000U
#define DAC_CHANNEL_1               0x00000000U
#define DAC_CHANNEL_2               0x00000010U
#define DAC_OUTPUTBUFFER_DISABLE    0
#define DAC_ALIGN_8B_R              0x00000008U
#define DAC_TRIGGER_T7_TRGO         0


typedef struct
{
    DAC_TypeDef                 *Instance;     /*!< Register base address             */

    //__IO HAL_DAC_StateTypeDef   State;         /*!< DAC communication state           */
    //
    //HAL_LockTypeDef             Lock;          /*!< DAC locking object                */
    //
    DMA_HandleTypeDef           *DMA_Handle1;  /*!< Pointer DMA handler for channel 1 */
    //
    //DMA_HandleTypeDef           *DMA_Handle2;  /*!< Pointer DMA handler for channel 2 */
    //
    //__IO uint32_t               ErrorCode;     /*!< DAC Error code                    */

}DAC_HandleTypeDef;


extern "C"
{
    HAL_StatusTypeDef HAL_DAC_Init(DAC_HandleTypeDef* hdac);
    HAL_StatusTypeDef HAL_DAC_ConfigChannel(DAC_HandleTypeDef* hdac, DAC_ChannelConfTypeDef* sConfig, uint32_t Channel);
    HAL_StatusTypeDef HAL_DAC_DeInit(DAC_HandleTypeDef* hdac);
    HAL_StatusTypeDef HAL_DAC_Stop_DMA(DAC_HandleTypeDef* hdac, uint32_t Channel);
    HAL_StatusTypeDef HAL_DAC_Start_DMA(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t* pData, uint32_t Length, uint32_t Alignment);
    HAL_StatusTypeDef HAL_DAC_Start(DAC_HandleTypeDef* hdac, uint32_t Channel);
    HAL_StatusTypeDef HAL_DAC_SetValue(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t Alignment, uint32_t Data);
}

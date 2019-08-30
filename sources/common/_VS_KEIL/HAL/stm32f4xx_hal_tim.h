#pragma once
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_dma.h"


#define TIM_COUNTERMODE_UP          0x00000000U
#define TIM_CLOCKDIVISION_DIV1      0x00000000U

#define TIM_MASTERSLAVEMODE_ENABLE  0x00000080U
#define TIM_MASTERSLAVEMODE_DISABLE 0x00000000U

#define	TIM_TRGO_RESET              0x00000000U
#define	TIM_TRGO_ENABLE             (0)
#define	TIM_TRGO_UPDATE             (0)
#define	TIM_TRGO_OC1                (0)
#define	TIM_TRGO_OC1REF             (0)
#define	TIM_TRGO_OC2REF             (0)
#define	TIM_TRGO_OC3REF             (0)
#define	TIM_TRGO_OC4REF             (0)


typedef enum
{
    HAL_TIM_ACTIVE_CHANNEL_1        = 0x01U,    /*!< The active channel is 1     */
    HAL_TIM_ACTIVE_CHANNEL_2        = 0x02U,    /*!< The active channel is 2     */
    HAL_TIM_ACTIVE_CHANNEL_3        = 0x04U,    /*!< The active channel is 3     */
    HAL_TIM_ACTIVE_CHANNEL_4        = 0x08U,    /*!< The active channel is 4     */
    HAL_TIM_ACTIVE_CHANNEL_CLEARED  = 0x00U     /*!< All active channels cleared */

} HAL_TIM_ActiveChannel;


typedef enum
{
    HAL_TIM_STATE_RESET     = 0x00U,    /*!< Peripheral not yet initialized or disabled  */
    HAL_TIM_STATE_READY     = 0x01U,    /*!< Peripheral Initialized and ready for use    */
    HAL_TIM_STATE_BUSY      = 0x02U,    /*!< An internal process is ongoing              */
    HAL_TIM_STATE_TIMEOUT   = 0x03U,    /*!< Timeout state                               */
    HAL_TIM_STATE_ERROR     = 0x04U     /*!< Reception process is ongoing                */

}HAL_TIM_StateTypeDef;


typedef struct
{
    uint32_t Prescaler;         /*!< Specifies the prescaler value used to divide the TIM clock.
                                This parameter can be a number between Min_Data = 0x0000U and Max_Data = 0xFFFFU */

    uint32_t CounterMode;       /*!< Specifies the counter mode.
                                This parameter can be a value of @ref TIM_Counter_Mode */

    uint32_t Period;            /*!< Specifies the period value to be loaded into the active
                                Auto-Reload Register at the next update event.
                                This parameter can be a number between Min_Data = 0x0000U and Max_Data = 0xFFFF.  */

    uint32_t ClockDivision;     /*!< Specifies the clock division.
                                This parameter can be a value of @ref TIM_ClockDivision */

    uint32_t RepetitionCounter;  /*!< Specifies the repetition counter value. Each time the RCR downcounter
                                 reaches zero, an update event is generated and counting restarts
                                 from the RCR value (N).
                                 This means in PWM mode that (N+1) corresponds to:
                                 - the number of PWM periods in edge-aligned mode
                                 - the number of half PWM period in center-aligned mode
                                 This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF. 
                                 @note This parameter is valid only for TIM1 and TIM8. */

} TIM_Base_InitTypeDef;


typedef struct
{
    TIM_TypeDef             *Instance;     /*!< Register base address             */
    TIM_Base_InitTypeDef    Init;          /*!< TIM Time Base required parameters */
    HAL_TIM_ActiveChannel   Channel;       /*!< Active channel                    */
    DMA_HandleTypeDef       *hdma[7];      /*!< DMA Handlers array. This array is accessed by a @ref DMA_Handle_index */
    HAL_LockTypeDef         Lock;          /*!< Locking object                    */
    HAL_TIM_StateTypeDef    State;         /*!< TIM operation state               */

}TIM_HandleTypeDef;


#include "stm32f4xx_hal_tim_ex.h"

#ifndef GUI

extern "C"
{
    HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_HandleTypeDef *htim);
    HAL_StatusTypeDef HAL_TIM_Base_Start(TIM_HandleTypeDef *htim);
    HAL_StatusTypeDef HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim);
    HAL_StatusTypeDef HAL_TIM_Base_DeInit(TIM_HandleTypeDef *htim);
    HAL_StatusTypeDef HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim);
    HAL_StatusTypeDef HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim);
}

#endif

void HAL_TIM_IRQHandler(TIM_HandleTypeDef *htim);

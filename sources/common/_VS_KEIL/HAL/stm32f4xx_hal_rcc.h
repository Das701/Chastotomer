#pragma once
#include "stm32f4xx_hal_rcc_ex.h"


#define __HAL_RCC_PWR_CLK_ENABLE()
#define __HAL_RCC_GPIOB_CLK_ENABLE()
#define __HAL_RCC_GPIOC_CLK_ENABLE()
#define __HAL_RCC_GPIOA_CLK_ENABLE()
#define __HAL_RCC_GPIOD_CLK_ENABLE()
#define __HAL_RCC_GPIOE_CLK_ENABLE()
#define __HAL_RCC_DMA2D_CLK_ENABLE()
#define __HAL_RCC_LTDC_CLK_ENABLE()
#define __HAL_RCC_TIM4_CLK_ENABLE()
#define __HAL_RCC_SPI4_CLK_DISABLE()
#define __HAL_RCC_USART3_CLK_ENABLE()
#define __HAL_RCC_USART3_CLK_DISABLE()
#define __HAL_RCC_SYSCFG_CLK_ENABLE()
#define __HAL_RCC_GPIOA_CLK_ENABLE()

#define __HAL_RCC_RTC_ENABLE()
#define __HAL_RCC_RTC_DISABLE()

#define __HAL_RCC_PLL_PLLM_CONFIG(x)
#define __HAL_RCC_PLL_PLLSOURCE_CONFIG(__PLLSOURCE__)

#define RCC_HSICALIBRATION_DEFAULT       0x10U
#define RCC_OSCILLATORTYPE_HSE      0x00000001U
#define RCC_HSE_ON                  0
#define RCC_PLL_ON                  ((uint8_t)0x02)
#define RCC_PLLSOURCE_HSE           1
#define RCC_PLLP_DIV2               0x00000002U
#define RCC_CLOCKTYPE_HCLK          0x00000002U
#define RCC_CLOCKTYPE_SYSCLK        0x00000001U
#define RCC_CLOCKTYPE_PCLK1         0x00000004U
#define RCC_CLOCKTYPE_PCLK2         0x00000008U
#define RCC_SYSCLKSOURCE_PLLCLK     0
#define RCC_SYSCLK_DIV1             0
#define RCC_HCLK_DIV4               0
#define RCC_HCLK_DIV2               0
#define RCC_SYSCLKSOURCE_HSI        0
#define RCC_HCLK_DIV1               0

#define RCC_OSCILLATORTYPE_NONE            0x00000000U
#define RCC_OSCILLATORTYPE_HSE             0x00000001U
#define RCC_OSCILLATORTYPE_HSI             0x00000002U
#define RCC_OSCILLATORTYPE_LSE             0x00000004U
#define RCC_OSCILLATORTYPE_LSI             0x00000008U

#define RCC_HSI_OFF                      ((uint8_t)0x00)
#define RCC_HSI_ON                       ((uint8_t)0x01)

#define RCC_PLLSOURCE_HSI           0

#define RCC_PLL_NONE                      ((uint8_t)0x00)
#define RCC_PLL_OFF                       ((uint8_t)0x01)
#define RCC_PLL_ON                        ((uint8_t)0x02)

#define RCC_LSI_OFF                      ((uint8_t)0x00)
#define RCC_LSI_ON                       ((uint8_t)0x01)

#define RCC_RTCCLKSOURCE_NO_CLK          0x00000000U
#define RCC_RTCCLKSOURCE_LSE             0x00000100U
#define RCC_RTCCLKSOURCE_LSI             0x00000200U
#define RCC_RTCCLKSOURCE_HSE_DIVX        0x00000300U
#define RCC_RTCCLKSOURCE_HSE_DIV2        0x00020300U
#define RCC_RTCCLKSOURCE_HSE_DIV3        0x00030300U
#define RCC_RTCCLKSOURCE_HSE_DIV4        0x00040300U
#define RCC_RTCCLKSOURCE_HSE_DIV5        0x00050300U
#define RCC_RTCCLKSOURCE_HSE_DIV6        0x00060300U
#define RCC_RTCCLKSOURCE_HSE_DIV7        0x00070300U
#define RCC_RTCCLKSOURCE_HSE_DIV8        0x00080300U
#define RCC_RTCCLKSOURCE_HSE_DIV9        0x00090300U
#define RCC_RTCCLKSOURCE_HSE_DIV10       0x000A0300U
#define RCC_RTCCLKSOURCE_HSE_DIV11       0x000B0300U
#define RCC_RTCCLKSOURCE_HSE_DIV12       0x000C0300U
#define RCC_RTCCLKSOURCE_HSE_DIV13       0x000D0300U
#define RCC_RTCCLKSOURCE_HSE_DIV14       0x000E0300U
#define RCC_RTCCLKSOURCE_HSE_DIV15       0x000F0300U
#define RCC_RTCCLKSOURCE_HSE_DIV16       0x00100300U
#define RCC_RTCCLKSOURCE_HSE_DIV17       0x00110300U
#define RCC_RTCCLKSOURCE_HSE_DIV18       0x00120300U
#define RCC_RTCCLKSOURCE_HSE_DIV19       0x00130300U
#define RCC_RTCCLKSOURCE_HSE_DIV20       0x00140300U
#define RCC_RTCCLKSOURCE_HSE_DIV21       0x00150300U
#define RCC_RTCCLKSOURCE_HSE_DIV22       0x00160300U
#define RCC_RTCCLKSOURCE_HSE_DIV23       0x00170300U
#define RCC_RTCCLKSOURCE_HSE_DIV24       0x00180300U
#define RCC_RTCCLKSOURCE_HSE_DIV25       0x00190300U
#define RCC_RTCCLKSOURCE_HSE_DIV26       0x001A0300U
#define RCC_RTCCLKSOURCE_HSE_DIV27       0x001B0300U
#define RCC_RTCCLKSOURCE_HSE_DIV28       0x001C0300U
#define RCC_RTCCLKSOURCE_HSE_DIV29       0x001D0300U
#define RCC_RTCCLKSOURCE_HSE_DIV30       0x001E0300U
#define RCC_RTCCLKSOURCE_HSE_DIV31       0x001F0300U

#define RCC_PLLP_DIV4                  0x00000004U


typedef struct
{
    uint32_t OscillatorType;       /*!< The oscillators to be configured.
                                   This parameter can be a value of @ref RCC_Oscillator_Type                   */

    uint32_t HSEState;             /*!< The new state of the HSE.
                                   This parameter can be a value of @ref RCC_HSE_Config                        */

    uint32_t LSEState;             /*!< The new state of the LSE.
                                   This parameter can be a value of @ref RCC_LSE_Config                        */

    uint32_t HSIState;             /*!< The new state of the HSI.
                                   This parameter can be a value of @ref RCC_HSI_Config                        */

    uint32_t HSICalibrationValue;  /*!< The HSI calibration trimming value (default is RCC_HSICALIBRATION_DEFAULT).
                                   This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F */

    uint32_t LSIState;             /*!< The new state of the LSI.
                                   This parameter can be a value of @ref RCC_LSI_Config                        */

    RCC_PLLInitTypeDef PLL;        /*!< PLL structure parameters                                                    */
}RCC_OscInitTypeDef;


typedef struct
{
    uint32_t ClockType;             /*!< The clock to be configured.
                                    This parameter can be a value of @ref RCC_System_Clock_Type      */

    uint32_t SYSCLKSource;          /*!< The clock source (SYSCLKS) used as system clock.
                                    This parameter can be a value of @ref RCC_System_Clock_Source    */

    uint32_t AHBCLKDivider;         /*!< The AHB clock (HCLK) divider. This clock is derived from the system clock (SYSCLK).
                                    This parameter can be a value of @ref RCC_AHB_Clock_Source       */

    uint32_t APB1CLKDivider;        /*!< The APB1 clock (PCLK1) divider. This clock is derived from the AHB clock (HCLK).
                                    This parameter can be a value of @ref RCC_APB1_APB2_Clock_Source */

    uint32_t APB2CLKDivider;        /*!< The APB2 clock (PCLK2) divider. This clock is derived from the AHB clock (HCLK).
                                    This parameter can be a value of @ref RCC_APB1_APB2_Clock_Source */

}RCC_ClkInitTypeDef;


HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef *RCC_OscInitStruct);
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *RCC_ClkInitStruct, uint32_t FLatency);
uint32_t HAL_RCC_GetHCLKFreq(void);

#pragma once
#include "stm32f4xx_hal_def.h"


#define __HAL_RCC_FMC_CLK_ENABLE()
#define __HAL_RCC_GPIOF_CLK_ENABLE()
#define __HAL_RCC_TIM2_CLK_ENABLE()
#define __HAL_RCC_TIM3_CLK_ENABLE()
#define __HAL_RCC_TIM2_CLK_DISABLE()
#define __HAL_RCC_TIM3_CLK_DISABLE()
#define __HAL_RCC_ADC3_CLK_ENABLE()
#define __HAL_RCC_ADC3_CLK_DISABLE()
#define __HAL_RCC_DAC_CLK_ENABLE()
#define __HAL_RCC_DAC_CLK_DISABLE()
#define __HAL_RCC_SPI4_CLK_ENABLE()
#define __HAL_RCC_USB_OTG_FS_CLK_ENABLE()
#define __HAL_RCC_USB_OTG_HS_CLK_ENABLE()

#define RCC_PERIPHCLK_I2S             0x00000001U
#define RCC_PERIPHCLK_SAI_PLLI2S      0x00000002U
#define RCC_PERIPHCLK_SAI_PLLSAI      0x00000004U
#define RCC_PERIPHCLK_LTDC            0x00000008U
#define RCC_PERIPHCLK_TIM             0x00000010U
#define RCC_PERIPHCLK_RTC             0x00000020U
#define RCC_PERIPHCLK_PLLI2S          0x00000040U
#define RCC_PERIPHCLK_CLK48           0x00000080U
#define RCC_PERIPHCLK_SDIO            0x00000100U

#define RCC_PLLSAIDIVR_2                0x00000000U
#define RCC_PLLSAIDIVR_4                0x00010000U
#define RCC_PLLSAIDIVR_8                0x00020000U
#define RCC_PLLSAIDIVR_16               0x00030000U


typedef struct
{
    uint32_t PLLState;   /*!< The new state of the PLL.
                         This parameter can be a value of @ref RCC_PLL_Config                      */

    uint32_t PLLSource;  /*!< RCC_PLLSource: PLL entry clock source.
                         This parameter must be a value of @ref RCC_PLL_Clock_Source               */

    uint32_t PLLM;       /*!< PLLM: Division factor for PLL VCO input clock.
                         This parameter must be a number between Min_Data = 0 and Max_Data = 63    */

    uint32_t PLLN;       /*!< PLLN: Multiplication factor for PLL VCO output clock.
                         This parameter must be a number between Min_Data = 50 and Max_Data = 432 
                         except for STM32F411xE devices where the Min_Data = 192 */

    uint32_t PLLP;       /*!< PLLP: Division factor for main system clock (SYSCLK).
                         This parameter must be a value of @ref RCC_PLLP_Clock_Divider             */

    uint32_t PLLQ;       /*!< PLLQ: Division factor for OTG FS, SDIO and RNG clocks.
                         This parameter must be a number between Min_Data = 2 and Max_Data = 15    */
#if defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) || defined(STM32F446xx) || defined(STM32F469xx) ||\
    defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) ||\
    defined(STM32F413xx) || defined(STM32F423xx)
    uint32_t PLLR;       /*!< PLLR: PLL division factor for I2S, SAI, SYSTEM, SPDIFRX clocks.
                         This parameter is only available in STM32F410xx/STM32F446xx/STM32F469xx/STM32F479xx
                         and STM32F412Zx/STM32F412Vx/STM32F412Rx/STM32F412Cx/STM32F413xx/STM32F423xx devices. 
                         This parameter must be a number between Min_Data = 2 and Max_Data = 7     */
#endif /* STM32F410xx || STM32F446xx || STM32F469xx || STM32F479xx || STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F412Cx || STM32F413xx || STM32F423xx */ 
} RCC_PLLInitTypeDef;

typedef struct
{
    uint32_t PLLI2SM;    /*!< Specifies division factor for PLL VCO input clock.
                              This parameter must be a number between Min_Data = 2 and Max_Data = 63       */

    uint32_t PLLI2SN;    /*!< Specifies the multiplication factor for PLLI2S VCO output clock.
                              This parameter must be a number between Min_Data = 50 and Max_Data = 432    */

    uint32_t PLLI2SP;    /*!< Specifies division factor for SPDIFRX Clock.
                              This parameter must be a value of @ref RCCEx_PLLI2SP_Clock_Divider           */

    uint32_t PLLI2SQ;    /*!< Specifies the division factor for SAI clock.
                              This parameter must be a number between Min_Data = 2 and Max_Data = 15.
                              This parameter will be used only when PLLI2S is selected as Clock Source SAI */

    uint32_t PLLI2SR;    /*!< Specifies the division factor for I2S clock.
                              This parameter must be a number between Min_Data = 2 and Max_Data = 7.
                              This parameter will be used only when PLLI2S is selected as Clock Source I2S */
}RCC_PLLI2SInitTypeDef;

typedef struct
{
    uint32_t PLLSAIN;    /*!< Specifies the multiplication factor for PLLI2S VCO output clock.
                              This parameter must be a number between Min_Data = 50 and Max_Data = 432.
                              This parameter will be used only when PLLSAI is selected as Clock Source SAI or LTDC */
#if defined(STM32F469xx) || defined(STM32F479xx)
    uint32_t PLLSAIP;    /*!< Specifies division factor for OTG FS and SDIO clocks.
                              This parameter is only available in STM32F469xx/STM32F479xx devices.
                              This parameter must be a value of @ref RCCEx_PLLSAIP_Clock_Divider  */
#endif /* STM32F469xx || STM32F479xx */

    uint32_t PLLSAIQ;    /*!< Specifies the division factor for SAI1 clock.
                              This parameter must be a number between Min_Data = 2 and Max_Data = 15.
                              This parameter will be used only when PLLSAI is selected as Clock Source SAI or LTDC */

    uint32_t PLLSAIR;    /*!< specifies the division factor for LTDC clock
                              This parameter must be a number between Min_Data = 2 and Max_Data = 7.
                              This parameter will be used only when PLLSAI is selected as Clock Source LTDC */

}RCC_PLLSAIInitTypeDef;

typedef struct
{
    uint32_t PeriphClockSelection; /*!< The Extended Clock to be configured.
                                        This parameter can be a value of @ref RCCEx_Periph_Clock_Selection */

    RCC_PLLI2SInitTypeDef PLLI2S;  /*!< PLL I2S structure parameters.
                                        This parameter will be used only when PLLI2S is selected as Clock Source I2S or SAI */

    RCC_PLLSAIInitTypeDef PLLSAI;  /*!< PLL SAI structure parameters.
                                        This parameter will be used only when PLLI2S is selected as Clock Source SAI or LTDC */

    uint32_t PLLI2SDivQ;           /*!< Specifies the PLLI2S division factor for SAI1 clock.
                                        This parameter must be a number between Min_Data = 1 and Max_Data = 32
                                        This parameter will be used only when PLLI2S is selected as Clock Source SAI */

    uint32_t PLLSAIDivQ;           /*!< Specifies the PLLI2S division factor for SAI1 clock.
                                        This parameter must be a number between Min_Data = 1 and Max_Data = 32
                                        This parameter will be used only when PLLSAI is selected as Clock Source SAI */

    uint32_t PLLSAIDivR;           /*!< Specifies the PLLSAI division factor for LTDC clock.
                                        This parameter must be one value of @ref RCCEx_PLLSAI_DIVR */

    uint32_t RTCClockSelection;      /*!< Specifies RTC Clock Prescalers Selection.
                                        This parameter can be a value of @ref RCC_RTC_Clock_Source */

    uint8_t TIMPresSelection;      /*!< Specifies TIM Clock Prescalers Selection.
                                        This parameter can be a value of @ref RCCEx_TIM_PRescaler_Selection */
}RCC_PeriphCLKInitTypeDef;

HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit);

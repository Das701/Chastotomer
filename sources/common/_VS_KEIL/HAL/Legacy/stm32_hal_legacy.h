#pragma once


#define DAC1_CHANNEL_2                  DAC_CHANNEL_2
#define __ADC1_CLK_ENABLE()
#define __ADC3_CLK_ENABLE()
#define ADC_CLOCKPRESCALER_PCLK_DIV2    0
#define ADC_RESOLUTION12b               0
#define ADC_CLOCKPRESCALER_PCLK_DIV4    1


#define TYPEPROGRAM_BYTE                0
#define TYPEPROGRAM_HALFWORD            0
#define TYPEPROGRAM_WORD                0
#define TYPEPROGRAM_DOUBLEWORD          0


#define FORMAT_BIN                      0
#define FORMAT_BCD                      0

#define __DMA1_CLK_ENABLE               __noop
#define __TIM7_CLK_ENABLE               __noop
#define __DAC_CLK_ENABLE                __noop

#define HAL_ADC_STATE_BUSY_REG          1
#define HAL_ADC_STATE_BUSY_INJ          1
#define HAL_ADC_STATE_EOC_REG           1
#define HAL_ADC_STATE_EOC_INJ           1
#define HAL_ADC_STATE_ERROR             1
#define HAL_ADC_STATE_BUSY              1
#define HAL_ADC_STATE_AWD               1

#define __GPIOA_CLK_ENABLE      __HAL_RCC_GPIOA_CLK_ENABLE
#define __USB_OTG_FS_CLK_ENABLE __HAL_RCC_USB_OTG_FS_CLK_ENABLE
#define __SYSCFG_CLK_ENABLE     __HAL_RCC_SYSCFG_CLK_ENABLE
#define __GPIOB_CLK_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE
#define __USB_OTG_HS_CLK_ENABLE          __HAL_RCC_USB_OTG_HS_CLK_ENABLE


#define  GPIO_SPEED_LOW                           GPIO_SPEED_FREQ_LOW
#define  GPIO_SPEED_MEDIUM                        GPIO_SPEED_FREQ_MEDIUM
#define  GPIO_SPEED_FAST                          GPIO_SPEED_FREQ_HIGH
#define  GPIO_SPEED_HIGH                          GPIO_SPEED_FREQ_VERY_HIGH
#pragma once
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_dma.h"


#define ADC_CLOCK_SYNC_PCLK_DIV2        0x00000000U
#define ADC_RESOLUTION_12B              0x00000000U
#define ADC_EXTERNALTRIGCONVEDGE_RISING 0
#define ADC_EXTERNALTRIGCONV_Ext_IT11   0
#define ADC_DATAALIGN_RIGHT             0x00000000U
#define ADC_EOC_SINGLE_CONV             0x00000001U
#define ADC_CHANNEL_2                   2
#define ADC_CHANNEL_8                   8
#define ADC_CHANNEL_9                   9
#define ADC_SAMPLETIME_3CYCLES          0x00000000U
#define ADC_SAMPLETIME_28CYCLES         0
#define ADC_EXTERNALTRIGCONV_T1_CC1     0x00000000U
#define ADC_EXTERNALTRIGCONVEDGE_NONE   0x00000000U
#define ADC_SOFTWARE_START              0


typedef struct
{
    uint32_t ClockPrescaler;        /*!< Select ADC clock prescaler. The clock is common for 
                                    all the ADCs.
                                    This parameter can be a value of @ref ADC_ClockPrescaler */
    uint32_t Resolution;            /*!< Configures the ADC resolution.
                                    This parameter can be a value of @ref ADC_Resolution */
    uint32_t DataAlign;             /*!< Specifies ADC data alignment to right (MSB on register bit 11 and LSB on register bit 0) (default setting)
                                    or to left (if regular group: MSB on register bit 15 and LSB on register bit 4, if injected group (MSB kept as signed value due to potential negative value after offset application): MSB on register bit 14 and LSB on register bit 3).
                                    This parameter can be a value of @ref ADC_Data_align */
    uint32_t ScanConvMode;          /*!< Configures the sequencer of regular and injected groups.
                                    This parameter can be associated to parameter 'DiscontinuousConvMode' to have main sequence subdivided in successive parts.
                                    If disabled: Conversion is performed in single mode (one channel converted, the one defined in rank 1).
                                    Parameters 'NbrOfConversion' and 'InjectedNbrOfConversion' are discarded (equivalent to set to 1).
                                    If enabled:  Conversions are performed in sequence mode (multiple ranks defined by 'NbrOfConversion'/'InjectedNbrOfConversion' and each channel rank).
                                    Scan direction is upward: from rank1 to rank 'n'.
                                    This parameter can be set to ENABLE or DISABLE */
    uint32_t EOCSelection;          /*!< Specifies what EOC (End Of Conversion) flag is used for conversion by polling and interruption: end of conversion of each rank or complete sequence.
                                    This parameter can be a value of @ref ADC_EOCSelection.
                                    Note: For injected group, end of conversion (flag&IT) is raised only at the end of the sequence.
                                    Therefore, if end of conversion is set to end of each conversion, injected group should not be used with interruption (HAL_ADCEx_InjectedStart_IT)
                                    or polling (HAL_ADCEx_InjectedStart and HAL_ADCEx_InjectedPollForConversion). By the way, polling is still possible since driver will use an estimated timing for end of injected conversion.
                                    Note: If overrun feature is intended to be used, use ADC in mode 'interruption' (function HAL_ADC_Start_IT() ) with parameter EOCSelection set to end of each conversion or in mode 'transfer by DMA' (function HAL_ADC_Start_DMA()).
                                    If overrun feature is intended to be bypassed, use ADC in mode 'polling' or 'interruption' with parameter EOCSelection must be set to end of sequence */
    uint32_t ContinuousConvMode;    /*!< Specifies whether the conversion is performed in single mode (one conversion) or continuous mode for regular group,
                                    after the selected trigger occurred (software start or external trigger).
                                    This parameter can be set to ENABLE or DISABLE. */
    uint32_t NbrOfConversion;       /*!< Specifies the number of ranks that will be converted within the regular group sequencer.
                                    To use regular group sequencer and convert several ranks, parameter 'ScanConvMode' must be enabled.
                                    This parameter must be a number between Min_Data = 1 and Max_Data = 16. */
    uint32_t DiscontinuousConvMode; /*!< Specifies whether the conversions sequence of regular group is performed in Complete-sequence/Discontinuous-sequence (main sequence subdivided in successive parts).
                                    Discontinuous mode is used only if sequencer is enabled (parameter 'ScanConvMode'). If sequencer is disabled, this parameter is discarded.
                                    Discontinuous mode can be enabled only if continuous mode is disabled. If continuous mode is enabled, this parameter setting is discarded.
                                    This parameter can be set to ENABLE or DISABLE. */
    uint32_t NbrOfDiscConversion;   /*!< Specifies the number of discontinuous conversions in which the  main sequence of regular group (parameter NbrOfConversion) will be subdivided.
                                    If parameter 'DiscontinuousConvMode' is disabled, this parameter is discarded.
                                    This parameter must be a number between Min_Data = 1 and Max_Data = 8. */
    uint32_t ExternalTrigConv;      /*!< Selects the external event used to trigger the conversion start of regular group.
                                    If set to ADC_SOFTWARE_START, external triggers are disabled.
                                    If set to external trigger source, triggering is on event rising edge by default.
                                    This parameter can be a value of @ref ADC_External_trigger_Source_Regular */
    uint32_t ExternalTrigConvEdge;  /*!< Selects the external trigger edge of regular group.
                                    If trigger is set to ADC_SOFTWARE_START, this parameter is discarded.
                                    This parameter can be a value of @ref ADC_External_trigger_edge_Regular */
    uint32_t DMAContinuousRequests; /*!< Specifies whether the DMA requests are performed in one shot mode (DMA transfer stop when number of conversions is reached)
                                    or in Continuous mode (DMA transfer unlimited, whatever number of conversions).
                                    Note: In continuous mode, DMA must be configured in circular mode. Otherwise an overrun will be triggered when DMA buffer maximum pointer is reached.
                                    Note: This parameter must be modified when no conversion is on going on both regular and injected groups (ADC disabled, or ADC enabled without continuous mode or external trigger that could launch a conversion).
                                    This parameter can be set to ENABLE or DISABLE. */
}ADC_InitTypeDef;


typedef struct
{
    ADC_TypeDef                   *Instance;                   /*!< Register base address */

    ADC_InitTypeDef               Init;                        /*!< ADC required parameters */

    __IO uint32_t                 NbrOfCurrentConversionRank;  /*!< ADC number of current conversion rank */

    DMA_HandleTypeDef             *DMA_Handle;                 /*!< Pointer DMA Handler */

    HAL_LockTypeDef               Lock;                        /*!< ADC locking object */

    __IO uint32_t                 State;                       /*!< ADC communication state */

    __IO uint32_t                 ErrorCode;                   /*!< ADC Error code */
}ADC_HandleTypeDef;


typedef struct 
{
    uint32_t Channel;                /*!< Specifies the channel to configure into ADC regular group.
                                     This parameter can be a value of @ref ADC_channels */
    uint32_t Rank;                   /*!< Specifies the rank in the regular group sequencer.
                                     This parameter must be a number between Min_Data = 1 and Max_Data = 16 */
    uint32_t SamplingTime;           /*!< Sampling time value to be set for the selected channel.
                                     Unit: ADC clock cycles
                                     Conversion time is the addition of sampling time and processing time (12 ADC clock cycles at ADC resolution 12 bits, 11 cycles at 10 bits, 9 cycles at 8 bits, 7 cycles at 6 bits).
                                     This parameter can be a value of @ref ADC_sampling_times
                                     Caution: This parameter updates the parameter property of the channel, that can be used into regular and/or injected groups.
                                     If this same channel has been previously configured in the other group (regular/injected), it will be updated to last setting.
                                     Note: In case of usage of internal measurement channels (VrefInt/Vbat/TempSensor),
                                     sampling time constraints must be respected (sampling time can be adjusted in function of ADC clock frequency and sampling time setting)
                                     Refer to device datasheet for timings values, parameters TS_vrefint, TS_temp (values rough order: 4us min). */
    uint32_t Offset;                 /*!< Reserved for future use, can be set to 0 */
}ADC_ChannelConfTypeDef;

typedef struct
{
    uint32_t DAC_Trigger;       /*!< Specifies the external trigger for the selected DAC channel.
                                This parameter can be a value of @ref DAC_trigger_selection */

    uint32_t DAC_OutputBuffer;  /*!< Specifies whether the DAC channel output buffer is enabled or disabled.
                                This parameter can be a value of @ref DAC_output_buffer */
}DAC_ChannelConfTypeDef;


extern "C"
{
    HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef* hadc);
    HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef* hadc, ADC_ChannelConfTypeDef* sConfig);
    uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef* hadc);
    HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc);
    HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout);
    HAL_StatusTypeDef HAL_ADC_Start_IT(ADC_HandleTypeDef* hadc);
    void HAL_ADC_IRQHandler(ADC_HandleTypeDef* hadc);
}

uint32_t HAL_ADC_GetState(ADC_HandleTypeDef* hadc);

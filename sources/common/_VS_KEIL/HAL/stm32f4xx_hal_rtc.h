#pragma once


#define RTC_HOURFORMAT_24   0x00000000U
#define RTC_HOURFORMAT_12   0x00000040U


#define RTC_OUTPUT_DISABLE          0x00000000U
#define RTC_OUTPUT_ALARMA           0x00200000U
#define RTC_OUTPUT_ALARMB           0x00400000U
#define RTC_OUTPUT_WAKEUP           0x00600000U

#define RTC_OUTPUT_POLARITY_HIGH    0x00000000U
#define RTC_OUTPUT_POLARITY_LOW     0x00100000U

#define RTC_OUTPUT_TYPE_OPENDRAIN   0x00000000U
#define RTC_OUTPUT_TYPE_PUSHPULL    0x00040000U

#define RTC_WEEKDAY_MONDAY          ((uint8_t)0x01)
#define RTC_WEEKDAY_TUESDAY         ((uint8_t)0x02)
#define RTC_WEEKDAY_WEDNESDAY       ((uint8_t)0x03)
#define RTC_WEEKDAY_THURSDAY        ((uint8_t)0x04)
#define RTC_WEEKDAY_FRIDAY          ((uint8_t)0x05)
#define RTC_WEEKDAY_SATURDAY        ((uint8_t)0x06)
#define RTC_WEEKDAY_SUNDAY          ((uint8_t)0x07)

#define RTC_DAYLIGHTSAVING_SUB1H    0x00020000U
#define RTC_DAYLIGHTSAVING_ADD1H    0x00010000U
#define RTC_DAYLIGHTSAVING_NONE     0x00000000U

#define RTC_STOREOPERATION_RESET    0x00000000U
#define RTC_STOREOPERATION_SET      0x00040000U

#define RTC_BKP_DR0                       0x00000000U
#define RTC_BKP_DR1                       0x00000001U
#define RTC_BKP_DR2                       0x00000002U
#define RTC_BKP_DR3                       0x00000003U
#define RTC_BKP_DR4                       0x00000004U
#define RTC_BKP_DR5                       0x00000005U
#define RTC_BKP_DR6                       0x00000006U
#define RTC_BKP_DR7                       0x00000007U
#define RTC_BKP_DR8                       0x00000008U
#define RTC_BKP_DR9                       0x00000009U
#define RTC_BKP_DR10                      0x0000000AU
#define RTC_BKP_DR11                      0x0000000BU
#define RTC_BKP_DR12                      0x0000000CU
#define RTC_BKP_DR13                      0x0000000DU
#define RTC_BKP_DR14                      0x0000000EU
#define RTC_BKP_DR15                      0x0000000FU
#define RTC_BKP_DR16                      0x00000010U
#define RTC_BKP_DR17                      0x00000011U
#define RTC_BKP_DR18                      0x00000012U
#define RTC_BKP_DR19                      0x00000013U


typedef struct
{
    uint32_t HourFormat;      /*!< Specifies the RTC Hour Format.
                              This parameter can be a value of @ref RTC_Hour_Formats */

    uint32_t AsynchPrediv;    /*!< Specifies the RTC Asynchronous Predivider value.
                              This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x7F */

    uint32_t SynchPrediv;     /*!< Specifies the RTC Synchronous Predivider value.
                              This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x7FFFU */

    uint32_t OutPut;          /*!< Specifies which signal will be routed to the RTC output.   
                              This parameter can be a value of @ref RTC_Output_selection_Definitions */

    uint32_t OutPutPolarity;  /*!< Specifies the polarity of the output signal.  
                              This parameter can be a value of @ref RTC_Output_Polarity_Definitions */

    uint32_t OutPutType;      /*!< Specifies the RTC Output Pin mode.   
                              This parameter can be a value of @ref RTC_Output_Type_ALARM_OUT */

} RTC_InitTypeDef;


typedef enum
{
    HAL_RTC_STATE_RESET = 0x00U,  /*!< RTC not yet initialized or disabled */
    HAL_RTC_STATE_READY = 0x01U,  /*!< RTC initialized and ready for use   */
    HAL_RTC_STATE_BUSY = 0x02U,  /*!< RTC process is ongoing              */
    HAL_RTC_STATE_TIMEOUT = 0x03U,  /*!< RTC timeout state                   */
    HAL_RTC_STATE_ERROR = 0x04U   /*!< RTC error state                     */
} HAL_RTCStateTypeDef;


typedef struct
{
    uint8_t Hours;            /*!< Specifies the RTC Time Hour.
                              This parameter must be a number between Min_Data = 0 and Max_Data = 12 if the RTC_HourFormat_12 is selected.
                              This parameter must be a number between Min_Data = 0 and Max_Data = 23 if the RTC_HourFormat_24 is selected  */

    uint8_t Minutes;          /*!< Specifies the RTC Time Minutes.
                              This parameter must be a number between Min_Data = 0 and Max_Data = 59 */

    uint8_t Seconds;          /*!< Specifies the RTC Time Seconds.
                              This parameter must be a number between Min_Data = 0 and Max_Data = 59 */

    uint8_t TimeFormat;       /*!< Specifies the RTC AM/PM Time.
                              This parameter can be a value of @ref RTC_AM_PM_Definitions */

    uint32_t SubSeconds;     /*!< Specifies the RTC_SSR RTC Sub Second register content.
                             This parameter corresponds to a time unit range between [0-1] Second
                             with [1 Sec / SecondFraction +1] granularity */

    uint32_t SecondFraction;  /*!< Specifies the range or granularity of Sub Second register content
                              corresponding to Synchronous pre-scaler factor value (PREDIV_S)
                              This parameter corresponds to a time unit range between [0-1] Second
                              with [1 Sec / SecondFraction +1] granularity.
                              This field will be used only by HAL_RTC_GetTime function */

    uint32_t DayLightSaving;  /*!< Specifies DayLight Save Operation.
                              This parameter can be a value of @ref RTC_DayLightSaving_Definitions */

    uint32_t StoreOperation;  /*!< Specifies RTC_StoreOperation value to be written in the BCK bit 
                              in CR register to store the operation.
                              This parameter can be a value of @ref RTC_StoreOperation_Definitions */

} RTC_TimeTypeDef; 


typedef struct
{
    RTC_TypeDef                 *Instance;  /*!< Register base address    */
    
    RTC_InitTypeDef             Init;       /*!< RTC required parameters  */
    
    HAL_LockTypeDef             Lock;       /*!< RTC locking object       */
    
    __IO HAL_RTCStateTypeDef    State;      /*!< Time communication state */

} RTC_HandleTypeDef;


typedef struct
{
    uint8_t WeekDay;  /*!< Specifies the RTC Date WeekDay.
                      This parameter can be a value of @ref RTC_WeekDay_Definitions */

    uint8_t Month;    /*!< Specifies the RTC Date Month (in BCD format).
                      This parameter can be a value of @ref RTC_Month_Date_Definitions */

    uint8_t Date;     /*!< Specifies the RTC Date.
                      This parameter must be a number between Min_Data = 1 and Max_Data = 31 */

    uint8_t Year;     /*!< Specifies the RTC Date Year.
                      This parameter must be a number between Min_Data = 0 and Max_Data = 99 */

}RTC_DateTypeDef;


HAL_StatusTypeDef HAL_RTC_GetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format);
HAL_StatusTypeDef HAL_RTC_GetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format);
HAL_StatusTypeDef HAL_RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format);
HAL_StatusTypeDef HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format);
HAL_StatusTypeDef HAL_RTC_Init(RTC_HandleTypeDef *hrtc);
uint32_t HAL_RTCEx_BKUPRead(RTC_HandleTypeDef *hrtc, uint32_t BackupRegister);
void HAL_RTCEx_BKUPWrite(RTC_HandleTypeDef *hrtc, uint32_t BackupRegister, uint32_t Data);

#pragma once
#include "stm32f4xx_hal_def.h"


#define SPI_MODE_MASTER             0
#define SPI_DIRECTION_2LINES        0x00000000U
#define SPI_DATASIZE_8BIT           0x00000000U
#define SPI_POLARITY_LOW            0x00000000U
#define SPI_PHASE_1EDGE             0x00000000U
#define SPI_NSS_HARD_INPUT          0x00000000U
#define SPI_BAUDRATEPRESCALER_2     0x00000000U
#define SPI_FIRSTBIT_MSB            0x00000000U
#define SPI_TIMODE_DISABLE          0x00000000U
#define SPI_CRCCALCULATION_DISABLE  0x00000000U


#define SPI_POLARITY_LOW            0x00000000U
#define SPI_POLARITY_HIGH           0


#define SPI_NSS_SOFT                0
#define SPI_NSS_HARD_INPUT          0x00000000U
#define SPI_NSS_HARD_OUTPUT         0x00040000U


typedef enum
{
    HAL_SPI_STATE_RESET = 0x00U,    /*!< Peripheral not Initialized                         */
    HAL_SPI_STATE_READY = 0x01U,    /*!< Peripheral Initialized and ready for use           */
    HAL_SPI_STATE_BUSY = 0x02U,    /*!< an internal process is ongoing                     */
    HAL_SPI_STATE_BUSY_TX = 0x03U,    /*!< Data Transmission process is ongoing               */
    HAL_SPI_STATE_BUSY_RX = 0x04U,    /*!< Data Reception process is ongoing                  */
    HAL_SPI_STATE_BUSY_TX_RX = 0x05U,    /*!< Data Transmission and Reception process is ongoing */
    HAL_SPI_STATE_ERROR = 0x06U     /*!< SPI error state                                    */
} HAL_SPI_StateTypeDef;


typedef struct
{
    uint32_t Mode;               /*!< Specifies the SPI operating mode.
                                 This parameter can be a value of @ref SPI_Mode */

    uint32_t Direction;           /*!< Specifies the SPI bidirectional mode state.
                                  This parameter can be a value of @ref SPI_Direction */

    uint32_t DataSize;           /*!< Specifies the SPI data size.
                                 This parameter can be a value of @ref SPI_Data_Size */

    uint32_t CLKPolarity;        /*!< Specifies the serial clock steady state.
                                 This parameter can be a value of @ref SPI_Clock_Polarity */

    uint32_t CLKPhase;           /*!< Specifies the clock active edge for the bit capture.
                                 This parameter can be a value of @ref SPI_Clock_Phase */

    uint32_t NSS;                /*!< Specifies whether the NSS signal is managed by
                                 hardware (NSS pin) or by software using the SSI bit.
                                 This parameter can be a value of @ref SPI_Slave_Select_management */

    uint32_t BaudRatePrescaler;  /*!< Specifies the Baud Rate prescaler value which will be
                                 used to configure the transmit and receive SCK clock.
                                 This parameter can be a value of @ref SPI_BaudRate_Prescaler
                                 @note The communication clock is derived from the master
                                 clock. The slave clock does not need to be set. */

    uint32_t FirstBit;           /*!< Specifies whether data transfers start from MSB or LSB bit.
                                 This parameter can be a value of @ref SPI_MSB_LSB_transmission */

    uint32_t TIMode;             /*!< Specifies if the TI mode is enabled or not.
                                 This parameter can be a value of @ref SPI_TI_mode */

    uint32_t CRCCalculation;     /*!< Specifies if the CRC calculation is enabled or not.
                                 This parameter can be a value of @ref SPI_CRC_Calculation */

    uint32_t CRCPolynomial;      /*!< Specifies the polynomial used for the CRC calculation.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 65535 */
}SPI_InitTypeDef;


typedef struct __SPI_HandleTypeDef
{
    SPI_TypeDef                *Instance;    /* SPI registers base address */

    SPI_InitTypeDef            Init;         /* SPI communication parameters */

    uint8_t                    *pTxBuffPtr;  /* Pointer to SPI Tx transfer Buffer */

    uint16_t                   TxXferSize;   /* SPI Tx Transfer size */

    __IO uint16_t              TxXferCount;  /* SPI Tx Transfer Counter */

    uint8_t                    *pRxBuffPtr;  /* Pointer to SPI Rx transfer Buffer */

    uint16_t                   RxXferSize;   /* SPI Rx Transfer size */

    __IO uint16_t              RxXferCount;  /* SPI Rx Transfer Counter */

    void(*RxISR)(struct __SPI_HandleTypeDef * hspi); /* function pointer on Rx ISR */

    void(*TxISR)(struct __SPI_HandleTypeDef * hspi); /* function pointer on Tx ISR */

    DMA_HandleTypeDef          *hdmatx;      /* SPI Tx DMA Handle parameters   */

    DMA_HandleTypeDef          *hdmarx;      /* SPI Rx DMA Handle parameters   */

    HAL_LockTypeDef            Lock;         /* Locking object                 */

    __IO HAL_SPI_StateTypeDef  State;        /* SPI communication state */

    __IO uint32_t              ErrorCode;    /* SPI Error code */
}SPI_HandleTypeDef;


HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi);

#pragma once
#include "stm32f4xx_hal_def.h"


#define UART_WORDLENGTH_8B      0x00000000U
#define UART_STOPBITS_1         0x00000000U
#define UART_PARITY_NONE        0x00000000U
#define UART_MODE_TX_RX         0
#define UART_HWCONTROL_NONE     0x00000000U
#define UART_OVERSAMPLING_16    0x00000000U


typedef struct
{
    uint32_t BaudRate;                  /*!< This member configures the UART communication baud rate.
                                        The baud rate is computed using the following formula:
                                        - IntegerDivider = ((PCLKx) / (8 * (OVR8+1) * (huart->Init.BaudRate)))
                                        - FractionalDivider = ((IntegerDivider - ((uint32_t) IntegerDivider)) * 8 * (OVR8+1)) + 0.5 
                                        Where OVR8 is the "oversampling by 8 mode" configuration bit in the CR1 register. */

    uint32_t WordLength;                /*!< Specifies the number of data bits transmitted or received in a frame.
                                        This parameter can be a value of @ref UART_Word_Length */

    uint32_t StopBits;                  /*!< Specifies the number of stop bits transmitted.
                                        This parameter can be a value of @ref UART_Stop_Bits */

    uint32_t Parity;                    /*!< Specifies the parity mode.
                                        This parameter can be a value of @ref UART_Parity
                                        @note When parity is enabled, the computed parity is inserted
                                        at the MSB position of the transmitted data (9th bit when
                                        the word length is set to 9 data bits; 8th bit when the
                                        word length is set to 8 data bits). */

    uint32_t Mode;                      /*!< Specifies whether the Receive or Transmit mode is enabled or disabled.
                                        This parameter can be a value of @ref UART_Mode */

    uint32_t HwFlowCtl;                 /*!< Specifies whether the hardware flow control mode is enabled
                                        or disabled.
                                        This parameter can be a value of @ref UART_Hardware_Flow_Control */

    uint32_t OverSampling;              /*!< Specifies whether the Over sampling 8 is enabled or disabled, to achieve higher speed (up to fPCLK/8).
                                        This parameter can be a value of @ref UART_Over_Sampling */ 
}UART_InitTypeDef;


typedef struct
{
    USART_TypeDef                 *Instance;        /*!< UART registers base address        */

    UART_InitTypeDef              Init;             /*!< UART communication parameters      */
//
//    uint8_t                       *pTxBuffPtr;      /*!< Pointer to UART Tx transfer Buffer */
//
//    uint16_t                      TxXferSize;       /*!< UART Tx Transfer size              */
//
//    __IO uint16_t                 TxXferCount;      /*!< UART Tx Transfer Counter           */
//
//    uint8_t                       *pRxBuffPtr;      /*!< Pointer to UART Rx transfer Buffer */
//
//    uint16_t                      RxXferSize;       /*!< UART Rx Transfer size              */
//
//    __IO uint16_t                 RxXferCount;      /*!< UART Rx Transfer Counter           */  
//
//    DMA_HandleTypeDef             *hdmatx;          /*!< UART Tx DMA Handle parameters      */
//
//    DMA_HandleTypeDef             *hdmarx;          /*!< UART Rx DMA Handle parameters      */
//
//    HAL_LockTypeDef               Lock;             /*!< Locking object                     */
//
//    __IO HAL_UART_StateTypeDef    gState;           /*!< UART state information related to global Handle management 
//                                                    and also related to Tx operations.
//                                                    This parameter can be a value of @ref HAL_UART_StateTypeDef */
//
//    __IO HAL_UART_StateTypeDef    RxState;          /*!< UART state information related to Rx operations.
//                                                    This parameter can be a value of @ref HAL_UART_StateTypeDef */
//
//    __IO uint32_t                 ErrorCode;        /*!< UART Error code                    */

}UART_HandleTypeDef;


extern "C"
{
    HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);
    HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
    HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
    void HAL_UART_IRQHandler(UART_HandleTypeDef *huart);
}

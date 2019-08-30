#pragma once


#define FMC_NORSRAM_TypeDef             FMC_Bank1_TypeDef
#define FMC_NORSRAM_EXTENDED_TypeDef    FMC_Bank1E_TypeDef
#define FMC_NORSRAM_DEVICE              FMC_Bank1
#define FMC_NORSRAM_EXTENDED_DEVICE     FMC_Bank1E
#define FMC_NORSRAM_BANK1               0x00000000U
#define FMC_DATA_ADDRESS_MUX_DISABLE    0x00000000U
#define FMC_MEMORY_TYPE_NOR             0x00000008U
#define FMC_NORSRAM_MEM_BUS_WIDTH_8     0x00000000U
#define FMC_BURST_ACCESS_MODE_DISABLE   0x00000000U
#define FMC_WAIT_SIGNAL_POLARITY_LOW    0x00000000U
#define FMC_WRAP_MODE_DISABLE           0x00000000U
#define FMC_WAIT_TIMING_BEFORE_WS       0x00000000U
#define FMC_WRITE_OPERATION_ENABLE      0x00001000U
#define FMC_WAIT_SIGNAL_DISABLE         0x00000000U
#define FMC_EXTENDED_MODE_DISABLE       0x00000000U
#define FMC_ASYNCHRONOUS_WAIT_DISABLE   0x00000000U
#define FMC_WRITE_BURST_DISABLE         0x00000000U
#define FMC_ACCESS_MODE_C               0x20000000U


#define __FMC_NORSRAM_ENABLE(__INSTANCE__, __BANK__)  ((__INSTANCE__)->BTCR[(__BANK__)] |= FMC_BCR1_MBKEN)


typedef struct
{
    uint32_t NSBank;                       /*!< Specifies the NORSRAM memory device that will be used.
                                           This parameter can be a value of @ref FMC_NORSRAM_Bank                     */

    uint32_t DataAddressMux;               /*!< Specifies whether the address and data values are
                                           multiplexed on the data bus or not. 
                                           This parameter can be a value of @ref FMC_Data_Address_Bus_Multiplexing    */

    uint32_t MemoryType;                   /*!< Specifies the type of external memory attached to
                                           the corresponding memory device.
                                           This parameter can be a value of @ref FMC_Memory_Type                      */

    uint32_t MemoryDataWidth;              /*!< Specifies the external memory device width.
                                           This parameter can be a value of @ref FMC_NORSRAM_Data_Width               */

    uint32_t BurstAccessMode;              /*!< Enables or disables the burst access mode for Flash memory,
                                           valid only with synchronous burst Flash memories.
                                           This parameter can be a value of @ref FMC_Burst_Access_Mode                */

    uint32_t WaitSignalPolarity;           /*!< Specifies the wait signal polarity, valid only when accessing
                                           the Flash memory in burst mode.
                                           This parameter can be a value of @ref FMC_Wait_Signal_Polarity             */

    uint32_t WrapMode;                     /*!< Enables or disables the Wrapped burst access mode for Flash
                                           memory, valid only when accessing Flash memories in burst mode.
                                           This parameter can be a value of @ref FMC_Wrap_Mode
                                           This mode is not available for the STM32F446/467/479xx devices                    */

    uint32_t WaitSignalActive;             /*!< Specifies if the wait signal is asserted by the memory one
                                           clock cycle before the wait state or during the wait state,
                                           valid only when accessing memories in burst mode. 
                                           This parameter can be a value of @ref FMC_Wait_Timing                      */

    uint32_t WriteOperation;               /*!< Enables or disables the write operation in the selected device by the FMC. 
                                           This parameter can be a value of @ref FMC_Write_Operation                  */

    uint32_t WaitSignal;                   /*!< Enables or disables the wait state insertion via wait
                                           signal, valid for Flash memory access in burst mode. 
                                           This parameter can be a value of @ref FMC_Wait_Signal                      */

    uint32_t ExtendedMode;                 /*!< Enables or disables the extended mode.
                                           This parameter can be a value of @ref FMC_Extended_Mode                    */

    uint32_t AsynchronousWait;             /*!< Enables or disables wait signal during asynchronous transfers,
                                           valid only with asynchronous Flash memories.
                                           This parameter can be a value of @ref FMC_AsynchronousWait                 */

    uint32_t WriteBurst;                   /*!< Enables or disables the write burst operation.
                                           This parameter can be a value of @ref FMC_Write_Burst                      */

    uint32_t ContinuousClock;              /*!< Enables or disables the FMC clock output to external memory devices.
                                           This parameter is only enabled through the FMC_BCR1 register, and don't care 
                                           through FMC_BCR2..4 registers.
                                           This parameter can be a value of @ref FMC_Continous_Clock                  */

    uint32_t WriteFifo;                    /*!< Enables or disables the write FIFO used by the FMC controller.
                                           This parameter is only enabled through the FMC_BCR1 register, and don't care 
                                           through FMC_BCR2..4 registers.
                                           This parameter can be a value of @ref FMC_Write_FIFO
                                           This mode is available only for the STM32F446/469/479xx devices            */

    uint32_t PageSize;                     /*!< Specifies the memory page size.
                                           This parameter can be a value of @ref FMC_Page_Size                        */
} FMC_NORSRAM_InitTypeDef;


typedef struct
{
    uint32_t AddressSetupTime;             /*!< Defines the number of HCLK cycles to configure
                                           the duration of the address setup time. 
                                           This parameter can be a value between Min_Data = 0 and Max_Data = 15.
                                           @note This parameter is not used with synchronous NOR Flash memories.      */

    uint32_t AddressHoldTime;              /*!< Defines the number of HCLK cycles to configure
                                           the duration of the address hold time.
                                           This parameter can be a value between Min_Data = 1 and Max_Data = 15. 
                                           @note This parameter is not used with synchronous NOR Flash memories.      */

    uint32_t DataSetupTime;                /*!< Defines the number of HCLK cycles to configure
                                           the duration of the data setup time.
                                           This parameter can be a value between Min_Data = 1 and Max_Data = 255.
                                           @note This parameter is used for SRAMs, ROMs and asynchronous multiplexed 
                                           NOR Flash memories.                                                        */

    uint32_t BusTurnAroundDuration;        /*!< Defines the number of HCLK cycles to configure
                                           the duration of the bus turnaround.
                                           This parameter can be a value between Min_Data = 0 and Max_Data = 15.
                                           @note This parameter is only used for multiplexed NOR Flash memories.      */

    uint32_t CLKDivision;                  /*!< Defines the period of CLK clock output signal, expressed in number of 
                                           HCLK cycles. This parameter can be a value between Min_Data = 2 and Max_Data = 16.
                                           @note This parameter is not used for asynchronous NOR Flash, SRAM or ROM 
                                           accesses.                                                                  */

    uint32_t DataLatency;                  /*!< Defines the number of memory clock cycles to issue
                                           to the memory before getting the first data.
                                           The parameter value depends on the memory type as shown below:
                                           - It must be set to 0 in case of a CRAM
                                           - It is don't care in asynchronous NOR, SRAM or ROM accesses
                                           - It may assume a value between Min_Data = 2 and Max_Data = 17 in NOR Flash memories
                                           with synchronous burst mode enable                                       */

    uint32_t AccessMode;                   /*!< Specifies the asynchronous access mode. 
                                           This parameter can be a value of @ref FMC_Access_Mode                      */
} FMC_NORSRAM_TimingTypeDef;


HAL_StatusTypeDef  FMC_NORSRAM_Init(FMC_NORSRAM_TypeDef *Device, FMC_NORSRAM_InitTypeDef *Init);
HAL_StatusTypeDef  FMC_NORSRAM_Timing_Init(FMC_NORSRAM_TypeDef *Device, FMC_NORSRAM_TimingTypeDef *Timing, uint32_t Bank);

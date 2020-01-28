#pragma once


#define DMA_CHANNEL_7           0x0E000000U     /*!< DMA Channel 7 */

#define DMA_PERIPH_TO_MEMORY    0x00000000U     /*!< Peripheral to memory direction */
#define DMA_MEMORY_TO_PERIPH    ((uint32_t)0)   /*!< Memory to peripheral direction */
#define DMA_MEMORY_TO_MEMORY    ((uint32_t)0)   /*!< Memory to memory direction     */

#define DMA_PINC_ENABLE         ((uint32_t)0)   /*!< Peripheral increment mode enable  */
#define DMA_PINC_DISABLE        0x00000000U     /*!< Peripheral increment mode disable */

#define DMA_MINC_ENABLE         ((uint32_t)0)   /*!< Memory increment mode enable  */
#define DMA_MINC_DISABLE        0x00000000U     /*!< Memory increment mode disable */

#define DMA_PDATAALIGN_BYTE     0x00000000U     /*!< Peripheral data alignment: Byte     */
#define DMA_PDATAALIGN_HALFWORD ((uint32_t)0)   /*!< Peripheral data alignment: HalfWord */
#define DMA_PDATAALIGN_WORD     ((uint32_t)0)   /*!< Peripheral data alignment: Word     */

#define DMA_MDATAALIGN_BYTE     0x00000000U     /*!< Memory data alignment: Byte     */
#define DMA_MDATAALIGN_HALFWORD ((uint32_t)0)   /*!< Memory data alignment: HalfWord */
#define DMA_MDATAALIGN_WORD     ((uint32_t)0)   /*!< Memory data alignment: Word     */

#define DMA_NORMAL              0x00000000U     /*!< Normal mode                  */
#define DMA_CIRCULAR            ((uint32_t)0)   /*!< Circular mode                */
#define DMA_PFCTRL              ((uint32_t)0)   /*!< Peripheral flow control mode */

#define DMA_PRIORITY_LOW        0x00000000U     /*!< Priority level: Low       */
#define DMA_PRIORITY_MEDIUM     ((uint32_t)0)   /*!< Priority level: Medium    */
#define DMA_PRIORITY_HIGH       ((uint32_t)0)   /*!< Priority level: High      */
#define DMA_PRIORITY_VERY_HIGH  ((uint32_t)0)   /*!< Priority level: Very High */

#define DMA_FIFOMODE_DISABLE    0x00000000U     /*!< FIFO mode disable */
#define DMA_FIFOMODE_ENABLE     ((uint32_t)0)   /*!< FIFO mode enable  */

#define DMA_FIFO_THRESHOLD_1QUARTERFULL     0x00000000U     /*!< FIFO threshold 1 quart full configuration  */
#define DMA_FIFO_THRESHOLD_HALFFULL         ((uint32_t)0)   /*!< FIFO threshold half full configuration     */
#define DMA_FIFO_THRESHOLD_3QUARTERSFULL    ((uint32_t)0)   /*!< FIFO threshold 3 quarts full configuration */
#define DMA_FIFO_THRESHOLD_FULL             ((uint32_t)0)   /*!< FIFO threshold full configuration          */

#define DMA_MBURST_SINGLE       0x00000000U
#define DMA_MBURST_INC4         ((uint32_t)0)  
#define DMA_MBURST_INC8         ((uint32_t)0)  
#define DMA_MBURST_INC16        ((uint32_t)0)

#define DMA_PBURST_SINGLE       0x00000000U
#define DMA_PBURST_INC4         ((uint32_t)0)
#define DMA_PBURST_INC8         ((uint32_t)0)
#define DMA_PBURST_INC16        ((uint32_t)0)


typedef enum
{
    HAL_DMA_STATE_RESET = 0x00U,  /*!< DMA not yet initialized or disabled */
    HAL_DMA_STATE_READY = 0x01U,  /*!< DMA initialized and ready for use   */
    HAL_DMA_STATE_BUSY = 0x02U,  /*!< DMA process is ongoing              */
    HAL_DMA_STATE_TIMEOUT = 0x03U,  /*!< DMA timeout state                   */
    HAL_DMA_STATE_ERROR = 0x04U,  /*!< DMA error state                     */
    HAL_DMA_STATE_ABORT = 0x05U,  /*!< DMA Abort state                     */

} HAL_DMA_StateTypeDef;


typedef struct
{
    uint32_t Channel;              /*!< Specifies the channel used for the specified stream. 
                                   This parameter can be a value of @ref DMA_Channel_selection                    */

    uint32_t Direction;            /*!< Specifies if the data will be transferred from memory to peripheral, 
                                   from memory to memory or from peripheral to memory.
                                   This parameter can be a value of @ref DMA_Data_transfer_direction              */

    uint32_t PeriphInc;            /*!< Specifies whether the Peripheral address register should be incremented or not.
                                   This parameter can be a value of @ref DMA_Peripheral_incremented_mode          */

    uint32_t MemInc;               /*!< Specifies whether the memory address register should be incremented or not.
                                   This parameter can be a value of @ref DMA_Memory_incremented_mode              */

    uint32_t PeriphDataAlignment;  /*!< Specifies the Peripheral data width.
                                   This parameter can be a value of @ref DMA_Peripheral_data_size                 */

    uint32_t MemDataAlignment;     /*!< Specifies the Memory data width.
                                   This parameter can be a value of @ref DMA_Memory_data_size                     */

    uint32_t Mode;                 /*!< Specifies the operation mode of the DMAy Streamx.
                                   This parameter can be a value of @ref DMA_mode
                                   @note The circular buffer mode cannot be used if the memory-to-memory
                                   data transfer is configured on the selected Stream                        */

    uint32_t Priority;             /*!< Specifies the software priority for the DMAy Streamx.
                                   This parameter can be a value of @ref DMA_Priority_level                       */

    uint32_t FIFOMode;             /*!< Specifies if the FIFO mode or Direct mode will be used for the specified stream.
                                   This parameter can be a value of @ref DMA_FIFO_direct_mode
                                   @note The Direct mode (FIFO mode disabled) cannot be used if the 
                                   memory-to-memory data transfer is configured on the selected stream       */

    uint32_t FIFOThreshold;        /*!< Specifies the FIFO threshold level.
                                   This parameter can be a value of @ref DMA_FIFO_threshold_level                  */

    uint32_t MemBurst;             /*!< Specifies the Burst transfer configuration for the memory transfers. 
                                   It specifies the amount of data to be transferred in a single non interruptible
                                   transaction.
                                   This parameter can be a value of @ref DMA_Memory_burst 
                                   @note The burst mode is possible only if the address Increment mode is enabled. */

    uint32_t PeriphBurst;          /*!< Specifies the Burst transfer configuration for the peripheral transfers. 
                                   It specifies the amount of data to be transferred in a single non interruptible 
                                   transaction. 
                                   This parameter can be a value of @ref DMA_Peripheral_burst
                                   @note The burst mode is possible only if the address Increment mode is enabled. */

} DMA_InitTypeDef;


typedef struct __DMA_HandleTypeDef
{
    DMA_Stream_TypeDef         *Instance;                                                        /*!< Register base address                  */

    DMA_InitTypeDef            Init;                                                             /*!< DMA communication parameters           */

    HAL_LockTypeDef            Lock;                                                             /*!< DMA locking object                     */

    __IO HAL_DMA_StateTypeDef  State;                                                            /*!< DMA transfer state                     */

    void                       *Parent;                                                          /*!< Parent object state                    */

    void(*XferCpltCallback)(struct __DMA_HandleTypeDef * hdma);         /*!< DMA transfer complete callback         */

    void(*XferHalfCpltCallback)(struct __DMA_HandleTypeDef * hdma);     /*!< DMA Half transfer complete callback    */

    void(*XferM1CpltCallback)(struct __DMA_HandleTypeDef * hdma);       /*!< DMA transfer complete Memory1 callback */

    void(*XferM1HalfCpltCallback)(struct __DMA_HandleTypeDef * hdma);   /*!< DMA transfer Half complete Memory1 callback */

    void(*XferErrorCallback)(struct __DMA_HandleTypeDef * hdma);        /*!< DMA transfer error callback            */

    void(*XferAbortCallback)(struct __DMA_HandleTypeDef * hdma);        /*!< DMA transfer Abort callback            */

    __IO uint32_t              ErrorCode;                                                        /*!< DMA Error code                          */

    uint32_t                   StreamBaseAddress;                                                /*!< DMA Stream Base Address                */

    uint32_t                   StreamIndex;                                                      /*!< DMA Stream Index                       */

} DMA_HandleTypeDef;


void HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_Init(DMA_HandleTypeDef *hdma);


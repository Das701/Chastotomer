#pragma once

#define DMA2D_M2M                   0x00000000U              /*!< DMA2D memory to memory transfer mode  */

#define DMA2D_INPUT_ARGB8888        0x00000000U              /*!< ARGB8888 color mode */
#define DMA2D_INPUT_RGB888          0x00000001U              /*!< RGB888 color mode   */
#define DMA2D_INPUT_RGB565          0x00000002U              /*!< RGB565 color mode   */
#define DMA2D_INPUT_ARGB1555        0x00000003U              /*!< ARGB1555 color mode */
#define DMA2D_INPUT_ARGB4444        0x00000004U              /*!< ARGB4444 color mode */
#define DMA2D_INPUT_L8              0x00000005U              /*!< L8 color mode       */
#define DMA2D_INPUT_AL44            0x00000006U              /*!< AL44 color mode     */
#define DMA2D_INPUT_AL88            0x00000007U              /*!< AL88 color mode     */
#define DMA2D_INPUT_L4              0x00000008U              /*!< L4 color mode       */
#define DMA2D_INPUT_A8              0x00000009U              /*!< A8 color mode       */
#define DMA2D_INPUT_A4              0x0000000AU              /*!< A4 color mode       */                            */

typedef struct
{
    uint32_t             Mode;               /*!< Configures the DMA2D transfer mode.
                                                  This parameter can be one value of @ref DMA2D_Mode. */

    uint32_t             ColorMode;          /*!< Configures the color format of the output image.
                                                  This parameter can be one value of @ref DMA2D_Output_Color_Mode. */

    uint32_t             OutputOffset;       /*!< Specifies the Offset value.
                                                  This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0x3FFF. */

} DMA2D_InitTypeDef;

typedef struct
{
    uint32_t             InputOffset;        /*!< Configures the DMA2D foreground or background offset.
                                                  This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0x3FFF. */

    uint32_t             InputColorMode;     /*!< Configures the DMA2D foreground or background color mode.
                                                  This parameter can be one value of @ref DMA2D_Input_Color_Mode. */

    uint32_t             AlphaMode;          /*!< Configures the DMA2D foreground or background alpha mode.
                                                  This parameter can be one value of @ref DMA2D_Alpha_Mode. */

    uint32_t             InputAlpha;         /*!< Specifies the DMA2D foreground or background alpha value and color value in case of A8 or A4 color mode.
                                                  This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF except for the color modes detailed below.
                                                  @note In case of A8 or A4 color mode (ARGB), this parameter must be a number between
                                                  Min_Data = 0x00000000 and Max_Data = 0xFFFFFFFF where
                                                  - InputAlpha[24:31] is the alpha value ALPHA[0:7]
                                                  - InputAlpha[16:23] is the red value RED[0:7]
                                                  - InputAlpha[8:15] is the green value GREEN[0:7]
                                                  - InputAlpha[0:7] is the blue value BLUE[0:7]. */

} DMA2D_LayerCfgTypeDef;

#define MAX_DMA2D_LAYER  2U

#define DMA2D_NO_MODIF_ALPHA        0x00000000U              /*!< No modification of the alpha channel value                     */
#define DMA2D_REPLACE_ALPHA         0x00000001U              /*!< Replace original alpha channel value by programmed alpha value */
#define DMA2D_COMBINE_ALPHA         0x00000002U              /*!< Replace original alpha channel value by programmed alpha value
                                                                  with original alpha channel value                                */


typedef struct __DMA2D_HandleTypeDef
{
    DMA2D_TypeDef               *Instance;                                                    /*!< DMA2D register base address.               */
//
    DMA2D_InitTypeDef           Init;                                                         /*!< DMA2D communication parameters.            */
//
    void(*XferCpltCallback)(struct __DMA2D_HandleTypeDef * hdma2d);  /*!< DMA2D transfer complete callback.          */
//
//    void(*XferErrorCallback)(struct __DMA2D_HandleTypeDef * hdma2d); /*!< DMA2D transfer error callback.             */
//
    DMA2D_LayerCfgTypeDef       LayerCfg[MAX_DMA2D_LAYER];                                    /*!< DMA2D Layers parameters                    */
//
//    HAL_LockTypeDef             Lock;                                                         /*!< DMA2D lock.                                */
//
//    __IO HAL_DMA2D_StateTypeDef State;                                                        /*!< DMA2D transfer state.                      */
//
//    __IO uint32_t               ErrorCode;                                                    /*!< DMA2D error code.                          */
} DMA2D_HandleTypeDef;


HAL_StatusTypeDef HAL_DMA2D_Init(DMA2D_HandleTypeDef *hdma2d);
HAL_StatusTypeDef HAL_DMA2D_ConfigLayer(DMA2D_HandleTypeDef *hdma2d, uint32_t LayerIdx);
HAL_StatusTypeDef HAL_DMA2D_Start(DMA2D_HandleTypeDef *hdma2d, uint32_t pdata, uint32_t DstAddress, uint32_t Width, uint32_t Height);
HAL_StatusTypeDef HAL_DMA2D_PollForTransfer(DMA2D_HandleTypeDef *hdma2d, uint32_t Timeout);

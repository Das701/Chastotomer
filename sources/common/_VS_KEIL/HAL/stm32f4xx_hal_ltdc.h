#pragma once

#define LTDC_HSPOLARITY_AL                0x00000000U   /*!< Horizontal Synchronization is active low. */
#define LTDC_HSPOLARITY_AH                0            /*!< Horizontal Synchronization is active high. */

#define LTDC_VSPOLARITY_AL                0x00000000U   /*!< Vertical Synchronization is active low. */
#define LTDC_VSPOLARITY_AH                0            /*!< Vertical Synchronization is active high. */

#define LTDC_DEPOLARITY_AL                0x00000000U   /*!< Data Enable, is active low. */
#define LTDC_DEPOLARITY_AH                0            /*!< Data Enable, is active high. */

#define LTDC_PCPOLARITY_IPC               0x00000000U   /*!< input pixel clock. */
#define LTDC_PCPOLARITY_IIPC              0            /*!< inverted input pixel clock. */

#define LTDC_PIXEL_FORMAT_ARGB8888        0x00000000U   /*!< ARGB8888 LTDC pixel format */
#define LTDC_PIXEL_FORMAT_RGB888          0x00000001U   /*!< RGB888 LTDC pixel format   */
#define LTDC_PIXEL_FORMAT_RGB565          0x00000002U   /*!< RGB565 LTDC pixel format   */
#define LTDC_PIXEL_FORMAT_ARGB1555        0x00000003U   /*!< ARGB1555 LTDC pixel format */
#define LTDC_PIXEL_FORMAT_ARGB4444        0x00000004U   /*!< ARGB4444 LTDC pixel format */
#define LTDC_PIXEL_FORMAT_L8              0x00000005U   /*!< L8 LTDC pixel format       */
#define LTDC_PIXEL_FORMAT_AL44            0x00000006U   /*!< AL44 LTDC pixel format     */
#define LTDC_PIXEL_FORMAT_AL88            0x00000007U   /*!< AL88 LTDC pixel format     */

#define LTDC_BLENDING_FACTOR1_CA          0x00000400U   /*!< Blending factor : Cte Alpha */
#define LTDC_BLENDING_FACTOR1_PAxCA       0x00000600U   /*!< Blending factor : Cte Alpha x Pixel Alpha*/

#define LTDC_BLENDING_FACTOR2_CA          0x00000005U   /*!< Blending factor : Cte Alpha */
#define LTDC_BLENDING_FACTOR2_PAxCA       0x00000007U   /*!< Blending factor : Cte Alpha x Pixel Alpha*/


typedef struct
{
    uint8_t Blue;                    /*!< Configures the blue value.
                                          This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF. */

    uint8_t Green;                   /*!< Configures the green value.
                                          This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF. */

    uint8_t Red;                     /*!< Configures the red value.
                                          This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF. */

    uint8_t Reserved;                /*!< Reserved 0xFF */
} LTDC_ColorTypeDef;

typedef struct
{
    uint32_t            HSPolarity;                /*!< configures the horizontal synchronization polarity.
                                                        This parameter can be one value of @ref LTDC_HS_POLARITY */

    uint32_t            VSPolarity;                /*!< configures the vertical synchronization polarity.
                                                        This parameter can be one value of @ref LTDC_VS_POLARITY */

    uint32_t            DEPolarity;                /*!< configures the data enable polarity.
                                                        This parameter can be one of value of @ref LTDC_DE_POLARITY */

    uint32_t            PCPolarity;                /*!< configures the pixel clock polarity.
                                                        This parameter can be one of value of @ref LTDC_PC_POLARITY */

    uint32_t            HorizontalSync;            /*!< configures the number of Horizontal synchronization width.
                                                        This parameter must be a number between Min_Data = 0x000 and Max_Data = 0xFFF. */

    uint32_t            VerticalSync;              /*!< configures the number of Vertical synchronization height.
                                                        This parameter must be a number between Min_Data = 0x000 and Max_Data = 0x7FF. */

    uint32_t            AccumulatedHBP;            /*!< configures the accumulated horizontal back porch width.
                                                        This parameter must be a number between Min_Data = LTDC_HorizontalSync and Max_Data = 0xFFF. */

    uint32_t            AccumulatedVBP;            /*!< configures the accumulated vertical back porch height.
                                                        This parameter must be a number between Min_Data = LTDC_VerticalSync and Max_Data = 0x7FF. */

    uint32_t            AccumulatedActiveW;        /*!< configures the accumulated active width.
                                                        This parameter must be a number between Min_Data = LTDC_AccumulatedHBP and Max_Data = 0xFFF. */

    uint32_t            AccumulatedActiveH;        /*!< configures the accumulated active height.
                                                        This parameter must be a number between Min_Data = LTDC_AccumulatedVBP and Max_Data = 0x7FF. */

    uint32_t            TotalWidth;                /*!< configures the total width.
                                                        This parameter must be a number between Min_Data = LTDC_AccumulatedActiveW and Max_Data = 0xFFF. */

    uint32_t            TotalHeigh;                /*!< configures the total height.
                                                        This parameter must be a number between Min_Data = LTDC_AccumulatedActiveH and Max_Data = 0x7FF. */

    LTDC_ColorTypeDef   Backcolor;                 /*!< Configures the background color. */
} LTDC_InitTypeDef;

typedef struct
{
    int i;
    LTDC_TypeDef                *Instance;                /*!< LTDC Register base address                */
//
    LTDC_InitTypeDef            Init;                     /*!< LTDC parameters                           */
//
//    LTDC_LayerCfgTypeDef        LayerCfg[MAX_LAYER];      /*!< LTDC Layers parameters                    */
//
//    HAL_LockTypeDef             Lock;                     /*!< LTDC Lock                                 */
//
//    __IO HAL_LTDC_StateTypeDef  State;                    /*!< LTDC state                                */
//
//    __IO uint32_t               ErrorCode;                /*!< LTDC Error code                           */
} LTDC_HandleTypeDef;

typedef struct
{
    uint32_t WindowX0;                   /*!< Configures the Window Horizontal Start Position.
                                              This parameter must be a number between Min_Data = 0x000 and Max_Data = 0xFFF. */

    uint32_t WindowX1;                   /*!< Configures the Window Horizontal Stop Position.
                                              This parameter must be a number between Min_Data = 0x000 and Max_Data = 0xFFF. */

    uint32_t WindowY0;                   /*!< Configures the Window vertical Start Position.
                                              This parameter must be a number between Min_Data = 0x000 and Max_Data = 0x7FF. */

    uint32_t WindowY1;                   /*!< Configures the Window vertical Stop Position.
                                              This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0x7FF. */

    uint32_t PixelFormat;                /*!< Specifies the pixel format.
                                              This parameter can be one of value of @ref LTDC_Pixelformat */

    uint32_t Alpha;                      /*!< Specifies the constant alpha used for blending.
                                              This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF. */

    uint32_t Alpha0;                     /*!< Configures the default alpha value.
                                              This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF. */

    uint32_t BlendingFactor1;            /*!< Select the blending factor 1.
                                              This parameter can be one of value of @ref LTDC_BlendingFactor1 */

    uint32_t BlendingFactor2;            /*!< Select the blending factor 2.
                                              This parameter can be one of value of @ref LTDC_BlendingFactor2 */

    uint32_t FBStartAdress;              /*!< Configures the color frame buffer address */

    uint32_t ImageWidth;                 /*!< Configures the color frame buffer line length.
                                              This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0x1FFF. */

    uint32_t ImageHeight;                /*!< Specifies the number of line in frame buffer.
                                              This parameter must be a number between Min_Data = 0x000 and Max_Data = 0x7FF. */

    LTDC_ColorTypeDef   Backcolor;       /*!< Configures the layer background color. */
} LTDC_LayerCfgTypeDef;

HAL_StatusTypeDef HAL_LTDC_Init(LTDC_HandleTypeDef *hltdc);
HAL_StatusTypeDef HAL_LTDC_ConfigLayer(LTDC_HandleTypeDef *hltdc, LTDC_LayerCfgTypeDef *pLayerCfg, uint32_t LayerIdx);
HAL_StatusTypeDef HAL_LTDC_ConfigCLUT(LTDC_HandleTypeDef *hltdc, uint32_t *pCLUT, uint32_t CLUTSize, uint32_t LayerIdx);
HAL_StatusTypeDef HAL_LTDC_EnableCLUT(LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx);

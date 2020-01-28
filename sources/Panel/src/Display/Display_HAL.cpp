#include "defines.h"
#include "stm32f4xx_hal.h"
#include "Display.h"
#include "Hardware/HAL.h"
#include <cstring>
#include <cstdlib>
#include "Display/Primitives.h"

using namespace Display::Primitives;


#define CS_OPEN         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define CS_CLOSE        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

#define SET_DC_COMMAND  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET)
#define SET_DC_DATA     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET)

#define SET_RES_LOW     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
#define SET_RES_HI      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)

using namespace Display::Primitives;

/// В этом буфере будем рисовать. Ширина равна 480 / 2 потому, что в байте хранятся 2 пикселя с 1 из 16-ти возможных градация каждая.
#define WIDTH_BUFFER    (240)
#define HEIGHT_BUFFER   (272)
static uint8 buffer[HEIGHT_BUFFER][WIDTH_BUFFER];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void SetLShiftFreq(uint freq)
{
    HAL_FSMC::WriteCommand(0xe6);   // set the LSHIFT (pixel clock) frequency
    HAL_FSMC::WriteData((uint8)(freq >> 16));
    HAL_FSMC::WriteData((uint8)(freq >> 8));
    HAL_FSMC::WriteData((uint8)(freq));
}


static void SetHorizPeriod(uint16 HT,   // Horizontal total period
                           uint16 HPS,  // Non-display period between the start of the horizontal sync signal nad the first display data
                           uint8  HPW,  // Sync pulse width
                           uint16 LPS,  // Horizontal sync pulse start location
                           uint16 LPSPP // for serial TFT interfact
)
{
    HAL_FSMC::WriteCommand(0xb4);
    HAL_FSMC::WriteData((uint8)(HT >> 8));      // 0x020d 525
    HAL_FSMC::WriteData((uint8)HT);
    HAL_FSMC::WriteData((uint8)(HPS >> 8));     // 0x0014 20
    HAL_FSMC::WriteData((uint8)(HPS));
    HAL_FSMC::WriteData(HPW);                   // 0x05
    HAL_FSMC::WriteData((uint8)(LPS >> 8));
    HAL_FSMC::WriteData((uint8)(LPS));
    HAL_FSMC::WriteData(LPSPP);
}


static void SetModeLCD(uint16 width, uint16 height)
{
    HAL_FSMC::WriteCommand(0xb0);
    HAL_FSMC::WriteData(0x20);
    HAL_FSMC::WriteData(0x80);
    HAL_FSMC::WriteData(0x01);
    HAL_FSMC::WriteData(0xdf);
    HAL_FSMC::WriteData(0x01);
    HAL_FSMC::WriteData(0x0f);
    HAL_FSMC::WriteData(0x00);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Init()
{
    HAL_FSMC::Reset();
    
    HAL_FSMC::WriteCommand(0x01);   // soft reset
    HAL_Delay(10);
    HAL_FSMC::WriteCommand(0xe0);   // set pll
    HAL_FSMC::WriteData(0x01);
    HAL_Delay(10);
    HAL_FSMC::WriteCommand(0xe0);   // set pll
    HAL_FSMC::WriteData(0x03);
    HAL_Delay(10);
    
    //HAL_FSMC::WriteCommand(0x29);   // Включить дисплей

    SetModeLCD(480, 272);

    HAL_FSMC::WriteCommand(0xf0);   // set pixel data interface
    HAL_FSMC::WriteData(0x03);      // 0x03 for 16bit, 0x00 for 8bit
    
    HAL_FSMC::WriteCommand(0x3a);
    HAL_FSMC::WriteData(0x50);

    // Set the MN of PLL
    HAL_FSMC::WriteCommand(0xe2);   // Set the PLL
    HAL_FSMC::WriteData(0x1d);
    HAL_FSMC::WriteData(0x02);
    HAL_FSMC::WriteData(0x54);

    HAL_Delay(100);

    SetLShiftFreq(0xfffff / 8);

    
    SetHorizPeriod(525, 25, 5, 0, 0);
    
    HAL_FSMC::WriteCommand(0xb6);   // set vert period
    HAL_FSMC::WriteData(0x01);
    HAL_FSMC::WriteData(0x24);
    HAL_FSMC::WriteData(0x00);
    HAL_FSMC::WriteData(0x0a);
    HAL_FSMC::WriteData(0x05);
    HAL_FSMC::WriteData(0x00);
    HAL_FSMC::WriteData(0x00);

    HAL_FSMC::WriteCommand(0x29);   // Включить дисплей
    
    HAL_FSMC::WriteCommand(0x2a);
    HAL_FSMC::WriteData(0x00);
    HAL_FSMC::WriteData(0x00);
    HAL_FSMC::WriteData(0x01);
    HAL_FSMC::WriteData(0xdf); // 0..479
    
    HAL_FSMC::WriteCommand(0x2b);
    HAL_FSMC::WriteData(0x00);
    HAL_FSMC::WriteData(0x00);
    HAL_FSMC::WriteData(0x01);
    HAL_FSMC::WriteData(0x0f); //0..271
    
    HAL_FSMC::WriteCommand(0x2c);
}

/// Этот буфер отображается на экране
//static uint8 front[240][320];
/// В этом буфере рисуем
//static uint8 RGB565_240x320[240][320] = { 0x00000000 };

//LTDC_HandleTypeDef hltdc;
//static void MX_LTDC_Init(void);
//static void MX_GPIO_Init(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void Display::Init()
//{
    //MX_GPIO_Init();
    //MX_LTDC_Init();

    //uint ColorTable[3] = { 0x00000000U, 0x7F7F7F7FU, 0xFFFFFFFFU };
    //
    //HAL_LTDC_EnableCLUT(&hltdc, 0);
    //
    //HAL_LTDC_ConfigCLUT(&hltdc, ColorTable, 2, 0);
    //
    //HAL_LTDC_SetAddress(&hltdc, (uint)&front, 0);
//}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Display::GetPixel(int x, int y)
{
    return &buffer[y][x / 2];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::BeginScene(Color color)
{
    std::memset(buffer, 0, HEIGHT_BUFFER * WIDTH_BUFFER * sizeof(buffer[0][0]));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::EndScene()
{
    HAL_FSMC::SendBuffer(buffer[0]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//static void MX_LTDC_Init(void)
//{
//    LTDC_LayerCfgTypeDef pLayerCfg = { 0 };
//
//    hltdc.Instance = LTDC;
//    hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
//    hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
//    hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AH;
//    hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IIPC;
//    hltdc.Init.HorizontalSync = 0;
//    hltdc.Init.VerticalSync = 0;
//    hltdc.Init.AccumulatedHBP = 70;
//    hltdc.Init.AccumulatedVBP = 13;
//    hltdc.Init.AccumulatedActiveW = 390;
//    hltdc.Init.AccumulatedActiveH = 253;
//    hltdc.Init.TotalWidth = 408;
//    hltdc.Init.TotalHeigh = 263;
//    hltdc.Init.Backcolor.Blue = 0;
//    hltdc.Init.Backcolor.Green = 0;
//    hltdc.Init.Backcolor.Red = 0;
//
//    if (HAL_LTDC_Init(&hltdc) != HAL_OK)
//    {
//        ERROR_HANDLER();
//    }
//
//    pLayerCfg.WindowX0 = 0;
//    pLayerCfg.WindowX1 = 320;
//    pLayerCfg.WindowY0 = 0;
//    pLayerCfg.WindowY1 = 240;
//    pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_L8;
//    pLayerCfg.Alpha = 0xFF;
//    pLayerCfg.Alpha0 = 0xFF;
//    pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
//    pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
//    pLayerCfg.FBStartAdress = (uint32_t)&(RGB565_240x320[0][0]);
//    pLayerCfg.ImageWidth = 320;
//    pLayerCfg.ImageHeight = 240;
//    pLayerCfg.Backcolor.Blue = 0;
//    pLayerCfg.Backcolor.Green = 0;
//    pLayerCfg.Backcolor.Red = 0;
//
//    if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
//    {
//        ERROR_HANDLER();
//    }
//}
//
////-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//static void MX_GPIO_Init(void)
//{
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    __HAL_RCC_GPIOE_CLK_ENABLE();
//    __HAL_RCC_GPIOD_CLK_ENABLE();
//    __HAL_RCC_GPIOC_CLK_ENABLE();
//
//    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
//
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
//
//    GPIO_InitStruct.Pin = GPIO_PIN_6;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//}

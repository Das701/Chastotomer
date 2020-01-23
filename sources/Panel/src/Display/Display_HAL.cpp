#include "defines.h"
#include "stm32f4xx_hal.h"
#include "Display.h"
#include "Hardware/HAL.h"
#include "Display/Primitives.h"
#include <cstring>
#include <cstdlib>


#define COLOR_BLACK 0x00
#define COLOR_WHITE 0xFF

#define CS_OPEN         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define CS_CLOSE        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

#define SET_DC_COMMAND  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET)
#define SET_DC_DATA     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET)

#define SET_RES_LOW     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
#define SET_RES_HI      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)

using namespace Display::Primitives;

/// В этом буфере будем рисовать. Ширина равна 256 / 2 потому, что в байте хранятся 2 пикселя с 1 из 16-ти возможных градация каждая.
#define WIDTH_BUFFER    (256)
#define HEIGHT_BUFFER   (64)
static uint8 front[HEIGHT_BUFFER][WIDTH_BUFFER];


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
    
    HAL_FSMC::WriteCommand(0x29);   // Включить дисплей

    HAL_FSMC::WriteCommand(0xb0);   // set lcd mode
    HAL_FSMC::WriteData(0x20);
    HAL_FSMC::WriteData(0x80);
    HAL_FSMC::WriteData(0x01);
    HAL_FSMC::WriteData(0xdf);
    HAL_FSMC::WriteData(0x01);
    HAL_FSMC::WriteData(0x0f);
    HAL_FSMC::WriteData(0x00);

    HAL_FSMC::WriteCommand(0xf0);   // set pixel data interface
    HAL_FSMC::WriteData(0x00);      // 0x03 for 16bit, 0x00 for 8bit

    // Set the MN of PLL
    HAL_FSMC::WriteCommand(0xe2);   // Set the PLL
    HAL_FSMC::WriteData(0x1d);
    HAL_FSMC::WriteData(0x02);
    HAL_FSMC::WriteData(0x54);

    HAL_Delay(100);

    HAL_FSMC::WriteCommand(0xe6);   // set the LSHIFT (pixel clock) frequency
    HAL_FSMC::WriteData(0x01);
    HAL_FSMC::WriteData(0x99);
    HAL_FSMC::WriteData(0x9a);

    // Set front porch and back porch
    HAL_FSMC::WriteCommand(0xb4);   // set hori period
    HAL_FSMC::WriteData(0x02);
    HAL_FSMC::WriteData(0x0d);
    HAL_FSMC::WriteData(0x00);
    HAL_FSMC::WriteData(0x14);
    HAL_FSMC::WriteData(0x05);
    HAL_FSMC::WriteData(0x00);
    HAL_FSMC::WriteData(0x00);
    HAL_FSMC::WriteData(0x00);

    HAL_FSMC::WriteCommand(0xb6);   // set vert period
    HAL_FSMC::WriteData(0x01);
    HAL_FSMC::WriteData(0x24);
    HAL_FSMC::WriteData(0x00);
    HAL_FSMC::WriteData(0x0a);
    HAL_FSMC::WriteData(0x05);
    HAL_FSMC::WriteData(0x00);
    HAL_FSMC::WriteData(0x00);

    HAL_FSMC::WriteCommand(0x29);   // Включить дисплей

    while(true)
    {
        HAL_FSMC::WriteCommand(0x2a);   // set column address
        HAL_FSMC::WriteData(0x00);
        HAL_FSMC::WriteData(0x00);
        HAL_FSMC::WriteData(0x01);
        HAL_FSMC::WriteData(0xdf);

        HAL_FSMC::WriteCommand(0x2b);   // set page address
        HAL_FSMC::WriteData(0x00);
        HAL_FSMC::WriteData(0x00);
        HAL_FSMC::WriteData(0x01);
        HAL_FSMC::WriteData(0x0f);

        HAL_FSMC::WriteCommand(0x2c);   // Write memory start

        uint16 data = (uint16)std::rand();

        for(int i = 0; i < 272 * 480; i++)
        {
            HAL_FSMC::WriteData(data++);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Display::GetPixel(int x, int y)
{
    return &front[y][x];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::BeginScene(Color color)
{
    std::memset(front, color.value, WIDTH_BUFFER * HEIGHT_BUFFER);
}

void Display::EndScene()
{
}

#include "defines.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/String.h"
#include <cstring>
#include <cstdlib>


using namespace Display::Primitives;


#define CS_OPEN         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define CS_CLOSE        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

#define SET_DC_COMMAND  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET)
#define SET_DC_DATA     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET)

#define SET_RES_LOW     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
#define SET_RES_HI      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)

using namespace Display::Primitives;

/// В этом буфере будем рисовать. Ширина равна 480 / 2 потому, что в байте хранятся 2 пикселя с 1 из 16-ти возможных градация каждая.
#define WIDTH_BUFFER    (480)
#define HEIGHT_BUFFER   (272 / 2)
static uint8 buffer[HEIGHT_BUFFER][WIDTH_BUFFER];


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


static void SetModeLCD()
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
    HAL_TIM::DelayMS(10);
    HAL_FSMC::WriteCommand(0xe0);   // set pll
    HAL_FSMC::WriteData(0x01);
    HAL_TIM::DelayMS(10);
    HAL_FSMC::WriteCommand(0xe0);   // set pll
    HAL_FSMC::WriteData(0x03);
    HAL_TIM::DelayMS(10);
    
    //HAL_FSMC::WriteCommand(0x29);   // Включить дисплей

    SetModeLCD();

    HAL_FSMC::WriteCommand(0xf0);   // set pixel data interface
    HAL_FSMC::WriteData(0x03);      // 0x03 for 16bit, 0x00 for 8bit
    
    HAL_FSMC::WriteCommand(0x3a);
    HAL_FSMC::WriteData(0x50);

    // Set the MN of PLL
    HAL_FSMC::WriteCommand(0xe2);   // Set the PLL
    HAL_FSMC::WriteData(0x1d);
    HAL_FSMC::WriteData(0x02);
    HAL_FSMC::WriteData(0x54);

    HAL_TIM::DelayMS(100);

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


    for (int i = 0; i < 16; i++)
    {
        float step = 1.0F / 16.0F * i;

        COLOR(i) = Color::MakeFromFloat(step, step, step);
    }
}


uint8 *Display::GetPixel(int x, int y)
{
    return &buffer[y][x];
}


static uint timeStart = 0;


void Display::BeginScene()
{
    timeStart = TIME_MS;

    std::memset(&buffer[0][0], Color::BLACK.value, Display::WIDTH * Display::HEIGHT / 2);
}


void Display::EndScene()
{
    Text(String("%d ms", TIME_MS - timeStart).c_str()).Write(100, 100);

    uint start = TIME_MS;
    static uint timeSend = 0;
    static uint timeFull = 0;

    Text(String("Время засылки %d", timeSend).c_str()).Write(100, 120);
    Text(String("Полное время %d", timeFull).c_str()).Write(100, 80);

    HAL_FSMC::SendBuffer(buffer[0], TopRow());

    timeSend = TIME_MS - start;

    timeFull = TIME_MS - timeStart;
}


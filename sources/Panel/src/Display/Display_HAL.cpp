#include "defines.h"
#include "stm32f4xx_hal.h"
#include "Display.h"
#include "Hardware/HAL.h"
#include <cstring>
#include <cstdlib>
#include "Display/Primitives.h"

#include "Display/Font/BigFont1.h"

using namespace Display::Primitives;


#define CS_OPEN         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define CS_CLOSE        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

#define SET_DC_COMMAND  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET)
#define SET_DC_DATA     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET)

#define SET_RES_LOW     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
#define SET_RES_HI      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)

using namespace Display::Primitives;

/// � ���� ������ ����� ��������. ������ ����� 480 / 2 ������, ��� � ����� �������� 2 ������� � 1 �� 16-�� ��������� �������� ������.
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
    HAL_Delay(10);
    HAL_FSMC::WriteCommand(0xe0);   // set pll
    HAL_FSMC::WriteData(0x01);
    HAL_Delay(10);
    HAL_FSMC::WriteCommand(0xe0);   // set pll
    HAL_FSMC::WriteData(0x03);
    HAL_Delay(10);
    
    //HAL_FSMC::WriteCommand(0x29);   // �������� �������

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

    HAL_FSMC::WriteCommand(0x29);   // �������� �������
    
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Display::GetPixel(int x, int y)
{
    return &buffer[y][x / 2];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::BeginScene()
{
    std::memset(buffer, 0, HEIGHT_BUFFER * WIDTH_BUFFER * sizeof(buffer[0][0]));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::EndScene()
{
    HAL_FSMC::SendBuffer(buffer[0]);
}


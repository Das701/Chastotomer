#include "defines.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"
#include "Utils/String.h"
#include <cmath>
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
#define HEIGHT_BUFFER   (272 / Display::NUM_PARTS)
static uint8 buffer[HEIGHT_BUFFER][WIDTH_BUFFER];

static const uint8 *startBuffer = &buffer[0][0];
static const uint8 *endBuffer = startBuffer + WIDTH_BUFFER * HEIGHT_BUFFER;


static uint8 lineBackground[Display::WIDTH * 2];    // Эта последовательность байт используется для отрисовки фона


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



void Display::Init()
{
    HAL_FSMC::Reset();
    
    HAL_FSMC::WriteCommand(0x01);                       // soft reset

    HAL_TIM::DelayMS(10);

    HAL_FSMC::WriteCommand(0xe0, 0x01);                 // set pll

    HAL_TIM::DelayMS(10);
    
    HAL_FSMC::WriteCommand(0xe0, 0x03);                 // set pll
    
    HAL_TIM::DelayMS(10);
    
    SetModeLCD();

    HAL_FSMC::WriteCommand(0xf0, 0x02);                 // set pixel data interface 0x03 for 16bit, 0x00 for 8bit

    HAL_FSMC::WriteCommand(0x3a, 0x50);

    // Set the MN of PLL
    HAL_FSMC::WriteCommand(0xe2, 0x1d, 0x02, 0x54);     // Set the PLL

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

    Font::Set(PTypeFont::GOST16B);

    Font::SetSpacing(2);


//    int num = 4;

    uint8 *pointer = lineBackground;

    for (int i = 0; i < Display::WIDTH * 2; i += 2)
    {
        *pointer++ = Color::GREEN_10.value;
        *pointer++ = Color::GREEN_25.value;
    }
}


void Display::BeginScene()
{
    for (int i = 0; i < Display::HEIGHT / Display::NUM_PARTS; i += 2)
    {
          std::memcpy(&buffer[i][0], lineBackground, Display::WIDTH);
          std::memcpy(&buffer[i + 1][0], lineBackground + 1, Display::WIDTH);

//        std::memcpy(&buffer[i][0], lineBackground, Display::WIDTH);
//        std::memcpy(&buffer[i + 1][0], lineBackground + 1, Display::WIDTH);
//        std::memcpy(&buffer[i + 2][0], lineBackground + 2, Display::WIDTH);
//        std::memcpy(&buffer[i + 3][0], lineBackground + 3, Display::WIDTH);
    }
}


void Display::EndScene()
{
    HAL_FSMC::SendBuffer(buffer[0], TopRow());
}


static Color current = Color::BLACK;

Color Color::GetCurrent()
{
    return current;
}


void Color::SetAsCurrent()
{
    current = Color(value);
}


void Point::Draw(int x, int y, Color color)
{
    current = color;

    y -= Display::TopRow();

    if (x >= 0 && x < Display::WIDTH && y >= 0 && y < Display::HEIGHT / Display::NUM_PARTS)
    {
        buffer[y][x] = current.value;
    }
}


void Point::Draw(int x, int y)
{
    y -= Display::TopRow();

    if (x >= 0 && x < Display::WIDTH && y >= 0 && y < Display::HEIGHT / Display::NUM_PARTS)
    {
        buffer[y][x] = current.value; 
    }
}


void HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    Draw(x, y);
}


void HLine::Draw(int x, int y)
{
    y -= Display::TopRow();

    if (x >= 0 && x < Display::WIDTH && y >= 0 && y < Display::HEIGHT / Display::NUM_PARTS)
    {
        int end = x + length;

        if (end >= Display::WIDTH)
        {
            end = Display::WIDTH - 1;
        }

        uint8 *pointer = &buffer[y][x];

        for (int i = x; i < end; i++)
        {
            *pointer++ = current.value;
        }
    }
}


void VLine::Draw(int x, int y)
{
    y -= Display::TopRow();

    if (x >= 0 && x < Display::WIDTH)
    {
        int height = length;

        if (y < 0)
        {
            while (y < 0 && height > 0)
            {
                y++;
                height--;
            }
        }

        if (height > 0)
        {
            if (y < Display::HEIGHT / Display::NUM_PARTS)
            {
                uint8 *pointer = &buffer[y][x];

                while (pointer < endBuffer && height > 0)
                {
                    *pointer = current.value;
                    pointer += Display::WIDTH;
                    height--;
                }
            }
        }
    }
}


void Line::Draw(int x1, int y1, int x2, int y2)
{
    if ((x2 - x1) == 0 && (y2 - y1) == 0)
    {
        ++x1;
    }
    int x = x1;
    int y = y1;
    int dx = (int)std::fabsf((float)(x2 - x1));
    int dy = (int)std::fabsf((float)(y2 - y1));
    int s1 = Math::Sign(x2 - x1);
    int s2 = Math::Sign(y2 - y1);
    int temp;
    int exchange = 0;
    if (dy > dx)
    {
        temp = dx;
        dx = dy;
        dy = temp;
        exchange = 1;
    }
    int e = 2 * dy - dx;
    int i = 0;
    for (; i <= dx; i++)
    {
        Point().Draw(x, y);

        while (e >= 0)
        {
            if (exchange)
            {
                x += s1;
            }
            else
            {
                y += s2;
            }
            e = e - 2 * dx;
        }
        if (exchange)
        {
            y += s2;
        }
        else
        {
            x += s1;
        }
        e = e + 2 * dy;
    }
}

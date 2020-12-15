#include "defines.h"
#include "Display/Colors.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/VCP.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"
#include "Utils/String.h"
#include <cmath>
#include <cstring>
#include <cstdlib>


using namespace Primitives;


#define CS_OPEN         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define CS_CLOSE        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

#define SET_DC_COMMAND  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET)
#define SET_DC_DATA     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET)

#define SET_RES_LOW     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
#define SET_RES_HI      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)

using namespace Primitives;

/// В этом буфере будем рисовать. Ширина равна 480 / 2 потому, что в байте хранятся 2 пикселя с 1 из 16-ти возможных градация каждая.
#define WIDTH_BUFFER    (480)
#define HEIGHT_BUFFER   (272 / Display::NUM_PARTS)
static uint8 buffer[HEIGHT_BUFFER][WIDTH_BUFFER];

static const uint8 *startBuffer = &buffer[0][0];
static const uint8 *endBuffer = startBuffer + WIDTH_BUFFER * HEIGHT_BUFFER; //-V2563


uint8 lineBackground[Display::PHYSICAL_WIDTH * 2];    // Эта последовательность байт используется для отрисовки фона


static void SetLShiftFreq(uint freq)
{
    HAL_FSMC::WriteCommand(0xe6);   // set the LSHIFT (pixel clock) frequency
    HAL_FSMC::WriteData((uint8)(freq >> 16)); //-V2533
    HAL_FSMC::WriteData((uint8)(freq >> 8)); //-V2533
    HAL_FSMC::WriteData((uint8)(freq)); //-V2533
}


static void SetHorizPeriod(uint16 HT,   // Horizontal total period
                           uint16 HPS,  // Non-display period between the start of the horizontal sync signal nad the first display data
                           uint8  HPW,  // Sync pulse width
                           uint16 LPS,  // Horizontal sync pulse start location
                           uint16 LPSPP // for serial TFT interfact
)
{
    HAL_FSMC::WriteCommand(0xb4);
    HAL_FSMC::WriteData((uint8)(HT >> 8));      // 0x020d 525 //-V2533
    HAL_FSMC::WriteData((uint8)HT); //-V2533
    HAL_FSMC::WriteData((uint8)(HPS >> 8));     // 0x0014 20 //-V2533
    HAL_FSMC::WriteData((uint8)(HPS)); //-V2533
    HAL_FSMC::WriteData(HPW);                   // 0x05
    HAL_FSMC::WriteData((uint8)(LPS >> 8)); //-V2533
    HAL_FSMC::WriteData((uint8)(LPS)); //-V2533
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



void Display::InitHardware()
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

    uint8 *pointer = lineBackground;

    for (int i = 0; i < Display::PHYSICAL_WIDTH * 2; i += 2)
    {
        *pointer++ = Color::GREEN_10.value;
        *pointer++ = Color::GREEN_25.value;
    }
}


#define POINTER_BUFFER(x, y) (&buffer[0][0] + ((y) * Display::Width()) + (x))


void Display::BeginScene(int x, int y)
{
    if (x == -1)
    {
        drawingScene = true;

        for (int i = 0; i < PHYSICAL_HEIGHT / NUM_PARTS; i += 2)
        {
            std::memcpy(&buffer[i][0], lineBackground, PHYSICAL_WIDTH);
            std::memcpy(&buffer[i + 1][0], lineBackground + 1, PHYSICAL_WIDTH); //-V2563
        }
    }
    else
    {
        uint8 *pointer = lineBackground;

        if ((x + y) % 2 != 0)
        {
            pointer++;
        }

        for (int row = 0; row < Display::Height(); row++)
        {
            std::memcpy(POINTER_BUFFER(0, row), pointer + (row % 2 == 0 ? 0 : 1), (size_t)Display::Width()); //-V2533 //-V2563
        }
    }
}


void Display::EndScene()
{
    HAL_FSMC::SendBuffer(buffer[0], 0, TopRow(), PHYSICAL_WIDTH, PHYSICAL_HEIGHT, 2);

    if (sendToSCPI)
    {
        int numPart = (PHYSICAL_HEIGHT / NUM_PARTS) / TopRow();
        
        uint data[WIDTH_BUFFER];
        
        for (int row = 0; row < HEIGHT_BUFFER; row++)
        {
            for (int col = 0; col < WIDTH_BUFFER; col++)
            {
                data[col] = COLOR(buffer[row][col]);
            }
        
            VCP::SendDataAsynch((const uint8 *)data, WIDTH_BUFFER * 4); //-V2533
        }

        if (numPart == NUM_PARTS - 1)
        {
            sendToSCPI = false;
        }
    }

    drawingScene = false;
}


void Display::SendToFSMC(int x0, int y0)
{
    HAL_FSMC::SendBuffer(buffer[0], x0, y0, Width(), Height(), 1);
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


static int Ymax()
{
    return (Display::Height() == Display::PHYSICAL_HEIGHT) ? (Display::Height() / Display::NUM_PARTS) : Display::Height();
}


void Point::Draw(int x, int y, Color color)
{
    current = color;

    y -= Display::TopRow();

    if (x >= 0 && x < Display::Width() && y >= 0 && y < Ymax())
    {
        *POINTER_BUFFER(x, y) = current.value; //-V2563
    }
}


void Point::Draw(int x, int y)
{
    y -= Display::TopRow();

    if (x >= 0 && x < Display::Width() && y >= 0 && y < Ymax())
    {
        *POINTER_BUFFER(x, y) = current.value;  //-V2563
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

    if (x >= 0 && x < Display::Width() && y >= 0 && y < Ymax())
    {
        int end = x + length;

        if (end >= Display::Width())
        {
            end = Display::Width() - 1;
        }

        uint8 *pointer = POINTER_BUFFER(x, y); //-V2563

        for (int i = x; i < end; i++)
        {
            *pointer++ = current.value;
        }
    }
}


void VLine::Draw(int x, int y)
{
    y -= Display::TopRow();

    if (x >= 0 && x < Display::Width())
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
            if (y < Ymax())
            {
                uint8 *pointer = POINTER_BUFFER(x, y); //-V2563

                while (pointer < endBuffer && height > 0)
                {
                    *pointer = current.value;
                    pointer += Display::Width(); //-V2563
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
    int dx = (int)std::fabsf((float)(x2 - x1)); //-V2533
    int dy = (int)std::fabsf((float)(y2 - y1)); //-V2533
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

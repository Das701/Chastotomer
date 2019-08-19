#include "defines.h"
#include "stm32f4xx_hal.h"
#include "Display.h"
#include "Hardware/HAL.h"
#include "Display/Primitives.h"
#include "Menu/Menu.h"
#include <cstring>


using namespace Display::Primitives;

/// Буфер с изображение
static uint8 buffer[256 * 64 / 8];
static uint8 buf[256][64];

void DrawPoint(int x, int y)
{
    if (x < 256 && x >= 0 && y < 64 && y  >= 0)
    {
        buf[x][y] = 0xFF; 
    }        
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void DrawHLine(int x, int y, int length)
{
    for(uint i = 0; i < length; i++)
    {
        DrawPoint(x, y);
        x++;        
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void DrawVLine(int x, int y, int length)
{
    for(uint i = 0; i < length; i++)
    {
        DrawPoint(x, y);
        y++;        
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void DrawRectangle(int x, int y, int height, int width)
{
    DrawHLine(x, y, width);
    DrawVLine(x, y, height);
    DrawHLine(x, y + height, width);
    DrawVLine(x + width, y, height);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void FillRectangle(int x, int y, int height, int width)
{
    for(uint y0 = y; y0 < y + height; y0++)
    {
        DrawHLine(x, y0, width);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Delay(uint ms)
{
    uint timestamp = HAL_GetTick();
    while(HAL_GetTick() - timestamp < ms)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Command(uint8 command)
{
    Delay(1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   //CS pin low
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); //set D/C# pin low
    HAL::SPI::Send(&command, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   //CS pin high
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Data(uint8 data)
{
    Delay(1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   //CS pin low
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);   //set D/C# pin high
    HAL::SPI::Send(&data, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   //CS pin high
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8* Display::GetBuff()
{
    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Init()
{
    
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);   //Reset pin low
    Delay(1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);     //Reset pin high
    Delay(1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   //CS pin high
    Command(0xFD);      //set Command unlock
    Data(0x12);
    Command(0xAE);      //set display off
    Command(0xB3);      //set display clock divide ratio...
    Data(0x91);         //...to 135 Frames/sec
    Command(0xCA);      //set multiplex ratio...
    Data(0x3F);         //...to 64?1
    Command(0xA2);      //set display offset
    Data(0x00);         //...to 0
    Command(0xA1);      //start display start line
    Data(0x00);         //...to 0
    Command(0xA0);      //set Re?Map & Dual COM Line Mode
    Data(0x14);
    Data(0x11);
    Command(0xB5);      //disable IO intput
    Data(0x00);
    Command(0xAB);      //function select
    Data(0x01);
    Command(0xB4);      //enable VSL extern
    Data(0xA0);
    Data(0xFD);
    Command(0xC1);      //set contrast current
    Data(0xFF);
    Command(0xC7);      //set master contrast current
    Data(0x0F);
    Command(0xB8);      //set gray scale table
    Data(0x00);
    Data(0x00);
    Data(0x00);
    Data(0x03);
    Data(0x06);
    Data(0x10);
    Data(0x1D);
    Data(0x2A);
    Data(0x37);
    Data(0x46);
    Data(0x58);
    Data(0x6A);
    Data(0x7F);
    Data(0x96);
    Data(0xB4);
    Command(0xB1);      //set phase length
    Data(0xE8);
    Command(0xD1);      //enhance driving scheme capability
    Data(0x82);
    Data(0x20);
    Command(0xBB);      //first pre charge voltage
    Data(0x1F);
    Command(0xB6);      //second pre charge voltage
    Data(0x08);
    Command(0xBE);      //VCOMH
    Data(0x07);
    Command(0xA9);      //no partial mode
    Command(0xA6);      //set normal display mode
    Delay(1);           //stabilize VDD
    Command(0xAF);      //display on
    Delay(200);         //stabilize VDD
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void InitWindow(uint8 startcol, uint8 stopcol, uint8 startrow,uint8 stoprow)
{

    Command(0x15);
    Data(28+startcol);
    Data(28+stopcol);

    Command(0x75);
    Data(startrow);
    Data(stoprow);

    Command(0x5C);
    Delay(50);
    
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Update()
{
    uint8 buffer1[128];
    //Преобразование в grayscale
    for(uint y = 0; y < 64; y++)
    {
        uint8 totcount = 0;
        for(uint x = 0; x < 256; x += 2) 
        {
            uint8 low = buf[x][y] & 0x0F;
            uint8 high = buf[x + 1][y] & 0xF0;
            buffer1[totcount] = low | high;
            totcount++;
        }
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   //CS pin low
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);   //set D/C# pin high
        HAL::SPI::Send(buffer1, 128);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   //CS pin high
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::BeginScene(Color color)
{
    Rectangle(320, 240).Fill(0, 0, color);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::EndScene()
{

}

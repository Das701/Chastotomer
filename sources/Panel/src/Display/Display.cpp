#include "defines.h"
#include "stm32f4xx_hal.h"
#include "Display.h"
#include "Hardware/HAL.h"
#include "Menu/Menu.h"
#include <cstring>


#define COLOR_BLACK 0x00
#define COLOR_WHITE 0xFF

/// Буфер с изображение
static uint8 buf[256][64];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawPoint(int x, int y, uint8 color = COLOR_WHITE)
{
    if (x < 256 && x >= 0 && y < 64 && y  >= 0)
    {     
        buf[x][y] = color;      
    }        
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawHLine(int x, int y, int length, uint8 color)
{
    for(int i = 0; i < length; i++)
    {
        DrawPoint(x, y, color);
        x++;        
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawVLine(int x, int y, int length, uint8 color)
{
    for(int i = 0; i < length; i++)
    {
        DrawPoint(x, y, color);
        y++;        
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawRectangle(int x, int y, int height, int width, uint8 color)
{
    DrawHLine(x, y, width, color);
    DrawVLine(x, y, height, color);
    DrawHLine(x, y + height, width, color);
    DrawVLine(x + width, y, height, color);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FillRectangle(int x, int y, int width, int height, uint8 color)
{
    for(int y0 = y; y0 < y + height; y0++)
    {
        DrawHLine(x, y0, width, color);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BeginScreen()
{
    FillRectangle(0, 0, 64, 256, COLOR_BLACK);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Delay(uint ms)
{
    uint timestamp = HAL_GetTick();
    while(HAL_GetTick() - timestamp < ms)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SendCommand(uint8 command)
{
    Delay(1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   //CS pin low
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); //set D/C# pin low
    HAL::SPI::Send(&command, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   //CS pin high
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SendData(uint8 data)
{
    Delay(1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   //CS pin low
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);   //set D/C# pin high
    HAL::SPI::Send(&data, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   //CS pin high
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Init()
{
    
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);   //Reset pin low
    Delay(100);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);     //Reset pin high
    Delay(100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   //CS pin high
    SendCommand(0xFD);      //set Command unlock
    SendData(0x12);
    SendCommand(0xAE);      //set display off
    SendCommand(0xB3);      //set display clock divide ratio...
    SendData(0x91);         //...to 135 Frames/sec
    SendCommand(0xCA);      //set multiplex ratio...
    SendData(0x3F);         //...to 64?1
    SendCommand(0xA2);      //set display offset
    SendData(0x00);         //...to 0
    SendCommand(0xA1);      //start display start line
    SendData(0x00);         //...to 0
    SendCommand(0xA0);      //set Re?Map & Dual COM Line Mode
    SendData(0x14);
    SendData(0x11);
    SendCommand(0xB5);      //disable IO intput
    SendData(0x00);
    SendCommand(0xAB);      //function select
    SendData(0x01);
    SendCommand(0xB4);      //enable VSL extern
    SendData(0xA0);
    SendData(0xFD);
    SendCommand(0xC1);      //set contrast current
    SendData(0xFF);
    SendCommand(0xC7);      //set master contrast current
    SendData(0x0F);
    SendCommand(0xB8);      //set gray scale table
    SendData(0x00);
    SendData(0x00);
    SendData(0x00);
    SendData(0x03);
    SendData(0x06);
    SendData(0x10);
    SendData(0x1D);
    SendData(0x2A);
    SendData(0x37);
    SendData(0x46);
    SendData(0x58);
    SendData(0x6A);
    SendData(0x7F);
    SendData(0x96);
    SendData(0xB4);
    SendCommand(0xB1);      //set phase length
    SendData(0xE8);
    SendCommand(0xD1);      //enhance driving scheme capability
    SendData(0x82);
    SendData(0x20);
    SendCommand(0xBB);      //first pre charge voltage
    SendData(0x1F);
    SendCommand(0xB6);      //second pre charge voltage
    SendData(0x08);
    SendCommand(0xBE);      //VCOMH
    SendData(0x07);
    SendCommand(0xA9);      //no partial mode
    SendCommand(0xA6);      //set normal display mode
    Delay(100);           //stabilize VDD
    SendCommand(0xAF);      //display on
    Delay(200);         //stabilize VDD

    SendCommand(0xA5);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EndScreen()
{
    SendCommand(0x15);      // Устанавливаем начальную и конечную колонки
    SendData(0x00);         // начальная
    SendData(0xFF);         // конечная

    SendCommand(0x75);      // Устанавливаем начальную и конечную строки
    SendData(0x00);         // начальная
    SendData(0xFF);         // Конечная

    SendCommand(0x5C);      // Записываем данные

    Delay(50);

    uint8 buffer1[128];
    //Преобразование в grayscale
    for(uint y = 0; y < 64; y++)
    {
        uint8 totcount = 0;
        for(uint x = 0; x < 256; x += 2) 
        {
            uint8 low = (uint8)(buf[x][y] & 0x0F);
            uint8 high = (uint8)(buf[x + 1][y] & 0xF0);
            buffer1[totcount] = (uint8)(low | high);
            totcount++;
        }
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   //CS pin low
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);   //set D/C# pin high
        HAL::SPI::Send(buffer1, 128);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   //CS pin high
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Update()
{
    BeginScreen();
    
    DrawRectangle(10, 5, 10, 15, COLOR_WHITE);
   
    EndScreen();
}

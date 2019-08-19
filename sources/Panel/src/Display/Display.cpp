#include "defines.h"
#include "stm32f4xx_hal.h"
#include "Display.h"
#include "Hardware/HAL.h"
#include "Menu/Menu.h"
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

#define COM_ENABLE_GRAY_SCALE_TABLE 0x00    /* Enable Gray Scale Table */
#define COM_COL_ADDRESS             0x15    /* Set Column Address */
#define COM_WRITE_RAM  	            0x5C    /* Write RAM Command */
#define COM_ROW_ADDRESS             0x75    /* Set Row Address */
#define COM_REMAP_AND_DUAL          0xA0    /* Set Re-map and Dual COM Line mode */
#define COM_DISPLAY_START_LINE      0xA1    /* Set Display Start Line */
#define COM_DISPLAY_OFFSET          0xA2    /* Set Display Offset */
#define COM_DISPLAY_MODE_GS15       0xA5    /* ƒисплей переводитс€ в режим, в котором все пиксели имеют максимальную €ркость GS15 */
#define COM_FUNC_SELECT             0xAB    /* Function Selection */    
                                            // A[0] = 0b, выбрать внешнее питание дл€ VDD (внутренний регул€тор запрещен).
                                            // A[0] = 1b, разрешить работу внутреннего регул€тора дл€ VDD(состо€ние по умолчанию после сброса).
#define COM_SLEEP_MODE_ON           0xAE    /* Set Sleep mode ON */
#define COM_SLEEP_MODE_OFF          0xAF    /* Set Sleep mode OFF */
#define COM_FRONT_CLOCK_DIV         0xB3    /* Set Front Clock Divider / Oscillator Frequency */
#define COM_DISPLAY_ENHANCEMENT_A   0xB4    /* Display Enhancement A */
                                            // A[1:0] = 00b: разрешить внешнее VSL
                                            // A[1:0] = 10b : внутреннее VSL(после сброса)
                                            // B[7:3] = 11111b : улучшает качество GS диспле€
                                            // B[7:3] = 10110b : обычный режим(после сброса)
#define COM_GPIO                    0xB5    /* Set GPIO */
#define COM_GRAY_SCALE_TABLE        0xB8    /* Set Gray Scale Table */
#define COM_MUX_RATIO               0xCA    /* Set MUX Ratio */
#define COM_LOCK                    0xFD    /* Set Command Lock */


/// Ѕуфер с изображение
static uint8 buf[256][64];

static void Delay(uint ms);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendCommand(uint8 command)
{
    Delay(1);
    CS_OPEN;
    Delay(1);
    SET_DC_COMMAND;
    Delay(1);
    HAL::SPI::Send(&command, 1);
    Delay(1);
    CS_CLOSE;
    Delay(1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SendData(uint8 data)
{
    Delay(1);
    CS_OPEN;
    Delay(1);
    SET_DC_DATA;
    Delay(1);
    HAL::SPI::Send(&data, 1);
    Delay(1);
    CS_CLOSE;
    Delay(1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SendData(uint8 *data, int num)
{
    for (int i = 0; i < num; i++)
    {
        SendData(data[i]);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SendCommand(uint8 command, uint8 data)
{
    SendCommand(command);
    SendData(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SendCommand(uint8 command, uint8 data0, uint8 data1)
{
    SendCommand(command);
    SendData(data0);
    SendData(data1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
void Display::Init()
{
    
    SET_RES_LOW;
    Delay(500);
    SET_RES_HI;
    Delay(500);
    CS_CLOSE;
    SendCommand(COM_LOCK, 0x12);                        // unlock
    SendCommand(COM_FRONT_CLOCK_DIV, 0x91);             // to 135 fps
    SendCommand(COM_MUX_RATIO, 0x3F);
    SendCommand(COM_DISPLAY_OFFSET, 0x00);
    SendCommand(COM_DISPLAY_START_LINE, 0x00);
    SendCommand(COM_REMAP_AND_DUAL, 0x14, 0x11);
    SendCommand(COM_GPIO, 0x00);                        //disable IO intput
    SendCommand(COM_FUNC_SELECT, 0x01);
    SendCommand(COM_DISPLAY_ENHANCEMENT_A, 0xA0, 0xFD); //enable VSL extern
    SendCommand(0xC1);      //set contrast current
    SendData(0xFF);
    SendCommand(0xC7);      //set master contrast current
    SendData(0x0F);
    SendCommand(COM_GRAY_SCALE_TABLE);
    SendData(0);
    SendData(0);
    SendData(0);
    SendData(0);
    SendData(0);
    SendData(0);
    SendData(0);
    SendData(0);
    SendData(180);
    SendData(180);
    SendData(180);
    SendData(180);
    SendData(180);
    SendData(180);
    SendData(180);
    SendData(180);
    SendCommand(COM_ENABLE_GRAY_SCALE_TABLE);
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
    Delay(200);           //stabilize VDD
    SendCommand(COM_SLEEP_MODE_OFF);
    Delay(200);         //stabilize VDD

    SendCommand(COM_DISPLAY_MODE_GS15);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EndScreen()
{
    SendCommand(COM_COL_ADDRESS, 0x00, 0xFF);

    SendCommand(COM_ROW_ADDRESS, 0x00, 0xFF);

    SendCommand(COM_WRITE_RAM);

    Delay(50);

    uint8 buffer1[128];

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

        SendData(buffer1, 128);
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Update()
{
    DrawRectangle(std::rand() % 256, std::rand() % 64, std::rand() % 256, std::rand() % 256, (uint8)((std::rand() % 2) ? COLOR_WHITE : COLOR_BLACK));
   
    EndScreen();
}

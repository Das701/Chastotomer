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

// http://microsin.net/adminstuff/hardware/ssd1322-oled-controller.html

#define COM_ENABLE_GRAY_SCALE_TABLE 0x00    /* Enable Gray Scale Table */
#define COM_COL_ADDRESS             0x15    /* Set Column Address */
#define COM_WRITE_RAM  	            0x5C    /* Write RAM Command */
#define COM_ROW_ADDRESS             0x75    /* Set Row Address */
#define COM_REMAP_AND_DUAL          0xA0    /* Set Re-map and Dual COM Line mode */
#define COM_DISPLAY_START_LINE      0xA1    /* Set Display Start Line */
#define COM_DISPLAY_OFFSET          0xA2    /* Set Display Offset */
#define COM_DISPLAY_MODE_GS0        0xA4    /* Все пикселы погашены */
#define COM_DISPLAY_MODE_GS15       0xA5    /* Дисплей переводится в режим, в котором все пиксели имеют максимальную яркость GS15 */
#define COM_DISPLAY_MODE_NORMAL     0xA6    /* Нормальная работа (состояние после сброса). */
#define COM_DISPLAY_MODE_INVERSE    0xA7    /* Инверсия дисплея */
#define COM_EXIT_PARTIAL_DISPLAY    0xA9    /* Exit Partial Display */
#define COM_FUNC_SELECT_VDD         0xAB    /* Function Selection */    
                                            // A[0] = 0b, выбрать внешнее питание для VDD (внутренний регулятор запрещен).
                                            // A[0] = 1b, разрешить работу внутреннего регулятора для VDD(состояние по умолчанию после сброса).
//#define COM_SLEEP_MODE_ON           0xAE    /* Set Sleep mode ON */
#define COM_SLEEP_MODE_OFF          0xAF    /* Set Sleep mode OFF */
#define COM_PHASE_LENGTH            0xB1    /* Set Phase Length */
#define COM_FRONT_CLOCK_DIV         0xB3    /* Set Front Clock Divider / Oscillator Frequency */
#define COM_DISPLAY_ENHANCEMENT_A   0xB4    /* Display Enhancement A */
                                            // A[1:0] = 00b: разрешить внешнее VSL
                                            // A[1:0] = 10b : внутреннее VSL(после сброса)
                                            // B[7:3] = 11111b : улучшает качество GS дисплея
                                            // B[7:3] = 10110b : обычный режим(после сброса)
#define COM_GPIO                    0xB5    /* Set GPIO */
#define COM_SECOND_PRECHARGE_T      0xB6    /* Set Second Precharge Period */
                                            // A[3:0] устанавливает длительность второго периода предзаряда. 
#define COM_GRAY_SCALE_TABLE        0xB8    /* Set Gray Scale Table */
#define COM_DEFAULT_GRAY_SCALE_TABLE 0xB9   /* Select Default Linear Gray Scale table */
#define COM_PRECHARGE_VOLTAGE       0xBB    /* Set Pre-charge voltage */
#define COM_VCOMH                   0xBE    /* Set VCOMH */
#define COM_CONTRAST                0xC1    /* Set Contrast Current */
#define COM_MASTER_CONTRAST         0xC7    /* Master Contrast Current Control */
#define COM_MUX_RATIO               0xCA    /* Set MUX Ratio */
#define COM_DISPLAY_ENHANCEMENT_B   0xD1    /* Display Enhancement B */
                                            // A[5:4] = 00b: зарезервированное состояние
                                            // A[5:4] = 10b : нормальное состояние(после сброса).
#define COM_LOCK                    0xFD    /* Set Command Lock */
                                            // A[2] определяет защиту интерфейса от доступа со стороны микроконтроллера(значение после сброса 12h).


using namespace Display::Primitives;

static void Delay(uint ms);
static void SendData(uint8 data);
static void SendData(uint8 *data, uint16 num);
static void SendCommand(uint8 command);
static void SendCommand(uint8 command, uint8 data);
static void SendCommand(uint8 command, uint8 data0, uint8 data1);

/// В этом буфере будем рисовать. Ширина равна 256 / 2 потому, что в байте хранятся 2 пикселя с 1 из 16-ти возможных градация каждая.
#define WIDTH_BUFFER    (256 / 2)
#define HEIGHT_BUFFER   (64)
static uint8 front[HEIGHT_BUFFER][WIDTH_BUFFER];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Init()
{
    CS_CLOSE;

    Delay(500);
    SET_RES_HI;
    Delay(500);
    SET_RES_LOW;
    Delay(500);
    SET_RES_HI;
    Delay(500);
    SendCommand(COM_LOCK, 0x12);                        // unlock

    SendCommand(COM_FRONT_CLOCK_DIV, 0x91);             // to 135 fps
    SendCommand(COM_MUX_RATIO, 0x3F);                   // set multiplex ratio
    SendCommand(COM_REMAP_AND_DUAL, 0x14, 0x11);
    SendCommand(COM_DISPLAY_OFFSET, 0x00);              // set display offset
    SendCommand(COM_DISPLAY_START_LINE, 0x00);          // set display start line
    SendCommand(COM_GPIO, 0x00);                        //disable IO intput
    SendCommand(COM_FUNC_SELECT_VDD, 0x01);
    SendCommand(COM_DEFAULT_GRAY_SCALE_TABLE);
    SendCommand(COM_ENABLE_GRAY_SCALE_TABLE);
    SendCommand(COM_PRECHARGE_VOLTAGE, 0x1F);
    SendCommand(COM_CONTRAST, 0xFF);

    Delay(200);                 //stabilize VDD
    SendCommand(COM_SLEEP_MODE_OFF);
    Delay(200);                 //stabilize VDD

    SendCommand(COM_DISPLAY_MODE_GS15);

    SendCommand(COM_VCOMH, 0x00);
    SendCommand(COM_VCOMH, 0x01);
    SendCommand(COM_VCOMH, 0x02);
    SendCommand(COM_VCOMH, 0x03);
    SendCommand(COM_VCOMH, 0x04);
    SendCommand(COM_VCOMH, 0x05);
    SendCommand(COM_VCOMH, 0x06);
    SendCommand(COM_VCOMH, 0x07);

    SendCommand(COM_DISPLAY_MODE_GS0);
    SendCommand(COM_DISPLAY_MODE_GS15);
    SendCommand(COM_DISPLAY_MODE_GS0);
    SendCommand(COM_DISPLAY_MODE_GS15);
    SendCommand(COM_DISPLAY_MODE_GS0);
    SendCommand(COM_DISPLAY_MODE_GS15);
    SendCommand(COM_DISPLAY_MODE_GS0);
    SendCommand(COM_DISPLAY_MODE_NORMAL);
    SendCommand(COM_DISPLAY_MODE_INVERSE);
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::EndScene()
{
    /*
        1. 0x15. Устанавливает адреса начального и конечного столбца. A[6:0] задает начальный адрес (после сброса =0), B[6:0] конечный адрес (после сброса =119). Адрес может быть установлен
                 в диапазоне от 0 до 119.
        2. 0x75. Устанавливает адреса начальной и конечной строки. A[6:0] задает начальный адрес (после сброса =0), B[6:0] конечный адрес (после сброса =127). Адрес может быть установлен в
                 диапазоне от 0 до 127.
        3. 0x5C. Разрешает микроконтроллеру записать данные в RAM.
    */

    for (int row = 0; row < HEIGHT_BUFFER; row++)
    {
        for (int col = 0; col < WIDTH_BUFFER; col++)
        {
            front[row][col] = (uint8)std::rand();
        }
    }

    SendCommand(COM_COL_ADDRESS, 0, 119);
    SendCommand(COM_ROW_ADDRESS, 0, 63);
    SendCommand(COM_WRITE_RAM);

    Delay(50);

    for (int row = 0; row < HEIGHT_BUFFER; row++)
    {
        SendData(&front[row][0], 120);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Delay(uint ms)
{
    uint timestamp = HAL_GetTick();
    while (HAL_GetTick() - timestamp < ms)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendCommand(uint8 command)
{
    CS_OPEN;
    SET_DC_COMMAND;
    HAL::SPI1_::Send(&command, 1);
    CS_CLOSE;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendData(uint8 data)
{
    CS_OPEN;
    SET_DC_DATA;
    HAL::SPI1_::Send(&data, 1);
    CS_CLOSE;
}

////-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendData(uint8 *data, uint16 num)
{
    CS_OPEN;
    SET_DC_DATA;
    HAL::SPI1_::Send(data, num);
    CS_CLOSE;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendCommand(uint8 command, uint8 data)
{
    SendCommand(command);
    SendData(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendCommand(uint8 command, uint8 data0, uint8 data1)
{
    SendCommand(command);
    SendData(data0);
    SendData(data1);
}

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

#define COM_ENABLE_GRAY_SCALE_TABLE     0x00    /// Enable Gray Scale Table
#define COM_COL_ADDRESS                 0x15    /// Set Column Address
#define COM_WRITE_RAM  	                0x5C    /// Write RAM Command
#define COM_ROW_ADDRESS                 0x75    /// Set Row Address
#define COM_REMAP_AND_DUAL              0xA0    /// Set Re-map and Dual COM Line mode
#define COM_DISPLAY_START_LINE          0xA1    /// Set Display Start Line
#define COM_DISPLAY_OFFSET              0xA2    /// Set Display Offset
#define COM_DISPLAY_MODE_GS0            0xA4    /// Все пикселы погашены
#define COM_DISPLAY_MODE_GS15           0xA5    /// Дисплей переводится в режим, в котором все пиксели имеют максимальную яркость GS15
#define COM_DISPLAY_MODE_NORMAL         0xA6    /// Нормальная работа (состояние после сброса).
#define COM_DISPLAY_MODE_INVERSE        0xA7    /// Инверсия дисплея
#define COM_EXIT_PARTIAL_DISPLAY        0xA9    /// Exit Partial Display
#define COM_FUNC_SELECT_VDD             0xAB    /// Function Selection
                                                /// A[0] = 0b, выбрать внешнее питание для VDD (внутренний регулятор запрещен).
                                                /// A[0] = 1b, разрешить работу внутреннего регулятора для VDD(состояние по умолчанию после сброса).
#define COM_SLEEP_MODE_ON               0xAE    /// Set Sleep mode ON
#define COM_SLEEP_MODE_OFF              0xAF    /// Set Sleep mode OFF
#define COM_PHASE_LENGTH                0xB1    /// Set Phase Length
#define COM_FRONT_CLOCK_DIV             0xB3    /// Set Front Clock Divider / Oscillator Frequency
#define COM_DISPLAY_ENHANCEMENT_A       0xB4    /// Display Enhancement A
                                                /// A[1:0] = 00b: разрешить внешнее VSL
                                                /// A[1:0] = 10b : внутреннее VSL(после сброса)
                                                /// B[7:3] = 11111b : улучшает качество GS дисплея
                                                /// B[7:3] = 10110b : обычный режим(после сброса)
#define COM_GPIO                        0xB5    /// Set GPIO
#define COM_SECOND_PRECHARGE_T          0xB6    /// Set Second Precharge Period
                                                /// A[3:0] устанавливает длительность второго периода предзаряда. 
#define COM_GRAY_SCALE_TABLE            0xB8    /// Set Gray Scale Table
#define COM_DEFAULT_GRAY_SCALE_TABLE    0xB9    /// Select Default Linear Gray Scale table
#define COM_PRECHARGE_VOLTAGE           0xBB    /// Set Pre-charge voltage
#define COM_VCOMH                       0xBE    /// Set VCOMH
#define COM_CONTRAST                    0xC1    /// Set Contrast Current
#define COM_MASTER_CONTRAST             0xC7    /// Master Contrast Current Control
#define COM_MUX_RATIO                   0xCA    /// Set MUX Ratio
#define COM_DISPLAY_ENHANCEMENT_B       0xD1    /// Display Enhancement B
                                                /// A[5:4] = 00b: зарезервированное состояние
                                                /// A[5:4] = 10b : нормальное состояние(после сброса).
#define COM_LOCK                        0xFD    /// Set Command Lock
                                                /// A[2] определяет защиту интерфейса от доступа со стороны микроконтроллера(значение после сброса 12h).


using namespace Display::Primitives;

static void ClearScreen();

static void Delay(uint ms);
static void SendData(uint8 data);
static void SendCommand(uint8 command);
static void SendCommand(uint8 command, uint8 data);
static void SendCommand(uint8 command, uint8 data0, uint8 data1);
static void SendCommand(uint8 command, uint8 *data, uint16 num);

/// В этом буфере будем рисовать. Ширина равна 256 / 2 потому, что в байте хранятся 2 пикселя с 1 из 16-ти возможных градация каждая.
#define WIDTH_BUFFER    (256)
#define HEIGHT_BUFFER   (64)
static uint8 front[HEIGHT_BUFFER][WIDTH_BUFFER];

/// Данные одной строки
static uint8 line[128];


void InverseMode()
{
    static bool isInverse = false;

    SendCommand(isInverse ? (uint8)(COM_DISPLAY_MODE_NORMAL) : (uint8)(COM_DISPLAY_MODE_INVERSE));
    
    isInverse = !isInverse;
}

void ChangeFrequency()
{
    static uint freq = 255;
    
    
    SendCommand(COM_FRONT_CLOCK_DIV, (uint8)freq);
    
    freq--;
    
    if(freq == 0)
    {
        freq = 0;
    }
}

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

    SendCommand(COM_LOCK, 0x12);        // Разблокировка команд

    Delay(500);                         //stabilize VDD
    SendCommand(COM_SLEEP_MODE_OFF);
    Delay(500);                         //stabilize VDD

    SendCommand(COM_FRONT_CLOCK_DIV, 0x90 | 0x01);  // 0x90 - Частота генератора, 0x01 - коэффициент деления частоты генератора

    SendCommand(COM_MUX_RATIO, 0x40);               // Переключения режима мультиплексирования

    SendCommand(COM_DISPLAY_MODE_NORMAL);           // Зажигаем все точки

    SendCommand(COM_DISPLAY_OFFSET, 0x00);          // Устновка смещения дисплея

    SendCommand(COM_DISPLAY_START_LINE, 0x00);      // Установка начальной строки дисплея

    SendCommand(COM_REMAP_AND_DUAL, 0x14, 0x11);    // 

    SendCommand(COM_GPIO, 0x00);

    SendCommand(COM_FUNC_SELECT_VDD, 0x01);

    SendCommand(COM_DISPLAY_ENHANCEMENT_A, 0xA2, 0xFD);

    SendCommand(COM_CONTRAST, 0xFF);

    SendCommand(COM_MASTER_CONTRAST, 0x0F);

    uint8 data[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xED, 0xED };

    SendCommand(COM_GRAY_SCALE_TABLE, data, 15);

    SendCommand(COM_ENABLE_GRAY_SCALE_TABLE);

    SendCommand(COM_PHASE_LENGTH, 0xFF);                        // От этой величины зависит отражение изображения на предыдущую строку

    SendCommand(COM_DISPLAY_ENHANCEMENT_B, 0x82, 0x20);

    SendCommand(COM_PRECHARGE_VOLTAGE, 0x1F);

    SendCommand(COM_SECOND_PRECHARGE_T, 0x0F);

    SendCommand(COM_VCOMH, 0x00);
    
    SendCommand(COM_COL_ADDRESS, 28, 91);

    ClearScreen();
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
static void SendData(uint8 *data, uint16 num)
{
    CS_OPEN;
    SET_DC_DATA;
    HAL::SPI1_::Send(data, num);
    CS_CLOSE;
}

////-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendDataScreen(uint8 row, uint8 *data, uint16 num)
{
    SendCommand(COM_ROW_ADDRESS, row, row);     // Устанавливает адреса начальной и конечной строки. Первое число задает начальный адрес, второе - конечный адрес.
    SendCommand(COM_WRITE_RAM);                 // Разрешаем микроконтроллеру записать данные в RAM.
    SendData(data, num);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::EndScene()
{
    for (int row = 1; row < HEIGHT_BUFFER; row++)
    {
        for (int i = 0; i < WIDTH_BUFFER; i += 2)
        {
            line[127 - i / 2] = (uint8)(front[row][i] | (front[row][i + 1] << 4));
        }

        SendDataScreen((uint8)(HEIGHT_BUFFER - row), line, 128);
    }

    for (int i = 0; i < WIDTH_BUFFER; i += 2)
    {
        line[127 - i / 2] = (uint8)(front[0][i] | (front[0][i + 1] << 4));
    }

    SendDataScreen(0, line, 128);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ClearScreen()
{
    std::memset(line, 0, 128);

    for (int row = 0; row < HEIGHT_BUFFER; row++)
    {
        SendDataScreen((uint8)row, line, 128);
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendCommand(uint8 command, uint8 *data, uint16 num)
{
    SendCommand(command);
    SendData(data, num);
}

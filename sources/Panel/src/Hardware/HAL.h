#pragma once
#include "defines.h"


void ERROR_HANDLER();

struct HAL
{
    static void Init();
    
    /// Используется для записи в дисплей
    struct SPI1_
    {
        //static void Init();
        //static void Send(uint8 *data, uint16 num);
    };
};


struct HAL_FSMC
{
    static void Init();
    static void Reset();
    static void WriteCommand(uint16 command);
    static void WriteData(uint16 data);
    static uint16 ReadData();

    static uint16 GetData(uint16 address);

public:
    struct Test
    {
        static void Init();
        static void Run();
    };
};

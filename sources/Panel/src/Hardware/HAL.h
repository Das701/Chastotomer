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
    static void WriteCommand(uint8 command);
    static void WriteData(uint8 data);
private:
    static void SetDataBus(uint8 data);
    static void SetDataBus(uint16 data);
};

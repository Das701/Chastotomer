#pragma once
#include "defines.h"


void ERROR_HANDLER();

struct HAL
{
    static void Init();
    
    /// Используется для записи в дисплей
    struct SPI1_
    {
        static void Init();
        static void Send(uint8 *data, uint16 num);
    };
};

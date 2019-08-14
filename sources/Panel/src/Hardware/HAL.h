#pragma once
#include "defines.h"

void ERROR_HANDLER();


struct HAL
{
    static void Init();

    struct SPI
    {
        static void Init();
        static void Send(uint8 *data, uint16 num);
    };
};

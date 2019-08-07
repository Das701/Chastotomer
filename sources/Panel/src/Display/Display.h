#pragma once
#include "stm32f4xx_hal.h"
#include "Display/Colors.h"


namespace Display
{
    void Init();

    void Update();

    uint8_t* GetBuff();
};

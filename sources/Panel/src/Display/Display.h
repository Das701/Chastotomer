#pragma once
#include "stm32f4xx_hal.h"
#include "Display/Colors.h"


namespace Display
{
    void Init();
    /// Инициализация чёрно-белого режима
    void InitBlack();

    void Update();
};

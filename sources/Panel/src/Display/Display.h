#pragma once
#include "stm32f4xx_hal.h"
#include "Display/Colors.h"


namespace Display
{
    void Init();

    void Init2();
    /// ������������� �����-������ ������
    void InitBlack();

    void Update();
};

#pragma once
#include "stm32f4xx_hal.h"
#include "Display/Colors.h"


namespace Display
{
    void Init();

    void Update();

    uint8 *GetPixel(int x, int y);

    inline int GetWidth() { return 320; };

    inline int GetHeight() { return 240; };
    
    void BeginScene(Color color);
    
    void EndScene();
};

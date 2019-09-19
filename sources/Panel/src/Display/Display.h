#pragma once
#include "stm32f4xx_hal.h"
#include "Display/Colors.h"


namespace Display
{
	static const int WIDTH = 320;
	static const int HEIGHT = 240;

    void Init();

    void Update();

    uint8 *GetPixel(int x, int y);
   
    void BeginScene(Color color);
    
    void EndScene(); 
};

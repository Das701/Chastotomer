#pragma once
#include "stm32f4xx_hal.h"
#include "Display/Colors.h"


namespace Display
{
	static const int WIDTH = 480;
	static const int HEIGHT = 272;

    void Init();

    void Update();

    uint8 *GetPixel(int x, int y);
   
    void BeginScene();
    void StartScreen();
    
    void EndScene(); 
};

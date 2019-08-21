#include "stm32f4xx_hal.h"
#include "Hardware/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Display/Display.h"
#include "Menu/Menu.h"
#include "Device.h"
#include "Data/Heap.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    HAL::Init();
    Display::Init();
    Keyboard::Init();
    Menu::Init();
    Heap::Init();

    uint time = 0;
    
    while (1)
    {
        uint time = 0;
        uint start = HAL_GetTick();
        Device::Update();
        time = HAL_GetTick() - start;
        time = time;
    }
}


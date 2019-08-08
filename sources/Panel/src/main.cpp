#include "stm32f4xx_hal.h"
#include "Hardware/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Display/Display.h"
#include "Menu/Menu.h"
#include "Device.h"
#include "Data/Heap.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main1(void)
{
    HAL::Init();
    Display::Init();
    Keyboard::Init();
    Menu::Init();
    Heap::Init();
    
    while (1)
    {
        Device::Update();
    }
}


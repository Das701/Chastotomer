#include "stm32f4xx_hal.h"
#include "Hardware/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Display/Display.h"
#include "Menu/Menu.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    HAL::Init();
    Display::Init();
    Keyboard::Init();
    Menu::Init();   
        
    while (1)
    {
        PLIS::Update();
        Menu::Update();
        if(Keyboard::Used())
        {
            Display::Update(); 
            Keyboard::ResetUsed();
        }
    }
}


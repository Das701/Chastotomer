#include "stm32f4xx_hal.h"
#include "Hardware/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Display/Display.h"
#include "Menu/Menu.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    HAL::Init();
    Display::InitBlack();
    //Keyboard::Init();
    //Menu::Init();
    
    while (1)
    {
        Display::Update();
        
        Display::Update();
    }
}


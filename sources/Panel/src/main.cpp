#include "stm32f4xx_hal.h"
#include "Hardware/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Display/Display.h"
#include "Menu/Menu.h"
#include "Display/Text.h"
using Display::Text;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    HAL::Init();
    Display::Init();
    Keyboard::Init();
    Menu::Init();   
    Menu::Update();
    Display::Update();
    int time = HAL_GetTick();
    
    while (1)
    {
        
        PLIS::Update();
        Menu::Update();
//        Text(PLIS::GiveData()).Write(120, 120);
        if(HAL_GetTick() > time + 10)
        {
            Display::Update(); 
        }     
    }
}


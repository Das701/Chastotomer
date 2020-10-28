#include "stm32f4xx_hal.h"
#include "Hardware/HAL.h"
#include "Hardware/VCP.h"
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
    VCP::Init();
    int time = HAL_GetTick();
    while((time + 3000) > HAL_GetTick())
    {
        Display::StartScreen();
    }
    while (1)
    {
        PLIS::Update();
        Menu::Update();
        if(HAL_GetTick() > time + 10)
        {
            Display::Update(); 
        }     
    }
}


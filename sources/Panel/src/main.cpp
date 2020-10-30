#include "Display/Display.h"
#include "Display/Text.h"
#include "Hardware/HAL.h"
#include "Hardware/PLIS.h"
#include "Hardware/VCP.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "SCPI/SCPI.h"
#include <stm32f4xx_hal.h>


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
    uint time = HAL_GetTick();
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
        SCPI::Update();
    }
}


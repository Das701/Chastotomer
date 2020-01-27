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
    //Keyboard::Init();
    //Menu::Init();

    //HAL_FSMC::Test::Init();
    //HAL_FSMC::Test::Run();

    while (1)
    {
        //Menu::Update();
        Display::Update();
    }
}


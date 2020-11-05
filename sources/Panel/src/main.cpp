#include "Display/Display.h"
#include "Display/Text.h"
#include "Hardware/FPGA.h"
#include "Hardware/VCP.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "SCPI/SCPI.h"


using Display::Text;


int main(void)
{
    HAL::Init();
    FPGA::Init();
    Display::Init();
    Keyboard::Init();
    Menu::Init();   
    Menu::Update();
    Display::Update();
    VCP::Init();
    uint time = TIME_MS;

    //while((time + 3000) > TIME_MS)
    //{
    //    Display::DrawWelcomeScreen();
    //}

    while (1)
    {
        FPGA::Update();
        Menu::Update();
        Display::Update(); 
        SCPI::Update();
    }
}


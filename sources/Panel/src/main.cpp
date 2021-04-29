#include "Log.h"
#include "Display/Display.h"
#include "Hardware/FPGA.h"
#include "Hardware/VCP.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "SCPI/SCPI.h"
#include "Tests.h"


int main(void)
{
    HAL::Init();
    FPGA::Init();
    Display::Init();
    Keyboard::Init();
    VCP::Init();
    Menu::Init();
//    Tests::Run();

    Display::DrawWelcomeScreen();

    while (1)
    {
        FPGA::Update();
        Menu::Update();
        SCPI::Update();
        Display::Update();
    }
}

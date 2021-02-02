#include "Log.h"
#include "Display/Display.h"
#include "Hardware/FPGA.h"
#include "Hardware/VCP.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "SCPI/SCPI.h"
#include "Tests.h"
#include "Utils/Debug.h"


int main(void)
{
    HAL::Init();
    FPGA::Init();
    Display::Init();
    Keyboard::Init();
    VCP::Init();
    Menu::Init();
//    Tests::Run();

    //Display::DrawWelcomeScreen();

    while (1)
    {
        DEBUG_POINT_0;
        FPGA::Update();
        DEBUG_POINT_0;
        Menu::Update();
        DEBUG_POINT_0;
        SCPI::Update();
        DEBUG_POINT_0;
        Display::Update();
        DEBUG_POINT_0;
    }
}

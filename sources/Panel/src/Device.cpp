#include "Device.h"
#include "Keyboard/Keyboard.h"
#include "Display/Colors.h"
#include "stm32f4xx_hal.h"
#include "Menu/Menu.h"

#include "Display/Primitives.h"
#include "Display/Display.h"


using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Device::Update()
{
    Display::Update();
    Menu::Update();
}

#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Display.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"


void init()
{
	Display::Init();
	Keyboard::Init();
}


void update()
{
}

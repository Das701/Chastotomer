#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Display.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"


void init()
{
	HAL::Init();
	Display::Init();
	Keyboard::Init();
	Menu::Init();
}


void update()
{
	Display::Refresh();
	Menu::Update();
	Display::Update();
}

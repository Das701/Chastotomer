#include "defines.h"
#include "Hardware/MathFPGA.h"
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
	MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::Comparator, 49999999, 4839, 1661, 4757, 1661);
	Display::Refresh();
	Menu::Update();
	Display::Update();
}

#include "defines.h"
#include "Calculate/MathFPGA.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Tests.h"


void init()
{
	HAL::Init();
	Display::Init();
	Keyboard::Init();
	Menu::Init();
	Tests::Run();
}


void update()
{
	MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::MainCounters, 248024680, 53234, 3334, 160, 34);

	Display::Refresh();
	Menu::Update();
	Display::Update();
}

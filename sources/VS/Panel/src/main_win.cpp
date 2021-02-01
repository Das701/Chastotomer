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
//	uint value = 995150078;
//
//	switch (Channel::Current()->mod.timeMeasure.value)
//	{
//	case TimeMeasure::_1ms:
//		value /= 100000;
//		break;
//
//	case TimeMeasure::_10ms:
//		value /= 10000;
//		break;
//
//	case TimeMeasure::_100ms:
//		value /= 1000;
//		break;
//
//	case TimeMeasure::_1s:
//		value /= 100;
//		break;
//
//	case TimeMeasure::_10s:
//		value /= 10;
//		break;
//	}
//
//	MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::MainCounters, value, 4839, 1661, 4757, 1661);

	MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::Comparator, 10000, 5238, 278, 32388, 3252);

	Display::Refresh();
	Menu::Update();
	Display::Update();
}

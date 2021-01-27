#include "defines.h"
#include "Hardware/MathFPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Display.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Channels/Channels.h"


void init()
{
	HAL::Init();
	Display::Init();
	Keyboard::Init();
	Menu::Init();
}


void update()
{
	uint value = 995150078;

	switch (Channel::Current()->mod.timeMeasure.value)
	{
	case TimeMeasure::_1ms:
		value /= 100000;
		break;

	case TimeMeasure::_10ms:
		value /= 10000;
		break;

	case TimeMeasure::_100ms:
		value /= 1000;
		break;

	case TimeMeasure::_1s:
		value /= 100;
		break;

	case TimeMeasure::_10s:
		value /= 10;
		break;
	}

	MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::FillFactorPhase, value, 4839, 1661, 4757, 1661);
	Display::Refresh();
	Menu::Update();
	Display::Update();
}

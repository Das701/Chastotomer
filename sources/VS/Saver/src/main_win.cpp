#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Display.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "GUI/ComPort.h"
#include <ctime>


void init()
{
	Display::Init();
	Keyboard::Init();
	ComPort::Open();
}


void update()
{
	static uint time = 0;

#define SIZE_FRAME (272 * 480)

	static uint displayFrame[SIZE_FRAME];

	if (clock() > time + 1000)
	{
		ComPort::Send(":PICTURE");

		ComPort::Receive((char *)displayFrame, SIZE_FRAME);

		time = clock();
	}
}

#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Display.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "GUI/ComPort.h"
#include <ctime>
#include <cstring>


void init()
{
	Display::Init();
	Keyboard::Init();
	ComPort::Open();
}


void update()
{
	static clock_t time = 0;

#define SIZE_FRAME (272 * 480)

	static uint displayFrame[SIZE_FRAME];

	uint *pointer = displayFrame;
	pointer = pointer;

	if (clock() > time + 1000)
	{
        while (ComPort::Receive((char *)displayFrame, 100, 0) != 0)
        {

        }

		std::memset(displayFrame, 0, SIZE_FRAME * 4);

		ComPort::Send(":picture\x0d");

     	ComPort::Receive((char *)displayFrame, SIZE_FRAME * 4, 10000);

		Display::Draw(displayFrame);

		time = clock();
	}
}

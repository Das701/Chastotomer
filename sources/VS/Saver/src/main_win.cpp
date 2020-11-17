#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Display.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "GUI/ComPort.h"
#include "Application_win.h"
#include <ctime>
#include <cstring>


void init()
{
	Display::Init();
	Keyboard::Init();
	for (int i = 0; i < 10; i++)
	{
		ComPort::Open();
		if (ComPort::IsOpened())
		{
			break;
		}
	}
}


void update()
{
#define SIZE_FRAME (272 * 480)

	static uint displayFrame[SIZE_FRAME];

	uint *pointer = displayFrame;
	pointer = pointer;

	if (Frame::Self()->needSave)
	{
        while (ComPort::Receive((char *)displayFrame, 100, 0) != 0)
        {

        }

		std::memset(displayFrame, 0, SIZE_FRAME * 4);

		ComPort::Send(":picture\x0d");

     	ComPort::Receive((char *)displayFrame, SIZE_FRAME * 4, 10000);

		Display::Draw(displayFrame);

		Frame::Self()->needSave = false;
	}
}

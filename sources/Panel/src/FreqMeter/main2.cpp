#include "defines.h"
#include "Hardware/HAL.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Device.h"
#include "Keyboard/Keyboard.h"
#include "Display/Display.h"
#include "Menu/Menu.h"
#include "Device.h"



using namespace Display::Primitives;


extern "C"
{
void MeasureScreen();
void ChannelPrest();
void CalibrateMode();


char ModeScreen = 1;
char ModeScreenPre;
struct Flags_ Flags;
}


void main(void)
{
    HAL::Init();
    Display::Init();
    Display::BeginScene(Color::BLACK);
        //Keyboard::Init();
    //Menu::Init();
//    Heap::Init();

	Flags.RefrCreen = 1;
	Flags.Auto = 0;

    //Text_To_Display(255,0," OAO MNIPI,Ч3-XX,Cher V 1.1");	

	//Command_To_Display(CLEAR_DYSP);
	//	C0=20; C1=20; C2=20; C3=20; C4=20; C5=20; C6=20; C7=20; C8=20, C9=20;		//Заставим число на индикаторе целиком обновиться

	Flags.Test = 0;

	while(1)
	{

//        Device::Update();
            
        if(ModeScreen != ModeScreenPre)		//Если переменная изменилась, то устанавливаем флаг обновления экрана
		{
            Flags.RefrCreen = 1;
			ModeScreenPre = ModeScreen;
		}

//		if(ErrReg) MesErr (ErrReg);

		switch(ModeScreen)
		{
			case MEASURE_SCREEN:
			MeasureScreen();
			break;

			case CHANNEL_PREST:
			ChannelPrest();
			break;

			case CALIBRATE_MODE:
			CalibrateMode();
			break;

			default:
			break; 
		}

        
        Display::EndScene();
	}
}

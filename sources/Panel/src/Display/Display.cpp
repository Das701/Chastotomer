#include "Display.h"
#include "Primitives.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageModes.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageIndication.h"
#include "Text.h"
#include "Menu/MenuItems.h"
#include <cstdlib>


using namespace Display::Primitives;
using Display::Text;

/// ���������� ��� ���������
static void DrawTypeMeasure();
/// ���������� ����� ���������
static void DrawModeMeasure();
/// ���������� ���������
static void DrawHint();
/// ���������� ������-���
static void DrawStatusBar();
/// ���������� ������ �������� �������� ������
static void DrawChannelSettings();

static void DrawScreen();


static void CalculateCoord(int &x, int &y, int sizeX, int sizeY)
{
    static int deltaX = 1;
    static int deltaY = 0;

    x += deltaX;
    y += deltaY;

    if (x < 0)
    {
        x = 0;
        deltaX = -deltaX;
    }
    if (y < 0)
    {
        y = 0;
        deltaY = 1;
    }
    if (x + sizeX > 250)
    {
        deltaX = -deltaX;
    }
    if (y + sizeY > 60)
    {
        deltaY = -1;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//static void Delay(uint ms)
//{
//    uint timestamp = HAL_GetTick();
//    while (HAL_GetTick() - timestamp < ms)
//    {
//    }
//}

extern void InverseMode();

void Func1()
{
    for (int row = 0; row < 64; row++)
    {
        if ((row % 3) == 0)
        {
            HLine(row * 4).Draw(0, row, Color::WHITE);
        }
    }
}

void Func2()
{
    DrawScreen();
}

void Func3()
{
    static int x = 0;
    static int y = 0;

    int sizeX = 8;
    int sizeY = 3;

    static Rectangle rect(sizeX, sizeY);

    CalculateCoord(x, y, sizeX, sizeY);

    rect.Fill(x, y, Color::WHITE);

    rect.Fill(x, y + 5, Color::WHITE);
}

void Func4()
{
    HLine line(205);

    for (int y = 15; y < 50; y += 5)
    {
        line.Draw(50, y, Color::WHITE);
        line.Draw(50, y + 1, Color::WHITE);
        line.Draw(50, y + 2, Color::WHITE);
        line.Draw(50, y + 3, Color::WHITE);
        line.Draw(50, y + 4, Color::WHITE);
    }
}

void Func5()
{
    for(int y = 15; y < 50; y++)
    {
        HLine((y - 15) * 6).Draw(50, y, Color::WHITE);
    }
}


void(*func)() = Func4;


//void Display::Update()
//{
//    BeginScene(Color::BLACK);
//    
//    static bool forward = true;
//    static float delta = 0.1F;
//    for (int y = 15; y < 50; y++)
//    {
//        int length = (y - 15) * delta;
//        if (length > 255)
//        {
//            length = 255;
//        }
//        HLine(length).Draw(0, y, Color::WHITE);
//    }
//    
//    delta += 0.1F;
//    
//    if(forward)
//    {
//        delta += 0.1F;
//        if(delta > 100.
//    }
//    else
//    {
//    }
//    
//    EndScene();
//}

extern void ChangeFrequency();

void ChangeFreq()
{
    static uint timeStamp = 0;
    
    if(HAL_GetTick() - timeStamp > 2000)
    {
        timeStamp = HAL_GetTick();
        ChangeFrequency();
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Update()
{
    BeginScene(Color::BLACK);

    Func2();
    
    Func3();
    
    //func();

    for (int y = 0; y < 64; y += 4)
    {
        HLine(2).Draw(0, y);
    }

    EndScene();
    
    static uint timeStamp = 0;
    
    static const uint d = 2000;
    
    uint dT = HAL_GetTick() - timeStamp;
    
    if(dT > d && func == Func4)
    {
        func = Func5;
    }
    else if(dT > 2 * d)
    {
        func = Func4;
        timeStamp = HAL_GetTick();
     //   InverseMode();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawScreen()
{
    if(PageIndication::calibration.Is(Calibration::Pressed))
    {
        Text("---����� ����������---").Write(38, 5);
        Text("������� ���. ��� ����������").Write(5, 15);
        Text("������� ����� ������� ��� ������").Write(5, 50);
    }
    else
    {
        Rectangle(255, 63).Draw(0, 0, Color::WHITE);

        DrawStatusBar();    
            
        DrawTypeMeasure();
        
        DrawModeMeasure();
    
        DrawHint();
    
        DrawChannelSettings();

        Menu::Draw();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawChannelSettings()
{
    Text(Menu::ChannelSettings()).Write(38, 5);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawTypeMeasure()
{
    int x = 0;
    int y = 0;
    int width = 25; 
    Text(PageModes::typeMeasure.ToText()).Write(x, width, y + 5);
    Rectangle(width, 18).Draw(x, y, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawModeMeasure()
{
    static const Enumeration *modes[4] =
    {
        &PageModes::modeMeasureFrequency,
        &PageModes::modeMeasurePeriod,
        &PageModes::modeMeasureDuration,
        &PageModes::modeMeasureCountPulse
    };

    Text(modes[PageModes::typeMeasure.value]->ToText()).Write(10, 30);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawStatusBar()
{
    static const Enumeration * const modes[TypeMeasure::Count] =
    {///            Frequency                         Period                         Duration                         CountPulse
        &PageModes::modeMeasureFrequency, &PageModes::modeMeasurePeriod, &PageModes::modeMeasureDuration, &PageModes::modeMeasureCountPulse
    };

    static const Enumeration * const enums[TypeMeasure::Count][6] =
    {
        {   /// ModeMeasureFrequency::
            &PageModes::timeMeasure,        /// Freq
            &PageModes::numberPeriods,      /// AC
            &PageModes::numberPeriods,      /// AB
            &PageModes::timeMeasure,        /// T_1
            &PageModes::timeMeasure         /// Tachometer
        },
        {   /// ModeMeasurePeriod::
            &PageModes::numberPeriods,      /// Period
            &PageModes::timeMeasure         /// F_1
        },
        {   /// ModeMeasureDuration::
            nullptr,                        /// Ndt
            &PageModes::numberPeriods,      /// Ndt_1
            nullptr,                        /// Ndt_1ns
            nullptr,                        /// Interval
            nullptr,                        /// S_1
            nullptr                         /// Phase
        },
        {   /// ModeMeasureCountPulse::
            nullptr,                        /// Manual
            nullptr,                        /// ATC
            &PageModes::numberPeriods       /// ATC_1
        }
    };

    const Enumeration *mode = modes[PageModes::typeMeasure];

    const Enumeration *toText = enums[PageModes::typeMeasure][mode->value];

    int y = 48;
    int x = 0;
    int width = 31;

    if (toText)
    {
        Text(toText->ToText()).Write(x, width, y + 4);
    }

    Rectangle(width, 15).Draw(x, y, Color::WHITE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawHint()
{
    Text(Menu::Hint()).Write(38, 15);
}


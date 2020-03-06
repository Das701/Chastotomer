#include "Display.h"
#include "Primitives.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageModes.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageIndication.h"
#include "Text.h"
#include "Menu/MenuItems.h"
#include "Menu/Hint.h"
#include <cstdlib>
#include <cstring>
#include "Hardware/HAL.h"

using namespace Display::Primitives;
using Display::Text;

/// Нарисовать тип измерения
static void DrawTypeMeasure();
/// Нарисовать режим измерения
static void DrawModeMeasure();
/// Нарисовать подсказку
static void DrawHint();
/// Нарисовать статус-бар
static void DrawStatusBar();
/// Нарисовать строку настроек текущего канала
static void DrawChannelSettings();

static void DrawScreen();

static void DrawInfo();

static void DrawData();

extern int info;

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

static void Func3()
{
    static int x = 0;
    static int y = 0;

    int sizeX = 8;
    int sizeY = 3;

    static HLine line(sizeX);

    CalculateCoord(x, y, sizeX, sizeY);

    line.Draw(x, y, Color::WHITE);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Update()
{
    BeginScene(Color::BLACK);

    DrawScreen();
    
    Func3();

    EndScene();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawScreen()
{
    if(PageIndication::calibration.Is(Calibration::Pressed))
    {
        Text("---Режим Калибровка---").Write(38, 5);
        Text("Нажмите ЭНК. для сохранения").Write(5, 15);
        Text("Нажмите любую клавишу для выхода").Write(5, 50);
    }
    else
    {
        DrawStatusBar();    
            
        DrawTypeMeasure();
        
        DrawModeMeasure();
        
        DrawHint();
        
        DrawChannelSettings();
        
        DrawInfo();
        
        Menu::Draw();
        
        DrawData();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawChannelSettings()
{
    Text(Menu::ChannelSettings()).Write(102, 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawTypeMeasure()
{
    int x = 0;
    int width = 100; 
    Text(PageModes::typeMeasure.ToText()).Write(x, 10, width);
    Rectangle(width, 30).Draw(x, 0, Color::WHITE);
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

    Text(modes[PageModes::typeMeasure.value]->ToText()).Write(0, 40);
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

    int y = 65;
    int x = 0;
    int width = 60;

    if (toText)
    {
        Text(toText->ToText()).Write(x + 2, y + 7, width, Color::WHITE);
    }

    Rectangle(width, 30).Draw(x, y, Color::WHITE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawHint()
{
    Text(Hint::Text()).Write(102, 22);
}

static void DrawInfo()
{
    switch (info)
    {
        case 1:
            Text("Right").Write(100, 100);
            break;
        case 2:
            Text("Left").Write(100, 100);
            break;
        case 3:
            Text("GovLeft").Write(100, 100);
            break;
        case 4:
            Text("GovRight").Write(100, 100);
            break;
        case 5:
            Text("Enter").Write(100, 100);
            break;
        case 6:
            Text("Count").Write(100, 100);
            break;
        case 7:
            Text("None").Write(100, 100);
            break;
        case 8:
            Text("Mode").Write(100, 100);
            break;
        case 9:
            Text("Indication").Write(100, 100);
            break;
        case 10:
            Text("Channels").Write(100, 100);
            break;
        case 11:
            Text("GovButton").Write(100, 100);
            break;
        case 12:
            Text("Esc").Write(100, 100);
            break;
        case 13:
            Text("Test").Write(100, 100);
            break;
        case 14:
            Text("Auto").Write(100, 100);
            break;
    }
}

static void DrawData()
{
    Text(PLIS::GiveData()).Write(120, 120);
}

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
static void Delay(uint ms)
{
    uint timestamp = HAL_GetTick();
    while (HAL_GetTick() - timestamp < ms)
    {
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Update()
{
    BeginScene(Color::BLACK);

    //DrawScreen();

    static int x = 0;
    static int y = 0;

    int sizeX = 4;
    int sizeY = 4;

    CalculateCoord(x, y, sizeX, sizeY);

    Rectangle(sizeX, sizeY).Fill(x, y, Color::WHITE);

    static int x2 = 0;
    static int y2 = 0;

    CalculateCoord(x2, y2, sizeX, sizeY);

    Rectangle(sizeX, sizeY).Fill(x2, y2, Color::WHITE);

    for (int i = 0; i < 100; i++)
    {
        Point().Draw(std::rand() % 250, std::rand() % 60, Color::WHITE);
        Point().Draw(std::rand() % 250, std::rand() % 60, Color::BLACK);
    }

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
    int x = 5;
    int width = 25; 
    Text(PageModes::typeMeasure.ToText()).Write(x, width, 10);
    Rectangle(width, 18).Draw(x, 5, Color::WHITE);
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

    int y = 45;
    int x = 2;
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


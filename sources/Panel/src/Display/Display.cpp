#include "Display.h"
#include "Primitives.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageModes.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageIndication.h"
#include "Text.h"
#include "Menu/MenuItems.h"


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
    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void Display::Update()
//{
//    BeginScene(Color::BLACK);
//
//    Rectangle(256, 64).Draw(0, 0, Color::WHITE);
//
//    DrawScreen();
//
//    EndScene();
//}

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


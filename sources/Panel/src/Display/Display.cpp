#include "Display.h"
#include "Primitives.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageModes.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageIndication.h"
#include "Text.h"


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
    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Update()
{
    BeginScene(Color::BLACK);

    Rectangle(256, 64).Draw(0, 0, Color::WHITE);

    DrawScreen();

    EndScene();
}

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
    int x = 2;
    int y = 49;
    int width = 31;

    if(PageModes::typeMeasure.value == TypeMeasure::Frequency)
    {
        if (PageModes::modeMeasureFrequency == ModeMeasureFrequency::AC || 
        PageModes::modeMeasureFrequency == ModeMeasureFrequency::T_1)
        {
            Text(PageModes::numberPeriods.ToText()).Write(x, width, y);
        }
        else
        {
            Text(PageModes::timeMeasure.ToText()).Write(x, width, y);
        }
    }
    else if(PageModes::typeMeasure.value == TypeMeasure::Period)
    {
        if (PageModes::modeMeasurePeriod == ModeMeasurePeriod::Period)
        {
            Text(PageModes::numberPeriods.ToText()).Write(x, width, y);
        }
        else
        {
            Text(PageModes::timeMeasure.ToText()).Write(x, width, y);
        }
    }
    else if(PageModes::typeMeasure.value == TypeMeasure::Duration)
    {
        if (PageModes::modeMeasureDuration == ModeMeasureDuration::Ndt_1)
        {
            Text(PageModes::numberPeriods.ToText()).Write(x, width, y);
        }
    }
    else if(PageModes::typeMeasure.value == TypeMeasure::CountPulse)
    {
        if (PageModes::modeMeasureCountPulse == ModeMeasureCountPulse::ATC_1)
        {
            Text(PageModes::numberPeriods.ToText()).Write(x, width, y);
        }
    }

    Rectangle(width, 15).Draw(x, 45, Color::WHITE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawHint()
{
    Text(Menu::Hint()).Write(38, 15);
}


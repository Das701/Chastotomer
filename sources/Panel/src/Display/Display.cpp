#include "Display.h"
#include "Primitives.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageModes.h"
#include "Menu/Pages/PageChannelA.h"
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Update()
{
    BeginScene(Color::BLACK);

    Rectangle(256, 64).Draw(0, 0, Color::WHITE);

    DrawStatusBar();
    
    DrawTypeMeasure();
    
    DrawModeMeasure();

    DrawHint();

    Menu::Draw();

    EndScene();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawTypeMeasure()
{
    int x = 5;
    int y = 10;
    int width = 25; 
    Text(PageModes::typeMeasure.ToText()).Write(x, width, y);
    Rectangle(width, 18).Draw(x, 5, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawModeMeasure()
{
    int x = 10;
    int y = 30;

    if(PageModes::typeMeasure.value == TypeMeasure::Frequency)
    {
        Text(PageModes::modeMeasureFrequency.ToText()).Write(x, y);
    }
    else if(PageModes::typeMeasure.value == TypeMeasure::Period)
    {
        Text(PageModes::modeMeasurePeriod.ToText()).Write(x, y);
    }
    else if(PageModes::typeMeasure.value == TypeMeasure::Duration)
    {
        Text(PageModes::modeMeasureDuration.ToText()).Write(x, y);
    }
    else
    {
        Text(PageModes::modeCountPulse.ToText()).Write(x, y);
    }
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
        if (PageModes::modeCountPulse == ModeCountPulse::ATC_1)
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

#include "Display.h"
#include "Primitives.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageModes.h"
#include "Text.h"


using namespace Display::Primitives;
using Display::Text;

/// Нарисовать тип измерения
static void DrawType(int x, int y, int width, int height, int rectY);
/// Нарисовать режим измерения
static void DrawMode(int x, int y);
/// Нарисовать подсказки
static void DrawHints(int x, int y);
/// Нарисовать статус-бар
static void DrawStatusBar(int x, int y, int width, int height, int rectY);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Update()
{
    BeginScene(Color::BLACK);

    Rectangle(256, 64).Draw(0, 0, Color::WHITE);

    DrawStatusBar(2, 49, 31, 15, 45);
    
    DrawType(5, 10, 25, 18, 5);
    
    DrawMode(10, 30);

    DrawHints(38, 15);

    Menu::Draw();

    EndScene();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawType(int x, int y, int width, int height, int rectY)
{
    if(PageModes::typeMeasure.value == TypeMeasure::Frequency)
    {
        Text(PageModes::typeMeasure.ToText()).Write(x, width, y);
    }
    else if(PageModes::typeMeasure.value == TypeMeasure::Period)
    {
        Text(PageModes::typeMeasure.ToText()).Write(x, width, y);
    }
    else if(PageModes::typeMeasure.value == TypeMeasure::Duration)
    {
        Text(PageModes::typeMeasure.ToText()).Write(x, width, y);
    }
    else
    {
        Text(PageModes::typeMeasure.ToText()).Write(x, width, y);
    }   
    Rectangle(width, height).Draw(x, rectY, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawMode(int x, int y)
{
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
static void DrawStatusBar(int x, int y, int width, int height, int rectY)
{
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
    if(PageModes::typeMeasure.value == TypeMeasure::Period)
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
    if(PageModes::typeMeasure.value == TypeMeasure::Duration)
    {
        if (PageModes::modeMeasureDuration == ModeMeasureDuration::Ndt_1)
        {
            Text(PageModes::numberPeriods.ToText()).Write(x, width, y);
        }
    }
    if(PageModes::typeMeasure.value == TypeMeasure::CountPulse)
    {
        if (PageModes::modeCountPulse == ModeCountPulse::ATC_1)
        {
            Text(PageModes::numberPeriods.ToText()).Write(x, width, y);
        }
    }
    Rectangle(width, height).Draw(x, rectY, Color::WHITE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawHints(int x, int y)
{
    if(PageModes::hintsMode.value == Hints::FrequencyHints)
    {
        Text(PageModes::modeMeasureFrequency.ToText()).Write(Text("Измерение частоты : ").Write(x, y), y);
    }
    else if (PageModes::hintsMode.value == Hints::PeriodHints)
    {
        Text(PageModes::modeMeasurePeriod.ToText()).Write(Text("Измерение периода : ").Write(x, y), y);
    }
    else if (PageModes::hintsMode.value == Hints::DurationHints)
    {
        Text(PageModes::modeMeasureDuration.ToText()).Write(Text("Измерение длительности : ").Write(x, y), y);
    }
    else if (PageModes::hintsMode.value == Hints::CountPulseHints)
    {
        Text(PageModes::modeCountPulse.ToText()).Write(Text("Счет числа импульсов : ").Write(x, y), y);
    }
    else if (PageModes::hintsMode.value == Hints::TimeLabelsHints)
    {
        Text(PageModes::periodTimeLabels.ToText()).Write(Text("Длительность временных меток : ").Write(x, y), y);
    }
    else if (PageModes::hintsMode.value == Hints::TimeMeasureHints)
    {  
        Text(PageModes::timeMeasure.ToText()).Write(Text("Время счета : ").Write(x, y), y);
    }
    else if (PageModes::hintsMode.value == Hints::NumberPeriodsHints)
    {
        Text(PageModes::numberPeriods.ToText()).Write(Text("Число периодов измерения : ").Write(x, y), y);
    }
}

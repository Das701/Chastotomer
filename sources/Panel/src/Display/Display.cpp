#include "Display.h"
#include "Primitives.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageModes.h"
#include "Menu/Pages/PageChannelA.h"
#include "Text.h"


using namespace Display::Primitives;
using Display::Text;

/// Нарисовать тип измерения
static void DrawTypeMeasure(int x, int y);
/// Нарисовать режим измерения
static void DrawModeMeasure(int x, int y);
/// Нарисовать подсказку
static void DrawHint(int x, int y);
/// Нарисовать статус-бар
static void DrawStatusBar(int x, int y);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Update()
{
    BeginScene(Color::BLACK);

    Rectangle(256, 64).Draw(0, 0, Color::WHITE);

    DrawStatusBar(2, 49);
    
    DrawTypeMeasure(5, 10);
    
    DrawModeMeasure(10, 30);

    DrawHint(38, 15);

    Menu::Draw();

    EndScene();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawTypeMeasure(int x, int y)
{
    int width = 25; 
    Text(PageModes::typeMeasure.ToText()).Write(x, width, y);
    Rectangle(width, 18).Draw(x, 5, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawModeMeasure(int x, int y)
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
static void DrawStatusBar(int x, int y)
{
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
static void DrawHint(int x, int y)
{
    if(PageModes::hint.value == Hint::Frequency)
    {
        Text(PageModes::modeMeasureFrequency.ToText()).Write(Text("Измерение частоты : ").Write(x, y), y);
    }
    else if (PageModes::hint.value == Hint::Period)
    {
        Text(PageModes::modeMeasurePeriod.ToText()).Write(Text("Измерение периода : ").Write(x, y), y);
    }
    else if (PageModes::hint.value == Hint::Duration)
    {
        Text(PageModes::modeMeasureDuration.ToText()).Write(Text("Измерение длительности : ").Write(x, y), y);
    }
    else if (PageModes::hint.value == Hint::CountPulse)
    {
        Text(PageModes::modeCountPulse.ToText()).Write(Text("Счет числа импульсов : ").Write(x, y), y);
    }
    else if (PageModes::hint.value == Hint::TimeLabels)
    {
        Text(PageModes::periodTimeLabels.ToText()).Write(Text("Длительность временных меток : ").Write(x, y), y);
    }
    else if (PageModes::hint.value == Hint::TimeMeasure)
    {  
        Text(PageModes::timeMeasure.ToText()).Write(Text("Время счета : ").Write(x, y), y);
    }
    else if (PageModes::hint.value == Hint::NumberPeriods)
    {
        Text(PageModes::numberPeriods.ToText()).Write(Text("Число периодов измерения : ").Write(x, y), y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawChannelA(int x, int y)
{
    if(PageChannelA::hintA.value == HintA::OCI)
    {
        Text(PageChannelA::modeOCI.ToText()).Write(x, y);
    }
    else if(PageChannelA::hintA.value == HintA::Impedance)
    {
        Text(PageChannelA::modeImpedance.ToText()).Write(x, y);
    }
    else if(PageChannelA::hintA.value == HintA::Filter)
    {
        Text(PageChannelA::modeFilter.ToText()).Write(x, y);
    }
    else if(PageChannelA::hintA.value == HintA::Front)
    {
        Text(PageChannelA::modeFront.ToText()).Write(x, y);
    }
    else if(PageChannelA::hintA.value == HintA::VD)
    {
        Text(PageChannelA::modeVD.ToText()).Write(x, y);
    }
    else if(PageChannelA::hintA.value == HintA::Sync)
    {
        Text(PageChannelA::modeSync.ToText()).Write(x, y);
    }
}

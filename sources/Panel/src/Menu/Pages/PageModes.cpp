#include "defines.h"
#include "PageModes.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"

using namespace Display::Primitives;
using namespace Display;

extern Item *items[];
extern Switch sPeriodTimeLabels;
extern Switch sTimeMeasure;
extern Switch sNumberPeriods;
int drawMode = 0;

ModeMeasure          PageModes::modeMeasure(ModeMeasure::Frequency);
ModeMeasureFrequency PageModes::modeMeasureFrequency(ModeMeasureFrequency::Freq);
ModeMeasurePeriod    PageModes::modeMeasurePeriod(ModeMeasurePeriod::Period);
ModeMeasureDuration  PageModes::modeMeasureDuration(ModeMeasureDuration::Ndt);
ModeCountPulse       PageModes::modeCountPulse(ModeCountPulse::Manual);

PeriodTimeLabels     PageModes::periodTimeLabels(PeriodTimeLabels::T_3);
NumberPeriods        PageModes::numberPeriods(NumberPeriods::_1);
TimeMeasure          PageModes::timeMeasure(TimeMeasure::_1ms);



//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Frequency()
{
    if (PageModes::modeMeasureFrequency == ModeMeasureFrequency::AC || 
        PageModes::modeMeasureFrequency == ModeMeasureFrequency::T_1)
    {
        items[5] = &sNumberPeriods;
    }
    else
    {
        items[5] = &sTimeMeasure;
    }
    drawMode = 1;

    PageModes::modeMeasure.value = ModeMeasure::Frequency;
}

/// Выбор режима измерения частоты, отношения частот, "тахометра"
static Switch sFrequency("f", (uint8 *)&PageModes::modeMeasureFrequency, 5, &OnPress_Frequency);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Period()
{
    if (PageModes::modeMeasurePeriod == ModeMeasurePeriod::Period)
    {
        items[4] = &sPeriodTimeLabels;
        items[5] = &sNumberPeriods;
    }
    else
    {
        items[4] = &sTimeMeasure;
        items[5] = nullptr;
    }
    drawMode = 2;

    PageModes::modeMeasure.value = ModeMeasure::Period;
}

/// Выбор режима измерения периода
static Switch sPeriod("T", (uint8 *)&PageModes::modeMeasurePeriod, 2, &OnPress_Period);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Duration()
{
    if (PageModes::modeMeasureDuration == ModeMeasureDuration::Ndt_1)
    {
        items[4] = &sPeriodTimeLabels;
        items[5] = &sNumberPeriods;
    }
    else if (PageModes::modeMeasureDuration == ModeMeasureDuration::Ndt_1ns)
    {
        items[4] = nullptr;
        items[5] = nullptr;
    }
    else
    {
        items[4] = &sPeriodTimeLabels;
        items[5] = nullptr;
    }
    drawMode = 3;

    PageModes::modeMeasure.value = ModeMeasure::Duration;
}
/// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
static Switch sDuration("t", (uint8 *)&PageModes::modeMeasureDuration, 6, &OnPress_Duration);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_CountPulse()
{
    if (PageModes::modeCountPulse == ModeCountPulse::ATC_1)
    {

        items[4] = &sNumberPeriods;
        items[5] = nullptr;
    }
    else 
    {
        items[4] = nullptr;
        items[5] = nullptr;
    }
    drawMode = 4;

    PageModes::modeMeasure.value = ModeMeasure::CountPulse;
}
/// Выбор режима счёта импульсов
static Switch sCountPulse("Счёт", (uint8 *)&PageModes::modeCountPulse, 3, &OnPress_CountPulse);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeLabels()
{
    drawMode = 5;
}
/// Выбор периода меток времени
static Switch sPeriodTimeLabels("Метки", (uint8 *)&PageModes::periodTimeLabels, 6, &OnPress_TimeLabels);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeMeasure()
{
    drawMode = 6;
}
/// Выбор времени измерения
static Switch sTimeMeasure("Время",(uint8 *)&PageModes::timeMeasure, 6, &OnPress_TimeMeasure);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_NumberPeriods()
{
    drawMode = 7;
}
/// Выбор числа усредняемых периодов входного сигнала
static Switch sNumberPeriods("N",(uint8 *)&PageModes::numberPeriods, 6, &OnPress_NumberPeriods);


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static Item *items[] =
{
    &sFrequency,
    &sPeriod,
    &sDuration,
    &sCountPulse,
    &sPeriodTimeLabels,
    &sTimeMeasure,
    nullptr
};

static Page pageModes(items);

Page *PageModes::self = &pageModes;

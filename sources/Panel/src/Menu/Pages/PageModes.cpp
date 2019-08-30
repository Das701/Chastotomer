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

TypeMeasure          PageModes::typeMeasure(TypeMeasure::Frequency);
Hints                PageModes::hintsMode(Hints::FrequencyHints);
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
        items[4] = &sPeriodTimeLabels;
        items[5] = &sNumberPeriods;
    }
    else
    {
        items[4] = &sPeriodTimeLabels;
        items[5] = &sTimeMeasure;
    }
    PageModes::hintsMode.value = Hints::FrequencyHints;
    
    PageModes::typeMeasure.value = TypeMeasure::Frequency;
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
    PageModes::hintsMode.value = Hints::PeriodHints;

    PageModes::typeMeasure.value = TypeMeasure::Period;
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
    PageModes::hintsMode.value = Hints::DurationHints;

    PageModes::typeMeasure.value = TypeMeasure::Duration;
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
    PageModes::hintsMode.value = Hints::CountPulseHints;

    PageModes::typeMeasure.value = TypeMeasure::CountPulse;
}
/// Выбор режима счёта импульсов
static Switch sCountPulse("Счёт", (uint8 *)&PageModes::modeCountPulse, 3, &OnPress_CountPulse);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeLabels()
{
    PageModes::hintsMode.value = Hints::TimeLabelsHints;
}
/// Выбор периода меток времени
static Switch sPeriodTimeLabels("Метки", (uint8 *)&PageModes::periodTimeLabels, 6, &OnPress_TimeLabels);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeMeasure()
{
    PageModes::hintsMode.value = Hints::TimeMeasureHints;
}
/// Выбор времени измерения
static Switch sTimeMeasure("Время",(uint8 *)&PageModes::timeMeasure, 6, &OnPress_TimeMeasure);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_NumberPeriods()
{
    PageModes::hintsMode.value = Hints::NumberPeriodsHints;
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

pString NumberPeriods::ToText()
{
    static const pString names[6] = { "1", "10", "100", "1K", "10K", "100K" };
    return names[value];
}

pString TimeMeasure::ToText()
{
    static const pString names[6] = { "1ms", "10ms", "100ms", "1s", "10s", "100s" };
    return names[value];
}

pString PeriodTimeLabels::ToText()
{
    static const pString names[6] = { "10-3", "10-4", "10-5", "10-6", "10-7", "10-8" };
    return names[value];
}

pString ModeCountPulse::ToText()
{
    static const pString names[3] = { "Ручн.", "А(tC)", "А(TC)" };
    return names[value];
}

pString ModeMeasureDuration::ToText()
{
    static const pString names[6] = { "ndt", "<ndt>", "ndt/1нс", "Интервал", "1/S", "Фаза" };
    return names[value];
}

pString ModeMeasurePeriod::ToText()
{
    static const pString names[2] = { "Период", "T=1/f" };
    return names[value];
}

pString ModeMeasureFrequency::ToText()
{
    static const pString names[5] = { "Частота", "f(A)/f(C)", "f(A)/f(B)", "f=1/T", "Тахометр" };
    return names[value];
}

pString TypeMeasure::ToText()
{
    static const pString names[4] = { "f", "T", "t", "Счет"};
    return names[value];
}

#include "defines.h"
#include "PageModes.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"

using namespace Display::Primitives;
using namespace Display;

extern Item *items[];
int statusBar = 1;
extern Switch sPeriodTimeLabels;
extern Switch sTimeMeasure;
extern Switch sNumberPeriods;
int drawMode = 0;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static MeasureFrequency::E modeFreq = MeasureFrequency::Freq;

MeasureFrequency::E PageModes::ModeMeasureFrequency()
{
    return modeFreq;
}

static void OnPress_Frequency()
{
    if (modeFreq == MeasureFrequency::AC || modeFreq == MeasureFrequency::T_1)
    {
        items[5] = &sNumberPeriods;
        statusBar = 2;
    }
    else
    {
        items[5] = &sTimeMeasure;
        statusBar = 1;
    }
    drawMode = 1;
}

/// Выбор режима измерения частоты, отношения частот, "тахометра"
static Switch sFrequency("f", (uint8 *)&modeFreq, 5, &OnPress_Frequency);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static MeasurePeriod::E modePeriod = MeasurePeriod::Period;

MeasurePeriod::E PageModes::ModeMeasurePeriod()
{
    return modePeriod;
}

static void OnPress_Period()
{
    if (modePeriod == MeasurePeriod::Period)
    {
        items[4] = &sPeriodTimeLabels;
        items[5] = &sNumberPeriods;
        statusBar = 2;
    }
    else
    {
        items[4] = &sTimeMeasure;
        items[5] = nullptr;
        statusBar = 1;
    }
    drawMode = 2;
}

/// Выбор режима измерения периода
static Switch sPeriod("T", (uint8 *)&modePeriod, 2, &OnPress_Period);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static MeasureDuration::E modeDuration = MeasureDuration::Ndt;

MeasureDuration::E PageModes::ModeMeasureDuration()
{
    return modeDuration;
}

static void OnPress_Duration()
{
    if (modeDuration == MeasureDuration::Ndt_1)
    {
        items[4] = &sPeriodTimeLabels;
        items[5] = &sNumberPeriods;
        statusBar = 2;
    }
    else if (modeDuration == MeasureDuration::Ndt_1ns)
    {
        items[4] = nullptr;
        items[5] = nullptr;
        statusBar = 0;
    }
    else
    {
        items[4] = &sPeriodTimeLabels;
        items[5] = nullptr;
        statusBar = 0;
    }
    drawMode = 3;
}
/// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
static Switch sDuration("t", (uint8 *)&modeDuration, 6, &OnPress_Duration);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static CountPulse::E modeCountPulse = CountPulse::Manual;

CountPulse::E PageModes::ModeCountPulse()
{
    return modeCountPulse;
}

static void OnPress_CountPulse()
{
    if (modeCountPulse == CountPulse::ATC_1)
    {

        items[4] = &sNumberPeriods;
        items[5] = nullptr;
        statusBar = 2;
    }
    else 
    {
        items[4] = nullptr;
        items[5] = nullptr;
        statusBar = 0;
    }
    drawMode = 4;
}
/// Выбор режима счёта импульсов
static Switch sCountPulse("Счёт", (uint8 *)&modeCountPulse, 3, &OnPress_CountPulse);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static TimeLabelLenght::E modeTimeLabels = TimeLabelLenght::T_3;

TimeLabelLenght::E PageModes::ModeTimeLabelLenght()
{
    return modeTimeLabels;
}

static void OnPress_TimeLabels()
{
    drawMode = 5;
}
/// Выбор периода меток времени
static Switch sPeriodTimeLabels("Метки", (uint8 *)&modeTimeLabels, 6, &OnPress_TimeLabels);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static TimeMeasure::E modeTimeMeasure = TimeMeasure::MSec_1;

TimeMeasure::E PageModes::ModeTimeMeasure()
{
    return modeTimeMeasure;
}

static void OnPress_TimeMeasure()
{
    drawMode = 6;
    statusBar = 1;
}
/// Выбор времени измерения
static Switch sTimeMeasure("Время",(uint8 *)&modeTimeMeasure, 6, &OnPress_TimeMeasure);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static NumberPeriods::E modeNumberPeriods = NumberPeriods::NP_1;

NumberPeriods::E PageModes::ModeNumberPeriods()
{
    return modeNumberPeriods;
}

static void OnPress_NumberPeriods()
{
    drawMode = 7;
    statusBar = 2;
}
/// Выбор числа усредняемых периодов входного сигнала
static Switch sNumberPeriods("N",(uint8 *)&modeNumberPeriods, 6, &OnPress_NumberPeriods);


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

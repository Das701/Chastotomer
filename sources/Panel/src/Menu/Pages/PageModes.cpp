#include "defines.h"
#include "PageModes.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"


using namespace Display::Primitives;
using namespace Display;

static void OnPressFreq();
static void OnPressPeriod();
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 modeFreq = 0;
static uint8 modePeriod = 0;
MeasureFrequency PageModes::ModeMeasureFrequency()
{
    return (MeasureFrequency)modeFreq;
}

MeasurePeriod PageModes::ModeMeasurePeriod()
{
    return (MeasurePeriod)modePeriod;
}

/// Выбор режима измерения частоты, отношения частот, "тахометра"
static Switch sFrequency("f", &modeFreq, 5, &OnPressFreq);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// Выбор режима измерения периода
static Switch sPeriod("T", &modePeriod, 2, &OnPressPeriod);
static Button bPeriod("T");

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
static Button bDuration("t");

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// Выбор режима счёта импульсов
static Button bCountPulse("Счёт");

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// Выбор периода меток времени
static Button bPeriodTimeLabels("Метки");

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// Выбор времени измерения
static Button bTimeMeasure("Время");

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// Выбор числа усредняемых периодов входного сигнала
static Button bNumberPeriods("N");

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


static Item *items[] =
{
    &sFrequency,
    &sPeriod,
    &bDuration,
    &bCountPulse,
    &bPeriodTimeLabels,
    &bTimeMeasure,
    nullptr
};

static void OnPressFreq()
{
    if((MeasureFrequency)modeFreq == _AC || (MeasureFrequency)modeFreq == _fT)
    {
        items[5] = &bNumberPeriods;
    }
    else
    {
        items[5] = &bTimeMeasure;
    }
    
}

static void OnPressPeriod()
{
    if((MeasurePeriod)modePeriod == _Period)
    {
        items[4] = &bPeriodTimeLabels;
        items[5] = &bNumberPeriods;
    }
    else
    {
        items[4] = &bTimeMeasure;
        items[5] = nullptr;
    }

}

static Page pageModes(items);

Page *PageModes::self = &pageModes;

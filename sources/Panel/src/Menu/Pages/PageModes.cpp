#include "defines.h"
#include "PageModes.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Menu/MenuItemsDef.h"
#include "FreqMeter/FreqMeter.h"


using namespace Display::Primitives;
using namespace Display;

extern Item *items[7];
extern Switch sModeFrequency;
extern Switch sModePeriod;
extern Switch sModeDuration;
extern Switch sModeCountPulse;
extern Switch sPeriodTimeLabels;
extern Switch sTimeMeasure;
extern Switch sNumberPeriods;


TypeMeasure             PageModes::typeMeasure(TypeMeasure::Frequency);
ModeMeasureFrequency    PageModes::modeMeasureFrequency(ModeMeasureFrequency::Freq);
ModeMeasurePeriod       PageModes::modeMeasurePeriod(ModeMeasurePeriod::Period);
ModeMeasureDuration     PageModes::modeMeasureDuration(ModeMeasureDuration::Ndt);
ModeMeasureCountPulse   PageModes::modeMeasureCountPulse(ModeMeasureCountPulse::Manual);

PeriodTimeLabels        PageModes::periodTimeLabels(PeriodTimeLabels::T_3);
NumberPeriods           PageModes::numberPeriods(NumberPeriods::_1);
TimeMeasure             PageModes::timeMeasure(TimeMeasure::_1ms);

/// Очистить массив указателей на итемы, начиная с i-го итема
static void ClearItems(int i);

static void OnPress_ModeFrequency();

static void OnPress_ModePeriod();

static void OnPress_ModeDuration();

static void OnPress_ModeCountPulse();


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageModes::Init()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ClearItems(int num)
{
    for (int i = num; i < 7; i++)
    {
        items[i] = nullptr;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TypeMeasure()
{
    switch (PageModes::typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        OnPress_ModeFrequency();
        break;

    case TypeMeasure::Period:
        OnPress_ModePeriod();
        break;

    case TypeMeasure::Duration:
        OnPress_ModeDuration();
        break;

    case TypeMeasure::CountPulse:
        OnPress_ModeCountPulse();
        break;
    }
}

DEF_SWITCH_4(sTypeMeasure,
    "Вид измерения", "Выбор измерения",
    "Частота", "Период", "Длительность", "Счёт импульсов",
    PageModes::typeMeasure, OnPress_TypeMeasure
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeFrequency()
{
    ClearItems(2);

    items[1] = &sModeFrequency;
    items[2] = &sPeriodTimeLabels;

    if (PageModes::modeMeasureFrequency == ModeMeasureFrequency::AC || 
        PageModes::modeMeasureFrequency == ModeMeasureFrequency::T_1)
    {
        items[3] = &sNumberPeriods;
    }
    else
    {
        items[3] = &sTimeMeasure;
    }

    FreqMeter::LoadModeMeasureFrequency();
}

/// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_5(sModeFrequency,
    "f", "Измерение частоты",
    "Частота", "f(A)/f(C)", "f(A)/f(B)", "f=1/T", "Тахометр",
    PageModes::modeMeasureFrequency, OnPress_ModeFrequency);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModePeriod()
{
    ClearItems(2);

    items[1] = &sModePeriod;

    if (PageModes::modeMeasurePeriod == ModeMeasurePeriod::Period)
    {
        items[2] = &sPeriodTimeLabels;
        items[3] = &sNumberPeriods;
    }
    else
    {
        items[2] = &sTimeMeasure;
    }

    FreqMeter::LoadModeMeasurePeriod();
}

/// Выбор режима измерения периода
DEF_SWITCH_2(sModePeriod,
    "T", "Измерение периода",
    "Период", "T=1/f",
    PageModes::modeMeasurePeriod, OnPress_ModePeriod
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeDuration()
{
    ClearItems(2);

    items[1] = &sModeDuration;

    if (PageModes::modeMeasureDuration == ModeMeasureDuration::Ndt_1)
    {
        items[2] = &sPeriodTimeLabels;
        items[3] = &sNumberPeriods;
    }
    else if (PageModes::modeMeasureDuration == ModeMeasureDuration::Ndt_1ns)
    {
    }
    else
    {
        items[2] = &sPeriodTimeLabels;
    }

    FreqMeter::LoadModeMeasureDuration();
}

/// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
DEF_SWITCH_6(sModeDuration,
    "t", "Измерение длительности",
    "ndt", "<ndt>", "ndt/1нс", "Интервал", "1/S", "Фаза",
    PageModes::modeMeasureDuration, OnPress_ModeDuration
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeCountPulse()
{
    ClearItems(2);

    items[1] = &sModeCountPulse;

    if (PageModes::modeMeasureCountPulse == ModeMeasureCountPulse::ATC_1)
    {
        items[2] = &sNumberPeriods;
    }

    FreqMeter::LoadModeMeasureCountPulse();
}

/// Выбор режима счёта импульсов
DEF_SWITCH_3(sModeCountPulse,
    "Счёт", "Счёт числа импульсов",
    "Ручн.", "А(tC)", "А(TC)",
    PageModes::modeMeasureCountPulse, OnPress_ModeCountPulse
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeLabels()
{
    FreqMeter::LoadPeriodTimeLabels();
}

/// Выбор периода меток времени
DEF_SWITCH_6(sPeriodTimeLabels,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModes::periodTimeLabels,  OnPress_TimeLabels
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeMeasure()
{
    FreqMeter::LoadTimeMeasure();
}

/// Выбор времени измерения
DEF_SWITCH_6(sTimeMeasure,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s",
    PageModes::timeMeasure, OnPress_TimeMeasure
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_NumberPeriods()
{
    FreqMeter::LoadNumerPeriodsMeasure();
}

/// Выбор числа усредняемых периодов входного сигнала
DEF_SWITCH_6(sNumberPeriods,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K",
    PageModes::numberPeriods,
    OnPress_NumberPeriods
);


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static Item *items[7] =
{
    &sTypeMeasure,
    &sModeFrequency,
    &sPeriodTimeLabels,
    &sTimeMeasure,
    nullptr,
    nullptr,
    nullptr
};

static Page pageModes(items);

Page *PageModes::self = &pageModes;

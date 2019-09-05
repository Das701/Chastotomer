#include "defines.h"
#include "PageModes.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Menu/MenuItemsDef.h"

using namespace Display::Primitives;
using namespace Display;

extern Item *items[];
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



//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageModes::Init()
{
    static char *names[] = { "f", "T", "t", "Счёт", nullptr };

    typeMeasure.names = names;
}

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

    PageModes::typeMeasure.value = TypeMeasure::Frequency;
}

/// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_5(sFrequency,
    "f", "Измерение частоты",
    "Частота", "f(A)/f(C)", "f(A)/f(B)", "f=1/T", "Тахометр",
    PageModes::modeMeasureFrequency, OnPress_Frequency);

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

    PageModes::typeMeasure.value = TypeMeasure::Period;
}

/// Выбор режима измерения периода
DEF_SWITCH_2(sPeriod,
    "T", "Измерение периода",
    "Период", "T=1/f",
    PageModes::modeMeasurePeriod, OnPress_Period
);

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

    PageModes::typeMeasure.value = TypeMeasure::Duration;
}

/// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
DEF_SWITCH_6(sDuration,
    "t", "Измерение длительности",
    "ndt", "<ndt>", "ndt/1нс", "Интервал", "1/S", "Фаза",
    PageModes::modeMeasureDuration,
    OnPress_Duration
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_CountPulse()
{
    if (PageModes::modeMeasureCountPulse == ModeMeasureCountPulse::ATC_1)
    {

        items[4] = &sNumberPeriods;
        items[5] = nullptr;
    }
    else 
    {
        items[4] = nullptr;
        items[5] = nullptr;
    }

    PageModes::typeMeasure.value = TypeMeasure::CountPulse;
}

/// Выбор режима счёта импульсов
DEF_SWITCH_3(sCountPulse,
    "Счёт", "Счёт числа импульсов",
    "Ручн.", "А(tC)", "А(TC)",
    PageModes::modeMeasureCountPulse,
    OnPress_CountPulse
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeLabels()
{
}

/// Выбор периода меток времени
DEF_SWITCH_6(sPeriodTimeLabels,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModes::periodTimeLabels,
    OnPress_TimeLabels
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeMeasure()
{
}

/// Выбор времени измерения
DEF_SWITCH_6(sTimeMeasure,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s",
    PageModes::timeMeasure,
    OnPress_TimeMeasure
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_NumberPeriods()
{
}

/// Выбор числа усредняемых периодов входного сигнала
DEF_SWITCH_6(sNumberPeriods,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K",
    PageModes::numberPeriods,
    OnPress_NumberPeriods
);


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

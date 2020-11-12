#include "defines.h"
#include "FreqMeter.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Hardware/MathFPGA.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Modes/PagesModes.h"


using namespace Primitives;


extern Item *items[7];
extern Switch sModeFrequencyC;
extern Switch sModeCountPulseC;
extern Switch sPeriodTimeLabelsC;
extern Switch sTimeMeasureC;
extern Switch sNumberPeriodsC;


static const bool correctTypeMeasure[TypeMeasure::Count] = { true, false, false, true };
TypeMeasure           PageModesC::typeMeasure(TypeMeasure::Frequency, correctTypeMeasure, TypeMeasure::Count);
static const bool correctModeMeasureFrequency[ModeMeasureFrequency::Count] = { true, false, false, false, false, false, true, true, false, false };
ModeMeasureFrequency  PageModesC::modeMeasureFrequency(ModeMeasureFrequency::Frequency, correctModeMeasureFrequency, ModeMeasureFrequency::Count);
static const bool correctModeMeasureCountPulse[ModeMeasureCountPulse::Count] = { false, false, false, false, true, true, true, true, false };
ModeMeasureCountPulse PageModesC::modeMeasureCountPulse(ModeMeasureCountPulse::CtA, correctModeMeasureCountPulse, ModeMeasureCountPulse::Count);
PeriodTimeLabels      PageModesC::periodTimeLabels(PeriodTimeLabels::T_8);
NumberPeriods         PageModesC::numberPeriods(NumberPeriods::_1);
TimeMeasure           PageModesC::timeMeasure(TimeMeasure::_1ms);


void PageModesC::OnChanged_TypeMeasure()
{
    switch (PageModesC::typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        PageModesA::typeMeasure.value = TypeMeasure::Frequency;
        PageModesB::typeMeasure.value = TypeMeasure::Frequency;

        PageModes::ResetModeCurrentMeasure();
        PageModesC::OnChanged_ModeFrequency();
        break;
    case TypeMeasure::CountPulse:
        PageModesB::typeMeasure.value = TypeMeasure::CountPulse;
        PageModesA::typeMeasure.value = TypeMeasure::CountPulse;

        PageModes::ResetModeCurrentMeasure();
        PageModesC::OnChanged_ModeCountPulse();
        break;
    }
}

DEF_SWITCH_2(sTypeMeasureC,
    "Измерение", "Выбор измерения",
    "Частота", "Сч. имп.",
    PageModesC::typeMeasure, PageModesC::OnChanged_TypeMeasure
)


void PageModesC::OnChanged_ModeFrequency()
{
    items[1] = &sModeFrequencyC;
    items[2] = &sPeriodTimeLabelsC;

    switch (PageModesC::modeMeasureFrequency.value)
    {
    case ModeMeasureFrequency::Frequency:
        items[3] = &sTimeMeasureC;
        items[4] = nullptr;
        break;

    case ModeMeasureFrequency::RatioCA:
    case ModeMeasureFrequency::RatioCB:
        items[3] = &sNumberPeriodsC;
        items[4] = nullptr;
        break;
    }

    PageModesA::RelationOff();
    
    ModeMeasureFrequency::LoadToFPGA();
}

// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_3(sModeFrequencyC,
    "Режим", "Измерение частоты",
    "Частота", "f(C)/f(A)", "f(C)/f(B)",
    PageModesC::modeMeasureFrequency, PageModesC::OnChanged_ModeFrequency
);


void PageModesC::OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulseC;

    switch (PageModesC::modeMeasureCountPulse.value)
    {
    case ModeMeasureCountPulse::CtA:
    case ModeMeasureCountPulse::CtB:
        items[2] = nullptr;
        break;

    case ModeMeasureCountPulse::CTA:
    case ModeMeasureCountPulse::CTB:
        items[2] = &sNumberPeriodsC;
        items[3] = nullptr;
        break;
    }

    items[3] = nullptr;

    PageModesA::RelationOff();
    
    ModeMeasureCountPulse::LoadToFPGA();
}

// Выбор режима счёта импульсов
DEF_SWITCH_4(sModeCountPulseC,
    "Режим", "Счёт числа импульсов",
    "C(tA)", "C(tB)", "C(TA)", "C(TB)",
    PageModesC::modeMeasureCountPulse, PageModesC::OnChanged_ModeCountPulse
);


static void OnPress_TimeLabelsC()
{
    uint8 period = PageModesC::periodTimeLabels.value;

    PageModesA::periodTimeLabels.value = period;
    PageModesB::periodTimeLabels.value = period;

    FreqMeter::LoadPeriodTimeLabels();
}

// Выбор периода меток времени
DEF_SWITCH_6(sPeriodTimeLabelsC,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModesC::periodTimeLabels, OnPress_TimeLabelsC
);


static void OnPress_TimeMeasure()
{
    uint8 time = PageModesC::timeMeasure.value;

    PageModesA::timeMeasure.value = time;
    PageModesB::timeMeasure.value = time;
    PageModesD::timeMeasure.value = time;

    static const uint8 periods[TimeMeasure::Count] =
    {
        NumberPeriods::_1,
        NumberPeriods::_10,
        NumberPeriods::_100,
        NumberPeriods::_1K,
        NumberPeriods::_10K,
        NumberPeriods::_100K,
        NumberPeriods::_1000K
    };

    uint8 period = periods[time];

    PageModesA::numberPeriods.value = period;
    PageModesB::numberPeriods.value = period;
    PageModesC::numberPeriods.value = period;

    FreqMeter::LoadTimeMeasure();
}

// Выбор времени измерения
DEF_SWITCH_7(sTimeMeasureC,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModesC::timeMeasure, OnPress_TimeMeasure
);


static void OnPress_NumberPeriodsC()
{
    uint8 period = PageModesC::numberPeriods.value;

    PageModesA::numberPeriods.value = period;
    PageModesB::numberPeriods.value = period;

    static const uint8 times[NumberPeriods::Count] =
    {
        TimeMeasure::_1ms,
        TimeMeasure::_10ms,
        TimeMeasure::_100ms,
        TimeMeasure::_1s,
        TimeMeasure::_10s,
        TimeMeasure::_100s,
        TimeMeasure::_1000s
    };

    uint8 time = times[PageModesC::numberPeriods.value];

    PageModesA::timeMeasure.value = time;
    PageModesB::timeMeasure.value = time;
    PageModesC::timeMeasure.value = time;

    FreqMeter::LoadNumerPeriodsMeasure();
}

// Выбор числа усредняемых периодов входного сигнала
DEF_SWITCH_7(sNumberPeriodsC,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K", "1000K",
    PageModesC::numberPeriods,
    OnPress_NumberPeriodsC
);



static Item *items[7] =
{
    &sTypeMeasureC,
    &sModeFrequencyC,
    &sPeriodTimeLabelsC,
    &sTimeMeasureC,
    nullptr
};

static Page pageModesC(items, nullptr);

Page *PageModesC::self = &pageModesC;


void PageModesC::ResetModeCurrentMeasure()
{
    switch (typeMeasure)
    {
    case TypeMeasure::Frequency:  modeMeasureFrequency.value = 0;  break;
    case TypeMeasure::CountPulse: modeMeasureCountPulse.value = 0; break;
    }
}

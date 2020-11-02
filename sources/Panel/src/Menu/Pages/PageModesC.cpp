#include "defines.h"
#include "FreqMeter.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageModesC.h"
#include "Menu/Pages/PageModesA.h"
#include "Menu/Pages/PageModesB.h"
#include "Menu/Pages/PageModesD.h"


using namespace Display::Primitives;
using namespace Display;

extern Item *items[7];
extern Switch sModeFrequencyC;
extern Switch sModeCountPulseC;
extern Switch sPeriodTimeLabelsC;
extern Switch sTimeMeasureC;
extern Switch sNumberPeriodsC;


TypeMeasureC             PageModesC::typeMeasure(TypeMeasureC::Frequency);
ModeMeasureFrequencyC    PageModesC::modeMeasureFrequency(ModeMeasureFrequencyC::Freq);
ModeMeasureCountPulseC   PageModesC::modeMeasureCountPulse(ModeMeasureCountPulseC::CtA);
PeriodTimeLabels         PageModesC::periodTimeLabels(PeriodTimeLabels::T_8);
NumberPeriods            PageModesC::numberPeriods(NumberPeriods::_1);
TimeMeasure              PageModesC::timeMeasure(TimeMeasure::_1ms);


void PageModesC::Init()
{
}


void PageModesC::OnChanged_TypeMeasure()
{
    switch (PageModesC::typeMeasure.value)
    {
    case TypeMeasureC::Frequency:
        PageModesA::typeMeasure.value = TypeMeasureAB::Frequency;
        PageModesB::typeMeasure.value = TypeMeasureAB::Frequency;

        PageModes::ResetModeCurrentMeasure();
        PageModesC::OnChanged_ModeFrequency();
        break;
    case TypeMeasureC::CountPulse:
        PageModesB::typeMeasure.value = TypeMeasureAB::CountPulse;
        PageModesA::typeMeasure.value = TypeMeasureAB::CountPulse;

        PageModes::ResetModeCurrentMeasure();
        PageModesC::OnChanged_ModeCountPulse();
        break;
    }
}

DEF_SWITCH_2(sTypeMeasureC,
    "Вид изм.", "Выбор измерения",
    "Частота", "Сч. имп.",
    PageModesC::typeMeasure, PageModesC::OnChanged_TypeMeasure
)


void PageModesC::OnChanged_ModeFrequency()
{
    items[1] = &sModeFrequencyC;
    items[2] = &sPeriodTimeLabelsC;

    if ((PageModesC::modeMeasureFrequency == ModeMeasureFrequencyC::CB) || (PageModesC::modeMeasureFrequency == ModeMeasureFrequencyC::CA))
    {
        items[3] = &sNumberPeriodsC;
    }
    else
    {
        items[3] = &sTimeMeasureC;
    }

    items[4] = nullptr;

    PageModesA::RelationOff();
    PageModesA::InterpoleOff();
    PageModesA::DCycleOff();
    FreqMeter::LoadModeMeasureFrequency();
}

// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_3(sModeFrequencyC,
    "Режим изм.", "Измерение частоты",
    "Частота", "f(C )/f(A)", "f(C )/f(B)",
    PageModesC::modeMeasureFrequency, PageModesC::OnChanged_ModeFrequency
);


void PageModesC::OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulseC;

    if (PageModesC::modeMeasureCountPulse == ModeMeasureCountPulseC::CTA)
    {
        items[2] = &sNumberPeriodsC;
    }
    if (PageModesC::modeMeasureCountPulse == ModeMeasureCountPulseC::CTB)
    {
        items[2] = &sNumberPeriodsC;
    }

    items[3] = nullptr;

    PageModesA::RelationOff();
    PageModesA::InterpoleOff();
    PageModesA::DCycleOff();
    FreqMeter::LoadModeMeasureCountPulse();
}

// Выбор режима счёта импульсов
DEF_SWITCH_4(sModeCountPulseC,
    "Режим изм.", "Счёт числа импульсов",
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
    nullptr,
    nullptr,
    nullptr
};

static Page pageModesC(items);

Page *PageModesC::self = &pageModesC;


void PageModesC::ResetModeCurrentMeasure()
{
    switch (typeMeasure)
    {
    case TypeMeasureC::Frequency:  modeMeasureFrequency.value = 0;  break;
    case TypeMeasureC::CountPulse: modeMeasureCountPulse.value = 0; break;
    }
}

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


// Выбор периода меток времени
DEF_SWITCH_6(sPeriodTimeLabelsC,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModes::timeLabels, PeriodTimeLabels::LoadToFPGA
);


// Выбор времени измерения
#ifdef USE_1000s

DEF_SWITCH_7(sTimeMeasureC,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModes::timeMeasure, TimeMeasure::LoadToFPGA
);

#else

DEF_SWITCH_6(sTimeMeasureC,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s",
    PageModes::timeMeasure, TimeMeasure::LoadToFPGA
);

#endif


static void OnPress_NumberPeriodsC()
{
    FreqMeter::LoadNumerPeriodsMeasure();
}

// Выбор числа усредняемых периодов входного сигнала
#ifdef USE_1000K

DEF_SWITCH_7(sNumberPeriodsC,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K", "1000K",
    PageModes::numberPeriods,
    OnPress_NumberPeriodsC
);

#else

DEF_SWITCH_6(sNumberPeriodsC,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K",
    PageModes::numberPeriods,
    OnPress_NumberPeriodsC
);

#endif


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

#include "defines.h"
#include "FreqMeter.h"
#include "Settings.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Menu/MenuItems.h"
#include "Utils/Math.h"

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

static bool relationOn = false;
static bool startStop = false;


static const bool correctTypeMeasure[TypeMeasure::Count] = { true, true, true, true };
TypeMeasure           PageModesA::typeMeasure(TypeMeasure::Frequency, correctTypeMeasure, TypeMeasure::Count);
static const bool correctModeMeasureFrequency[ModeMeasureFrequency::Count] = { true, true, true, true, false, false, false, false, true, true };
ModeMeasureFrequency  PageModesA::modeMeasureFrequency(ModeMeasureFrequency::Frequency, correctModeMeasureFrequency, ModeMeasureFrequency::Count);
ModeMeasurePeriod     PageModesA::modeMeasurePeriod(ModeMeasurePeriod::Period);
ModeMeasureDuration   PageModesA::modeMeasureDuration(ModeMeasureDuration::Ndt);
static const bool correctModeMeasureCountPulse[ModeMeasureCountPulse::Count] = { true, true, false, false, false, false, false, false, true};
ModeMeasureCountPulse PageModesA::modeMeasureCountPulse(ModeMeasureCountPulse::AtC, correctModeMeasureCountPulse, ModeMeasureCountPulse::Count);

PeriodTimeLabels      PageModesA::periodTimeLabels(PeriodTimeLabels::T_8);
NumberPeriods         PageModesA::numberPeriods(NumberPeriods::_1);
TimeMeasure           PageModesA::timeMeasure(TimeMeasure::_1ms);


void PageModesA::RelationOn()
{
    relationOn = true;
}

void PageModesA::RelationOff()
{
    relationOn = false;
}

bool PageModesA::RelationCheck()
{
    return relationOn == true;
}


void PageModesA::PressSetup()
{
    switch (PageModesA::typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        items[1] = &sModeFrequency;
        break;

    case TypeMeasure::Period:
        items[1] = &sModePeriod;
        break;

    case TypeMeasure::Duration:
        items[1] = &sModeDuration;
        break;

    case TypeMeasure::CountPulse:
        items[1] = &sModeCountPulse;
        break;
    }

}


void PageModesA::OnChanged_TypeMeasure()
{
    switch (PageModesA::typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        PageModesB::typeMeasure.value = TypeMeasure::Frequency;
        PageModesC::typeMeasure.value = TypeMeasure::Frequency;

        PageModes::ResetModeCurrentMeasure();
        PageModesA::OnChanged_ModeFrequency();
        break;

    case TypeMeasure::Period:
        PageModesB::typeMeasure.value = TypeMeasure::Period;

        PageModes::ResetModeCurrentMeasure();
        PageModesA::OnChanged_ModePeriod();
        break;

    case TypeMeasure::Duration:
        PageModesB::typeMeasure.value = TypeMeasure::Duration;

        PageModes::ResetModeCurrentMeasure();
        PageModesA::OnChanged_ModeDuration();
        break;

    case TypeMeasure::CountPulse:
        PageModesB::typeMeasure.value = TypeMeasure::CountPulse;
        PageModesC::typeMeasure.value = TypeMeasure::CountPulse;

        PageModes::ResetModeCurrentMeasure();
        PageModesA::OnChanged_ModeCountPulse();
        break;
    }
}


DEF_SWITCH_4(sTypeMeasure,
    "Измерение", "Выбор измерения",
    "Частота", "Период", "Длит.", "Сч. имп.",
    PageModesA::typeMeasure, PageModesA::OnChanged_TypeMeasure
);


static void OnPress_Statistics()
{
    Menu::SetOpenedPage(PageStatistics::self);
}

DEF_BUTTON(bStatistics, "Стат", OnPress_Statistics);


void PageModesA::OnChanged_ModeFrequency()
{
    items[1] = &sModeFrequency;

    if (PageModesA::modeMeasureFrequency.IsFrequency())
    {
        items[2] = &sTimeMeasure;
        items[3] = FreqMeter::modeTest.IsEnabled() ? &sPeriodTimeLabels : nullptr;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if (PageModesA::modeMeasureFrequency.IsRatioAB())
    {
        items[2] = &sNumberPeriods;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (PageModesA::modeMeasureFrequency.IsRatioAC())
    {
        items[2] = &sTimeMeasure;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (PageModesA::modeMeasureFrequency.IsT_1())
    {
        items[2] = &sPeriodTimeLabels;
        items[3] = &sNumberPeriods;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if (PageModesA::modeMeasureFrequency.IsTachometer())
    {
        items[2] = &sPeriodTimeLabels;
        items[3] = nullptr;
        PageModesA::RelationOff();
    }
    else if (PageModesA::modeMeasureFrequency.IsComparator())
    {
        items[2] = &bStatistics;
        items[3] = nullptr;
        PageModesA::RelationOff();
    }

    MathFPGA::Interpolation::Disable();
    MathFPGA::DutyCycle::Disable();

    ModeMeasureFrequency::LoadToFPGA();
}

// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_6(sModeFrequency,
    "Режим", "Измерение частоты",
    "Частота",
    "f=1/T",
    "f(A)/f(B)",
    "f(A)/f(C)",
    "Тахометр",
    "Компаратор",
    PageModesA::modeMeasureFrequency, PageModesA::OnChanged_ModeFrequency
);


void PageModesA::OnChanged_ModePeriod()
{
    items[1] = &sModePeriod;

    if (PageModesA::modeMeasurePeriod.IsPeriod())
    {
        items[2] = &sPeriodTimeLabels;
        items[3] = &sNumberPeriods; 
        items[4] = nullptr;
    }
    else if (PageModesA::modeMeasurePeriod.IsF_1())
    {
        items[2] = &sTimeMeasure;
        items[3] = FreqMeter::modeTest.IsEnabled() ? &sPeriodTimeLabels : nullptr;
        items[4] = nullptr;
    }

    PageModesA::RelationOff();
    MathFPGA::Interpolation::Disable();
    MathFPGA::DutyCycle::Disable();

    ModeMeasurePeriod::LoadToFPGA();
}

// Выбор режима измерения периода
DEF_SWITCH_2(sModePeriod,
    "Режим", "Измерение периода",
    "Период", "T=1/f",
    PageModesA::modeMeasurePeriod, PageModesA::OnChanged_ModePeriod
);


void PageModesA::OnChanged_ModeDuration()
{
    items[1] = &sModeDuration;

    PageModesB::modeMeasureDuration.value = PageModesA::modeMeasureDuration.value;

    switch (PageModesA::modeMeasureDuration.value)
    {
    case ModeMeasureDuration::Ndt_1ns:
        MathFPGA::Interpolation::Enable();
        MathFPGA::DutyCycle::Disable();
        items[2] = nullptr;
        break;

    case ModeMeasureDuration::DutyCycle:
    case ModeMeasureDuration::Phase:
        MathFPGA::DutyCycle::Enable();
        MathFPGA::Interpolation::Disable();
        items[2] = &sPeriodTimeLabels;
        items[3] = nullptr;
        break;

    case ModeMeasureDuration::Ndt:
    case ModeMeasureDuration::Ndt2:
        MathFPGA::Interpolation::Disable();
        MathFPGA::DutyCycle::Disable();
        items[2] = &sPeriodTimeLabels;
        items[3] = nullptr;
        break;
    }

    PageModesA::RelationOff();

    ModeMeasureDuration::LoadToFPGA();
}

// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
DEF_SWITCH_5(sModeDuration,
    "Режим", "Измерение длительности",
    "ndt", "ndt/1нс", "S-S", "D", "Фаза",
    PageModesA::modeMeasureDuration, PageModesA::OnChanged_ModeDuration
);


void PageModesA::OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulse;
    items[2] = nullptr;

    switch (PageModesA::modeMeasureCountPulse.value)
    {
    case ModeMeasureCountPulse::AtC:
        break;

    case ModeMeasureCountPulse::ATB:
        items[2] = &sNumberPeriods;
        items[3] = nullptr;
        break;

    case ModeMeasureCountPulse::StartStop:
        PageModesB::modeMeasureCountPulse.value = ModeMeasureCountPulse::StartStop;
        break;
    }

    PageModesA::RelationOff();
    MathFPGA::Interpolation::Disable();
    MathFPGA::DutyCycle::Disable();

    ModeMeasureCountPulse::LoadToFPGA();
}

// Выбор режима счёта импульсов
DEF_SWITCH_3(sModeCountPulse,
    "Режим", "Счёт числа импульсов",
    "А(tB)", "А(TB)", "Старт/Стоп",
    PageModesA::modeMeasureCountPulse, PageModesA::OnChanged_ModeCountPulse
);


static void OnPress_TimeLabels()
{
    uint8 period = PageModesA::periodTimeLabels.value;

    PageModesB::periodTimeLabels.value = period;
    PageModesC::periodTimeLabels.value = period;

    FreqMeter::LoadPeriodTimeLabels();
}

// Выбор периода меток времени
DEF_SWITCH_6(sPeriodTimeLabels,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModesA::periodTimeLabels, OnPress_TimeLabels
);


static void OnPress_TimeMeasure()
{
    uint8 time = PageModesA::timeMeasure.value;

    PageModesB::timeMeasure.value = time;
    PageModesC::timeMeasure.value = time;
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
DEF_SWITCH_7(sTimeMeasure,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModesA::timeMeasure, OnPress_TimeMeasure
);


static void OnPress_NumberPeriods()
{
    uint8 period = PageModesA::numberPeriods.value;

    PageModesB::numberPeriods.value = period;
    PageModesC::numberPeriods.value = period;

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

    uint8 time = times[PageModesA::numberPeriods.value];

    PageModesA::timeMeasure.value = time;
    PageModesB::timeMeasure.value = time;
    PageModesC::timeMeasure.value = time;

    FreqMeter::LoadNumerPeriodsMeasure();
}

// Выбор числа усредняемых периодов входного сигнала
DEF_SWITCH_7(sNumberPeriods,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K", "1000K",
    PageModesA::numberPeriods,
    OnPress_NumberPeriods
);

bool PageModesA::StartStop()
{
    return startStop;
}

void PageModesA::ToggleStartStop()
{
    if (startStop == false)
    {
        startStop = true;
    }
    else
    {
        startStop = false;
    }
}


static Item *items[7] =
{
    &sTypeMeasure,
    &sModeFrequency,
    &sTimeMeasure,
    nullptr
};


static void OnChanged_ModeTest()
{
    switch(PageModesA::typeMeasure.value)
    {
    case TypeMeasure::Frequency:  PageModesA::OnChanged_ModeFrequency();  break;
    case TypeMeasure::Period:     PageModesA::OnChanged_ModePeriod();     break;
    }

    PageModesA::self->VerifySelectedItem();
}

static void OnEvent(EventType::E event)
{
    if (event == EventType::ModeTestChanged)
    {
        OnChanged_ModeTest();
    }
}


static Page pageModes(items, OnEvent);

Page *PageModesA::self = &pageModes;


void PageModesA::ResetModeCurrentMeasure()
{
    switch (typeMeasure)
    {
    case TypeMeasure::Frequency:  modeMeasureFrequency.value = 0;     break;
    case TypeMeasure::Period:     modeMeasurePeriod.value = 0;        break;
    case TypeMeasure::Duration:   modeMeasureDuration.value = 0;      break;
    case TypeMeasure::CountPulse: modeMeasureCountPulse.value = 0;    break;
    }
}

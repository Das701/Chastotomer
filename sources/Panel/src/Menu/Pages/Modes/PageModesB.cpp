#include "defines.h"
#include "FreqMeter.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Hardware/MathFPGA.h"
#include "Menu/MenuItems.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Modes/PagesModes.h"


using namespace Primitives;


extern Item *items[7];
extern Switch sModeFrequency;       
extern Switch sModePeriod;
extern Switch sModeDuration;
extern Switch sModeCountPulse;
extern Switch sPeriodTimeLabels;
extern Switch sTimeMeasure;
extern Switch sNumberPeriods;


static const bool correctTypeMeasure[TypeMeasure::Count] = { true, true, true, true };
TypeMeasure           PageModesB::typeMeasure(TypeMeasure::Frequency, correctTypeMeasure, TypeMeasure::Count);
static const bool correctModeMeasureFrequency[ModeMeasureFrequency::Count] = { true, true, false, false, true, true, false, false, true, false};
ModeMeasureFrequency  PageModesB::modeMeasureFrequency(ModeMeasureFrequency::Frequency, correctModeMeasureFrequency, ModeMeasureFrequency::Count);
ModeMeasurePeriod     PageModesB::modeMeasurePeriod(ModeMeasurePeriod::Period);
ModeMeasureDuration   PageModesB::modeMeasureDuration(ModeMeasureDuration::Ndt);
static const bool correctModeMeasureCountPulse[ModeMeasureCountPulse::Count] = { false, false, true, true, false, false, false, false, true };
ModeMeasureCountPulse PageModesB::modeMeasureCountPulse(ModeMeasureCountPulse::BtA, correctModeMeasureCountPulse, ModeMeasureCountPulse::Count);


void PageModesB::PressSetup()
{
    switch (PageModesB::typeMeasure.value)
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


void PageModesB::OnChanged_TypeMeasure()
{
    switch (PageModesB::typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        PageModesA::typeMeasure.value = TypeMeasure::Frequency;
        PageModesC::typeMeasure.value = TypeMeasure::Frequency;

        PageModes::ResetModeCurrentMeasure();
        PageModesB::OnChanged_ModeFrequency();
        break;

    case TypeMeasure::Period:
        PageModesA::typeMeasure.value = TypeMeasure::Period;

        PageModes::ResetModeCurrentMeasure();
        PageModesB::OnChanged_ModePeriod();
        break;

    case TypeMeasure::Duration:
        PageModesA::typeMeasure.value = TypeMeasure::Duration;

        PageModes::ResetModeCurrentMeasure();
        PageModesB::OnChanged_ModeDuration();
        break;

    case TypeMeasure::CountPulse:
        PageModesA::typeMeasure.value = TypeMeasure::CountPulse;
        PageModesC::typeMeasure.value = TypeMeasure::CountPulse;

        PageModes::ResetModeCurrentMeasure();
        PageModesB::OnChanged_ModeCountPulse();
        break;
    }
}


DEF_SWITCH_4(sTypeMeasure,
    "Измерение", "Выбор измерения",
    "Частота", "Период", "Длит.", "Сч. имп.",
    PageModesB::typeMeasure, PageModesB::OnChanged_TypeMeasure
)


void PageModesB::OnChanged_ModeFrequency()
{
    items[1] = &sModeFrequency;

    if (PageModesB::modeMeasureFrequency.IsFrequency())
    {
        items[2] = &sTimeMeasure;
        items[3] = FreqMeter::modeTest.IsEnabled() ? &sPeriodTimeLabels : nullptr;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if (PageModesB::modeMeasureFrequency.IsRatioBA())
    {
        items[2] = &sNumberPeriods;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (PageModesB::modeMeasureFrequency.IsRatioBC())
    {
        items[2] = &sTimeMeasure;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (PageModesB::modeMeasureFrequency.IsT_1())
    {
        items[2] = &sPeriodTimeLabels;
        items[3] = &sNumberPeriods;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if(PageModesB::modeMeasureFrequency.IsTachometer())
    {
        items[2] = &sPeriodTimeLabels;   
        items[3] = nullptr;
        PageModesA::RelationOff();
    }

    ModeMeasureFrequency::LoadToFPGA();
}

// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_5(sModeFrequency,
    "Режим", "Измерение частоты",
    "Частота", "f(B)/f(A)", "f(B)/f(C)", "f=1/T", "Тахометр",
    PageModesB::modeMeasureFrequency, PageModesB::OnChanged_ModeFrequency
);


void PageModesB::OnChanged_ModePeriod()
{
    items[1] = &sModePeriod;

    if (PageModesB::modeMeasurePeriod.IsPeriod())
    {
        items[2] = &sPeriodTimeLabels;
        items[3] = &sNumberPeriods;
        items[4] = nullptr;
    }
    else if (PageModesB::modeMeasurePeriod.IsF_1())
    {
        items[2] = &sTimeMeasure;
        items[3] = FreqMeter::modeTest.IsEnabled() ? &sPeriodTimeLabels : nullptr;
        items[4] = nullptr;
    }

    PageModesA::RelationOff();

    ModeMeasurePeriod::LoadToFPGA();
}

// Выбор режима измерения периода
DEF_SWITCH_2(sModePeriod,
    "Режим", "Измерение периода",
    "Период", "T=1/f",
    PageModesB::modeMeasurePeriod, PageModesB::OnChanged_ModePeriod
);


void PageModesB::OnChanged_ModeDuration()
{
    items[1] = &sModeDuration;

    PageModesA::modeMeasureDuration.value = PageModesB::modeMeasureDuration.value;
    
    switch (PageModesB::modeMeasureDuration.value)
    {
    case ModeMeasureDuration::Ndt_1ns:
        items[2] = nullptr;
        break;

    case ModeMeasureDuration::Ndt:
    case ModeMeasureDuration::Ndt2:
    case ModeMeasureDuration::FillFactor:
    case ModeMeasureDuration::Phase:
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
    "ndt", "ndt/1нс", "СтартА-СтопВ", "Коэфф. зап.", "Фаза",
    PageModesB::modeMeasureDuration, PageModesB::OnChanged_ModeDuration
);


void PageModesB::OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulse;
    items[2] = nullptr;

    switch (PageModesB::modeMeasureCountPulse.value)
    {
    case ModeMeasureCountPulse::BtA:
        break;

    case ModeMeasureCountPulse::BTA:
        items[2] = &sNumberPeriods;
        items[3] = nullptr;
        break;

    case ModeMeasureCountPulse::StartStop:
        PageModesA::modeMeasureCountPulse.value = ModeMeasureCountPulse::StartStop;
        break;
    }

    PageModesA::RelationOff();

    ModeMeasureCountPulse::LoadToFPGA();
}

// Выбор режима счёта импульсов
DEF_SWITCH_3(sModeCountPulse,
    "Режим", "Счёт числа импульсов",
    "B(tA)", "B(TA)", "Старт/Стоп",
    PageModesB::modeMeasureCountPulse, PageModesB::OnChanged_ModeCountPulse
);


// Выбор периода меток времени
DEF_SWITCH_6(sPeriodTimeLabels,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModes::timeLabels,  PeriodTimeLabels::LoadToFPGA
);


// Выбор времени измерения
#ifdef USE_1000s

DEF_SWITCH_7(sTimeMeasure,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModes::timeMeasure, TimeMeasure::LoadToFPGA
);

#else

DEF_SWITCH_6(sTimeMeasure,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s",
    PageModes::timeMeasure, TimeMeasure::LoadToFPGA
);

#endif


// Выбор числа усредняемых периодов входного сигнала

#ifdef USE_1000K

DEF_SWITCH_7(sNumberPeriods,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K", "1000K",
    PageModes::numberPeriods, NumberPeriods::LoadToFPGA
);

#else

DEF_SWITCH_6(sNumberPeriods,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K",
    PageModes::numberPeriods, NumberPeriods::LoadToFPGA
);

#endif



static Item *items[7] =
{
    &sTypeMeasure,
    &sModeFrequency,
    &sTimeMeasure,
    nullptr
};


static void OnChanged_ModeTest()
{
    switch (PageModesB::typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        if (PageModesB::modeMeasureFrequency.IsFrequency())
        {
            items[3] = FreqMeter::modeTest.IsEnabled() ? &sPeriodTimeLabels : nullptr;
        }
        break;
    case TypeMeasure::Period:
        items[3] = FreqMeter::modeTest.IsEnabled() ? &sPeriodTimeLabels : nullptr;
        break;
    }

    PageModesB::self->VerifySelectedItem();
}


static void OnEvent(EventType::E event)
{
    switch(event)
    {
    case EventType::ModeTestChanged:
        OnChanged_ModeTest();
        break;

    case EventType::Count:
        break;
    }
}


static Page pageModesB(items, OnEvent);

Page *PageModesB::self = &pageModesB;


void PageModesB::ResetModeCurrentMeasure()
{
    switch (typeMeasure)
    {
    case TypeMeasure::Frequency:  modeMeasureFrequency.value = 0;     break;
    case TypeMeasure::Period:     modeMeasurePeriod.value = 0;        break;
    case TypeMeasure::Duration:   modeMeasureDuration.value = 0;      break;
    case TypeMeasure::CountPulse: modeMeasureCountPulse.value = 0;    break;
    }
}

#include "defines.h"
#include "FreqMeter.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Menu/MenuItems.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageModesD.h"
#include "Menu/Pages/PageModesC.h"
#include "Menu/Pages/PageModesB.h"
#include "Menu/Pages/PageModesA.h"


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


TypeMeasureAB            PageModesB::typeMeasure(TypeMeasureAB::Frequency);
ModeMeasureFrequencyB    PageModesB::modeMeasureFrequency(ModeMeasureFrequencyB::Freq);
ModeMeasurePeriod        PageModesB::modeMeasurePeriod(ModeMeasurePeriod::Period);
ModeMeasureDuration      PageModesB::modeMeasureDuration(ModeMeasureDuration::Ndt);
ModeMeasureCountPulseB   PageModesB::modeMeasureCountPulse(ModeMeasureCountPulseB::BtA);

PeriodTimeLabels         PageModesB::periodTimeLabels(PeriodTimeLabels::T_8);
NumberPeriods            PageModesB::numberPeriods(NumberPeriods::_1);
TimeMeasure              PageModesB::timeMeasure(TimeMeasure::_1ms);


void PageModesB::Init()
{
}


void PageModesB::PressSetupB()
{
    switch (PageModesB::typeMeasure.value)
    {
    case TypeMeasureAB::Frequency:
        items[1] = &sModeFrequency;
        break;

    case TypeMeasureAB::Period:
        items[1] = &sModePeriod;
        break;

    case TypeMeasureAB::Duration:
        items[1] = &sModeDuration;
        break;

    case TypeMeasureAB::CountPulse:
        items[1] = &sModeCountPulse;
        break;
    }
}


void PageModesB::OnChanged_TypeMeasure()
{
    switch (PageModesB::typeMeasure.value)
    {
    case TypeMeasureAB::Frequency:
        PageModesA::typeMeasure.value = TypeMeasureAB::Frequency;
        PageModesC::typeMeasure.value = TypeMeasureC::Frequency;

        PageModes::ResetModeCurrentMeasure();
        PageModesB::OnChanged_ModeFrequency();
        break;

    case TypeMeasureAB::Period:
        PageModesA::typeMeasure.value = TypeMeasureAB::Period;

        PageModes::ResetModeCurrentMeasure();
        PageModesB::OnChanged_ModePeriod();
        break;

    case TypeMeasureAB::Duration:
        PageModesA::typeMeasure.value = TypeMeasureAB::Duration;

        PageModes::ResetModeCurrentMeasure();
        PageModesB::OnChanged_ModeDuration();
        break;

    case TypeMeasureAB::CountPulse:
        PageModesA::typeMeasure.value = TypeMeasureAB::CountPulse;
        PageModesC::typeMeasure.value = TypeMeasureC::CountPulse;

        PageModes::ResetModeCurrentMeasure();
        PageModesB::OnChanged_ModeCountPulse();
        break;
    }
}


DEF_SWITCH_4(sTypeMeasure,
    "Вид изм.", "Выбор измерения",
    "Частота", "Период", "Длит.", "Сч. имп.",
    PageModesB::typeMeasure, PageModesB::OnChanged_TypeMeasure
)


void PageModesB::OnChanged_ModeFrequency()
{
    if (PageModesB::modeMeasureFrequency == ModeMeasureFrequencyB::BC)
    {
        items[1] = &sModeFrequency;
        items[2] = &sTimeMeasure;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (PageModesB::modeMeasureFrequency == ModeMeasureFrequencyB::T_1)
    {
        items[1] = &sModeFrequency;
        items[2] = &sPeriodTimeLabels;
        items[3] = &sNumberPeriods;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if(PageModesB::modeMeasureFrequency == ModeMeasureFrequencyB::BA)
    {
        items[1] = &sModeFrequency;
        items[2] = &sNumberPeriods;   
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if(PageModesB::modeMeasureFrequency == ModeMeasureFrequencyB::Tachometer)
    {
        items[1] = &sModeFrequency;
        items[2] = &sNumberPeriods;   
        items[3] = nullptr;
        PageModesA::RelationOff();
    }
    else
    {
        items[1] = &sModeFrequency;
        items[2] = &sPeriodTimeLabels;
        items[3] = &sTimeMeasure;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }     
    PageModesA::InterpoleOff();
    PageModesA::DCycleOff();     
    FreqMeter::LoadModeMeasureFrequency();
}

// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_5(sModeFrequency,
    "Режим изм.", "Измерение частоты",
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
        items[2] = &sPeriodTimeLabels;
        items[3] = &sTimeMeasure;
        items[4] = nullptr;
    }
    else
    {
        items[2] = &sTimeMeasure;
        items[3] = nullptr;
    }

    PageModesA::RelationOff();
    PageModesA::InterpoleOff();
    PageModesA::DCycleOff();
    FreqMeter::LoadModeMeasurePeriod();

}

// Выбор режима измерения периода
DEF_SWITCH_2(sModePeriod,
    "Режим изм.", "Измерение периода",
    "Период", "T=1/f",
    PageModesB::modeMeasurePeriod, PageModesB::OnChanged_ModePeriod
);


void PageModesB::OnChanged_ModeDuration()
{
    items[1] = &sModeDuration;

    PageModesA::modeMeasureDuration.value = PageModesB::modeMeasureDuration.value;
    
    if (PageModesB::modeMeasureDuration.Is_Ndt_1ns())
    {
        PageModesA::InterpoleOn();
        PageModesA::DCycleOff();
        items[2] = nullptr;
    }
    else if (PageModesB::modeMeasureDuration.Is_Dcycle() || PageModesB::modeMeasureDuration.Is_Phase())
    {
        PageModesA::DCycleOn();
        PageModesA::InterpoleOff();
        items[2] = &sPeriodTimeLabels;
        items[3] = nullptr;
    }
    else
    {
        PageModesA::InterpoleOff();
        PageModesA::DCycleOff();
        items[2] = &sPeriodTimeLabels;
        items[3] = nullptr;
    }
    
    PageModesA::RelationOff();
    FreqMeter::LoadModeMeasureDuration();
}

// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
DEF_SWITCH_5(sModeDuration,
    "Режим изм.", "Измерение длительности",
    "ndt", "ndt/1нс", "S-S", "D", "Фаза",
    PageModesB::modeMeasureDuration, PageModesB::OnChanged_ModeDuration
);


void PageModesB::OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulse;

    if(PageModesB::modeMeasureCountPulse.value == ModeMeasureCountPulseB::StartStop)
    {
        PageModesA::modeMeasureCountPulse.value = ModeMeasureCountPulseA::StartStop;
        items[2] = nullptr;
    }
    if (PageModesB::modeMeasureCountPulse == ModeMeasureCountPulseB::BTA)
    {
        items[2] = &sNumberPeriods;
        items[3] = nullptr;
    }

    PageModesA::RelationOff();
    PageModesA::InterpoleOff();
    PageModesA::DCycleOff();
    FreqMeter::LoadModeMeasureCountPulse();

}

// Выбор режима счёта импульсов
DEF_SWITCH_3(sModeCountPulse,
    "Режим изм.", "Счёт числа импульсов",
    "B(tA)", "B(TA)", "Start/Stop",
    PageModesB::modeMeasureCountPulse, PageModesB::OnChanged_ModeCountPulse
);


static void OnPress_TimeLabelsB()
{
    uint8 period = PageModesB::periodTimeLabels.value;

    PageModesA::periodTimeLabels.value = period;
    PageModesC::periodTimeLabels.value = period;

    FreqMeter::LoadPeriodTimeLabels();
}

// Выбор периода меток времени
DEF_SWITCH_6(sPeriodTimeLabels,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModesB::periodTimeLabels,  OnPress_TimeLabelsB
);


static void OnPress_TimeMeasure()
{
    uint8 time = PageModesB::timeMeasure.value;

    PageModesA::timeMeasure.value = time;
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
    PageModesB::timeMeasure, OnPress_TimeMeasure
);


static void OnPress_NumberPeriodsB()
{
    uint8 period = PageModesB::numberPeriods.value;

    PageModesA::numberPeriods.value = period;
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

    uint8 time = times[PageModesB::numberPeriods.value];

    PageModesA::timeMeasure.value = time;
    PageModesB::timeMeasure.value = time;
    PageModesC::timeMeasure.value = time;

    FreqMeter::LoadNumerPeriodsMeasure();
}

// Выбор числа усредняемых периодов входного сигнала
DEF_SWITCH_7(sNumberPeriods,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K", "1000K",
    PageModesB::numberPeriods,
    OnPress_NumberPeriodsB
);



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

static Page pageModesB(items);

Page *PageModesB::self = &pageModesB;


void PageModesB::ResetModeCurrentMeasure()
{
    switch (typeMeasure)
    {
    case TypeMeasureAB::Frequency:  modeMeasureFrequency.value = 0;     break;
    case TypeMeasureAB::Period:     modeMeasurePeriod.value = 0;        break;
    case TypeMeasureAB::Duration:   modeMeasureDuration.value = 0;      break;
    case TypeMeasureAB::CountPulse: modeMeasureCountPulse.value = 0;    break;
    }
}

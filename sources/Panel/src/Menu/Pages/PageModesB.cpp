#include "defines.h"
#include "Menu/Pages/PageModesD.h"
#include "Menu/Pages/PageModesC.h"
#include "Menu/Pages/PageModesB.h"
#include "Menu/Pages/PageModesA.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Menu/MenuItemsDef.h"
#include "FreqMeter/FreqMeter.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageChannelB.h"
#include "Menu/Pages/PageChannelC.h"
#include "Menu/Pages/PageChannelD.h"

using namespace Display::Primitives;
using namespace Display;

extern Item *items[7];
extern Switch sModeFrequencyB;
extern Switch sModePeriodB;
extern Switch sModeDurationB;
extern Switch sModeCountPulseB;
extern Switch sPeriodTimeLabelsB;
extern Switch sTimeMeasureB;
extern Switch sNumberPeriodsB;


TypeMeasureAB            PageModesB::typeMeasure(TypeMeasureAB::Frequency);
ModeMeasureFrequencyB    PageModesB::modeMeasureFrequency(ModeMeasureFrequencyB::Freq);
ModeMeasurePeriod        PageModesB::modeMeasurePeriod(ModeMeasurePeriod::Period);
ModeMeasureDuration      PageModesB::modeMeasureDuration(ModeMeasureDuration::Ndt);
ModeMeasureCountPulseB   PageModesB::modeMeasureCountPulse(ModeMeasureCountPulseB::BtA);

PeriodTimeLabels         PageModesB::periodTimeLabels(PeriodTimeLabels::T_8);
NumberPeriods            PageModesB::numberPeriods(NumberPeriods::_1);
TimeMeasure              PageModesB::timeMeasure(TimeMeasure::_1ms);

/// Очистить массив указателей на итемы, начиная с i-го итема
static void ClearItems(int i);

static void OnPress_ModeFrequencyB();

static void OnPress_ModePeriodB();

static void OnPress_ModeDurationB();

static void OnPress_ModeCountPulseB();


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageModesB::Init()
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

void PageModesB::PressSetupB()
{
    switch (PageModesB::typeMeasure.value)
    {
        case TypeMeasureAB::Frequency:
            items[1] = &sModeFrequencyB;
            break;
    
        case TypeMeasureAB::Period:
            items[1] = &sModePeriodB;
            break;
    
        case TypeMeasureAB::Duration:
            items[1] = &sModeDurationB;
            break;
    
        case TypeMeasureAB::CountPulse:
            items[1] = &sModeCountPulseB;
            break;
    }    
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TypeMeasureB()
{
        switch (PageModesB::typeMeasure.value)
        {
        case TypeMeasureAB::Frequency:
            PageModesA::typeMeasure.value = TypeMeasureAB::Frequency;
            PageModesC::typeMeasure.value = TypeMeasureC::Frequency;
            OnPress_ModeFrequencyB();
            break;
    
        case TypeMeasureAB::Period:
            PageModesA::typeMeasure.value = TypeMeasureAB::Period;
            OnPress_ModePeriodB();
            break;
    
        case TypeMeasureAB::Duration:
            PageModesA::typeMeasure.value = TypeMeasureAB::Duration;
            OnPress_ModeDurationB();
            break;
    
        case TypeMeasureAB::CountPulse:
            PageModesA::typeMeasure.value = TypeMeasureAB::CountPulse;
            PageModesC::typeMeasure.value = TypeMeasureC::CountPulse;
            OnPress_ModeCountPulseB();
            break;
        }        
}


DEF_SWITCH_4(sTypeMeasureB,
    "Вид изм.", "Выбор измерения",
    "Частота", "Период", "Длит.", "Сч. имп.",
    PageModesB::typeMeasure, OnPress_TypeMeasureB
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeFrequencyB()
{
    ClearItems(2);
    if (PageModesB::modeMeasureFrequency == ModeMeasureFrequencyB::BC)
    {
        items[1] = &sModeFrequencyB;
        items[2] = &sTimeMeasureB;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (PageModesB::modeMeasureFrequency == ModeMeasureFrequencyB::T_1)
    {
        items[2] = &sPeriodTimeLabelsB;   
        items[1] = &sModeFrequencyB;
        items[3] = &sNumberPeriodsB;
        PageModesA::RelationOff();
    }
    else if(PageModesB::modeMeasureFrequency == ModeMeasureFrequencyB::BA)
    {
        items[2] = &sNumberPeriodsB;   
        items[1] = &sModeFrequencyB;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if(PageModesB::modeMeasureFrequency == ModeMeasureFrequencyB::Tachometer)
    {
        items[2] = &sNumberPeriodsB;   
        items[1] = &sModeFrequencyB;
        items[3] = nullptr;
        PageModesA::RelationOff();
    }
    else
    {
        items[2] = &sPeriodTimeLabelsB;   
        items[1] = &sModeFrequencyB;
        items[3] = &sTimeMeasureB;
        PageModesA::RelationOff();
    }     
    PageModesA::InterpoleOff();
    PageModesA::DCycleOff();     
    FreqMeter::LoadModeMeasureFrequency();
}

/// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_5(sModeFrequencyB,
    "Режим изм.", "Измерение частоты",
    "Частота", "f(B)/f(A)", "f(B)/f(C )", "f=1/T", "Тахометр",
    PageModesB::modeMeasureFrequency, OnPress_ModeFrequencyB);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModePeriodB()
{
    ClearItems(2);

    items[1] = &sModePeriodB;

    if (PageModesB::modeMeasurePeriod.IsPeriod())
    {
        items[2] = &sPeriodTimeLabelsB;
        items[3] = &sNumberPeriodsB;
    }
    else if (PageModesB::modeMeasurePeriod.IsF_1())
    {
        items[2] = &sPeriodTimeLabelsB;
        items[3] = &sTimeMeasureB;
    }
    else
    {
        items[2] = &sTimeMeasureB;
    }
    PageModesA::RelationOff();
    PageModesA::InterpoleOff();
    PageModesA::DCycleOff();
    FreqMeter::LoadModeMeasurePeriod();

}

/// Выбор режима измерения периода
DEF_SWITCH_2(sModePeriodB,
    "Режим изм.", "Измерение периода",
    "Период", "T=1/f",
    PageModesB::modeMeasurePeriod, OnPress_ModePeriodB
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeDurationB()
{
    ClearItems(2);

    items[1] = &sModeDurationB;

    PageModesA::modeMeasureDuration.value = PageModesB::modeMeasureDuration.value;
    
    if (PageModesB::modeMeasureDuration.Is_Ndt_1ns())
    {
        PageModesA::InterpoleOn();
        PageModesA::DCycleOff();
    }
    else if (PageModesB::modeMeasureDuration.Is_Dcycle() || PageModesB::modeMeasureDuration.Is_Phase())
    {
        PageModesA::DCycleOn();
        PageModesA::InterpoleOff();
        items[2] = &sPeriodTimeLabelsB;
    }
    else
    {
        items[2] = &sPeriodTimeLabelsB;
        PageModesA::InterpoleOff();
        PageModesA::DCycleOff();
    }
    
    PageModesA::RelationOff();
    FreqMeter::LoadModeMeasureDuration();
}

/// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
DEF_SWITCH_5(sModeDurationB,
    "Режим изм.", "Измерение длительности",
    "ndt", "ndt/1нс", "S-S", "D", "Фаза",
    PageModesB::modeMeasureDuration, OnPress_ModeDurationB
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeCountPulseB()
{
    ClearItems(2);

    items[1] = &sModeCountPulseB;
    if(PageModesB::modeMeasureCountPulse.value == ModeMeasureCountPulseB::StartStop)
    {
        PageModesA::modeMeasureCountPulse.value = ModeMeasureCountPulseA::StartStop;
//        PageModesC::modeMeasureCountPulse.value = ModeMeasureCountPulseC::StartStop;
    }
    if (PageModesB::modeMeasureCountPulse == ModeMeasureCountPulseB::BTA_1)
    {
        items[2] = &sNumberPeriodsB;
    }
    PageModesA::RelationOff();
    PageModesA::InterpoleOff();
    PageModesA::DCycleOff();
    FreqMeter::LoadModeMeasureCountPulse();

}

/// Выбор режима счёта импульсов
DEF_SWITCH_3(sModeCountPulseB,
    "Режим изм.", "Счёт числа импульсов",
    "B(tA)", "B(TA)", "Start/Stop",
    PageModesB::modeMeasureCountPulse, OnPress_ModeCountPulseB
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeLabelsB()
{
    uint8 period = PageModesB::periodTimeLabels.value;

    PageModesA::periodTimeLabels.value = period;
    PageModesC::periodTimeLabels.value = period;

    FreqMeter::LoadPeriodTimeLabels();
}

/// Выбор периода меток времени
DEF_SWITCH_6(sPeriodTimeLabelsB,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModesB::periodTimeLabels,  OnPress_TimeLabelsB
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeMeasureB()
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

    uint8 period = periods[PageModesA::timeMeasure.value];

    PageModesA::numberPeriods.value = period;
    PageModesB::numberPeriods.value = period;
    PageModesC::numberPeriods.value = period;

    FreqMeter::LoadTimeMeasure();
}

/// Выбор времени измерения
DEF_SWITCH_7(sTimeMeasureB,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModesB::timeMeasure, OnPress_TimeMeasureB
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

/// Выбор числа усредняемых периодов входного сигнала
DEF_SWITCH_7(sNumberPeriodsB,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K", "1000K",
    PageModesB::numberPeriods,
    OnPress_NumberPeriodsB
);


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static Item *items[7] =
{
    &sTypeMeasureB,
    &sModeFrequencyB,
    &sPeriodTimeLabelsB,
    &sTimeMeasureB,
    nullptr,
    nullptr,
    nullptr
};

static Page pageModesB(items);

Page *PageModesB::self = &pageModesB;

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


TypeMeasureB             PageModesB::typeMeasure(TypeMeasureB::Frequency);
ModeMeasureFrequencyB    PageModesB::modeMeasureFrequencyB(ModeMeasureFrequencyB::Freq);
ModeMeasurePeriodB       PageModesB::modeMeasurePeriodB(ModeMeasurePeriodB::Period);
ModeMeasureDurationB     PageModesB::modeMeasureDurationB(ModeMeasureDurationB::Ndt);
ModeMeasureCountPulseB   PageModesB::modeMeasureCountPulseB(ModeMeasureCountPulseB::BTA);

PeriodTimeLabelsB        PageModesB::periodTimeLabelsB(PeriodTimeLabelsB::T_8);
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
        case TypeMeasureB::Frequency:
            items[1] = &sModeFrequencyB;
            break;
    
        case TypeMeasureB::Period:
            items[1] = &sModePeriodB;
            break;
    
        case TypeMeasureB::Duration:
            items[1] = &sModeDurationB;
            break;
    
        case TypeMeasureB::CountPulse:
            items[1] = &sModeCountPulseB;
            break;
    }    
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TypeMeasureB()
{
        switch (PageModesB::typeMeasure.value)
        {
        case TypeMeasureB::Frequency:
            PageModesA::typeMeasure.value = TypeMeasureA::Frequency;
            PageModesC::typeMeasureC.value = TypeMeasureC::Frequency;
            OnPress_ModeFrequencyB();
            break;
    
        case TypeMeasureB::Period:
            PageModesA::typeMeasure.value = TypeMeasureA::Period;
            OnPress_ModePeriodB();
            break;
    
        case TypeMeasureB::Duration:
            PageModesA::typeMeasure.value = TypeMeasureA::Duration;
            OnPress_ModeDurationB();
            break;
    
        case TypeMeasureB::CountPulse:
            PageModesA::typeMeasure.value = TypeMeasureA::CountPulse;
            PageModesC::typeMeasureC.value = TypeMeasureC::CountPulse;
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
    if (PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::BC)
    {
        items[1] = &sModeFrequencyB;
        items[2] = &sTimeMeasureB;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::T_1)
    {
        items[2] = &sPeriodTimeLabelsB;   
        items[1] = &sModeFrequencyB;
        items[3] = &sNumberPeriodsB;
        PageModesA::RelationOff();
    }
    else if(PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::BA)
    {
        items[2] = &sNumberPeriodsB;   
        items[1] = &sModeFrequencyB;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if(PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::Tachometer)
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
    PageModesB::modeMeasureFrequencyB, OnPress_ModeFrequencyB);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModePeriodB()
{
    ClearItems(2);

    items[1] = &sModePeriodB;

    if (PageModesB::modeMeasurePeriodB == ModeMeasurePeriodB::Period)
    {
        items[2] = &sPeriodTimeLabelsB;
        items[3] = &sNumberPeriodsB;
    }
    else if (PageModesB::modeMeasurePeriodB == ModeMeasurePeriodB::F_1)
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
    PageModesB::modeMeasurePeriodB, OnPress_ModePeriodB
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeDurationB()
{
    ClearItems(2);

    items[1] = &sModeDurationB;

    switch(PageModesB::modeMeasureDurationB.value)
    {
        case ModeMeasureDurationB::Ndt:
            PageModesA::modeMeasureDuration.value = ModeMeasureDuration::Ndt;
            break;
        case ModeMeasureDurationB::Ndt_1ns:
            PageModesA::modeMeasureDuration.value = ModeMeasureDuration::Ndt_1ns;
            break;
        case ModeMeasureDurationB::Ndt2:
            PageModesA::modeMeasureDuration.value = ModeMeasureDuration::Ndt2;
            break;
        case ModeMeasureDurationB::Dcycle:
            PageModesA::modeMeasureDuration.value = ModeMeasureDuration::Dcycle;
            break;
        case ModeMeasureDurationB::Phase:
            PageModesA::modeMeasureDuration.value = ModeMeasureDuration::Phase;
            break;
    }
    if (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Ndt_1ns)
    {
        PageModesA::InterpoleOn();
        PageModesA::DCycleOff();
    }
    else if ((PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Dcycle) || 
            (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Phase))
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
    PageModesB::modeMeasureDurationB, OnPress_ModeDurationB
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeCountPulseB()
{
    ClearItems(2);

    items[1] = &sModeCountPulseB;
    if(PageModesB::modeMeasureCountPulseB.value == ModeMeasureCountPulseB::StartStop)
    {
        PageModesA::modeMeasureCountPulse.value = ModeMeasureCountPulse::StartStop;
//        PageModesC::modeMeasureCountPulseC.value = ModeMeasureCountPulseC::StartStop;
    }
    if (PageModesB::modeMeasureCountPulseB == ModeMeasureCountPulseB::BTA_1)
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
    PageModesB::modeMeasureCountPulseB, OnPress_ModeCountPulseB
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeLabelsB()
{
    switch (PageModesB::periodTimeLabelsB.value)
    {
        case PeriodTimeLabelsB::T_3:
            PageModesA::periodTimeLabels.value = PeriodTimeLabels::T_3;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_3;
            break;
        case PeriodTimeLabelsB::T_4:
            PageModesA::periodTimeLabels.value = PeriodTimeLabels::T_4;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_4;
            break;
        case PeriodTimeLabelsB::T_5:
            PageModesA::periodTimeLabels.value = PeriodTimeLabels::T_5;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_5;
            break;
        case PeriodTimeLabelsB::T_6:
            PageModesA::periodTimeLabels.value = PeriodTimeLabels::T_6;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_6;
            break;
        case PeriodTimeLabelsB::T_7:
            PageModesA::periodTimeLabels.value = PeriodTimeLabels::T_7;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_7;
            break;
        case PeriodTimeLabelsB::T_8:
            PageModesA::periodTimeLabels.value = PeriodTimeLabels::T_8;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_8;
            break;
    }
    FreqMeter::LoadPeriodTimeLabels();
}

/// Выбор периода меток времени
DEF_SWITCH_6(sPeriodTimeLabelsB,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModesB::periodTimeLabelsB,  OnPress_TimeLabelsB
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
    switch (PageModesB::numberPeriods.value)
    {
        case NumberPeriods::_1:
            PageModesA::numberPeriods.value = NumberPeriods::_1;
            PageModesC::numberPeriods.value = NumberPeriods::_1;
            PageModesB::timeMeasure.value = TimeMeasure::_1ms;
            PageModesC::timeMeasure.value = TimeMeasure::_1ms;
            PageModesA::timeMeasure.value = TimeMeasure::_1ms;
            break;
        case NumberPeriods::_10:
            PageModesA::numberPeriods.value = NumberPeriods::_10;
            PageModesC::numberPeriods.value = NumberPeriods::_10;
            PageModesB::timeMeasure.value = TimeMeasure::_10ms;
            PageModesC::timeMeasure.value = TimeMeasure::_10ms;
            PageModesA::timeMeasure.value = TimeMeasure::_10ms;
            break;
        case NumberPeriods::_100:
            PageModesA::numberPeriods.value = NumberPeriods::_100;
            PageModesC::numberPeriods.value = NumberPeriods::_100;
            PageModesB::timeMeasure.value = TimeMeasure::_100ms;
            PageModesC::timeMeasure.value = TimeMeasure::_100ms;
            PageModesA::timeMeasure.value = TimeMeasure::_100ms;
            break;
        case NumberPeriods::_1K:
            PageModesA::numberPeriods.value = NumberPeriods::_1K;
            PageModesC::numberPeriods.value = NumberPeriods::_1K;
            PageModesB::timeMeasure.value = TimeMeasure::_1s;
            PageModesC::timeMeasure.value = TimeMeasure::_1s;
            PageModesA::timeMeasure.value = TimeMeasure::_1s;
            break;
        case NumberPeriods::_10K:
            PageModesA::numberPeriods.value = NumberPeriods::_10K;
            PageModesC::numberPeriods.value = NumberPeriods::_10K;
            PageModesB::timeMeasure.value = TimeMeasure::_10s;
            PageModesC::timeMeasure.value = TimeMeasure::_10s;
            PageModesA::timeMeasure.value = TimeMeasure::_10s;
            break;
        case NumberPeriods::_100K:
            PageModesA::numberPeriods.value = NumberPeriods::_100K;
            PageModesC::numberPeriods.value = NumberPeriods::_100K;
            PageModesB::timeMeasure.value = TimeMeasure::_100s;
            PageModesC::timeMeasure.value = TimeMeasure::_100s;
            PageModesA::timeMeasure.value = TimeMeasure::_100s;
            break;
        case NumberPeriods::_1000K:
            PageModesA::numberPeriods.value = NumberPeriods::_1000K;
            PageModesC::numberPeriods.value = NumberPeriods::_1000K;
            PageModesB::timeMeasure.value = TimeMeasure::_1000s;
            PageModesC::timeMeasure.value = TimeMeasure::_1000s;
            PageModesA::timeMeasure.value = TimeMeasure::_1000s;
            break;
    }
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

#include "defines.h"
#include "Menu/Pages/PageModesA.h"
#include "Menu/Pages/PageModesB.h"
#include "Menu/Pages/PageModesC.h"
#include "Menu/Pages/PageModesD.h"
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

static bool interpoleOn = false;
static bool dCycleOn = false;
static bool relationOn = false;
static bool startStop = false;

TypeMeasureA             PageModesA::typeMeasure(TypeMeasureA::Frequency);
ModeMeasureFrequency    PageModesA::modeMeasureFrequency(ModeMeasureFrequency::Freq);
ModeMeasurePeriod       PageModesA::modeMeasurePeriod(ModeMeasurePeriod::Period);
ModeMeasureDuration     PageModesA::modeMeasureDuration(ModeMeasureDuration::Ndt);
ModeMeasureCountPulse   PageModesA::modeMeasureCountPulse(ModeMeasureCountPulse::ATC);

PeriodTimeLabels        PageModesA::periodTimeLabels(PeriodTimeLabels::T_8);
NumberPeriods           PageModesA::numberPeriods(NumberPeriods::_1);
TimeMeasure             PageModesA::timeMeasure(TimeMeasure::_1ms);

/// Очистить массив указателей на итемы, начиная с i-го итема
static void ClearItems(int i);

static void OnPress_ModeFrequency();

static void OnPress_ModePeriod();

static void OnPress_ModeDuration();

static void OnPress_ModeCountPulse();

void PageModesA::InterpoleOn()
{
    interpoleOn = true;
}

void PageModesA::InterpoleOff()
{
    interpoleOn = false;
}

bool PageModesA::InterpoleCheck()
{
    return interpoleOn == true;
}

void PageModesA::DCycleOn()
{
    dCycleOn = true;
}

void PageModesA::DCycleOff()
{
    dCycleOn = false;
}

bool PageModesA::DCycleCheck()
{
    return dCycleOn == true;
}

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



//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageModesA::Init()
{
//    FreqMeter::LoadModeMeasureFrequency();
//    FreqMeter::LoadPeriodTimeLabels();
//    FreqMeter::LoadTimeMeasure();
}

void PageModesA::PressSetup()
{
    switch (PageModesA::typeMeasure.value)
    {
        case TypeMeasureA::Frequency:
            items[1] = &sModeFrequency;
            break;
    
        case TypeMeasureA::Period:
            items[1] = &sModePeriod;
            break;
    
        case TypeMeasureA::Duration:
            items[1] = &sModeDuration;
            break;
    
        case TypeMeasureA::CountPulse:
            items[1] = &sModeCountPulse;
            break;
    }  
    
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
        switch (PageModesA::typeMeasure.value)
        {
        case TypeMeasureA::Frequency:
            PageModesB::typeMeasure.value = TypeMeasureB::Frequency;
            PageModesC::typeMeasureC.value = TypeMeasureC::Frequency;
            OnPress_ModeFrequency();
            break;
    
        case TypeMeasureA::Period:
            PageModesB::typeMeasure.value = TypeMeasureB::Period;
            OnPress_ModePeriod();
            break;
    
        case TypeMeasureA::Duration:
            PageModesB::typeMeasure.value = TypeMeasureB::Duration;
            OnPress_ModeDuration();
            break;
    
        case TypeMeasureA::CountPulse:
            PageModesB::typeMeasure.value = TypeMeasureB::CountPulse;
            PageModesC::typeMeasureC.value = TypeMeasureC::CountPulse;
            OnPress_ModeCountPulse();
            break;
        }        
}


DEF_SWITCH_4(sTypeMeasure,
    "Вид изм.", "Выбор измерения",
    "Частота", "Период", "Длит.", "Сч. имп.",
    PageModesA::typeMeasure, OnPress_TypeMeasure
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeFrequency()
{
    ClearItems(2);  
    if (PageModesA::modeMeasureFrequency == ModeMeasureFrequency::AC)
    {
        items[1] = &sModeFrequency;
        items[2] = &sTimeMeasure;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if(PageModesA::modeMeasureFrequency == ModeMeasureFrequency::T_1)
    {
        items[2] = &sPeriodTimeLabels;   
        items[1] = &sModeFrequency;
        items[3] = &sNumberPeriods;
        PageModesA::RelationOff();
    }
    else if(PageModesA::modeMeasureFrequency == ModeMeasureFrequency::Tachometer)
    {
        items[2] = &sPeriodTimeLabels;   
        items[1] = &sModeFrequency;
        items[3] = nullptr;
        PageModesA::RelationOff();
    }
    else if(PageModesA::modeMeasureFrequency == ModeMeasureFrequency::AB)
    {
        items[2] = &sNumberPeriods;   
        items[1] = &sModeFrequency;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if(PageModesA::modeMeasureFrequency == ModeMeasureFrequency::Comparator)
    {
        items[2] = nullptr;   
        items[1] = &sModeFrequency;
        items[3] = nullptr;
        PageModesA::RelationOff();
    }
    else
    {
        items[2] = &sPeriodTimeLabels;   
        items[1] = &sModeFrequency;
        items[3] = &sTimeMeasure;
        PageModesA::RelationOff();
    }
    PageModesA::InterpoleOff();
    PageModesA::DCycleOff();   
    FreqMeter::LoadModeMeasureFrequency();

}

/// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_6(sModeFrequency,
    "Режим изм.", "Измерение частоты",
    "Частота", "f(A)/f(B)", "f(A)/f(C )", "f=1/T", "Тахометр", "Компаратор",
    PageModesA::modeMeasureFrequency, OnPress_ModeFrequency);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModePeriod()
{
    ClearItems(2);

    items[1] = &sModePeriod;

    if (PageModesA::modeMeasurePeriod == ModeMeasurePeriod::Period)
    {
        items[2] = &sPeriodTimeLabels;
        items[3] = &sNumberPeriods;
    }
    else if (PageModesA::modeMeasurePeriod == ModeMeasurePeriod::F_1)
    {
        items[2] = &sPeriodTimeLabels;
        items[3] = &sTimeMeasure;
    }
    else
    {
        items[2] = &sTimeMeasure;
    }
    PageModesA::RelationOff();
    PageModesA::InterpoleOff();
    PageModesA::DCycleOff();
    FreqMeter::LoadModeMeasurePeriod();

}

/// Выбор режима измерения периода
DEF_SWITCH_2(sModePeriod,
    "Режим изм.", "Измерение периода",
    "Период", "T=1/f",
    PageModesA::modeMeasurePeriod, OnPress_ModePeriod
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeDuration()
{
    ClearItems(2);

    items[1] = &sModeDuration;
    switch(PageModesA::modeMeasureDuration.value)
    {
        case ModeMeasureDuration::Ndt:
            PageModesB::modeMeasureDurationB.value = ModeMeasureDurationB::Ndt;
            break;
        case ModeMeasureDuration::Ndt_1ns:
            PageModesB::modeMeasureDurationB.value = ModeMeasureDurationB::Ndt_1ns;
            break;
        case ModeMeasureDuration::Ndt2:
            PageModesB::modeMeasureDurationB.value = ModeMeasureDurationB::Ndt2;
            break;
        case ModeMeasureDuration::Dcycle:
            PageModesB::modeMeasureDurationB.value = ModeMeasureDurationB::Dcycle;
            break;
        case ModeMeasureDuration::Phase:
            PageModesB::modeMeasureDurationB.value = ModeMeasureDurationB::Phase;
            break;
    }
    if (PageModesA::modeMeasureDuration == ModeMeasureDuration::Ndt_1ns)
    {
        PageModesA::InterpoleOn();
        
        PageModesA::DCycleOff();
    }
    else if ((PageModesA::modeMeasureDuration == ModeMeasureDuration::Dcycle) || 
            (PageModesA::modeMeasureDuration == ModeMeasureDuration::Phase))
    {
        PageModesA::DCycleOn();
        PageModesA::InterpoleOff();
        items[2] = &sPeriodTimeLabels;
    }
    else
    {
        items[2] = &sPeriodTimeLabels;
        PageModesA::InterpoleOff();
        PageModesA::DCycleOff();
    }
    
    PageModesA::RelationOff();
    FreqMeter::LoadModeMeasureDuration();
}

/// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
DEF_SWITCH_5(sModeDuration,
    "Режим изм.", "Измерение длительности",
    "ndt", "ndt/1нс", "S-S", "D", "Фаза",
    PageModesA::modeMeasureDuration, OnPress_ModeDuration
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeCountPulse()
{
    ClearItems(2);

    items[1] = &sModeCountPulse;
    if(PageModesA::modeMeasureCountPulse.value == ModeMeasureCountPulse::StartStop)
    {
        PageModesB::modeMeasureCountPulseB.value = ModeMeasureCountPulseB::StartStop;
//        PageModesC::modeMeasureCountPulseC.value = ModeMeasureCountPulseC::StartStop;
    }
    if (PageModesA::modeMeasureCountPulse == ModeMeasureCountPulse::ATB)
    {
        items[2] = &sNumberPeriods;
    }
    PageModesA::RelationOff();
    PageModesA::InterpoleOff();
    PageModesA::DCycleOff();
    FreqMeter::LoadModeMeasureCountPulse();

}

/// Выбор режима счёта импульсов
DEF_SWITCH_3(sModeCountPulse,
    "Режим изм.", "Счёт числа импульсов",
    "А(tB)", "А(TB)", "Start/Stop",
    PageModesA::modeMeasureCountPulse, OnPress_ModeCountPulse
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeLabels()
{
    switch (PageModesA::periodTimeLabels.value)
    {
        case PeriodTimeLabels::T_3:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_3;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_3;
            break;
        case PeriodTimeLabels::T_4:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_4;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_4;
            break;
        case PeriodTimeLabels::T_5:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_5;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_5;
            break;
        case PeriodTimeLabels::T_6:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_6;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_6;
            break;
        case PeriodTimeLabels::T_7:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_7;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_7;
            break;
        case PeriodTimeLabels::T_8:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_8;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_8;
            break;
    }
    FreqMeter::LoadPeriodTimeLabels();
}

/// Выбор периода меток времени
DEF_SWITCH_6(sPeriodTimeLabels,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModesA::periodTimeLabels,  OnPress_TimeLabels
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeMeasure()
{
    switch (PageModesA::timeMeasure.value)
    {
        case TimeMeasure::_1ms:
            PageModesB::timeMeasure.value = TimeMeasure::_1ms;
            PageModesC::timeMeasure.value = TimeMeasure::_1ms;
            PageModesD::timeMeasure.value = TimeMeasure::_1ms;
            PageModesB::numberPeriods.value = NumberPeriods::_1;
            PageModesC::numberPeriods.value = NumberPeriods::_1;
            PageModesA::numberPeriods.value = NumberPeriods::_1;
            break;
        case TimeMeasure::_10ms:
            PageModesB::timeMeasure.value = TimeMeasure::_10ms;
            PageModesC::timeMeasure.value = TimeMeasure::_10ms;
            PageModesD::timeMeasure.value = TimeMeasure::_10ms;
            PageModesB::numberPeriods.value = NumberPeriods::_10;
            PageModesC::numberPeriods.value = NumberPeriods::_10;
            PageModesA::numberPeriods.value = NumberPeriods::_10;
            break;
        case TimeMeasure::_100ms:
            PageModesB::timeMeasure.value = TimeMeasure::_100ms;
            PageModesC::timeMeasure.value = TimeMeasure::_100ms;
            PageModesD::timeMeasure.value = TimeMeasure::_100ms;
            PageModesB::numberPeriods.value = NumberPeriods::_100;
            PageModesC::numberPeriods.value = NumberPeriods::_100;
            PageModesA::numberPeriods.value = NumberPeriods::_100;
            break;
        case TimeMeasure::_1s:
            PageModesB::timeMeasure.value = TimeMeasure::_1s;
            PageModesC::timeMeasure.value = TimeMeasure::_1s;
            PageModesD::timeMeasure.value = TimeMeasure::_1s;
            PageModesB::numberPeriods.value = NumberPeriods::_1K;
            PageModesC::numberPeriods.value = NumberPeriods::_1K;
            PageModesA::numberPeriods.value = NumberPeriods::_1K;
            break;
        case TimeMeasure::_10s:
            PageModesB::timeMeasure.value = TimeMeasure::_10s;
            PageModesC::timeMeasure.value = TimeMeasure::_10s;
            PageModesD::timeMeasure.value = TimeMeasure::_10s;
            PageModesB::numberPeriods.value = NumberPeriods::_10K;
            PageModesC::numberPeriods.value = NumberPeriods::_10K;
            PageModesA::numberPeriods.value = NumberPeriods::_10K;
            break;
        case TimeMeasure::_100s:
            PageModesB::timeMeasure.value = TimeMeasure::_100s;
            PageModesC::timeMeasure.value = TimeMeasure::_100s;
            PageModesD::timeMeasure.value = TimeMeasure::_100s;
            PageModesA::numberPeriods.value = NumberPeriods::_100K;
            PageModesB::numberPeriods.value = NumberPeriods::_100K;
            PageModesC::numberPeriods.value = NumberPeriods::_100K;
            break;
        case TimeMeasure::_1000s:
            PageModesB::timeMeasure.value = TimeMeasure::_1000s;
            PageModesC::timeMeasure.value = TimeMeasure::_1000s;
            PageModesD::timeMeasure.value = TimeMeasure::_1000s;
            PageModesA::numberPeriods.value = NumberPeriods::_1000K;
            PageModesB::numberPeriods.value = NumberPeriods::_1000K;
            PageModesC::numberPeriods.value = NumberPeriods::_1000K;
            break;
    }
    FreqMeter::LoadTimeMeasure();
}

/// Выбор времени измерения
DEF_SWITCH_7(sTimeMeasure,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModesA::timeMeasure, OnPress_TimeMeasure
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_NumberPeriods()
{
    switch (PageModesA::numberPeriods.value)
    {
        case NumberPeriods::_1:
            PageModesB::numberPeriods.value = NumberPeriods::_1;
            PageModesC::numberPeriods.value = NumberPeriods::_1;
            PageModesB::timeMeasure.value = TimeMeasure::_1ms;
            PageModesC::timeMeasure.value = TimeMeasure::_1ms;
            PageModesA::timeMeasure.value = TimeMeasure::_1ms;
            break;
        case NumberPeriods::_10:
            PageModesB::numberPeriods.value = NumberPeriods::_10;
            PageModesC::numberPeriods.value = NumberPeriods::_10;
            PageModesB::timeMeasure.value = TimeMeasure::_10ms;
            PageModesC::timeMeasure.value = TimeMeasure::_10ms;
            PageModesA::timeMeasure.value = TimeMeasure::_10ms;
            break;
        case NumberPeriods::_100:
            PageModesB::numberPeriods.value = NumberPeriods::_100;
            PageModesC::numberPeriods.value = NumberPeriods::_100;
            PageModesB::timeMeasure.value = TimeMeasure::_100ms;
            PageModesC::timeMeasure.value = TimeMeasure::_100ms;
            PageModesA::timeMeasure.value = TimeMeasure::_100ms;
            break;
        case NumberPeriods::_1K:
            PageModesB::numberPeriods.value = NumberPeriods::_1K;
            PageModesC::numberPeriods.value = NumberPeriods::_1K;
            PageModesB::timeMeasure.value = TimeMeasure::_1s;
            PageModesC::timeMeasure.value = TimeMeasure::_1s;
            PageModesA::timeMeasure.value = TimeMeasure::_1s;
            break;
        case NumberPeriods::_10K:
            PageModesB::numberPeriods.value = NumberPeriods::_10K;
            PageModesC::numberPeriods.value = NumberPeriods::_10K;
            PageModesB::timeMeasure.value = TimeMeasure::_10s;
            PageModesC::timeMeasure.value = TimeMeasure::_10s;
            PageModesA::timeMeasure.value = TimeMeasure::_10s;
            break;
        case NumberPeriods::_100K:
            PageModesB::numberPeriods.value = NumberPeriods::_100K;
            PageModesC::numberPeriods.value = NumberPeriods::_100K;
            PageModesB::timeMeasure.value = TimeMeasure::_100s;
            PageModesC::timeMeasure.value = TimeMeasure::_100s;
            PageModesA::timeMeasure.value = TimeMeasure::_100s;
            break;
        case NumberPeriods::_1000K:
            PageModesB::numberPeriods.value = NumberPeriods::_1000K;
            PageModesC::numberPeriods.value = NumberPeriods::_1000K;
            PageModesB::timeMeasure.value = TimeMeasure::_1000s;
            PageModesC::timeMeasure.value = TimeMeasure::_1000s;
            PageModesA::timeMeasure.value = TimeMeasure::_1000s;
            break;
    }
    FreqMeter::LoadNumerPeriodsMeasure();
}

/// Выбор числа усредняемых периодов входного сигнала
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
    if(startStop == false)
    {
        startStop = true;
    }
    else
    {
        startStop = false;
    }
}


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

Page *PageModesA::self = &pageModes;


int NumberPeriods::ToAbs() const
{
    static const int abs[Count] =
    {
        1, 10, 100, 1000, 10000, 100000, 1000000
    };
    
    return abs[value];
}

#include "defines.h"
#include "PageModes.h"
#include "PageModesB.h"
#include "PageModesC.h"
#include "PageModesD.h"
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

TypeMeasure             PageModes::typeMeasure(TypeMeasure::Frequency);
ModeMeasureFrequency    PageModes::modeMeasureFrequency(ModeMeasureFrequency::Freq);
ModeMeasurePeriod       PageModes::modeMeasurePeriod(ModeMeasurePeriod::Period);
ModeMeasureDuration     PageModes::modeMeasureDuration(ModeMeasureDuration::Ndt);
ModeMeasureCountPulse   PageModes::modeMeasureCountPulse(ModeMeasureCountPulse::ATC);

PeriodTimeLabels        PageModes::periodTimeLabels(PeriodTimeLabels::T_8);
NumberPeriods           PageModes::numberPeriods(NumberPeriods::_1);
TimeMeasure             PageModes::timeMeasure(TimeMeasure::_1ms);

/// Очистить массив указателей на итемы, начиная с i-го итема
static void ClearItems(int i);

static void OnPress_ModeFrequency();

static void OnPress_ModePeriod();

static void OnPress_ModeDuration();

static void OnPress_ModeCountPulse();

void PageModes::InterpoleOn()
{
    interpoleOn = true;
}

void PageModes::InterpoleOff()
{
    interpoleOn = false;
}

bool PageModes::InterpoleCheck()
{
    return interpoleOn == true;
}

void PageModes::DCycleOn()
{
    dCycleOn = true;
}

void PageModes::DCycleOff()
{
    dCycleOn = false;
}

bool PageModes::DCycleCheck()
{
    return dCycleOn == true;
}

void PageModes::RelationOn()
{
    relationOn = true;
}

void PageModes::RelationOff()
{
    relationOn = false;
}

bool PageModes::RelationCheck()
{
    return relationOn == true;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageModes::Init()
{
//    FreqMeter::LoadModeMeasureFrequency();
//    FreqMeter::LoadPeriodTimeLabels();
//    FreqMeter::LoadTimeMeasure();
}

void PageModes::PressSetup()
{
    switch (PageModes::typeMeasure.value)
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
        switch (PageModes::typeMeasure.value)
        {
        case TypeMeasure::Frequency:
            PageModesB::typeMeasureB.value = TypeMeasureB::Frequency;
            OnPress_ModeFrequency();
            break;
    
        case TypeMeasure::Period:
            PageModesB::typeMeasureB.value = TypeMeasureB::Period;
            OnPress_ModePeriod();
            break;
    
        case TypeMeasure::Duration:
            PageModesB::typeMeasureB.value = TypeMeasureB::Duration;
            OnPress_ModeDuration();
            break;
    
        case TypeMeasure::CountPulse:
            PageModesB::typeMeasureB.value = TypeMeasureB::CountPulse;
            OnPress_ModeCountPulse();
            break;
        }        
}


DEF_SWITCH_4(sTypeMeasure,
    "Вид изм.", "Выбор измерения",
    "Частота", "Период", "Длит.", "Сч. имп.",
    PageModes::typeMeasure, OnPress_TypeMeasure
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeFrequency()
{
    ClearItems(2);  
    if (PageModes::modeMeasureFrequency == ModeMeasureFrequency::AC)
    {
        items[1] = &sModeFrequency;
        items[2] = &sTimeMeasure;
        items[3] = nullptr;
        PageModes::RelationOn();
    }
    else if(PageModes::modeMeasureFrequency == ModeMeasureFrequency::T_1)
    {
        items[2] = &sPeriodTimeLabels;   
        items[1] = &sModeFrequency;
        items[3] = &sNumberPeriods;
        PageModes::RelationOff();
    }
    else if(PageModes::modeMeasureFrequency == ModeMeasureFrequency::Tachometer)
    {
        items[2] = &sPeriodTimeLabels;   
        items[1] = &sModeFrequency;
        items[3] = nullptr;
        PageModes::RelationOff();
    }
    else if(PageModes::modeMeasureFrequency == ModeMeasureFrequency::AB)
    {
        items[2] = &sNumberPeriods;   
        items[1] = &sModeFrequency;
        items[3] = nullptr;
        PageModes::RelationOn();
    }
    else
    {
        items[2] = &sPeriodTimeLabels;   
        items[1] = &sModeFrequency;
        items[3] = &sTimeMeasure;
        PageModes::RelationOff();
    }
    PageModes::InterpoleOff();
    PageModes::DCycleOff();   
    FreqMeter::LoadModeMeasureFrequency();

}

/// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_5(sModeFrequency,
    "Режим изм.", "Измерение частоты",
    "Частота", "f(A)/f(B)", "f(A)/f(C )", "f=1/T", "Тахометр",
    PageModes::modeMeasureFrequency, OnPress_ModeFrequency);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModePeriod()
{
    ClearItems(2);

    items[1] = &sModePeriod;

    if (PageModes::modeMeasurePeriod == ModeMeasurePeriod::Period)
    {
        items[2] = &sPeriodTimeLabels;
        items[3] = &sNumberPeriods;
    }
    else if (PageModes::modeMeasurePeriod == ModeMeasurePeriod::F_1)
    {
        items[2] = &sPeriodTimeLabels;
        items[3] = &sTimeMeasure;
    }
    else
    {
        items[2] = &sTimeMeasure;
    }
    PageModes::RelationOff();
    PageModes::InterpoleOff();
    PageModes::DCycleOff();
    FreqMeter::LoadModeMeasurePeriod();

}

/// Выбор режима измерения периода
DEF_SWITCH_2(sModePeriod,
    "Режим изм.", "Измерение периода",
    "Период", "T=1/f",
    PageModes::modeMeasurePeriod, OnPress_ModePeriod
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeDuration()
{
    ClearItems(2);

    items[1] = &sModeDuration;

    if (PageModes::modeMeasureDuration == ModeMeasureDuration::Ndt_1ns)
    {
        PageModes::InterpoleOn();
        
        PageModes::DCycleOff();
    }
    else if ((PageModes::modeMeasureDuration == ModeMeasureDuration::Dcycle) || 
            (PageModes::modeMeasureDuration == ModeMeasureDuration::Phase))
    {
        PageModes::DCycleOn();
        PageModes::InterpoleOff();
        items[2] = &sPeriodTimeLabels;
    }
    else
    {
        items[2] = &sPeriodTimeLabels;
        PageModes::InterpoleOff();
        PageModes::DCycleOff();
    }
    
    PageModes::RelationOff();
    FreqMeter::LoadModeMeasureDuration();
}

/// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
DEF_SWITCH_5(sModeDuration,
    "Режим изм.", "Измерение длительности",
    "ndt", "ndt/1нс", "ndt2", "S", "Фаза",
    PageModes::modeMeasureDuration, OnPress_ModeDuration
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeCountPulse()
{
    ClearItems(2);

    items[1] = &sModeCountPulse;

    if (PageModes::modeMeasureCountPulse == ModeMeasureCountPulse::ATB)
    {
        items[2] = &sNumberPeriods;
    }
    PageModes::RelationOff();
    PageModes::InterpoleOff();
    PageModes::DCycleOff();
    FreqMeter::LoadModeMeasureCountPulse();

}

/// Выбор режима счёта импульсов
DEF_SWITCH_2(sModeCountPulse,
    "Режим изм.", "Счёт числа импульсов",
    "А(tB)", "А(TB)",
    PageModes::modeMeasureCountPulse, OnPress_ModeCountPulse
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeLabels()
{
    switch (PageModes::periodTimeLabels.value)
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
    PageModes::periodTimeLabels,  OnPress_TimeLabels
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeMeasure()
{
    switch (PageModes::timeMeasure.value)
    {
        case TimeMeasure::_1ms:
            PageModesB::timeMeasureB.value = TimeMeasureB::_1ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1ms;
            PageModesD::timeMeasureD.value = TimeMeasureD::_1ms;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_1;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_1;
            PageModes::numberPeriods.value = NumberPeriods::_1;
            break;
        case TimeMeasure::_10ms:
            PageModesB::timeMeasureB.value = TimeMeasureB::_10ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_10ms;
            PageModesD::timeMeasureD.value = TimeMeasureD::_10ms;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_10;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_10;
            PageModes::numberPeriods.value = NumberPeriods::_10;
            break;
        case TimeMeasure::_100ms:
            PageModesB::timeMeasureB.value = TimeMeasureB::_100ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_100ms;
            PageModesD::timeMeasureD.value = TimeMeasureD::_100ms;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_100;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_100;
            PageModes::numberPeriods.value = NumberPeriods::_100;
            break;
        case TimeMeasure::_1s:
            PageModesB::timeMeasureB.value = TimeMeasureB::_1s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_1s;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_1K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_1K;
            PageModes::numberPeriods.value = NumberPeriods::_1K;
            break;
        case TimeMeasure::_10s:
            PageModesB::timeMeasureB.value = TimeMeasureB::_10s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_10s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_10s;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_10K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_10K;
            PageModes::numberPeriods.value = NumberPeriods::_10K;
            break;
        case TimeMeasure::_100s:
            PageModesB::timeMeasureB.value = TimeMeasureB::_100s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_100s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_100s;
            PageModes::numberPeriods.value = NumberPeriods::_100K;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_100K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_100K;
            break;
    }
    FreqMeter::LoadTimeMeasure();
}

/// Выбор времени измерения
DEF_SWITCH_6(sTimeMeasure,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s",
    PageModes::timeMeasure, OnPress_TimeMeasure
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_NumberPeriods()
{
    switch (PageModes::numberPeriods.value)
    {
        case NumberPeriods::_1:
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_1;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_1;
            PageModesB::timeMeasureB.value = TimeMeasureB::_1ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1ms;
            PageModes::timeMeasure.value = TimeMeasure::_1ms;
            break;
        case NumberPeriods::_10:
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_10;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_10;
            PageModesB::timeMeasureB.value = TimeMeasureB::_10ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_10ms;
            PageModes::timeMeasure.value = TimeMeasure::_10ms;
            break;
        case NumberPeriods::_100:
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_100;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_100;
            PageModesB::timeMeasureB.value = TimeMeasureB::_100ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_100ms;
            PageModes::timeMeasure.value = TimeMeasure::_100ms;
            break;
        case NumberPeriods::_1K:
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_1K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_1K;
            PageModesB::timeMeasureB.value = TimeMeasureB::_1s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1s;
            PageModes::timeMeasure.value = TimeMeasure::_1s;
            break;
        case NumberPeriods::_10K:
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_10K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_10K;
            PageModesB::timeMeasureB.value = TimeMeasureB::_10s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_10s;
            PageModes::timeMeasure.value = TimeMeasure::_10s;
            break;
        case NumberPeriods::_100K:
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_100K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_100K;
            PageModesB::timeMeasureB.value = TimeMeasureB::_100s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_100s;
            PageModes::timeMeasure.value = TimeMeasure::_100s;
            break;
    }
    FreqMeter::LoadNumerPeriodsMeasure();
}

/// Выбор числа усредняемых периодов входного сигнала
DEF_SWITCH_6(sNumberPeriods,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K",
    PageModes::numberPeriods,
    OnPress_NumberPeriods
);


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

Page *PageModes::self = &pageModes;

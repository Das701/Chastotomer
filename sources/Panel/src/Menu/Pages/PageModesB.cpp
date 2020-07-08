#include "defines.h"
#include "PageModesD.h"
#include "PageModesC.h"
#include "PageModesB.h"
#include "PageModes.h"
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


TypeMeasureB             PageModesB::typeMeasureB(TypeMeasureB::Frequency);
ModeMeasureFrequencyB    PageModesB::modeMeasureFrequencyB(ModeMeasureFrequencyB::Freq);
ModeMeasurePeriodB       PageModesB::modeMeasurePeriodB(ModeMeasurePeriodB::Period);
ModeMeasureDurationB     PageModesB::modeMeasureDurationB(ModeMeasureDurationB::Ndt);
ModeMeasureCountPulseB   PageModesB::modeMeasureCountPulseB(ModeMeasureCountPulseB::BTA);

PeriodTimeLabelsB        PageModesB::periodTimeLabelsB(PeriodTimeLabelsB::T_8);
NumberPeriodsB           PageModesB::numberPeriodsB(NumberPeriodsB::_1);
TimeMeasureB             PageModesB::timeMeasureB(TimeMeasureB::_1ms);

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
    switch (PageModesB::typeMeasureB.value)
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
        switch (PageModesB::typeMeasureB.value)
        {
        case TypeMeasureB::Frequency:
            PageModes::typeMeasure.value = TypeMeasure::Frequency;
            OnPress_ModeFrequencyB();
            break;
    
        case TypeMeasureB::Period:
            PageModes::typeMeasure.value = TypeMeasure::Period;
            OnPress_ModePeriodB();
            break;
    
        case TypeMeasureB::Duration:
            PageModes::typeMeasure.value = TypeMeasure::Duration;
            OnPress_ModeDurationB();
            break;
    
        case TypeMeasureB::CountPulse:
            PageModes::typeMeasure.value = TypeMeasure::CountPulse;
            OnPress_ModeCountPulseB();
            break;
        }        
}


DEF_SWITCH_4(sTypeMeasureB,
    "Вид изм.", "Выбор измерения",
    "Частота", "Период", "Длит.", "Сч. имп.",
    PageModesB::typeMeasureB, OnPress_TypeMeasureB
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
        PageModes::RelationOn();
    }
    else if (PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::T_1)
    {
        items[2] = &sPeriodTimeLabelsB;   
        items[1] = &sModeFrequencyB;
        items[3] = &sNumberPeriodsB;
        PageModes::RelationOff();
    }
    else if(PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::BA)
    {
        items[2] = &sNumberPeriodsB;   
        items[1] = &sModeFrequencyB;
        items[3] = nullptr;
        PageModes::RelationOn();
    }
    else if(PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::Tachometer)
    {
        items[2] = &sNumberPeriodsB;   
        items[1] = &sModeFrequencyB;
        items[3] = nullptr;
        PageModes::RelationOff();
    }
    else
    {
        items[2] = &sPeriodTimeLabelsB;   
        items[1] = &sModeFrequencyB;
        items[3] = &sTimeMeasureB;
        PageModes::RelationOff();
    }     
    PageModes::InterpoleOff();
    PageModes::DCycleOff();     
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
    PageModes::RelationOff();
    PageModes::InterpoleOff();
    PageModes::DCycleOff();
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

    if (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Ndt_1ns)
    {
        PageModes::InterpoleOn();
        PageModes::DCycleOff();
    }
    else if ((PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Dcycle) || 
            (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Phase))
    {
        PageModes::DCycleOn();
        PageModes::InterpoleOff();
        items[2] = &sPeriodTimeLabelsB;
    }
    else
    {
        items[2] = &sPeriodTimeLabelsB;
        PageModes::InterpoleOff();
        PageModes::DCycleOff();
    }
    
    PageModes::RelationOff();
    FreqMeter::LoadModeMeasureDuration();
}

/// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
DEF_SWITCH_5(sModeDurationB,
    "Режим изм.", "Измерение длительности",
    "ndt", "ndt/1нс", "ndt2", "Скважность", "Фаза",
    PageModesB::modeMeasureDurationB, OnPress_ModeDurationB
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeCountPulseB()
{
    ClearItems(2);

    items[1] = &sModeCountPulseB;

    if (PageModesB::modeMeasureCountPulseB == ModeMeasureCountPulseB::BTA_1)
    {
        items[2] = &sNumberPeriodsB;
    }
    PageModes::RelationOff();
    PageModes::InterpoleOff();
    PageModes::DCycleOff();
    FreqMeter::LoadModeMeasureCountPulse();

}

/// Выбор режима счёта импульсов
DEF_SWITCH_2(sModeCountPulseB,
    "Режим изм.", "Счёт числа импульсов",
    "B(tA)", "B(TA)",
    PageModesB::modeMeasureCountPulseB, OnPress_ModeCountPulseB
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeLabelsB()
{
    switch (PageModesB::periodTimeLabelsB.value)
    {
        case PeriodTimeLabelsB::T_3:
            PageModes::periodTimeLabels.value = PeriodTimeLabels::T_3;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_3;
            break;
        case PeriodTimeLabelsB::T_4:
            PageModes::periodTimeLabels.value = PeriodTimeLabels::T_4;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_4;
            break;
        case PeriodTimeLabelsB::T_5:
            PageModes::periodTimeLabels.value = PeriodTimeLabels::T_5;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_5;
            break;
        case PeriodTimeLabelsB::T_6:
            PageModes::periodTimeLabels.value = PeriodTimeLabels::T_6;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_6;
            break;
        case PeriodTimeLabelsB::T_7:
            PageModes::periodTimeLabels.value = PeriodTimeLabels::T_7;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_7;
            break;
        case PeriodTimeLabelsB::T_8:
            PageModes::periodTimeLabels.value = PeriodTimeLabels::T_8;
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
    switch (PageModesB::timeMeasureB.value)
    {
        case TimeMeasureB::_1ms:
            PageModes::timeMeasure.value = TimeMeasure::_1ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1ms;
            PageModesD::timeMeasureD.value = TimeMeasureD::_1ms;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_1;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_1;
            PageModes::numberPeriods.value = NumberPeriods::_1;
            break;
        case TimeMeasureB::_10ms:
            PageModes::timeMeasure.value = TimeMeasure::_10ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_10ms;
            PageModesD::timeMeasureD.value = TimeMeasureD::_10ms;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_10;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_10;
            PageModes::numberPeriods.value = NumberPeriods::_10;
            break;
        case TimeMeasureB::_100ms:
            PageModes::timeMeasure.value = TimeMeasure::_100ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_100ms;
            PageModesD::timeMeasureD.value = TimeMeasureD::_100ms;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_100;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_100;
            PageModes::numberPeriods.value = NumberPeriods::_100;
            break;
        case TimeMeasureB::_1s:
            PageModes::timeMeasure.value = TimeMeasure::_1s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_1s;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_1K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_1K;
            PageModes::numberPeriods.value = NumberPeriods::_1K;
            break;
        case TimeMeasureB::_10s:
            PageModes::timeMeasure.value = TimeMeasure::_10s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_10s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_10s;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_10K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_10K;
            PageModes::numberPeriods.value = NumberPeriods::_10K;
            break;
        case TimeMeasureB::_100s:
            PageModes::timeMeasure.value = TimeMeasure::_100s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_100s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_100s;
            PageModes::numberPeriods.value = NumberPeriods::_100K;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_100K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_100K;
            break;
        case TimeMeasureB::_1000s:
            PageModes::timeMeasure.value = TimeMeasure::_1000s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1000s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_1000s;
            PageModes::numberPeriods.value = NumberPeriods::_1000K;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_1000K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_1000K;
            break;
    }
    FreqMeter::LoadTimeMeasure();
}

/// Выбор времени измерения
DEF_SWITCH_7(sTimeMeasureB,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModesB::timeMeasureB, OnPress_TimeMeasureB
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_NumberPeriodsB()
{
    switch (PageModesB::numberPeriodsB.value)
    {
        case NumberPeriodsB::_1:
            PageModes::numberPeriods.value = NumberPeriods::_1;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_1;
            PageModesB::timeMeasureB.value = TimeMeasureB::_1ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1ms;
            PageModes::timeMeasure.value = TimeMeasure::_1ms;
            break;
        case NumberPeriodsB::_10:
            PageModes::numberPeriods.value = NumberPeriods::_10;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_10;
            PageModesB::timeMeasureB.value = TimeMeasureB::_10ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_10ms;
            PageModes::timeMeasure.value = TimeMeasure::_10ms;
            break;
        case NumberPeriodsB::_100:
            PageModes::numberPeriods.value = NumberPeriods::_100;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_100;
            PageModesB::timeMeasureB.value = TimeMeasureB::_100ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_100ms;
            PageModes::timeMeasure.value = TimeMeasure::_100ms;
            break;
        case NumberPeriodsB::_1K:
            PageModes::numberPeriods.value = NumberPeriods::_1K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_1K;
            PageModesB::timeMeasureB.value = TimeMeasureB::_1s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1s;
            PageModes::timeMeasure.value = TimeMeasure::_1s;
            break;
        case NumberPeriodsB::_10K:
            PageModes::numberPeriods.value = NumberPeriods::_10K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_10K;
            PageModesB::timeMeasureB.value = TimeMeasureB::_10s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_10s;
            PageModes::timeMeasure.value = TimeMeasure::_10s;
            break;
        case NumberPeriodsB::_100K:
            PageModes::numberPeriods.value = NumberPeriods::_100K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_100K;
            PageModesB::timeMeasureB.value = TimeMeasureB::_100s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_100s;
            PageModes::timeMeasure.value = TimeMeasure::_100s;
            break;
        case NumberPeriodsB::_1000K:
            PageModes::numberPeriods.value = NumberPeriods::_1000K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_1000K;
            PageModesB::timeMeasureB.value = TimeMeasureB::_1000s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1000s;
            PageModes::timeMeasure.value = TimeMeasure::_1000s;
            break;
    }
    FreqMeter::LoadNumerPeriodsMeasure();
}

/// Выбор числа усредняемых периодов входного сигнала
DEF_SWITCH_7(sNumberPeriodsB,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K", "1000K",
    PageModesB::numberPeriodsB,
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

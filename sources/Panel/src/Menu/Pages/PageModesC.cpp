#include "defines.h"
#include "Menu/Pages/PageModesC.h"
#include "Menu/Pages/PageModesA.h"
#include "Menu/Pages/PageModesB.h"
#include "Menu/Pages/PageModesD.h"
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
extern Switch sModeFrequencyC;
extern Switch sModeCountPulseC;
extern Switch sPeriodTimeLabelsC;
extern Switch sTimeMeasureC;
extern Switch sNumberPeriodsC;


TypeMeasureC             PageModesC::typeMeasureC(TypeMeasureC::Frequency);
ModeMeasureFrequencyC    PageModesC::modeMeasureFrequencyC(ModeMeasureFrequencyC::Freq);
ModeMeasureCountPulseC   PageModesC::modeMeasureCountPulseC(ModeMeasureCountPulseC::CTA);
PeriodTimeLabelsC        PageModesC::periodTimeLabelsC(PeriodTimeLabelsC::T_8);
NumberPeriods            PageModesC::numberPeriods(NumberPeriods::_1);
TimeMeasureC             PageModesC::timeMeasureC(TimeMeasureC::_1ms);

/// Очистить массив указателей на итемы, начиная с i-го итема
static void ClearItems(int i);

static void OnPress_ModeFrequencyC();

static void OnPress_ModeCountPulseC();


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageModesC::Init()
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TypeMeasureC()
{
    
        switch (PageModesC::typeMeasureC.value)
        {
        case TypeMeasureC::Frequency:
            PageModes::typeMeasure.value = TypeMeasureA::Frequency;
            PageModesB::typeMeasure.value = TypeMeasureB::Frequency;
            OnPress_ModeFrequencyC();
            break;
        case TypeMeasureC::CountPulse:
            PageModesB::typeMeasure.value = TypeMeasureB::CountPulse;
            PageModes::typeMeasure.value = TypeMeasureA::CountPulse;
            OnPress_ModeCountPulseC();
            break;
        }        
}

DEF_SWITCH_2(sTypeMeasureC,
    "Вид изм.", "Выбор измерения",
    "Частота", "Сч. имп.",
    PageModesC::typeMeasureC, OnPress_TypeMeasureC
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeFrequencyC()
{
    ClearItems(2);
    items[2] = &sPeriodTimeLabelsC;   
    items[1] = &sModeFrequencyC;
    
    if ((PageModesC::modeMeasureFrequencyC == ModeMeasureFrequencyC::CB) || (PageModesC::modeMeasureFrequencyC == ModeMeasureFrequencyC::CA))
    {
        items[3] = &sNumberPeriodsC;
    }
    else
    {
        items[3] = &sTimeMeasureC;
    } 
    PageModes::RelationOff();
    PageModes::InterpoleOff();
    PageModes::DCycleOff();  
    FreqMeter::LoadModeMeasureFrequency();
}

/// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_3(sModeFrequencyC,
    "Режим изм.", "Измерение частоты",
    "Частота", "f(C )/f(A)", "f(C )/f(B)",
    PageModesC::modeMeasureFrequencyC, OnPress_ModeFrequencyC);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeCountPulseC()
{
    ClearItems(2);

    items[1] = &sModeCountPulseC;
//    if(PageModesC::modeMeasureCountPulseC.value == ModeMeasureCountPulseC::StartStop)
//    {
//        PageModesB::modeMeasureCountPulseB.value = ModeMeasureCountPulseB::StartStop;
//        PageModes::modeMeasureCountPulse.value = ModeMeasureCountPulse::StartStop;
//    }
    if (PageModesC::modeMeasureCountPulseC == ModeMeasureCountPulseC::CTA_1)
    {
        items[2] = &sNumberPeriodsC;
    }
    if (PageModesC::modeMeasureCountPulseC == ModeMeasureCountPulseC::CTB_1)
    {
        items[2] = &sNumberPeriodsC;
    }
    PageModes::RelationOff();
    PageModes::InterpoleOff();
    PageModes::DCycleOff();  
    FreqMeter::LoadModeMeasureCountPulse();
}

/// Выбор режима счёта импульсов
DEF_SWITCH_4(sModeCountPulseC,
    "Режим изм.", "Счёт числа импульсов",
    "C(tA)", "C(tB)", "C(TA)", "C(TB)",
    PageModesC::modeMeasureCountPulseC, OnPress_ModeCountPulseC
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeLabelsC()
{
    switch (PageModesC::periodTimeLabelsC.value)
    {
        case PeriodTimeLabelsC::T_3:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_3;
            PageModes::periodTimeLabels.value = PeriodTimeLabels::T_3;
            break;
        case PeriodTimeLabelsC::T_4:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_4;
            PageModes::periodTimeLabels.value = PeriodTimeLabels::T_4;
            break;
        case PeriodTimeLabelsC::T_5:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_5;
            PageModes::periodTimeLabels.value = PeriodTimeLabels::T_5;
            break;
        case PeriodTimeLabelsC::T_6:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_6;
            PageModes::periodTimeLabels.value = PeriodTimeLabels::T_6;
            break;
        case PeriodTimeLabelsC::T_7:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_7;
            PageModes::periodTimeLabels.value = PeriodTimeLabels::T_7;
            break;
        case PeriodTimeLabelsC::T_8:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_8;
            PageModes::periodTimeLabels.value = PeriodTimeLabels::T_8;
            break;
    }
    FreqMeter::LoadPeriodTimeLabels();
}

/// Выбор периода меток времени
DEF_SWITCH_6(sPeriodTimeLabelsC,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModesC::periodTimeLabelsC,  OnPress_TimeLabelsC
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeMeasureC()
{
    switch (PageModesC::timeMeasureC.value)
    {
        case TimeMeasureC::_1ms:
            PageModesB::timeMeasureB.value = TimeMeasureB::_1ms;
            PageModes::timeMeasure.value = TimeMeasure::_1ms;
            PageModesD::timeMeasureD.value = TimeMeasureD::_1ms;
            PageModesB::numberPeriods.value = NumberPeriods::_1;
            PageModesC::numberPeriods.value = NumberPeriods::_1;
            PageModes::numberPeriods.value = NumberPeriods::_1;
            break;
        case TimeMeasureC::_10ms:
            PageModesB::timeMeasureB.value = TimeMeasureB::_10ms;
            PageModes::timeMeasure.value = TimeMeasure::_10ms;
            PageModesD::timeMeasureD.value = TimeMeasureD::_10ms;
            PageModesB::numberPeriods.value = NumberPeriods::_10;
            PageModesC::numberPeriods.value = NumberPeriods::_10;
            PageModes::numberPeriods.value = NumberPeriods::_10;
            break;
        case TimeMeasureC::_100ms:
            PageModesB::timeMeasureB.value = TimeMeasureB::_100ms;
            PageModes::timeMeasure.value = TimeMeasure::_100ms;
            PageModesD::timeMeasureD.value = TimeMeasureD::_100ms;
            PageModesB::numberPeriods.value = NumberPeriods::_100;
            PageModesC::numberPeriods.value = NumberPeriods::_100;
            PageModes::numberPeriods.value = NumberPeriods::_100;
            break;
        case TimeMeasureC::_1s:
            PageModesB::timeMeasureB.value = TimeMeasureB::_1s;
            PageModes::timeMeasure.value = TimeMeasure::_1s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_1s;
            PageModesB::numberPeriods.value = NumberPeriods::_1K;
            PageModesC::numberPeriods.value = NumberPeriods::_1K;
            PageModes::numberPeriods.value = NumberPeriods::_1K;
            break;
        case TimeMeasureC::_10s:
            PageModesB::timeMeasureB.value = TimeMeasureB::_10s;
            PageModes::timeMeasure.value = TimeMeasure::_10s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_10s;
            PageModesB::numberPeriods.value = NumberPeriods::_10K;
            PageModesC::numberPeriods.value = NumberPeriods::_10K;
            PageModes::numberPeriods.value = NumberPeriods::_10K;
            break;
        case TimeMeasureC::_100s:
            PageModesB::timeMeasureB.value = TimeMeasureB::_100s;
            PageModes::timeMeasure.value = TimeMeasure::_100s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_100s;
            PageModes::numberPeriods.value = NumberPeriods::_100K;
            PageModesB::numberPeriods.value = NumberPeriods::_100K;
            PageModesC::numberPeriods.value = NumberPeriods::_100K;
            break;
        case TimeMeasureC::_1000s:
            PageModesB::timeMeasureB.value = TimeMeasureB::_1000s;
            PageModes::timeMeasure.value = TimeMeasure::_1000s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_1000s;
            PageModes::numberPeriods.value = NumberPeriods::_1000K;
            PageModesB::numberPeriods.value = NumberPeriods::_1000K;
            PageModesC::numberPeriods.value = NumberPeriods::_1000K;
            break;
    }
    FreqMeter::LoadTimeMeasure();
}

/// Выбор времени измерения
DEF_SWITCH_7(sTimeMeasureC,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModesC::timeMeasureC, OnPress_TimeMeasureC
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_NumberPeriodsC()
{
    switch (PageModesC::numberPeriods.value)
    {
        case NumberPeriods::_1:
            PageModesB::numberPeriods.value = NumberPeriods::_1;
            PageModes::numberPeriods.value = NumberPeriods::_1;
            PageModesB::timeMeasureB.value = TimeMeasureB::_1ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1ms;
            PageModes::timeMeasure.value = TimeMeasure::_1ms;
            break;
        case NumberPeriods::_10:
            PageModesB::numberPeriods.value = NumberPeriods::_10;
            PageModes::numberPeriods.value = NumberPeriods::_10;
            PageModesB::timeMeasureB.value = TimeMeasureB::_10ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_10ms;
            PageModes::timeMeasure.value = TimeMeasure::_10ms;
            break;
        case NumberPeriods::_100:
            PageModesB::numberPeriods.value = NumberPeriods::_100;
            PageModes::numberPeriods.value = NumberPeriods::_100;
            PageModesB::timeMeasureB.value = TimeMeasureB::_100ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_100ms;
            PageModes::timeMeasure.value = TimeMeasure::_100ms;
            break;
        case NumberPeriods::_1K:
            PageModesB::numberPeriods.value = NumberPeriods::_1K;
            PageModes::numberPeriods.value = NumberPeriods::_1K;
            PageModesB::timeMeasureB.value = TimeMeasureB::_1s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1s;
            PageModes::timeMeasure.value = TimeMeasure::_1s;
            break;
        case NumberPeriods::_10K:
            PageModesB::numberPeriods.value = NumberPeriods::_10K;
            PageModes::numberPeriods.value = NumberPeriods::_10K;
            PageModesB::timeMeasureB.value = TimeMeasureB::_10s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_10s;
            PageModes::timeMeasure.value = TimeMeasure::_10s;
            break;
        case NumberPeriods::_100K:
            PageModesB::numberPeriods.value = NumberPeriods::_100K;
            PageModes::numberPeriods.value = NumberPeriods::_100K;
            PageModesB::timeMeasureB.value = TimeMeasureB::_100s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_100s;
            PageModes::timeMeasure.value = TimeMeasure::_100s;
            break;
        case NumberPeriods::_1000K:
            PageModesB::numberPeriods.value = NumberPeriods::_1000K;
            PageModes::numberPeriods.value = NumberPeriods::_1000K;
            PageModesB::timeMeasureB.value = TimeMeasureB::_1000s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1000s;
            PageModes::timeMeasure.value = TimeMeasure::_1000s;
            break;
    }
    FreqMeter::LoadNumerPeriodsMeasure();
}

/// Выбор числа усредняемых периодов входного сигнала
DEF_SWITCH_7(sNumberPeriodsC,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K", "1000K",
    PageModesC::numberPeriods,
    OnPress_NumberPeriodsC
);


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

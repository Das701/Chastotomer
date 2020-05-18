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
ModeMeasureCountPulseB   PageModesB::modeMeasureCountPulseB(ModeMeasureCountPulseB::Manual);

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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TypeMeasureB()
{
        switch (PageModesB::typeMeasureB.value)
        {
        case TypeMeasureB::Frequency:
            OnPress_ModeFrequencyB();
            PageModes::typeMeasure.value = TypeMeasure::Frequency;
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
    }
    else if (PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::T_1 ||
        PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::BA)
    {
        items[2] = &sPeriodTimeLabelsB;   
        items[1] = &sModeFrequencyB;
        items[3] = &sNumberPeriodsB;
    }
    else if(PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::Tachometer)
    {
        items[2] = &sPeriodTimeLabelsB;   
        items[1] = &sModeFrequencyB;
        items[3] = nullptr;
    }
    else
    {
        items[2] = &sPeriodTimeLabelsB;   
        items[1] = &sModeFrequencyB;
        items[3] = &sTimeMeasureB;
    }      
    FreqMeter::LoadModeMeasureFrequency();
    PageModes::InterpoleOff();
    PageModes::DCycleOff();
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

    FreqMeter::LoadModeMeasurePeriod();
    PageModes::InterpoleOff();
    PageModes::DCycleOff();
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
    else
    {
        items[2] = &sPeriodTimeLabelsB;
        PageModes::InterpoleOff();
        PageModes::DCycleOff();
    }
    if (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Dcycle)
    {
        PageModes::DCycleOn();
        PageModes::InterpoleOff();
    }
    FreqMeter::LoadModeMeasureDuration();
}

/// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
DEF_SWITCH_4(sModeDurationB,
    "Режим изм.", "Измерение длительности",
    "ndt", "ndt/1нс", "ndt2", "Скважность",
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

    FreqMeter::LoadModeMeasureCountPulse();
    PageModes::InterpoleOff();
    PageModes::DCycleOff();
}

/// Выбор режима счёта импульсов
DEF_SWITCH_3(sModeCountPulseB,
    "Режим изм.", "Счёт числа импульсов",
    "Ручн.", "B(tA)", "B(TA)",
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
            break;
        case TimeMeasureB::_10ms:
            PageModes::timeMeasure.value = TimeMeasure::_10ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_10ms;
            PageModesD::timeMeasureD.value = TimeMeasureD::_10ms;
            break;
        case TimeMeasureB::_100ms:
            PageModes::timeMeasure.value = TimeMeasure::_100ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_100ms;
            PageModesD::timeMeasureD.value = TimeMeasureD::_100ms;
            break;
        case TimeMeasureB::_1s:
            PageModes::timeMeasure.value = TimeMeasure::_1s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_1s;
            break;
        case TimeMeasureB::_10s:
            PageModes::timeMeasure.value = TimeMeasure::_10s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_10s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_10s;
            break;
        case TimeMeasureB::_100s:
            PageModes::timeMeasure.value = TimeMeasure::_100s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_100s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_100s;
            break;
    }
    FreqMeter::LoadTimeMeasure();
}

/// Выбор времени измерения
DEF_SWITCH_6(sTimeMeasureB,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s",
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
            break;
        case NumberPeriodsB::_10:
            PageModes::numberPeriods.value = NumberPeriods::_10;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_10;
            break;
        case NumberPeriodsB::_100:
            PageModes::numberPeriods.value = NumberPeriods::_100;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_100;
            break;
        case NumberPeriodsB::_1K:
            PageModes::numberPeriods.value = NumberPeriods::_1K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_1K;
            break;
        case NumberPeriodsB::_10K:
            PageModes::numberPeriods.value = NumberPeriods::_10K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_10K;
            break;
        case NumberPeriodsB::_100K:
            PageModes::numberPeriods.value = NumberPeriods::_100K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_100K;
            break;
    }
    FreqMeter::LoadNumerPeriodsMeasure();
}

/// Выбор числа усредняемых периодов входного сигнала
DEF_SWITCH_6(sNumberPeriodsB,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K",
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

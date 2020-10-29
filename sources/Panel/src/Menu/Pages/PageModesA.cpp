#include "defines.h"
#include "Settings.h"
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

TypeMeasureA            PageModesA::typeMeasure(TypeMeasureA::Frequency);
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
            PageModesC::typeMeasure.value = TypeMeasureC::Frequency;
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
            PageModesC::typeMeasure.value = TypeMeasureC::CountPulse;
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
    if (PageModesA::modeMeasureFrequency.IsAC())
    {
        items[1] = &sModeFrequency;
        items[2] = &sTimeMeasure;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if(PageModesA::modeMeasureFrequency.IsT_1())
    {
        items[2] = &sPeriodTimeLabels;   
        items[1] = &sModeFrequency;
        items[3] = &sNumberPeriods;
        PageModesA::RelationOff();
    }
    else if(PageModesA::modeMeasureFrequency.IsTachometer())
    {
        items[2] = &sPeriodTimeLabels;   
        items[1] = &sModeFrequency;
        items[3] = nullptr;
        PageModesA::RelationOff();
    }
    else if(PageModesA::modeMeasureFrequency.IsAB())
    {
        items[2] = &sNumberPeriods;   
        items[1] = &sModeFrequency;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if(PageModesA::modeMeasureFrequency.IsComparator())
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

    if (PageModesA::modeMeasurePeriod.IsPeriod())
    {
        items[2] = &sPeriodTimeLabels;
        items[3] = &sNumberPeriods;
    }
    else if (PageModesA::modeMeasurePeriod.IsF_1())
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
            PageModesB::modeMeasureDuration.value = ModeMeasureDuration::Ndt;
            break;
        case ModeMeasureDuration::Ndt_1ns:
            PageModesB::modeMeasureDuration.value = ModeMeasureDuration::Ndt_1ns;
            break;
        case ModeMeasureDuration::Ndt2:
            PageModesB::modeMeasureDuration.value = ModeMeasureDuration::Ndt2;
            break;
        case ModeMeasureDuration::Dcycle:
            PageModesB::modeMeasureDuration.value = ModeMeasureDuration::Dcycle;
            break;
        case ModeMeasureDuration::Phase:
            PageModesB::modeMeasureDuration.value = ModeMeasureDuration::Phase;
            break;
    }
    if (PageModesA::modeMeasureDuration.Is_Ndt_1ns())
    {
        PageModesA::InterpoleOn();
        
        PageModesA::DCycleOff();
    }
    else if (PageModesA::modeMeasureDuration.Is_Dcycle() || PageModesA::modeMeasureDuration.Is_Phase())
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
        PageModesB::modeMeasureCountPulse.value = ModeMeasureCountPulseB::StartStop;
//        PageModesC::modeMeasureCountPulse.value = ModeMeasureCountPulseC::StartStop;
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
    uint8 period = PageModesA::periodTimeLabels.value;

    PageModesB::periodTimeLabels.value = period;
    PageModesC::periodTimeLabels.value = period;

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

    uint8 period = periods[PageModesA::timeMeasure.value];

    PageModesA::numberPeriods.value = period;
    PageModesB::numberPeriods.value = period;
    PageModesC::numberPeriods.value = period;

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


int TimeMeasure::ToMS() const
{
    static const int time[Count] =
    {
        1, 10, 100, 1000, 10000, 100000, 1000000
    };

    return time[value];
}


TimeMeasure &TimeMeasure::Current()
{
    if (CURRENT_CHANNEL_IS_A)
    {
        return PageModesA::timeMeasure;
    }
    else if (CURRENT_CHANNEL_IS_B)
    {
        return PageModesB::timeMeasure;
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        return PageModesC::timeMeasure;
    }

    return PageModesD::timeMeasure;
}


NumberPeriods &NumberPeriods::Current()
{
    NumberPeriods &result = PageModesA::numberPeriods;

    switch (CURRENT_CHANNEL)
    {
    case Channel::A:    result = PageModesA::numberPeriods; break;
    case Channel::B:    result = PageModesB::numberPeriods; break;
    case Channel::C:    result = PageModesC::numberPeriods; break;
    }

    return result;
}


PeriodTimeLabels &PeriodTimeLabels::Current()
{
    PeriodTimeLabels &result = PageModesA::periodTimeLabels;

    switch (CURRENT_CHANNEL)
    {
    case Channel::A:    result = PageModesA::periodTimeLabels; break;
    case Channel::B:    result = PageModesB::periodTimeLabels; break;
    case Channel::C:    result = PageModesC::periodTimeLabels; break;
    }

    return result;
}


int PeriodTimeLabels::ToZeros() const
{
    static const int zeros[Count] =
    {
        1000,
        10000,
        100000,
        1000000,
        10000000,
        100000000
    };

    return zeros[value];
}


ModeMeasurePeriod &ModeMeasurePeriod::Current()
{
    ModeMeasurePeriod &result = PageModesA::modeMeasurePeriod;

    if (CURRENT_CHANNEL_IS_B)
    {
        result = PageModesB::modeMeasurePeriod;
    }

    return result;
}

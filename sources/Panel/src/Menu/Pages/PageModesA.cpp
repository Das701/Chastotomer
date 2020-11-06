#include "defines.h"
#include "FreqMeter.h"
#include "Settings.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageModesA.h"
#include "Menu/Pages/PageModesB.h"
#include "Menu/Pages/PageModesC.h"
#include "Menu/Pages/PageModesD.h"
#include "Menu/MenuItems.h"
#include "Utils/Math.h"

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

TypeMeasureAB           PageModesA::typeMeasure(TypeMeasureAB::Frequency);
ModeMeasureFrequencyA   PageModesA::modeMeasureFrequency(ModeMeasureFrequencyA::Frequency);
ModeMeasurePeriod       PageModesA::modeMeasurePeriod(ModeMeasurePeriod::Period);
ModeMeasureDuration     PageModesA::modeMeasureDuration(ModeMeasureDuration::Ndt);
ModeMeasureCountPulseA  PageModesA::modeMeasureCountPulse(ModeMeasureCountPulseA::AtC);

PeriodTimeLabels        PageModesA::periodTimeLabels(PeriodTimeLabels::T_8);
NumberPeriods           PageModesA::numberPeriods(NumberPeriods::_1);
TimeMeasure             PageModesA::timeMeasure(TimeMeasure::_1ms);


void PageModesA::InterpolateOn()
{
    interpoleOn = true;
}

void PageModesA::InterpolateOff()
{
    interpoleOn = false;
}

bool PageModesA::InterpolateCheck()
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




void PageModesA::Init()
{
}

void PageModesA::PressSetup()
{
    switch (PageModesA::typeMeasure.value)
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


void PageModesA::OnChanged_TypeMeasure()
{
    switch (PageModesA::typeMeasure.value)
    {
    case TypeMeasureAB::Frequency:
        PageModesB::typeMeasure.value = TypeMeasureAB::Frequency;
        PageModesC::typeMeasure.value = TypeMeasureC::Frequency;

        PageModes::ResetModeCurrentMeasure();
        PageModesA::OnChanged_ModeFrequency();
        break;

    case TypeMeasureAB::Period:
        PageModesB::typeMeasure.value = TypeMeasureAB::Period;

        PageModes::ResetModeCurrentMeasure();
        PageModesA::OnChanged_ModePeriod();
        break;

    case TypeMeasureAB::Duration:
        PageModesB::typeMeasure.value = TypeMeasureAB::Duration;

        PageModes::ResetModeCurrentMeasure();
        PageModesA::OnChanged_ModeDuration();
        break;

    case TypeMeasureAB::CountPulse:
        PageModesB::typeMeasure.value = TypeMeasureAB::CountPulse;
        PageModesC::typeMeasure.value = TypeMeasureC::CountPulse;

        PageModes::ResetModeCurrentMeasure();
        PageModesA::OnChanged_ModeCountPulse();
        break;
    }
}


DEF_SWITCH_4(sTypeMeasure,
    "Измерение", "Выбор измерения",
    "Частота", "Период", "Длит.", "Сч. имп.",
    PageModesA::typeMeasure, PageModesA::OnChanged_TypeMeasure
);


static void OnPress_Statistics()
{

}

DEF_BUTTON(bStatistics, "Стат", OnPress_Statistics);


void PageModesA::OnChanged_ModeFrequency()
{
    if (PageModesA::modeMeasureFrequency.IsFrequency())
    {
        items[1] = &sModeFrequency;
        items[2] = &sPeriodTimeLabels;
        items[3] = &sTimeMeasure;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if (PageModesA::modeMeasureFrequency.IsRatioAB())
    {
        items[1] = &sModeFrequency;
        items[2] = &sNumberPeriods;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (PageModesA::modeMeasureFrequency.IsRatioAC())
    {
        items[1] = &sModeFrequency;
        items[2] = &sTimeMeasure;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (PageModesA::modeMeasureFrequency.IsT_1())
    {
        items[1] = &sModeFrequency;
        items[2] = &sPeriodTimeLabels;
        items[3] = &sNumberPeriods;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if (PageModesA::modeMeasureFrequency.IsTachometer())
    {
        items[1] = &sModeFrequency;
        items[2] = &sPeriodTimeLabels;
        items[3] = nullptr;
        PageModesA::RelationOff();
    }
    else if (PageModesA::modeMeasureFrequency.IsComparator())
    {
        items[1] = &sModeFrequency;
        items[2] = &bStatistics;
        items[3] = nullptr;
        PageModesA::RelationOff();
    }

    PageModesA::InterpolateOff();
    PageModesA::DCycleOff();
    FreqMeter::LoadModeMeasureFrequency();

}

// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_6(sModeFrequency,
    "Режим", "Измерение частоты",
    "Частота", "f(A)/f(B)", "f(A)/f(C)", "f=1/T", "Тахометр", "Компаратор",
    PageModesA::modeMeasureFrequency, PageModesA::OnChanged_ModeFrequency
);


void PageModesA::OnChanged_ModePeriod()
{
    items[1] = &sModePeriod;

    if (PageModesA::modeMeasurePeriod.IsPeriod())
    {
        items[2] = &sPeriodTimeLabels;
        items[3] = &sNumberPeriods;
        items[4] = nullptr;
    }
    else if (PageModesA::modeMeasurePeriod.IsF_1())
    {
        items[2] = &sPeriodTimeLabels;
        items[3] = &sTimeMeasure;
        items[4] = nullptr;
    }

    PageModesA::RelationOff();
    PageModesA::InterpolateOff();
    PageModesA::DCycleOff();
    FreqMeter::LoadModeMeasurePeriod();

}

// Выбор режима измерения периода
DEF_SWITCH_2(sModePeriod,
    "Режим", "Измерение периода",
    "Период", "T=1/f",
    PageModesA::modeMeasurePeriod, PageModesA::OnChanged_ModePeriod
);


void PageModesA::OnChanged_ModeDuration()
{
    items[1] = &sModeDuration;

    PageModesB::modeMeasureDuration.value = PageModesA::modeMeasureDuration.value;

    switch (PageModesA::modeMeasureDuration.value)
    {
    case ModeMeasureDuration::Ndt_1ns:
        PageModesA::InterpolateOn();
        PageModesA::DCycleOff();
        items[2] = nullptr;
        break;

    case ModeMeasureDuration::Dcycle:
    case ModeMeasureDuration::Phase:
        PageModesA::DCycleOn();
        PageModesA::InterpolateOff();
        items[2] = &sPeriodTimeLabels;
        items[3] = nullptr;
        break;

    case ModeMeasureDuration::Ndt:
    case ModeMeasureDuration::Ndt2:
        PageModesA::InterpolateOff();
        PageModesA::DCycleOff();
        items[2] = &sPeriodTimeLabels;
        items[3] = nullptr;
        break;
    }

    PageModesA::RelationOff();
    FreqMeter::LoadModeMeasureDuration();
}

// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
DEF_SWITCH_5(sModeDuration,
    "Режим", "Измерение длительности",
    "ndt", "ndt/1нс", "S-S", "D", "Фаза",
    PageModesA::modeMeasureDuration, PageModesA::OnChanged_ModeDuration
);


void PageModesA::OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulse;
    items[2] = nullptr;

    switch (PageModesA::modeMeasureCountPulse.value)
    {
    case ModeMeasureCountPulseA::AtC:
        break;

    case ModeMeasureCountPulseA::ATB:
        items[2] = &sNumberPeriods;
        items[3] = nullptr;
        break;

    case ModeMeasureCountPulseA::StartStop:
        PageModesB::modeMeasureCountPulse.value = ModeMeasureCountPulseB::StartStop;
        break;
    }

    PageModesA::RelationOff();
    PageModesA::InterpolateOff();
    PageModesA::DCycleOff();
    FreqMeter::LoadModeMeasureCountPulse();

}

// Выбор режима счёта импульсов
DEF_SWITCH_3(sModeCountPulse,
    "Режим", "Счёт числа импульсов",
    "А(tB)", "А(TB)", "Старт/Стоп",
    PageModesA::modeMeasureCountPulse, PageModesA::OnChanged_ModeCountPulse
);


static void OnPress_TimeLabels()
{
    uint8 period = PageModesA::periodTimeLabels.value;

    PageModesB::periodTimeLabels.value = period;
    PageModesC::periodTimeLabels.value = period;

    FreqMeter::LoadPeriodTimeLabels();
}

// Выбор периода меток времени
DEF_SWITCH_6(sPeriodTimeLabels,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModesA::periodTimeLabels, OnPress_TimeLabels
);


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
    PageModesA::timeMeasure, OnPress_TimeMeasure
);


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

// Выбор числа усредняемых периодов входного сигнала
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
    if (startStop == false)
    {
        startStop = true;
    }
    else
    {
        startStop = false;
    }
}



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
    case Channel::D:
    case Channel::Count:                                    break;
    }

    return result;
}


PeriodTimeLabels &PeriodTimeLabels::Current()
{
    PeriodTimeLabels &result = PageModesA::periodTimeLabels;

    switch (CURRENT_CHANNEL)
    {
    case Channel::A:    result = PageModesA::periodTimeLabels;  break;
    case Channel::B:    result = PageModesB::periodTimeLabels;  break;
    case Channel::C:    result = PageModesC::periodTimeLabels;  break;
    case Channel::D: case Channel::Count:                       break;
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


ModeMeasureDuration &ModeMeasureDuration::Current()
{
    ModeMeasureDuration &result = PageModesA::modeMeasureDuration;

    if (CURRENT_CHANNEL_IS_B)
    {
        result = PageModesB::modeMeasureDuration;
    }

    return result;
}


bool CurrentTypeMeasure::IsFrequency()
{
    return ((CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsFrequency()) ||
        (CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsFrequency()) ||
        (CURRENT_CHANNEL_IS_C && PageModesC::typeMeasure.IsFrequency()) ||
        CURRENT_CHANNEL_IS_D);
}


bool CurrentTypeMeasure::IsPeriod()
{
    return ((CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsPeriod()) ||
        (CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsPeriod()));
}


bool CurrentTypeMeasure::IsDuration()
{
    return ((CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsDuration()) ||
        (CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsDuration()));
}


bool CurrentTypeMeasure::IsCountPulse()
{
    return ((CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsCountPulse()) ||
        (CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsCountPulse()) ||
        (CURRENT_CHANNEL_IS_C && PageModesC::typeMeasure.IsCountPulse()));
}


bool CurrentModeMeasureFrequency::IsTachometer()
{
    return (((PageModesA::modeMeasureFrequency.IsTachometer() && CURRENT_CHANNEL_IS_A) ||
        (PageModesB::modeMeasureFrequency.IsTachometer() && CURRENT_CHANNEL_IS_B)));
}


bool CurrentModeMeasureFrequency::IsComparator()
{
    return CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsFrequency() && PageModesA::modeMeasureFrequency.IsComparator();
}


bool CurrentModeMeasureFrequency::IsT_1()
{
    return ((CURRENT_CHANNEL_IS_A && PageModesA::modeMeasureFrequency.IsT_1()) ||
        (CURRENT_CHANNEL_IS_B && PageModesB::modeMeasureFrequency.IsT_1()));
}


bool CurrentModeMeasureFrequency::Is_RatioAC_or_RatioBC()
{
    return ((CURRENT_CHANNEL_IS_A && PageModesA::modeMeasureFrequency.IsRatioAC()) ||
        (CURRENT_CHANNEL_IS_B && PageModesB::modeMeasureFrequency.IsRatioBC()));
}


bool CurrentModeMeasureFrequency::Is_RatioAB_or_RatioBA()
{
    return ((CURRENT_CHANNEL_IS_A && PageModesA::modeMeasureFrequency.IsRatioAB()) ||
        (CURRENT_CHANNEL_IS_B && PageModesB::modeMeasureFrequency.IsRatioBA()));
}


bool CurrentModeMeasureFrequency::Is_RatioCA_or_RatioCB()
{
    return (CURRENT_CHANNEL_IS_C && (PageModesC::modeMeasureFrequency.IsRatioCA() || PageModesC::modeMeasureFrequency.IsRatioCB()));
}


bool CurrentModeMeasureFrequency::Is_Ratio()
{
    return Is_RatioAB_or_RatioBA() || Is_RatioAC_or_RatioBC() || Is_RatioCA_or_RatioCB();
}


bool CurrentModeMeasureCountPulse::IsBig_T()
{
    return ((CURRENT_CHANNEL_IS_A && (PageModesA::modeMeasureCountPulse == ModeMeasureCountPulseA::ATB))
        || (CURRENT_CHANNEL_IS_B && (PageModesB::modeMeasureCountPulse == ModeMeasureCountPulseB::BTA))
        || (CURRENT_CHANNEL_IS_C && (PageModesC::modeMeasureCountPulse == ModeMeasureCountPulseC::CTB))
        || (CURRENT_CHANNEL_IS_C && (PageModesC::modeMeasureCountPulse == ModeMeasureCountPulseC::CTA)));
}


void PageModes::ResetModeCurrentMeasure()
{
    PageModesA::ResetModeCurrentMeasure();
    PageModesB::ResetModeCurrentMeasure();
    PageModesC::ResetModeCurrentMeasure();
}


void PageModesA::ResetModeCurrentMeasure()
{
    switch (typeMeasure)
    {
    case TypeMeasureAB::Frequency:  modeMeasureFrequency.value = 0;     break;
    case TypeMeasureAB::Period:     modeMeasurePeriod.value = 0;        break;
    case TypeMeasureAB::Duration:   modeMeasureDuration.value = 0;      break;
    case TypeMeasureAB::CountPulse: modeMeasureCountPulse.value = 0;    break;
    }
}

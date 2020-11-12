#include "Settings.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/Pages/Modes/PagesModes.h"


bool ModeMeasureCountPulse::IsFromPeriod() const
{
    return (value == ATB) || (value == BTA) || (value == CTA) || (value == CTB);
}


Enumeration &CurrentModeMeasure::ToEnumeration()
{
    if (CURRENT_CHANNEL_IS_A)
    {
        static Enumeration *const modes[4] =
        {
            &PageModesA::modeMeasureFrequency,
            &PageModesA::modeMeasurePeriod,
            &PageModesA::modeMeasureDuration,
            &PageModesA::modeMeasureCountPulse
        };

        return *modes[PageModesA::typeMeasure.value];
    }
    else if (CURRENT_CHANNEL_IS_B)
    {
        static Enumeration *const modesB[4] =
        {
            &PageModesB::modeMeasureFrequency,
            &PageModesB::modeMeasurePeriod,
            &PageModesB::modeMeasureDuration,
            &PageModesB::modeMeasureCountPulse
        };

        return *modesB[PageModesB::typeMeasure.value];
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        static Enumeration *const modesC[2] =
        {
            &PageModesC::modeMeasureFrequency,
            &PageModesC::modeMeasureCountPulse
        };

        return *modesC[PageModesC::typeMeasure.value];;
    }

    return PageModesD::modeMeasureFrequency;
}


void CurrentModeMeasure::Draw(int x, int y)
{
    int width = 120;

    Primitives::Rectangle(width, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::WHITE);

    Text(ToEnumeration().ToText()).Write(x + 10, y + 10, Color::WHITE);
}


void CurrentModeMeasure::DrawParameters(int x, int y)
{
    static const Enumeration *const types[Channel::Count][4] =
    {
        { &PageModesA::modeMeasureFrequency, &PageModesA::modeMeasurePeriod,     &PageModesA::modeMeasureDuration, &PageModesA::modeMeasureCountPulse },
        { &PageModesB::modeMeasureFrequency, &PageModesB::modeMeasurePeriod,     &PageModesB::modeMeasureDuration, &PageModesB::modeMeasureCountPulse },
        { &PageModesC::modeMeasureFrequency, &PageModesC::modeMeasureCountPulse, nullptr,                          nullptr },
        { nullptr,                           nullptr,                            nullptr,                          nullptr }
    };

    static const Enumeration *const modes[Channel::Count - 1][TypeMeasure::Count][ModeMeasureFrequency::Count] =
    {
{//   Frequency                   T_1                         RatioAB                     RatioAC                                                         Tachometer Comparator // ModeMeasureFrequency
    { &PageModesA::timeMeasure,   &PageModesA::numberPeriods, &PageModesA::numberPeriods, &PageModesA::timeMeasure,   nullptr, nullptr, nullptr, nullptr, nullptr,   nullptr},  // TypeMeasure::Frequency
 //   Period                      F_1                                                                                                                                          
    { &PageModesA::numberPeriods, &PageModesA::timeMeasure},                                                                                                                    // TypeMeasure::Period
 //   Ndt                         Ndt_1ns                     ndt2                        FillFactor                  Phase
    { nullptr,                    nullptr,                    nullptr,                    nullptr,                    nullptr },                                                // TypeMeasure::Duration
 //   AtC                         ATB                                                                                                                     StartStop
    { nullptr,                    &PageModesA::numberPeriods, nullptr,                    nullptr,                    nullptr, nullptr, nullptr, nullptr, nullptr }             // TypeMeasure::CountPulse
},                                                                                                                                                                 
{                                                                                                                                                                  
    { &PageModesA::timeMeasure,   &PageModesA::timeMeasure,   &PageModesA::numberPeriods, &PageModesA::numberPeriods, nullptr, nullptr, nullptr, nullptr, nullptr,   nullptr }, // TypeMeasure::Frequency
    { &PageModesA::numberPeriods, &PageModesA::timeMeasure,   nullptr,                    nullptr,                    nullptr, nullptr, nullptr, nullptr, nullptr,   nullptr }, // TypeMeasure::Period
    { nullptr,                    nullptr,                    nullptr,                    nullptr,                    nullptr, nullptr, nullptr, nullptr, nullptr,   nullptr }, // TypeMeasure::Duration
    { nullptr,                    &PageModesA::numberPeriods, nullptr,                    nullptr,                    nullptr, nullptr, nullptr, nullptr, nullptr,   nullptr }  // TypeMeasure::CountPulse
},                                                                                                                                                                 
{                                                                                                                                                                  
    { &PageModesC::timeMeasure,   &PageModesC::numberPeriods, &PageModesC::numberPeriods, nullptr,                    nullptr, nullptr, nullptr, nullptr, nullptr,   nullptr }, // TypeMeasure::Frequency
    { },                                                                                                                                                                        // TypeMeasure::Period
    { },                                                                                                                                                                        // TypeMeasure::Duration
    { nullptr,                    nullptr,                    nullptr,                    nullptr,                    nullptr, nullptr, nullptr, nullptr, nullptr,   nullptr }  // TypeMeasure::CountPulse
}
    };

    const Enumeration *mode = &PageModesD::timeMeasure;

    if (!CURRENT_CHANNEL_IS_D)
    {
        mode = modes[CURRENT_CHANNEL]
                    [TypeMeasure::Current().value]
                    [types[CURRENT_CHANNEL][TypeMeasure::Current().value]->value];
    }

    if (mode)
    {
        int width = 60;

        Primitives::Rectangle(width, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::WHITE);

        Text(mode->ToText()).Write(x + 2, y + 7, width, Color::WHITE);
    }
}


void TypeMeasure::Draw(int x, int y) const
{
    int width = 100;

    Primitives::Rectangle(width, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::WHITE);

    Text(ToText()).Write(x, y + 10, width, Color::WHITE);
}


void PageModes::ResetModeCurrentMeasure()
{
    PageModesA::ResetModeCurrentMeasure();
    PageModesB::ResetModeCurrentMeasure();
    PageModesC::ResetModeCurrentMeasure();
}


bool ModeMeasureFrequency::IsRatio() const
{
    return IsRatioAB() || IsRatioBA() ||  IsRatioAC() || IsRatioBC() || IsRatioCA() || IsRatioCB();
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


int TimeMeasure::ToMS() const
{
    static const int time[Count] =
    {
        1, 10, 100, 1000, 10000, 100000, 1000000
    };

    return time[value];
}


int NumberPeriods::ToAbs() const
{
    static const int abs[Count] =
    {
        1, 10, 100, 1000, 10000, 100000, 1000000
    };

    return abs[value];
}


TimeMeasure &TimeMeasure::Current()
{
    static TimeMeasure *const times[Channel::Count] =
    {
        &PageModesA::timeMeasure,
        &PageModesB::timeMeasure,
        &PageModesC::timeMeasure,
        &PageModesD::timeMeasure
    };

    return *times[CURRENT_CHANNEL];
}


NumberPeriods &NumberPeriods::Current()
{
    static NumberPeriods empty(NumberPeriods::Count);

    static NumberPeriods *const numbers[Channel::Count] =
    {
        &PageModesA::numberPeriods,
        &PageModesB::numberPeriods,
        &PageModesC::numberPeriods,
        &empty
    };

    return *numbers[CURRENT_CHANNEL];
}


ModeMeasureCountPulse &ModeMeasureCountPulse::Current()
{
    static const bool correct[1] = { true };
    static ModeMeasureCountPulse empty(ModeMeasureCountPulse::Count, correct, 1);

    static ModeMeasureCountPulse *const modes[Channel::Count] =
    {
        &PageModesA::modeMeasureCountPulse,
        &PageModesB::modeMeasureCountPulse,
        &PageModesC::modeMeasureCountPulse,
        &empty
    };

    return *modes[CURRENT_CHANNEL];
}


TypeMeasure &TypeMeasure::Current()
{
    static TypeMeasure *const types[TypeMeasure::Count] =
    {
        &PageModesA::typeMeasure,
        &PageModesB::typeMeasure,
        &PageModesC::typeMeasure,
        &PageModesD::typeMeasure
    };

    return *types[CURRENT_CHANNEL];
}


ModeMeasureFrequency &ModeMeasureFrequency::Current()
{
    ModeMeasureFrequency *const modes[Channel::Count] =
    {
        &PageModesA::modeMeasureFrequency,
        &PageModesB::modeMeasureFrequency,
        &PageModesC::modeMeasureFrequency,
        &PageModesD::modeMeasureFrequency
    };

    return *modes[CURRENT_CHANNEL];
}


ModeMeasurePeriod &ModeMeasurePeriod::Current()
{
    static ModeMeasurePeriod empty(ModeMeasurePeriod::Count);

    static ModeMeasurePeriod *const modes[Channel::Count] =
    {
        &PageModesA::modeMeasurePeriod,
        &PageModesB::modeMeasurePeriod,
        &empty,
        &empty
    };

    return *modes[CURRENT_CHANNEL];
}


ModeMeasureDuration &ModeMeasureDuration::Current()
{
    static ModeMeasureDuration empty(ModeMeasureDuration::Count);

    static ModeMeasureDuration *const modes[Channel::Count] =
    {
        &PageModesA::modeMeasureDuration,
        &PageModesB::modeMeasureDuration,
        &empty,
        &empty
    };

    return *modes[CURRENT_CHANNEL];
}


PeriodTimeLabels &PeriodTimeLabels::Current()
{
    static PeriodTimeLabels empty(PeriodTimeLabels::Count);

    static PeriodTimeLabels *const periods[Channel::Count] =
    {
        &PageModesA::timeLabels,
        &PageModesB::timeLabels,
        &PageModesC::timeLabels,
        &empty
    };

    return *periods[CURRENT_CHANNEL];
}

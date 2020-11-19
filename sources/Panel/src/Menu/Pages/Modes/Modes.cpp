#include "Settings.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/Pages/Modes/PagesModes.h"


uint TimeMeasure::ProgressBar::timeStart = 0U;

PeriodTimeLabels PageModes::timeLabels(PeriodTimeLabels::T_8);
NumberPeriods    PageModes::numberPeriods(NumberPeriods::_1);
TimeMeasure      PageModes::timeMeasure(TimeMeasure::_1ms);


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
        static Enumeration *const modesC[4] =
        {
            &PageModesC::modeMeasureFrequency,
            nullptr,
            nullptr,
            &PageModesC::modeMeasureCountPulse
        };

        return *modesC[PageModesC::typeMeasure.value];;
    }

    return PageModesD::modeMeasureFrequency;
}


void CurrentModeMeasure::Draw(int x, int y)
{
    int width = 120;

    static Enumeration &e = ToEnumeration();

    String text = ToEnumeration().ToString();

    if (Font::GetLengthText(text.c_str()) > 110)
    {
        width = Font::GetLengthText(text.c_str()) + 18;
    }

    Primitives::Rectangle(width, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::WHITE);

    Text(text).Write(x + 10, y + 10, Color::WHITE);
}


void TypeMeasure::Draw(int x, int y) const
{
    int width = 100;

    Primitives::Rectangle(width, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::WHITE);

    Text(ToString()).Write(x, y + 10, width, Color::WHITE);
}


Page *PageModes::Current()
{
    return PageForChannel(CURRENT_CHANNEL);
}


Page *PageModes::PageForChannel(Channel::E channel)
{
    Page *pages[Channel::Count] =
    {
        PageModesA::self,
        PageModesB::self,
        PageModesC::self,
        PageModesD::self
    };

    return pages[channel];
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

    return TypeMeasure::Current().IsCountPulse() ? *modes[CURRENT_CHANNEL] : empty;
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
    static bool current = true;
    static ModeMeasureFrequency empty(ModeMeasureFrequency::Count, &current, 1);

    ModeMeasureFrequency *const modes[Channel::Count] =
    {
        &PageModesA::modeMeasureFrequency,
        &PageModesB::modeMeasureFrequency,
        &PageModesC::modeMeasureFrequency,
        &PageModesD::modeMeasureFrequency
    };

    return TypeMeasure::Current().IsFrequency() ? *modes[CURRENT_CHANNEL] : empty;
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

    return TypeMeasure::Current().IsPeriod() ? *modes[CURRENT_CHANNEL] : empty;
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

    return TypeMeasure::Current().IsDuration() ? *modes[CURRENT_CHANNEL] : empty;
}


void TimeMeasure::ProgressBar::Draw(int x, int y)
{
    if (IsDrawable())
    {
        int width = 46;

        int timeCycle = PageModes::timeMeasure.ToMS();

        float part = ((float)(TIME_MS - timeStart) / (float)timeCycle);

        Primitives::Rectangle((int)((float)width * part) + 1, 5).Fill(x, y, Color::WHITE);
    }
}


bool TimeMeasure::ProgressBar::IsDrawable()
{
    return CurrentPageModes::ConsistTimeMeasure() && PageModes::timeMeasure.value > TimeMeasure::_100ms;
}

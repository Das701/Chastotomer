#include "Settings.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/FPGA.h"
#include "Hardware/MathFPGA.h"
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
    return Channel::PageForChannel(CURRENT_CHANNEL);
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


void TimeMeasure::Set(TimeMeasure::E v)
{
    Channel::timeMeasure.value = (uint8)v;
    LoadToFPGA();
}


int NumberPeriods::ToAbs() const
{
    static const int abs[Count] =
    {
        1, 10, 100, 1000, 10000, 100000, 1000000
    };

    return abs[value];
}


const ModeMeasureCountPulse &ModeMeasureCountPulse::Current()
{
    static const bool correct[1] = { true };
    static const ModeMeasureCountPulse null(ModeMeasureCountPulse::Count, correct, 1);

    static const ModeMeasureCountPulse *modes[Channel::Count] =
    {
        &PageModesA::modeMeasureCountPulse,
        &PageModesB::modeMeasureCountPulse,
        &PageModesC::modeMeasureCountPulse,
        &null
    };

    return TypeMeasure::Current().IsCountPulse() ? *modes[CURRENT_CHANNEL] : null;
}


const TypeMeasure &TypeMeasure::Current()
{
    static TypeMeasure *const types[Channel::Count] =
    {
        &PageModesA::typeMeasure,
        &PageModesB::typeMeasure,
        &PageModesC::typeMeasure,
        &PageModesD::typeMeasure
    };

    return *types[CURRENT_CHANNEL];
}


const ModeMeasureFrequency &ModeMeasureFrequency::Current()
{
    static bool current = true;
    static const ModeMeasureFrequency null(ModeMeasureFrequency::Count, &current, 1);

    ModeMeasureFrequency *const modes[Channel::Count] =
    {
        &PageModesA::modeMeasureFrequency,
        &PageModesB::modeMeasureFrequency,
        &PageModesC::modeMeasureFrequency,
        &PageModesD::modeMeasureFrequency
    };

    return TypeMeasure::Current().IsFrequency() ? *modes[CURRENT_CHANNEL] : null;
}


const ModeMeasurePeriod &ModeMeasurePeriod::Current()
{
    static const ModeMeasurePeriod null(ModeMeasurePeriod::Count);

    static const ModeMeasurePeriod *modes[Channel::Count] =
    {
        &PageModesA::modeMeasurePeriod,
        &PageModesB::modeMeasurePeriod,
        &null,
        &null
    };

    return TypeMeasure::Current().IsPeriod() ? *modes[CURRENT_CHANNEL] : null;
}


const ModeMeasureDuration &ModeMeasureDuration::Current()
{
    static ModeMeasureDuration null(ModeMeasureDuration::Count);

    static ModeMeasureDuration *const modes[Channel::Count] =
    {
        &PageModesA::modeMeasureDuration,
        &PageModesB::modeMeasureDuration,
        &null,
        &null
    };

    return TypeMeasure::Current().IsDuration() ? *modes[CURRENT_CHANNEL] : null;
}


void ModeMeasurePeriod::LoadToFPGA()
{
    Command command(Command::ModePeriod);

    command.SetBit(5);

    if (ModeMeasurePeriod::Current().IsF_1())
    {
        command.SetBit(9);
    }

    FPGA::WriteCommand(command);

    MathFPGA::Validator::SetInvalidData();
}


void ModeMeasureDuration::LoadToFPGA()
{
    Command command(Command::ModeDuration);

    command.SetBit(4);

    if (Current().IsNdt_1ns())
    {
        command.SetBit(8);
    }
    else if (Current().IsStartStop())
    {
        command.SetBit(9);
        command.SetBit(8);
    }
    else if (Current().IsFillFactor())
    {
        command.SetBit(7);
    }
    else if (Current().IsPhase())
    {
        command.SetBit(9);
        command.SetBit(7);
    }

    FPGA::WriteCommand(command);

    MathFPGA::Validator::SetInvalidData();
}

#include "Settings.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/FPGA.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Pages/Modes/PagesModes.h"


bool ModeCountPulse::IsFromPeriod() const
{
    return (value == ATB) || (value == BTA) || (value == CTA) || (value == CTB);
}


Enumeration &CurrentModeMeasure::ToEnumeration()
{
    if (CURRENT_CHANNEL_IS_A)
    {
        static Enumeration *const modes[4] =
        {
            &Channel::A.modeFrequency,
            &Channel::A.modePeriod,
            &Channel::A.modeDuration,
            &Channel::A.modeMeasureCountPulse
        };

        return *modes[Channel::A.typeMeasure.value];
    }
    else if (CURRENT_CHANNEL_IS_B)
    {
        static Enumeration *const modesB[4] =
        {
            &Channel::B.modeFrequency,
            &Channel::B.modePeriod,
            &Channel::B.modeDuration,
            &Channel::B.modeMeasureCountPulse
        };

        return *modesB[Channel::B.typeMeasure.value];
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        static Enumeration *const modesC[4] =
        {
            &Channel::C.modeFrequency,
            nullptr,
            nullptr,
            &Channel::C.modeMeasureCountPulse
        };

        return *modesC[Channel::C.typeMeasure.value];;
    }

    return Channel::D.modeFrequency;
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


bool ModeFrequency::IsRatio() const
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


const ModeCountPulse &ModeCountPulse::Current()
{
    static const bool correct[1] = { true };
    static const ModeCountPulse null(ModeCountPulse::Count, correct, 1);

    static const ModeCountPulse *modes[Channel::Count] =
    {
        &Channel::A.modeMeasureCountPulse,
        &Channel::B.modeMeasureCountPulse,
        &Channel::C.modeMeasureCountPulse,
        &null
    };

    return Channel::Current().typeMeasure.IsCountPulse() ? *modes[CURRENT_CHANNEL] : null;
}


const ModeFrequency &ModeFrequency::Current()
{
    static bool current = true;
    static const ModeFrequency null(ModeFrequency::Count, &current, 1);

    ModeFrequency *const modes[Channel::Count] =
    {
        &Channel::A.modeFrequency,
        &Channel::B.modeFrequency,
        &Channel::C.modeFrequency,
        &Channel::D.modeFrequency
    };

    return Channel::Current().typeMeasure.IsFrequency() ? *modes[CURRENT_CHANNEL] : null;
}


const ModePeriod &ModePeriod::Current()
{
    static const ModePeriod null(ModePeriod::Count);

    static const ModePeriod *modes[Channel::Count] =
    {
        &Channel::A.modePeriod,
        &Channel::B.modePeriod,
        &null,
        &null
    };

    return Channel::Current().typeMeasure.IsPeriod() ? *modes[CURRENT_CHANNEL] : null;
}


const ModeDuration &ModeDuration::Current()
{
    static ModeDuration null(ModeDuration::Count);

    static ModeDuration *const modes[Channel::Count] =
    {
        &Channel::A.modeDuration,
        &Channel::B.modeDuration,
        &null,
        &null
    };

    return Channel::Current().typeMeasure.IsDuration() ? *modes[CURRENT_CHANNEL] : null;
}


void ModePeriod::LoadToFPGA()
{
    Command command(Command::ModePeriod);

    command.SetBit(5);

    if (ModePeriod::Current().IsF_1())
    {
        command.SetBit(9);
    }

    FPGA::WriteCommand(command);

    MathFPGA::Validator::SetInvalidData();
}


void ModeDuration::LoadToFPGA()
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

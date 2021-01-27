#include "Settings.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/FPGA.h"
#include "Hardware/MathFPGA.h"


bool ModeStartStop::enabled = false;
bool Relation::enabled = false;


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
            &Channel::A->mod.modeFrequency,
            &Channel::A->mod.modePeriod,
            &Channel::A->mod.modeDuration,
            &Channel::A->mod.modeCountPulse
        };

        return *modes[Channel::A->mod.typeMeasure.value];
    }
    else if (CURRENT_CHANNEL_IS_B)
    {
        static Enumeration *const modesB[4] =
        {
            &Channel::B->mod.modeFrequency,
            &Channel::B->mod.modePeriod,
            &Channel::B->mod.modeDuration,
            &Channel::B->mod.modeCountPulse
        };

        return *modesB[Channel::B->mod.typeMeasure.value];
    }
    else if (CURRENT_CHANNEL_IS_C) //-V2516
    {
        static Enumeration *const modesC[4] =
        {
            &Channel::C->mod.modeFrequency,
            nullptr,
            nullptr,
            &Channel::C->mod.modeCountPulse
        };

        return *modesC[Channel::C->mod.typeMeasure.value];;
    }

    return Channel::D->mod.modeFrequency;
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
    ModesChannel::timeMeasure.value = (uint8)v;
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
        &Channel::A->mod.modeCountPulse,
        &Channel::B->mod.modeCountPulse,
        &Channel::C->mod.modeCountPulse,
        &null
    };

    return Channel::Current()->mod.typeMeasure.IsCountPulse() ? *modes[NUMBER_CURRENT_CHANNEL] : null;
}


const ModeFrequency &ModeFrequency::Current()
{
    static bool current = true;
    static const ModeFrequency null(ModeFrequency::Count, &current, 1);

    ModeFrequency *const modes[Channel::Count] =
    {
        &Channel::A->mod.modeFrequency,
        &Channel::B->mod.modeFrequency,
        &Channel::C->mod.modeFrequency,
        &Channel::D->mod.modeFrequency
    };

    return Channel::Current()->mod.typeMeasure.IsFrequency() ? *modes[NUMBER_CURRENT_CHANNEL] : null;
}


const ModePeriod &ModePeriod::Current()
{
    static const ModePeriod null(ModePeriod::Count);

    static const ModePeriod *modes[Channel::Count] =
    {
        &Channel::A->mod.modePeriod,
        &Channel::B->mod.modePeriod,
        &null,
        &null
    };

    return Channel::Current()->mod.typeMeasure.IsPeriod() ? *modes[NUMBER_CURRENT_CHANNEL] : null;
}


const ModeDuration &ModeDuration::Current()
{
    static ModeDuration null(ModeDuration::Count);

    static ModeDuration *const modes[Channel::Count] =
    {
        &Channel::A->mod.modeDuration,
        &Channel::B->mod.modeDuration,
        &null,
        &null
    };

    return Channel::Current()->mod.typeMeasure.IsDuration() ? *modes[NUMBER_CURRENT_CHANNEL] : null;
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
    else if (Current().IsPhase()) //-V2516
    {
        command.SetBit(9);
        command.SetBit(7);
    }

    FPGA::WriteCommand(command);

    MathFPGA::Validator::SetInvalidData();
}


void ModeStartStop::Toggle()
{
    if (!enabled)
    {
        enabled = true;
    }
    else
    {
        enabled = false;
    }
}


void Relation::On()
{
    enabled = true;
}


void Relation::Off()
{
    enabled = false;
}


bool Relation::IsEnabled()
{
    return enabled;
}

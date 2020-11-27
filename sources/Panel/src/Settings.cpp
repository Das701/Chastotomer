#include "defines.h"
#include "Settings.h"
#include "Hardware/FPGA.h"
#include "Menu/Pages/Settings/PagesSettings.h"
#include "Utils/Math.h"


Settings set =
{
    Channel::A,                                 // currentChannel
    {
        {0, 1}, {0, 1}                          // levelSynch
    },
    { 
        TypeSynch::Manual, TypeSynch::Manual    // typeSynch
    },
    false
};


#define DEFINE_ARGUMENT char argument[6] = {0, 0, 0, 0, 0, 0}


void InputCouple::LoadToFPGA()
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        Command command(Command::Couple);

        if (Current().IsDC())
        {
            command.SetBit(9);
        }

        FPGA::WriteCommand(command);
    }
}


const InputCouple &InputCouple::Current()
{
    static const InputCouple null(Count);

    static const InputCouple *inputs[Channel::Count] = { &PageSettingsA::couple, &PageSettingsB::couple, &null, &null };

    return *inputs[CURRENT_CHANNEL];
}


void InputCouple::Set(InputCouple::E v)
{
    static InputCouple null(Count);

    InputCouple *inputs[Channel::Count] = { &PageSettingsA::couple, &PageSettingsB::couple, &null, &null };

    inputs[CURRENT_CHANNEL]->value = (uint8)v;

    LoadToFPGA();
}


const ModeFilter &ModeFilter::Current()
{
    static const ModeFilter null(Count);

    static const ModeFilter *modes[Channel::Count] = { &PageSettingsA::modeFilter, &PageSettingsB::modeFilter, &null, &null };

    return *modes[CURRENT_CHANNEL];
}


void LevelSynch::Change(int delta)
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        int prev = LEVEL_SYNCH(CURRENT_CHANNEL);

        LEVEL_SYNCH(CURRENT_CHANNEL) += delta;

        int MIN = -800;
        int MAX = 800;

        if (TypeSynch::Current().IsHoldoff())
        {
            MIN = 1;
        }

        LIMITATION(LEVEL_SYNCH(CURRENT_CHANNEL), MIN, MAX);

        if (prev != LEVEL_SYNCH(CURRENT_CHANNEL))
        {
            if (delta < 0) { FPGA::DecreaseN(); }
            else           { FPGA::IncreaseN(); }

            FPGA::WriteDataGovernor();
        }
    }
}


const ModeFront &ModeFront::Current()
{
    static const ModeFront null(Count);

    static const ModeFront *modes[Channel::Count] = { &PageSettingsA::modeFront, &PageSettingsB::modeFront, &null, &null };

    return *modes[CURRENT_CHANNEL];
}


void ModeFront::LoadToFPGA()
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        char command[4] = { 0, 1, 0, 0 };

        DEFINE_ARGUMENT;

        if (!Current().IsFront())
        {
            argument[5] = 1;
        }

        FPGA::WriteCommand(command, argument);
    }
}


void TypeSynch::LoadToFPGA()
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        char command[4] = { 1, 1, 0, 1 };

        DEFINE_ARGUMENT;

        if (TypeSynch::Current().IsHoldoff())
        {
            argument[5] = 1;
        }

        FPGA::WriteCommand(command, argument);
    }
}


const TypeSynch &TypeSynch::Current()
{
    static const TypeSynch null(Count);

    static const TypeSynch *types[Channel::Count] = { &PageSettingsA::typeSynch, &PageSettingsB::typeSynch, &null, &null };

    return *types[CURRENT_CHANNEL];
}


void ModeFilter::Set(ModeFilter::E v)
{
    static ModeFilter null(ModeFilter::Count);

    static ModeFilter *modes[Channel::Count] = { &PageSettingsA::modeFilter, &PageSettingsB::modeFilter, &null, &null };

    modes[CURRENT_CHANNEL]->value = (uint8)v;

    LoadToFPGA();
}


void ModeFilter::LoadToFPGA()
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        char command[4] = { 0, 1, 0, 1 };

        DEFINE_ARGUMENT;

        if (Current().IsOff())
        {
            argument[5] = 1;
        }

        FPGA::WriteCommand(command, argument);
    }
}


const Divider &Divider::Current()
{
    static const Divider null(Count);

    static const Divider *dividers[Channel::Count] = { &PageSettingsA::divider, &PageSettingsB::divider, &null, &null };

    return *dividers[CURRENT_CHANNEL];
}


void Divider::LoadToFPGA()
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        char command[4] = { 0, 0, 1, 0 };

        DEFINE_ARGUMENT;

        if (!Divider::Current().Is1())
        {
            argument[5] = 1;
        }

        FPGA::WriteCommand(command, argument);
    }
}


const InputImpedance &InputImpedance::Current()
{
    static const InputImpedance null(Count);

    static const InputImpedance *impedances[Channel::Count] = { &PageSettingsA::impedance, &PageSettingsB::impedance, &null, &null };

    return *impedances[CURRENT_CHANNEL];
}

#include "defines.h"
#include "Settings.h"
#include "Hardware/FPGA.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Utils/Math.h"


Settings gset =
{
    {
        {0, 1}, {0, 1}                          // levelSynch
    },
    { 
        TypeSynch::Manual, TypeSynch::Manual    // typeSynch
    },
    false,
    StyleGUI::Modern
}; 


void InputCouple::LoadToFPGA()
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        Command command(Command::Couple);

        if (Channel::Current()->set.couple.IsDC())
        {
            command.SetBit(9);
        }

        FPGA::WriteCommand(command);
    }
}


void InputCouple::Set(InputCouple::E v)
{
    value = (uint8)v;

    LoadToFPGA();
}


void LevelSynch::Change(int delta)
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        int prev = LEVEL_SYNCH(Channel::Current());

        LEVEL_SYNCH(CURRENT_CHANNEL) += delta;

        int MIN = -800;
        int MAX = 800;

        if (Channel::Current()->set.typeSynch.IsHoldoff())
        {
            MIN = 1;
        }

        LIMITATION(LEVEL_SYNCH(CURRENT_CHANNEL), MIN, MAX); //-V2516

        if (prev != LEVEL_SYNCH(CURRENT_CHANNEL))
        {
            if (delta < 0) { FPGA::GovernorData::DecreaseN(); }
            else           { FPGA::GovernorData::IncreaseN(); }

            FPGA::GovernorData::Write();
        }
    }
}


void ModeFront::LoadToFPGA()
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        Command command(Command::ModeFront);

        if (!Channel::Current()->set.modeFront.IsFront())
        {
            command.SetBit(9);
        }

        FPGA::WriteCommand(command);
    }
}


void TypeSynch::LoadToFPGA()
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        Command command(Command::TypeSynch);

        if (Channel::Current()->set.typeSynch.IsHoldoff())
        {
            command.SetBit(9);
        }

        FPGA::WriteCommand(command);
    }
}


void ModeFilter::Set(ModeFilter::E v)
{
    value = (uint8)v;

    LoadToFPGA();
}


void InputImpedance::Set(E v)
{
    value = (uint8)v;

    LoadToFPGA();
}


void ModeFilter::LoadToFPGA()
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        Command command(Command::ModeFilter);

        if (Channel::Current()->set.modeFilter.IsOff())
        {
            command.SetBit(9);
        }

        FPGA::WriteCommand(command);
    }
}


void Divider::LoadToFPGA()
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        Command command(Command::Divider);

        if (!Channel::Current()->set.divider.Is1())
        {
            command.SetBit(9);
        }

        FPGA::WriteCommand(command);
    }
}

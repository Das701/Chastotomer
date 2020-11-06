#include "defines.h"
#include "Settings.h"
#include "Hardware/FPGA.h"
#include "Menu/Pages/PagesSettings.h"
#include "Utils/Math.h"


Settings set =
{
    Channel::A,                         // currentChannel
    {
        {0, 1}, {0, 1}    // levelSynch
    },
    { 
        TypeSynch::Manual, TypeSynch::Manual  // typeSynch
    }
};


#define DEFINE_ARGUMENT char argument[6] = {0, 0, 0, 0, 0, 0}


void InputCouple::Load()
{
    char command[4] = { 0, 0, 1, 1 };

    DEFINE_ARGUMENT;

    if(Current().IsDC())
    {
        argument[5] = 1;
    }

    FPGA::WriteCommand(command, argument);
}


InputCouple &InputCouple::Current()
{
    if(CURRENT_CHANNEL_IS_A)
    {
        return PageSettingsA::couple;
    }
    else if(CURRENT_CHANNEL_IS_B)
    {
        return PageSettingsB::couple;
    }
    
    static InputCouple couple(InputCouple::AC);
    
    return couple;
}


void InputCouple::Set(InputCouple::E v)
{
    if(CURRENT_CHANNEL_IS_A)
    {
        PageSettingsA::couple.value = (uint8)v;
        Load();
    }
    else if(CURRENT_CHANNEL_IS_B)
    {
        PageSettingsB::couple.value = (uint8)v;
        Load();
    }
}


void ModeFilter::Load()
{
    char command[4] = { 0, 1, 0, 1 };

    DEFINE_ARGUMENT;

    if(Current().IsOff())
    {
        argument[5] = 1;
    }

    FPGA::WriteCommand(command, argument);
}


ModeFilter &ModeFilter::Current()
{
    if(CURRENT_CHANNEL_IS_A)
    {
        return PageSettingsA::modeFilter;
    }
    else if(CURRENT_CHANNEL_IS_B)
    {
        return PageSettingsB::modeFilter;
    }
    
    static ModeFilter mode(ModeFilter::Off);
    
    return mode;
}


void ModeFilter::Set(ModeFilter::E v)
{
    if(CURRENT_CHANNEL_IS_A)
    {
        PageSettingsA::modeFilter.value = (uint8)v;
        Load();
    }
    else if(CURRENT_CHANNEL_IS_B)
    {
        PageSettingsB::modeFilter.value = (uint8)v;
        Load();
    }
}


void LevelSynch::Change(Channel::E ch, int delta)
{
    LEVEL_SYNCH(ch) += delta;

    LIMITATION(LEVEL_SYNCH(ch), MIN, MAX);
}

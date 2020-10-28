#include "defines.h"
#include "Settings.h"
#include "Hardware/HAL.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageChannelB.h"


Settings set =
{
    Channel::A,                         ///< currentChannel
    {
        {0, 1}, {0, 1}    ///< levelSynch
    },
    { 
        TypeSynch::Manual, TypeSynch::Manual  ///< typeSynch
    }
};


#define DEFINE_ARGUMENT char argument[6] = {0, 0, 0, 0, 0, 0}


void InputCouple::Load()
{
    char command[4] = { 0, 0, 1, 1 };

    DEFINE_ARGUMENT;

    if(Current() == InputCouple::DC)
    {
        argument[5] = 1;
    }

    PLIS::WriteCommand(command, argument);
}


InputCouple::E InputCouple::Current()
{
    if(CURRENT_CHANNEL_IS_A)
    {
        return (InputCouple::E)PageChannelA::couple.value;
    }
    else if(CURRENT_CHANNEL_IS_B)
    {
        return (InputCouple::E)PageChannelB::couple.value;
    }
    
    return InputCouple::AC;
}


void InputCouple::Set(InputCouple::E v)
{
    if(CURRENT_CHANNEL_IS_A)
    {
        PageChannelA::couple.value = v;
        Load();
    }
    else if(CURRENT_CHANNEL_IS_B)
    {
        PageChannelB::couple.value = v;
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

    PLIS::WriteCommand(command, argument);
}


ModeFilter &ModeFilter::Current()
{
    if(CURRENT_CHANNEL_IS_A)
    {
        return PageChannelA::modeFilter;
    }
    else if(CURRENT_CHANNEL_IS_B)
    {
        return PageChannelB::modeFilter;
    }
    
    static ModeFilter mode(ModeFilter::Off);
    
    return mode;
}


void ModeFilter::Set(ModeFilter::E v)
{
    if(CURRENT_CHANNEL_IS_A)
    {
        PageChannelA::modeFilter.value = v;
        Load();
    }
    else if(CURRENT_CHANNEL_IS_B)
    {
        PageChannelB::modeFilter.value = v;
        Load();
    }
}

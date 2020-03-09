#include "defines.h"
#include "FreqMeter.h"
#include "Menu/MenuItems.h"
#include "Hardware/HAL.h"
#include "Menu/Pages/PageModes.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageChannelB.h"
#include "Menu/Pages/PageChannelC.h"
#include "Menu/Pages/PageChannelD.h"
#include "Menu/Hint.h"



static char argument[6] = { 0, 0, 0, 0, 0, 0 };

#define DEFINE_ARGUMENT char argument[6] = {0, 0, 0, 0, 0, 0}


int channel = 1;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::UsedChannel(int channelNumber)
{
    channel = channelNumber;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ClearArgument()
{
    argument[0] = 0;
    argument[1] = 0;
    argument[2] = 0;
    argument[3] = 0;
    argument[4] = 0;
    argument[5] = 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadChannel()
{
    char command[4] = { 0, 0, 0, 0 };

    DEFINE_ARGUMENT;

    if(channel == 2)
    {
        argument[1] = 1;
    }
    else if(channel == 3)
    {
        argument[0] = 1;
    }
    
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadDisplayTime()
{
    char command[4] = { 1, 0, 1, 1 };
    ClearArgument();
    switch (PageIndication::displayTime)
    {
        case DisplayTime::_100ms:
            argument[0] = 0;
            argument[1] = 0;
            break;
        case DisplayTime::_1s:
            argument[0] = 1;
            argument[1] = 0;
            break;
        case DisplayTime::_10s:
            argument[0] = 0;
            argument[1] = 1;
            break;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadRefGenerator()
{
    char command[4] = { 1, 0, 0, 1 };
    ClearArgument();
    if(PageIndication::refGenerator == RefGenerator::Internal)
    {
        argument[0] = 0;
    }
    else
    {
        argument[0] = 1;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadLaunchSource()
{
    char command[4] = { 1, 0, 1, 0 };
    ClearArgument();
    switch (PageIndication::launchSource)
    {
        case LaunchSource::Internal:
            argument[0] = 0;
            argument[1] = 0;
            break;
        case LaunchSource::External:
            argument[0] = 1;
            argument[1] = 0;
            break;
        case LaunchSource::OneTime:
            argument[0] = 0;
            argument[1] = 1;
            break;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadCalibration()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadMemoryMode()
{
    char command[4] = { 1, 1, 0, 0 };
    ClearArgument();
    if(PageIndication::memoryMode == MemoryMode::Off)
    {
        argument[0] = 0;
    }
    else
    {
        argument[0] = 1;
    }
    PLIS::WriteCommand(command, argument);
}

void FreqMeter::LoadModeMeasureFrequency()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadModeMeasurePeriod()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadModeMeasureDuration()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadModeMeasureCountPulse()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadPeriodTimeLabels()
{
    char command[4] = { 0, 0, 0, 1 };
    ClearArgument();
    switch (PageModes::periodTimeLabels)
    {
        case PeriodTimeLabels::T_8:
            argument[0] = 0;
            argument[1] = 0;
            argument[2] = 0;
            break;
        case PeriodTimeLabels::T_7:
            argument[0] = 1;
            argument[1] = 0;
            argument[2] = 0;
            break;
        case PeriodTimeLabels::T_6:
            argument[0] = 0;
            argument[1] = 1;
            argument[2] = 0;
            break;
        case PeriodTimeLabels::T_5:
            argument[0] = 1;
            argument[1] = 1;
            argument[2] = 0;
            break;
        case PeriodTimeLabels::T_4:
            argument[0] = 0;
            argument[1] = 0;
            argument[2] = 1;
            break;
        case PeriodTimeLabels::T_3:
            argument[0] = 1;
            argument[1] = 0;
            argument[2] = 1;
            break;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadTimeMeasure()
{
    char command[4] = { 1, 1, 1, 0 };
    ClearArgument();
    switch (PageModes::timeMeasure)
    {
        case TimeMeasure::_1ms:
            argument[0] = 0;
            argument[1] = 0;
            argument[2] = 0;
            break;
        case TimeMeasure::_10ms:
            argument[0] = 1;
            argument[1] = 0;
            argument[2] = 0;
            break;
        case TimeMeasure::_100ms:
            argument[0] = 0;
            argument[1] = 1;
            argument[2] = 0;
            break;
        case TimeMeasure::_1s:
            argument[0] = 1;
            argument[1] = 1;
            argument[2] = 0;
            break;
        case TimeMeasure::_10s:
            argument[0] = 0;
            argument[1] = 0;
            argument[2] = 1;
            break;
        case TimeMeasure::_100s:
            argument[0] = 1;
            argument[1] = 0;
            argument[2] = 1;
            break;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadNumerPeriodsMeasure()
{
    char command[4] = { 0, 0, 0, 1 };
    ClearArgument();
    switch (PageModes::numberPeriods)
    {
        case NumberPeriods::_1:
            argument[0] = 0;
            argument[1] = 0;
            argument[2] = 0;
            break;
        case NumberPeriods::_10:
            argument[0] = 1;
            argument[1] = 0;
            argument[2] = 0;
            break;
        case NumberPeriods::_100:
            argument[0] = 0;
            argument[1] = 1;
            argument[2] = 0;
            break;
        case NumberPeriods::_1K:
            argument[0] = 1;
            argument[1] = 1;
            argument[2] = 0;
            break;
        case NumberPeriods::_10K:
            argument[0] = 0;
            argument[1] = 0;
            argument[2] = 1;
            break;
        case NumberPeriods::_100K:
            argument[0] = 1;
            argument[1] = 0;
            argument[2] = 1;
            break;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadInputCouple()
{
    char command[4] = { 1, 1, 0, 0 };
    ClearArgument();
    if((PageChannelA::couple == InputCouple::AC && channel == 1) | (PageChannelB::couple == InputCouple::AC && channel == 2))
    {
        argument[0] = 0;
    }
    else
    {
        argument[0] = 1;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadImpedance()
{
    char command[4] = { 1, 0, 0, 0 };
    ClearArgument();
    if((PageChannelA::impedance == InputImpedance::_1MOmh && channel == 1) | (PageChannelB::impedance == InputImpedance::_1MOmh && channel == 2))
    {
        argument[0] = 0;
    }
    else
    {
        argument[0] = 1;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadModeFilter()
{
    char command[4] = { 1, 0, 1, 0 };
    ClearArgument();
    if((PageChannelA::modeFilter == ModeFilter::On && channel == 1) | (PageChannelB::modeFilter == ModeFilter::On && channel == 2))
    {
        argument[0] = 0;
    }
    else
    {
        argument[0] = 1;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadModeFront()
{
    char command[4] = { 0, 0, 1, 0 };
    ClearArgument();
    if((PageChannelA::modeFront == ModeFront::Front && channel == 1) | (PageChannelB::modeFront == ModeFront::Front && channel == 2))
    {
        argument[0] = 0;
    }
    else
    {
        argument[0] = 1;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadDivider()
{
    char command[4] = { 0, 1, 0, 0 };
    ClearArgument();
    if((PageChannelA::divider == Divider::_1 && channel == 1) | (PageChannelB::divider == Divider::_1 && channel == 2))
    {
        argument[0] = 0;
    }
    else
    {
        argument[0] = 1;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadTypeSynch()
{
    char command[4] = { 1, 1, 0, 1 };
    ClearArgument();
    if((PageChannelA::typeSynch == TypeSynch::TTL && channel == 1) | (PageChannelB::typeSynch == TypeSynch::TTL && channel == 2))
    {
        argument[0] = 0;
    }
    else
    {
        argument[0] = 1;
    }
    PLIS::WriteCommand(command, argument);
}

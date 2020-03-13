#include "defines.h"
#include "FreqMeter.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Hardware/HAL.h"
#include "Menu/Pages/PageModes.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageChannelB.h"
#include "Menu/Pages/PageChannelC.h"
#include "Menu/Pages/PageChannelD.h"
#include "Menu/Hint.h"


#define DEFINE_ARGUMENT char argument[6] = {0, 0, 0, 0, 0, 0}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadChannel()
{
    char command[4] = { 0, 0, 0, 0 };

    DEFINE_ARGUMENT;

    if(Menu::UsedPage() == PageChannelB::self)
    {
        argument[1] = 1;
    }
    else if(Menu::UsedPage() == PageChannelC::self)
    {
        argument[0] = 1;
    }
    
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadDisplayTime()
{
    char command[4] = { 1, 0, 1, 1 };

    DEFINE_ARGUMENT;

    if(PageIndication::displayTime == DisplayTime::_1s)
    {
        argument[0] = 1;
    }
    else if(PageIndication::displayTime == DisplayTime::_10s)
    {
        argument[1] = 1;
    }

    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadRefGenerator()
{
    char command[4] = { 1, 0, 0, 1 };

    DEFINE_ARGUMENT;

    if(PageIndication::refGenerator != RefGenerator::Internal)
    {
        argument[0] = 1;
    }

    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadLaunchSource()
{
    char command[4] = { 1, 0, 1, 0 };

    DEFINE_ARGUMENT;

    if(PageIndication::launchSource == LaunchSource::External)
    {
        argument[0] = 1;
    }
    else if(PageIndication::launchSource == LaunchSource::OneTime)
    {
        argument[1] = 1;
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

    DEFINE_ARGUMENT;

    if(PageIndication::memoryMode == MemoryMode::On)
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

    DEFINE_ARGUMENT;

    if(PageModes::periodTimeLabels == PeriodTimeLabels::T_7)
    {
        argument[0] = 1;
    }
    else if(PageModes::periodTimeLabels == PeriodTimeLabels::T_6)
    {
        argument[1] = 1;
    }
    else if(PageModes::periodTimeLabels == PeriodTimeLabels::T_5)
    {
        argument[0] = 1;
        argument[1] = 1;
    }
    else if(PageModes::periodTimeLabels == PeriodTimeLabels::T_4)
    {
        argument[2] = 1;
    }
    else if(PageModes::periodTimeLabels == PeriodTimeLabels::T_3)
    {
        argument[0] = 1;
        argument[2] = 1;
    }

    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadTimeMeasure()
{
    char command[4] = { 1, 1, 1, 0 };

    DEFINE_ARGUMENT;

    switch(PageModes::timeMeasure)
    {
    case TimeMeasure::_10ms:
        argument[0] = 1;
        break;
    
    case TimeMeasure::_100ms:
        argument[1] = 1;
        break;

    case TimeMeasure::_10s:
        argument[2] = 1;
        break;

    case TimeMeasure::_100s:
        argument[0] = 1;
        argument[2] = 1;
        break;
    }

    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadNumerPeriodsMeasure()
{
    char command[4] = { 0, 0, 0, 1 };

    DEFINE_ARGUMENT;

    switch(PageModes::numberPeriods)
    {
    case NumberPeriods::_10:
        argument[0] = 1;
        break;

    case NumberPeriods::_100:
        argument[1] = 1;
        break;

    case NumberPeriods::_1K:
        argument[0] = 1;
        argument[1] = 1;
        break;

    case NumberPeriods::_10K:
        argument[2] = 1;
        break;

    case NumberPeriods::_100K:
        argument[0] = 1;
        argument[2] = 1;
        break;
    }

    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadInputCouple()
{
    char command[4] = { 1, 1, 0, 0 };

    DEFINE_ARGUMENT;

    if((PageChannelA::couple == InputCouple::AC && Menu::UsedPage() == PageChannelA::self) || (PageChannelB::couple == InputCouple::AC && Menu::UsedPage() == PageChannelB::self))
    {
        
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
    
    DEFINE_ARGUMENT;

    if((PageChannelA::impedance == InputImpedance::_1MOmh && Menu::UsedPage() == PageChannelA::self) || (PageChannelB::impedance == InputImpedance::_1MOmh && Menu::UsedPage() == PageChannelB::self))
    {
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

    DEFINE_ARGUMENT;

    if((PageChannelA::modeFilter == ModeFilter::On && Menu::UsedPage() == PageChannelA::self) || (PageChannelB::modeFilter == ModeFilter::On && Menu::UsedPage() == PageChannelB::self))
    {
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

    DEFINE_ARGUMENT;

    if((PageChannelA::modeFront == ModeFront::Front && Menu::UsedPage() == PageChannelA::self) || (PageChannelB::modeFront == ModeFront::Front && Menu::UsedPage() == PageChannelB::self))
    {
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

    DEFINE_ARGUMENT;

    if((PageChannelA::divider == Divider::_1 && Menu::UsedPage() == PageChannelA::self) || (PageChannelB::divider == Divider::_1 && Menu::UsedPage() == PageChannelB::self))
    {
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

    DEFINE_ARGUMENT;

    if((PageChannelA::typeSynch == TypeSynch::TTL && Menu::UsedPage() == PageChannelA::self) || (PageChannelB::typeSynch == TypeSynch::TTL && Menu::UsedPage() == PageChannelB::self))
    {
    }
    else
    {
        argument[0] = 1;
    }

    PLIS::WriteCommand(command, argument);
}

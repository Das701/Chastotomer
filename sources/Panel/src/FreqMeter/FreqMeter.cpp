#include "defines.h"
#include "FreqMeter.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Hardware/HAL.h"
#include "Settings.h"
#include "Menu/Pages/PageModesA.h"
#include "Menu/Pages/PageModesB.h"
#include "Menu/Pages/PageModesC.h"
#include "Menu/Pages/PageModesD.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageChannelB.h"
#include "Menu/Pages/PageChannelC.h"
#include "Menu/Pages/PageChannelD.h"
#include "Menu/Hint.h"


#define DEFINE_ARGUMENT char argument[6] = {0, 0, 0, 0, 0, 0}

static bool testMode = false;
static bool autoMode = false;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadChannel()
{
    char command[4] = { 0, 0, 0, 0 };

    DEFINE_ARGUMENT;

    if(Menu::UsedPage() == PageChannelB::self)
    {
        argument[4] = 1;
    }
    else if(Menu::UsedPage() == PageChannelC::self)
    {
        argument[5] = 1;
    }
    else if(Menu::UsedPage() == PageChannelD::self)
    {
        argument[5] = 1;
        argument[4] = 1;
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
        argument[5] = 1;
    }
    else if(PageIndication::displayTime == DisplayTime::_10s)
    {
        argument[4] = 1;
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
        argument[5] = 1;
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
        argument[5] = 1;
    }
    else if(PageIndication::launchSource == LaunchSource::OneTime)
    {
        argument[4] = 1;
    }

    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadCalibration()
{
    char command[4] = { 1, 1, 1, 1 };

    DEFINE_ARGUMENT;
    
    if(PageIndication::calibration == Calibration::Pressed)
    {
        argument[4] = 1;
    }
    else
    {
        argument[3] = 1;
    }
    
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadMemoryMode()
{
    char command[4] = { 1, 1, 0, 0 };

    DEFINE_ARGUMENT;

    if(PageIndication::memoryMode == MemoryMode::On)
    {
        argument[5] = 1;
    }

    PLIS::WriteCommand(command, argument);
}

void FreqMeter::LoadModeMeasureFrequency()
{
    char command[4] = { 0, 1, 1, 0 };

    DEFINE_ARGUMENT;

    if((PageModesA::modeMeasureFrequency == ModeMeasureFrequency::Freq && CURRENT_CHANNEL_IS_A) || 
       (PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::Freq && CURRENT_CHANNEL_IS_B) || 
       (PageModesC::modeMeasureFrequencyC == ModeMeasureFrequencyC::Freq && CURRENT_CHANNEL_IS_C))
    {
    }
    else if((PageModesA::modeMeasureFrequency == ModeMeasureFrequency::AB && CURRENT_CHANNEL_IS_A) || 
            (PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::BA && CURRENT_CHANNEL_IS_B) || 
            (PageModesC::modeMeasureFrequencyC == ModeMeasureFrequencyC::CA && CURRENT_CHANNEL_IS_C))
    {
        argument[5] = 1;
    }
    else if((PageModesA::modeMeasureFrequency == ModeMeasureFrequency::AC && CURRENT_CHANNEL_IS_A) || 
            (PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::BC && CURRENT_CHANNEL_IS_B) || 
            (PageModesC::modeMeasureFrequencyC == ModeMeasureFrequencyC::CB && CURRENT_CHANNEL_IS_C))
    {
        argument[4] = 1;
    }
    else if((PageModesA::modeMeasureFrequency == ModeMeasureFrequency::T_1 && CURRENT_CHANNEL_IS_A) || 
            (PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::T_1 && CURRENT_CHANNEL_IS_B))
    {
        argument[5] = 1;
        argument[4] = 1;
    }
    else if((PageModesA::modeMeasureFrequency == ModeMeasureFrequency::Tachometer && CURRENT_CHANNEL_IS_A) || 
            (PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::Tachometer && CURRENT_CHANNEL_IS_B))
    {
        argument[3] = 1;
    }
    else if(PageModesA::modeMeasureFrequency == ModeMeasureFrequency::Comparator && CURRENT_CHANNEL_IS_A)
    {
        argument[3] = 1;
        argument[5] = 1;
    }

    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadModeMeasurePeriod()
{
    char command[4] = { 0, 1, 1, 0 };

    DEFINE_ARGUMENT;

    argument[1] = 1;
    
    if((PageModesA::modeMeasurePeriod == ModeMeasurePeriod::Period && CURRENT_CHANNEL_IS_A) || 
       (PageModesB::modeMeasurePeriodB == ModeMeasurePeriodB::Period && CURRENT_CHANNEL_IS_B))
    {
    }
    else if((PageModesA::modeMeasurePeriod == ModeMeasurePeriod::F_1 && CURRENT_CHANNEL_IS_A) || 
            (PageModesB::modeMeasurePeriodB == ModeMeasurePeriodB::F_1 && CURRENT_CHANNEL_IS_B))
    {
        argument[5] = 1;
    }

    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadModeMeasureDuration()
{
    char command[4] = { 0, 1, 1, 0 };

    DEFINE_ARGUMENT;

    argument[0] = 1;
    
    if((PageModesA::modeMeasureDuration == ModeMeasureDuration::Ndt && CURRENT_CHANNEL_IS_A) || 
       (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Ndt && CURRENT_CHANNEL_IS_B))
    {
    }
    else if((PageModesA::modeMeasureDuration == ModeMeasureDuration::Ndt_1ns && CURRENT_CHANNEL_IS_A) || 
            (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Ndt_1ns && CURRENT_CHANNEL_IS_B))
    {
        argument[4] = 1;
    }
    else if((PageModesA::modeMeasureDuration == ModeMeasureDuration::Ndt2 && CURRENT_CHANNEL_IS_A) || 
            (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Ndt2 && CURRENT_CHANNEL_IS_B))
    {
        argument[5] = 1;
        argument[4] = 1;
    }
    else if((PageModesA::modeMeasureDuration == ModeMeasureDuration::Dcycle && CURRENT_CHANNEL_IS_A) || 
            (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Dcycle && CURRENT_CHANNEL_IS_B))
    {
        argument[3] = 1;
    }
    else if((PageModesA::modeMeasureDuration == ModeMeasureDuration::Phase && CURRENT_CHANNEL_IS_A) || 
            (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Phase && CURRENT_CHANNEL_IS_B))
    {
        argument[5] = 1;
        argument[3] = 1;
        
    }

    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadModeMeasureCountPulse()
{
    char command[4] = { 0, 1, 1, 0 };

    DEFINE_ARGUMENT;
    
    argument[1] = 1;
    argument[0] = 1;

    if((PageModesC::modeMeasureCountPulseC == ModeMeasureCountPulseC::CTA) && CURRENT_CHANNEL_IS_C)
    {
    }
    else if((PageModesA::modeMeasureCountPulse == ModeMeasureCountPulse::ATC && CURRENT_CHANNEL_IS_A) || 
       (PageModesB::modeMeasureCountPulseB == ModeMeasureCountPulseB::BTA && CURRENT_CHANNEL_IS_B) || 
       (PageModesC::modeMeasureCountPulseC == ModeMeasureCountPulseC::CTB && CURRENT_CHANNEL_IS_C))
    {
        argument[5] = 1;
    }
    else if((PageModesA::modeMeasureCountPulse == ModeMeasureCountPulse::ATB && CURRENT_CHANNEL_IS_A) || 
       (PageModesB::modeMeasureCountPulseB == ModeMeasureCountPulseB::BTA_1 && CURRENT_CHANNEL_IS_B) || 
       (PageModesC::modeMeasureCountPulseC == ModeMeasureCountPulseC::CTA_1 && CURRENT_CHANNEL_IS_C))
    {
        argument[4] = 1;
    }
    else if(PageModesC::modeMeasureCountPulseC == ModeMeasureCountPulseC::CTB_1 && CURRENT_CHANNEL_IS_C)
    {
        argument[5] = 1;
        argument[4] = 1;
    }
    else
    {
        argument[0] = 1;
        argument[1] = 1;
    }

    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadPeriodTimeLabels()
{
    char command[4] = { 0, 1, 1, 1 };

    DEFINE_ARGUMENT;

    if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_7 && CURRENT_CHANNEL_IS_A)||
       (PageModesB::periodTimeLabels == PeriodTimeLabels::T_7 && CURRENT_CHANNEL_IS_B)||
       (PageModesC::periodTimeLabels == PeriodTimeLabels::T_7 && CURRENT_CHANNEL_IS_C))
    {
        argument[5] = 1;
    }
    else if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_6 && CURRENT_CHANNEL_IS_A)||
       (PageModesB::periodTimeLabels == PeriodTimeLabels::T_6 && CURRENT_CHANNEL_IS_B)||
       (PageModesC::periodTimeLabels == PeriodTimeLabels::T_6 && CURRENT_CHANNEL_IS_C))
    {
        argument[4] = 1;
    }
    else if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_5 && CURRENT_CHANNEL_IS_A)||
       (PageModesB::periodTimeLabels == PeriodTimeLabels::T_5 && CURRENT_CHANNEL_IS_B)||
       (PageModesC::periodTimeLabels == PeriodTimeLabels::T_5 && CURRENT_CHANNEL_IS_C))
    {
        argument[5] = 1;
        argument[4] = 1;
    }
    else if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_4 && CURRENT_CHANNEL_IS_A)||
       (PageModesB::periodTimeLabels == PeriodTimeLabels::T_4 && CURRENT_CHANNEL_IS_B)||
       (PageModesC::periodTimeLabels == PeriodTimeLabels::T_4 && CURRENT_CHANNEL_IS_C))
    {
        argument[3] = 1;
    }
    else if((PageModesA::periodTimeLabels == PeriodTimeLabels::T_3 && CURRENT_CHANNEL_IS_A)||
       (PageModesB::periodTimeLabels == PeriodTimeLabels::T_3 && CURRENT_CHANNEL_IS_B)||
       (PageModesC::periodTimeLabels == PeriodTimeLabels::T_3 && CURRENT_CHANNEL_IS_C))
    {
        argument[5] = 1;
        argument[3] = 1;
    }

    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadTimeMeasure()
{
    char command[4] = { 1, 0, 0, 0 };

    DEFINE_ARGUMENT;

    switch (TimeMeasure::Current().value)
    {
    case TimeMeasure::_10ms:    argument[5] = 1;  break;
    case TimeMeasure::_100ms:   argument[4] = 1;  break;
    case TimeMeasure::_1s:      argument[4] = 1;
                                argument[5] = 1;  break;
    case TimeMeasure::_10s:     argument[3] = 1;  break;
    case TimeMeasure::_100s:    argument[3] = 1;
                                argument[5] = 1;  break;
    case TimeMeasure::_1000s:   argument[3] = 1;
                                argument[4] = 1;  break;
    }

    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadNumerPeriodsMeasure()
{
    char command[4] = { 1, 0, 0, 0 };

    DEFINE_ARGUMENT;

    switch (NumberPeriods::Current().value)
    {
    case NumberPeriods::_10:    argument[5] = 1;    break;
    case NumberPeriods::_100:   argument[4] = 1;    break;
    case NumberPeriods::_1K:    argument[5] = 1;
                                argument[4] = 1;    break;
    case NumberPeriods::_10K:   argument[3] = 1;    break;
    case NumberPeriods::_100K:  argument[5] = 1;
                                argument[3] = 1;    break;
    case NumberPeriods::_1000K: argument[4] = 1;
                                argument[3] = 1;    break;
    }

    PLIS::WriteCommand(command, argument);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadImpedance()
{
    char command[4] = { 0, 0, 0, 1 };
    
    DEFINE_ARGUMENT;

    if((PageChannelA::impedance == InputImpedance::_1MOmh && CURRENT_CHANNEL_IS_A) || (PageChannelB::impedance == InputImpedance::_1MOmh && CURRENT_CHANNEL_IS_B))
    {
    }
    else
    {
        argument[5] = 1;
    }

    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadModeFront()
{
    char command[4] = { 0, 1, 0, 0 };

    DEFINE_ARGUMENT;

    if((PageChannelA::modeFront == ModeFront::Front && CURRENT_CHANNEL_IS_A) || (PageChannelB::modeFront == ModeFront::Front && CURRENT_CHANNEL_IS_B))
    {
    }
    else
    {
        argument[5] = 1;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadDivider()
{
    char command[4] = { 0, 0, 1, 0 };

    DEFINE_ARGUMENT;

    if((PageChannelA::divider == Divider::_1 && CURRENT_CHANNEL_IS_A) || (PageChannelB::divider == Divider::_1 && CURRENT_CHANNEL_IS_B))
    {
    }
    else
    {
        argument[5] = 1;
    }

    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadTypeSynch()
{
    char command[4] = { 1, 1, 0, 1 };

    DEFINE_ARGUMENT;

    if((PageChannelA::typeSynch == TypeSynch::Holdoff && CURRENT_CHANNEL_IS_A) || (PageChannelB::typeSynch == TypeSynch::Holdoff && CURRENT_CHANNEL_IS_B))
    {
        argument[5] = 1;
    }
    else
    {   
    }

    PLIS::WriteCommand(command, argument);
}

void FreqMeter::LoadTest()
{
    char command[4] = { 1, 1, 1, 1 };

    DEFINE_ARGUMENT;

    if(testMode == false)
    {
        argument[5] = 1;
        testMode = true;
    }
    else 
    {
        testMode = false;
    }
    PLIS::WriteCommand(command, argument);
}

void FreqMeter::LoadAuto()
{
    char command[4] = { 1, 1, 1, 1 };

    DEFINE_ARGUMENT;

    argument[5] = 1;
    argument[4] = 1;
    autoMode = true;
    PLIS::WriteCommand(command, argument);
}

void FreqMeter::UnloadAuto()
{
    if(autoMode == true)
    {
        char command[4] = { 1, 1, 1, 1 };
    
        DEFINE_ARGUMENT;
    
        argument[5] = 1;
        argument[3] = 1;
        autoMode = false;
        PLIS::WriteCommand(command, argument);
    }
}

void FreqMeter::LoadMeasure()
{
    char command[4] = { 1, 1, 1, 1 };

    DEFINE_ARGUMENT;
    
    PLIS::WriteCommand(command, argument);
}

bool FreqMeter::TestModeStatus()
{
    return testMode;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadOneTime()
{
    char command[4] = { 1, 0, 1, 0 };

    DEFINE_ARGUMENT;

    argument[5] = 1;
    argument[4] = 1;

    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadStartStop()
{
    char command[4] = { 1, 0, 1, 0 };

    DEFINE_ARGUMENT;
    
    if(PageModesA::StartStop() == true)
    {
        argument[5] = 1;
        argument[4] = 1;
    }
    else
    {
        argument[3] = 1;
    }
    
    PLIS::WriteCommand(command, argument);
}

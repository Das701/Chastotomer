#include "defines.h"
#include "FreqMeter.h"
#include "Settings.h"
#include "Hardware/FPGA.h"
#include "Hardware/MathFPGA.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Menu/Pages/Settings/PagesSettings.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Hint.h"


#define DEFINE_ARGUMENT char argument[6] = {0, 0, 0, 0, 0, 0}

static bool autoMode = false;

ModeTest FreqMeter::modeTest(ModeTest::Disabled);


void Channel::LoadCurrentToFPGA()
{
    char command[4] = { 0, 0, 0, 0 };

    DEFINE_ARGUMENT;

    if (CURRENT_CHANNEL_IS_B)
    {
        argument[4] = 1;
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        argument[5] = 1;
    }
    else if (CURRENT_CHANNEL_IS_D)
    {
        argument[5] = 1;
        argument[4] = 1;
    }

    FPGA::WriteCommand(command, argument);

    MathFPGA::Validator::SetInvalidData();
}


void FreqMeter::LoadDisplayTime()
{
    char command[4] = { 1, 0, 1, 1 };

    DEFINE_ARGUMENT;

    if (PageIndication::displayTime.Is_1s())
    {
        argument[5] = 1;
    }
    else if (PageIndication::displayTime.Is_10s())
    {
        argument[4] = 1;
    }

    FPGA::WriteCommand(command, argument);
}


void RefGenerator::LoadToFPGA()
{
    char command[4] = { 1, 0, 0, 1 };

    DEFINE_ARGUMENT;

    if (PageIndication::refGenerator.IsExternal())
    {
        argument[5] = 1;
    }

    FPGA::WriteCommand(command, argument);

    MathFPGA::Validator::SetInvalidData();
}


void LaunchSource::LoadToFPGA()
{
    char command[4] = { 1, 0, 1, 0 };

    DEFINE_ARGUMENT;

    if (PageIndication::launchSource.IsExternal())
    {
        argument[5] = 1;
    }
    else if (PageIndication::launchSource.IsOneTime())
    {
        argument[4] = 1;
    }

    FPGA::WriteCommand(command, argument);

    MathFPGA::Validator::SetInvalidData();
}


void FreqMeter::LoadCalibration()
{
    char command[4] = { 1, 1, 1, 1 };

    DEFINE_ARGUMENT;

    if (PageIndication::calibration.IsPressed())
    {
        argument[4] = 1;
    }
    else
    {
        argument[3] = 1;
    }

    FPGA::WriteCommand(command, argument);
}


void MemoryMode::Load()
{
    char command[4] = { 1, 1, 0, 0 };

    DEFINE_ARGUMENT;

    if (PageIndication::memoryMode.IsOn())
    {
        argument[5] = 1;
    }

    FPGA::WriteCommand(command, argument);
}

void ModeMeasureFrequency::LoadToFPGA()
{
    char command[4] = { 0, 1, 1, 0 };

    DEFINE_ARGUMENT;

    if(Current().IsFrequency())
    {
    }
    else if ((PageModesA::modeMeasureFrequency.IsRatioAB() && CURRENT_CHANNEL_IS_A) ||
        (PageModesB::modeMeasureFrequency.IsRatioBA() && CURRENT_CHANNEL_IS_B) ||
        (PageModesC::modeMeasureFrequency.IsRatioCA() && CURRENT_CHANNEL_IS_C))
    {
        argument[5] = 1;
    }
    else if ((PageModesA::modeMeasureFrequency.IsRatioAC() && CURRENT_CHANNEL_IS_A) ||
        (PageModesB::modeMeasureFrequency.IsRatioBC() && CURRENT_CHANNEL_IS_B) ||
        (PageModesC::modeMeasureFrequency.IsRatioCB() && CURRENT_CHANNEL_IS_C))
    {
        argument[4] = 1;
    }
    else if (Current().IsT_1())
    {
        argument[5] = 1;
        argument[4] = 1;
    }
    else if (Current().IsTachometer())
    {
        argument[3] = 1;
    }
    else if (Current().IsComparator())
    {
        argument[3] = 1;
        argument[5] = 1;
    }

    FPGA::WriteCommand(command, argument);

    MathFPGA::Validator::SetInvalidData();
}


void ModeMeasureCountPulse::LoadToFPGA()
{
    char command[4] = { 0, 1, 1, 0 };

    DEFINE_ARGUMENT;

    argument[1] = 1;
    argument[0] = 1;

    const ModeMeasureCountPulse &current = Current();

    if (current.Is_CtA() && CURRENT_CHANNEL_IS_C)
    {
    }
    else if ((current.Is_AtB() && CURRENT_CHANNEL_IS_A) || (current.Is_BtA() && CURRENT_CHANNEL_IS_B) || (current.Is_CtB() && CURRENT_CHANNEL_IS_C))
    {
        argument[5] = 1;
    }
    else if ((current.Is_ATB() && CURRENT_CHANNEL_IS_A) || (current.Is_BTA() && CURRENT_CHANNEL_IS_B) || (current.Is_CTA() && CURRENT_CHANNEL_IS_C))
    {
        argument[4] = 1;
    }
    else if (current.Is_CTB() && CURRENT_CHANNEL_IS_C)
    {
        argument[5] = 1;
        argument[4] = 1;
    }

    FPGA::WriteCommand(command, argument);

    MathFPGA::Validator::SetInvalidData();
}


void PeriodTimeLabels::LoadToFPGA()
{
    char command[4] = { 0, 1, 1, 1 };

    DEFINE_ARGUMENT;

    switch (PageModes::timeLabels.value)
    {
    case PeriodTimeLabels::T_7:     argument[5] = 1;    break;
    case PeriodTimeLabels::T_6:     argument[4] = 1;    break;
    case PeriodTimeLabels::T_5:     argument[5] = 1;
                                    argument[4] = 1;    break;
    case PeriodTimeLabels::T_4:     argument[3] = 1;    break;
    case PeriodTimeLabels::T_3:     argument[5] = 1;
                                    argument[3] = 1;    break;
    }

    FPGA::WriteCommand(command, argument);

    MathFPGA::Validator::SetInvalidData();
}


void PeriodTimeLabels::Set(E v)
{
    PageModes::timeLabels.value = (uint8)v;
    LoadToFPGA();
}


void TimeMeasure::LoadToFPGA()
{
    PageModes::numberPeriods.value = PageModes::timeMeasure.value;

    char command[4] = { 1, 0, 0, 0 };

    DEFINE_ARGUMENT;

    switch (PageModes::timeMeasure.value)
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

    FPGA::WriteCommand(command, argument);

    MathFPGA::Validator::SetInvalidData();
}


void NumberPeriods::LoadToFPGA()
{
    PageModes::timeMeasure.value = PageModes::numberPeriods.value;

    char command[4] = { 1, 0, 0, 0 };

    DEFINE_ARGUMENT;

    switch (PageModes::numberPeriods.value)
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

    FPGA::WriteCommand(command, argument);

    MathFPGA::Validator::SetInvalidData();
}


void NumberPeriods::Set(E v)
{
    PageModes::numberPeriods.value = (uint8)v;
    LoadToFPGA();
}



void InputImpedance::LoadToFPGA()
{
    char command[4] = { 0, 0, 0, 1 };

    DEFINE_ARGUMENT;

    if (InputImpedance::Current().Is_50Ohm())
    {
        argument[5] = 1;
    }

    FPGA::WriteCommand(command, argument);
}


void FreqMeter::LoadAuto()
{
    char command[4] = { 1, 1, 1, 1 };

    DEFINE_ARGUMENT;

    argument[5] = 1;
    argument[4] = 1;
    autoMode = true;
    FPGA::WriteCommand(command, argument);
}


void FreqMeter::UnloadAuto()
{
    if (autoMode == true)
    {
        char command[4] = { 1, 1, 1, 1 };

        DEFINE_ARGUMENT;

        argument[5] = 1;
        argument[3] = 1;
        autoMode = false;
        FPGA::WriteCommand(command, argument);
    }
}


//void FreqMeter::LoadMeasure()
//{
//    char command[4] = { 1, 1, 1, 1 };
//
//    DEFINE_ARGUMENT;
//
//    FPGA::WriteCommand(command, argument);
//}


void FreqMeter::LoadOneTime()
{
    char command[4] = { 1, 0, 1, 0 };

    DEFINE_ARGUMENT;

    argument[5] = 1;
    argument[4] = 1;

    FPGA::WriteCommand(command, argument);
}


void FreqMeter::LoadStartStop()
{
    char command[4] = { 1, 0, 1, 0 };

    DEFINE_ARGUMENT;

    if (PageModesA::StartStop() == true)
    {
        argument[5] = 1;
        argument[4] = 1;
    }
    else
    {
        argument[3] = 1;
    }

    FPGA::WriteCommand(command, argument);
}


void ModeTest::Switch()
{
    char command[4] = { 1, 1, 1, 1 };

    DEFINE_ARGUMENT;

    if (IsEnabled())
    {
        value = ModeTest::Disabled;
    }
    else
    {
        argument[5] = 1;

        value = ModeTest::Enabled;
    }

    Event(EventType::ModeTestChanged);

    FPGA::WriteCommand(command, argument);

    MathFPGA::Validator::SetInvalidData();
}

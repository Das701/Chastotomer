#include "defines.h"
#include "FreqMeter.h"
#include "Settings.h"
#include "Hardware/FPGA.h"
#include "Hardware/MathFPGA.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Hint.h"


static bool autoMode = false;

ModeTest FreqMeter::modeTest(ModeTest::Disabled);


void Channel::LoadCurrentToFPGA()
{
    Command command(Command::CurrentChannel);

    if (CURRENT_CHANNEL_IS_B)       { command.SetBit(8); }
    else if (CURRENT_CHANNEL_IS_C)  { command.SetBit(9); }
    else if (CURRENT_CHANNEL_IS_D)  { command.SetBit(8);
                                      command.SetBit(9); }

    FPGA::WriteCommand(command);

    MathFPGA::Validator::SetInvalidData();
}


void FreqMeter::LoadDisplayTime()
{
    Command command(Command::DisplayTime);

    if (PageIndication::displayTime.Is_1s())        { command.SetBit(9); }
    else if (PageIndication::displayTime.Is_10s())  { command.SetBit(8); }

    FPGA::WriteCommand(command);
}


void RefGenerator::LoadToFPGA()
{
    Command command(Command::RefGenerator);

    if (PageIndication::refGenerator.IsExternal())    { command.SetBit(9); }

    FPGA::WriteCommand(command);

    MathFPGA::Validator::SetInvalidData();
}


void LaunchSource::LoadToFPGA()
{
    Command command(Command::LaunchSource);

    if (PageIndication::launchSource.IsExternal())      { command.SetBit(9); }
    else if (PageIndication::launchSource.IsOneTime())  { command.SetBit(8); }

    FPGA::WriteCommand(command);

    MathFPGA::Validator::SetInvalidData();
}


void FreqMeter::LoadCalibration()
{
    Command command(Command::Calibration);

    if (PageIndication::calibration.IsPressed())    { command.SetBit(8); }
    else                                            { command.SetBit(7); }

    FPGA::WriteCommand(command);
}


void MemoryMode::Load()
{
    Command command(Command::MemoryMode);

    if (PageIndication::memoryMode.IsOn()) { command.SetBit(9); }

    FPGA::WriteCommand(command);
}

void ModeMeasureFrequency::LoadToFPGA()
{
    Command command(Command::ModeFrequency);

    if(Current().IsFrequency())
    {
    }
    else if ((PageModesA::modeMeasureFrequency.IsRatioAB() && CURRENT_CHANNEL_IS_A) ||
        (PageModesB::modeMeasureFrequency.IsRatioBA() && CURRENT_CHANNEL_IS_B) ||
        (PageModesC::modeMeasureFrequency.IsRatioCA() && CURRENT_CHANNEL_IS_C))
    {
        command.SetBit(9);
    }
    else if ((PageModesA::modeMeasureFrequency.IsRatioAC() && CURRENT_CHANNEL_IS_A) ||
        (PageModesB::modeMeasureFrequency.IsRatioBC() && CURRENT_CHANNEL_IS_B) ||
        (PageModesC::modeMeasureFrequency.IsRatioCB() && CURRENT_CHANNEL_IS_C))
    {
        command.SetBit(8);
    }
    else if (Current().IsT_1())
    {
        command.SetBit(8);
        command.SetBit(9);
    }
    else if (Current().IsTachometer())
    {
        command.SetBit(7);
    }
    else if (Current().IsComparator())
    {
        command.SetBit(7);
        command.SetBit(9);
    }

    FPGA::WriteCommand(command);

    MathFPGA::Validator::SetInvalidData();
}


void ModeMeasureCountPulse::LoadToFPGA()
{
    Command command(Command::ModeCountPulse);

    command.SetBit(4);
    command.SetBit(5);

    const ModeMeasureCountPulse &current = Current();

    if (current.Is_CtA() && CURRENT_CHANNEL_IS_C)
    {
    }
    else if ((current.Is_AtB() && CURRENT_CHANNEL_IS_A) || (current.Is_BtA() && CURRENT_CHANNEL_IS_B) || (current.Is_CtB() && CURRENT_CHANNEL_IS_C))
    {
        command.SetBit(9);
    }
    else if ((current.Is_ATB() && CURRENT_CHANNEL_IS_A) || (current.Is_BTA() && CURRENT_CHANNEL_IS_B) || (current.Is_CTA() && CURRENT_CHANNEL_IS_C))
    {
        command.SetBit(8);
    }
    else if (current.Is_CTB() && CURRENT_CHANNEL_IS_C)
    {
        command.SetBit(8);
        command.SetBit(9);
    }

    FPGA::WriteCommand(command);

    MathFPGA::Validator::SetInvalidData();
}


void PeriodTimeLabels::LoadToFPGA()
{
    Command command(Command::TimeLabels);

    switch (Channel::timeLabels.value)
    {
    case PeriodTimeLabels::T_7:     command.SetBit(9);    break;
    case PeriodTimeLabels::T_6:     command.SetBit(8);    break;
    case PeriodTimeLabels::T_5:     command.SetBit(9);
                                    command.SetBit(8);    break;
    case PeriodTimeLabels::T_4:     command.SetBit(7);    break;
    case PeriodTimeLabels::T_3:     command.SetBit(9);
                                    command.SetBit(7);    break;
    }

    FPGA::WriteCommand(command);

    MathFPGA::Validator::SetInvalidData();
}


void PeriodTimeLabels::Set(E v)
{
    Channel::timeLabels.value = (uint8)v;
    LoadToFPGA();
}


void TimeMeasure::LoadToFPGA()
{
    Channel::numberPeriods.value = Channel::timeMeasure.value;

    Command command(Command::TimeMeasure);

    switch (Channel::timeMeasure.value)
    {
    case TimeMeasure::_10ms:    command.SetBit(9);  break;
    case TimeMeasure::_100ms:   command.SetBit(8);  break;
    case TimeMeasure::_1s:      command.SetBit(8);
                                command.SetBit(9);  break;
    case TimeMeasure::_10s:     command.SetBit(7);  break;
    case TimeMeasure::_100s:    command.SetBit(7);
                                command.SetBit(9);  break;
    case TimeMeasure::_1000s:   command.SetBit(7);
                                command.SetBit(8);  break;
    }

    FPGA::WriteCommand(command);

    MathFPGA::Validator::SetInvalidData();
}


void NumberPeriods::LoadToFPGA()
{
    Channel::timeMeasure.value = Channel::numberPeriods.value;

    Command command(Command::NumberPeriods);

    switch (Channel::numberPeriods.value)
    {
    case NumberPeriods::_10:    command.SetBit(9);    break;
    case NumberPeriods::_100:   command.SetBit(8);    break;
    case NumberPeriods::_1K:    command.SetBit(9);
                                command.SetBit(8);    break;
    case NumberPeriods::_10K:   command.SetBit(7);    break;
    case NumberPeriods::_100K:  command.SetBit(9);
                                command.SetBit(7);    break;
    case NumberPeriods::_1000K: command.SetBit(8);
                                command.SetBit(7);    break;
    }

    FPGA::WriteCommand(command);

    MathFPGA::Validator::SetInvalidData();
}


void NumberPeriods::Set(E v)
{
    Channel::numberPeriods.value = (uint8)v;
    LoadToFPGA();
}



void InputImpedance::LoadToFPGA()
{
    Command command(Command::Impedance);

    if (Channel::Current()->impedance.Is_50Ohm())
    {
        command.SetBit(9);
    }

    FPGA::WriteCommand(command);
}


void FreqMeter::LoadAuto()
{
    Command command(Command::Auto);

    command.SetBit(9);
    command.SetBit(8);
    
    autoMode = true;

    FPGA::WriteCommand(command);
}


void FreqMeter::UnloadAuto()
{
    if (autoMode == true)
    {
        Command command(Command::Auto);

        command.SetBit(9);
        command.SetBit(7);

        autoMode = false;

        FPGA::WriteCommand(command);
    }
}


void FreqMeter::LoadOneTime()
{
    Command command(Command::OneTime);

    command.SetBit(9);
    command.SetBit(8);

    FPGA::WriteCommand(command);
}


void FreqMeter::LoadStartStop()
{
    Command command(Command::StartStop);

    if (PageModesA::StartStop() == true)
    {
        command.SetBit(9);
        command.SetBit(8);
    }
    else
    {
        command.SetBit(7);
    }

    FPGA::WriteCommand(command);
}


void ModeTest::Switch()
{
    Command command(Command::Test);

    if (IsEnabled())
    {
        value = ModeTest::Disabled;
    }
    else
    {
        command.SetBit(9);

        value = ModeTest::Enabled;
    }

    Event(EventType::ModeTestChanged);

    FPGA::WriteCommand(command);

    MathFPGA::Validator::SetInvalidData();
}

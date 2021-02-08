#include "defines.h"
#include "FreqMeter.h"
#include "Settings.h"
#include "Calculate/ValueFPGA.h"
#include "Hardware/FPGA.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Hint.h"


static bool autoMode = false;

ModeTest FreqMeter::modeTest(ModeTest::Disabled);


void Channel::LoadToFPGA()
{
    Command command(Command::CurrentChannel);

    if (IsB())       { command.SetBit(8); }
    else if (IsC())  { command.SetBit(9); }
    else if (IsD())  { command.SetBit(8); //-V2516
                       command.SetBit(9); }

    FPGA::WriteCommand(command);
}


void FreqMeter::LoadDisplayTime()
{
    Command command(Command::DisplayTime);

    if (PageIndication::displayTime.Is_1s())        { command.SetBit(9); }
    else if (PageIndication::displayTime.Is_10s())  { command.SetBit(8); } //-V2516

    FPGA::WriteCommand(command);
}


void RefGenerator::LoadToFPGA()
{
    Command command(Command::RefGenerator);

    if (PageIndication::refGenerator.IsExternal())    { command.SetBit(9); }

    FPGA::WriteCommand(command);
}


void LaunchSource::LoadToFPGA()
{
    Command command(Command::LaunchSource);

    if (PageIndication::launchSource.IsExternal())      { command.SetBit(9); }
    else if (PageIndication::launchSource.IsOneTime())  { command.SetBit(8); } //-V2516

    FPGA::WriteCommand(command);
}


void FreqMeter::LoadCalibration()
{
    Command command(Command::Calibration);

    if (PageIndication::calibrationMode.IsEnabled())   { command.SetBit(8); }
    else                                               { command.SetBit(7); }

    FPGA::WriteCommand(command);
}

void FreqMeter::LoadCalibrationNoSave()
{
    Command command(Command::Calibration);

    if (PageIndication::calibrationMode.IsEnabled())   { }
    else                                               { command.SetBit(7); 
                                                         command.SetBit(8);
                                                         command.SetBit(9); }

    FPGA::WriteCommand(command);
}


void FreqMeter::HardwareReset()
{
    Command command(Command::Reset);

    command.SetBit(7);
    command.SetBit(8);

    FPGA::WriteCommand(command);
}


void MemoryMode::Load()
{
    Command command(Command::MemoryMode);

    if (PageIndication::memoryMode.IsOn()) { command.SetBit(9); }

    FPGA::WriteCommand(command);
}

void ModeFrequency::LoadToFPGA()
{
    Command command(Command::ModeFrequency);

    if(Current().IsFrequency())
    {
    }
    else if ((Channel::A->mod.modeFrequency.IsRatioAB() && CURRENT_CHANNEL_IS_A) ||
        (Channel::B->mod.modeFrequency.IsRatioBA() && CURRENT_CHANNEL_IS_B) ||
        (Channel::C->mod.modeFrequency.IsRatioCA() && CURRENT_CHANNEL_IS_C))
    {
        command.SetBit(9);
    }
    else if ((Channel::A->mod.modeFrequency.IsRatioAC() && CURRENT_CHANNEL_IS_A) ||
        (Channel::B->mod.modeFrequency.IsRatioBC() && CURRENT_CHANNEL_IS_B) ||
        (Channel::C->mod.modeFrequency.IsRatioCB() && CURRENT_CHANNEL_IS_C))
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
    else if (Current().IsComparator()) //-V2516
    {
        if (Channel::Current()->mod.timeComparator.Is_1s())
        {
            command.SetBit(7);
            command.SetBit(9);
        }
        else
        {
            command.SetBit(7);
            command.SetBit(8);
        }
    }

    FPGA::WriteCommand(command);
}


void TimeComparator::LoadToFPGA()
{
    ModeFrequency::LoadToFPGA();
}


void ModeCountPulse::LoadToFPGA()
{
    Command command(Command::ModeCountPulse);

    command.SetBit(4);
    command.SetBit(5);

    const ModeCountPulse &current = Current();

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
    else if (current.Is_CTB() && CURRENT_CHANNEL_IS_C) //-V2516
    {
        command.SetBit(8);
        command.SetBit(9);
    }

    FPGA::WriteCommand(command);
}


void PeriodTimeLabels::LoadToFPGA()
{
    Command command(Command::TimeLabels);

    switch (ModesChannel::timeLabels.value)
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
}


void PeriodTimeLabels::Set(E v)
{
    ModesChannel::timeLabels.value = (uint8)v;
    LoadToFPGA();
}


void TimeMeasure::LoadToFPGA()
{
    ModesChannel::numberPeriods.value = ModesChannel::timeMeasure.value;

    Command command(Command::TimeMeasure);

    switch (ModesChannel::timeMeasure.value)
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
}


void NumberPeriods::LoadToFPGA()
{
    ModesChannel::timeMeasure.value = ModesChannel::numberPeriods.value;

    Command command(Command::NumberPeriods);

    switch (ModesChannel::numberPeriods.value)
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
}


void NumberPeriods::Set(E v)
{
    ModesChannel::numberPeriods.value = (uint8)v;
    LoadToFPGA();
}



void InputImpedance::LoadToFPGA()
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        Command command(Command::Impedance);

        if (Channel::Current()->set.impedance.Is_50Ohm())
        {
            command.SetBit(9);
        }

        FPGA::WriteCommand(command);
    }
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


void ModeStartStop::LoadToFPGA()
{
    Command command(Command::StartStop);

    if (enabled)
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
}

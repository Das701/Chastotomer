#include "defines.h"
#include "Settings.h"
#include "SCPI/SCPI.h"
#include "Utils/String.h"
#include "Display/Display.h"

static pchar FuncCoupling(pchar);
static pchar FuncImpedance(pchar);
static pchar FuncFilter(pchar);
static pchar FuncTriggerSlope(pchar);
static pchar FuncDivider(pchar);
static pchar FuncTriggerLevel(pchar);
static pchar FuncTriggerAuto(pchar);

const StructSCPI SCPI::input[] =
{
    SCPI_LEAF(":COUPLE",        FuncCoupling),
    SCPI_LEAF(":IMPEDANCE",     FuncImpedance),
    SCPI_LEAF(":FILTER",        FuncFilter),
    SCPI_LEAF(":TRIGGER:SLOPE", FuncTriggerSlope),
    SCPI_LEAF(":TRIGGER:LEVEL", FuncTriggerLevel),
    SCPI_LEAF(":TRIGGER:AUTO",  FuncTriggerAuto),
    SCPI_LEAF(":DIVIDER",       FuncDivider),
    SCPI_EMPTY()
};


static pchar FuncCoupling(pchar buffer)
{
    static const pchar coupling[] =
    {
        " AC",
        " DC",
        ""
    };

    return SCPI::ProcessSimpleParameter(buffer, coupling, Channel::Current()->set.couple.sw);
}


static pchar FuncImpedance(pchar buffer)
{
    static const pchar impedance[] =
    {
        " 1MOHM",
        " 50OHM",
        ""
    };

    return SCPI::ProcessSimpleParameter(buffer, impedance, Channel::Current()->set.impedance.sw);
}


static pchar FuncFilter(pchar buffer)
{
    static const pchar filter[] =
    {
        " ON",
        " OFF",
        ""
    };

    return SCPI::ProcessSimpleParameter(buffer, filter, Channel::Current()->set.modeFilter.sw);
}


static pchar FuncTriggerSlope(pchar buffer)
{
    static const pchar modeFront[] =
    {
        " RISE",
        " FALL",
        ""
    };

    return SCPI::ProcessSimpleParameter(buffer, modeFront, Channel::Current()->set.modeFront.sw);
}

static pchar FuncTriggerLevel(pchar buffer)
{
    int result = 0;
    pchar end = nullptr;

    while (*buffer == ' ')              // Пропускаем все пробелы
    {
        buffer++;
    }

    if (SCPI::SU::ReadIntegerValue(buffer, &result, &end))
    {
        SCPI_PROLOG(end);

        LevelSynch::Set(result);
        Display::Refresh();
        SCPI_EPILOG(end);
    }

    if (SCPI::SU::IsLineEnding(&end))
    {
        SCPI::Answer::SendBadSymbols();
    }
    
    return nullptr;
}

static pchar FuncTriggerAuto(pchar buffer)
{
    SCPI_PROLOG(buffer);

    Keyboard::AppendControl(Control(Control::Auto, Control::Action::Press));
    Keyboard::AppendControl(Control(Control::Auto, Control::Action::Release));
    Display::Refresh();
    SCPI_EPILOG(buffer);

    return nullptr;
}

static pchar FuncDivider(pchar buffer)
{
    static const pchar divider[] =
    {
        " 1",
        " 10",
        ""
    };

    return SCPI::ProcessSimpleParameter(buffer, divider, Channel::Current()->set.divider.sw);
}

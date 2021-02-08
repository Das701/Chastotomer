#include "defines.h"
#include "Settings.h"
#include "SCPI/SCPI.h"
#include "Utils/String.h"


static pchar FuncCoupling(pchar);
static pchar FuncFilter(pchar);
static pchar FuncImpedance(pchar);
static pchar FuncDivider(pchar);


const StructSCPI SCPI::input[] =
{
    SCPI_LEAF(":COUPLING",  FuncCoupling),
    SCPI_LEAF(":FILTER",    FuncFilter),
    SCPI_LEAF(":IMPEDANCE", FuncImpedance),
    SCPI_LEAF(":DIVIDER",   FuncDivider),
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

    SCPI_REQUEST(SCPI::SendAnswer(coupling[Channel::Current()->set.couple.value]));
    SCPI_PROCESS_ARRAY(coupling, Channel::Current()->set.couple.Set((InputCouple::E)i));
}


static pchar FuncFilter(pchar buffer)
{
    static const pchar filter[] =
    {
        " ON",
        " OFF",
        ""
    };

    SCPI_REQUEST(SCPI::SendAnswer(filter[Channel::Current()->set.modeFilter.value]));
    SCPI_PROCESS_ARRAY(filter, Channel::Current()->set.modeFilter.Set((ModeFilter::E)i));
}


static pchar FuncImpedance(pchar buffer)
{
    static const pchar impedance[] =
    {
        " 1MOHM",
        " 50OHM",
        ""
    };

    SCPI_REQUEST(SCPI::SendAnswer(impedance[Channel::Current()->set.impedance.value]));
    SCPI_PROCESS_ARRAY(impedance, Channel::Current()->set.impedance.Set((InputImpedance::E)i));
}


static pchar FuncDivider(pchar)
{
    return 0;
}

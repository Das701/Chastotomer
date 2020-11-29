#include "defines.h"
#include "Settings.h"
#include "SCPI/SCPI.h"
#include "Utils/String.h"


static pCHAR FuncCoupling(pCHAR);
static pCHAR FuncFilter(pCHAR);
static pCHAR FuncImpedance(pCHAR);
static pCHAR FuncDivider(pCHAR);


const StructSCPI SCPI::input[] =
{
    SCPI_LEAF(":COUPLING",  FuncCoupling),
    SCPI_LEAF(":FILTER",    FuncFilter),
    SCPI_LEAF(":IMPEDANCE", FuncImpedance),
    SCPI_LEAF(":DIVIDER",   FuncDivider),
    SCPI_EMPTY()
};


static pCHAR FuncCoupling(pCHAR buffer)
{
    static pString coupling[] =
    {
        " AC",
        " DC",
        ""
    };

    SCPI_REQUEST(SCPI::SendAnswer(coupling[Channel::Current()->set.couple.value]));
    SCPI_PROCESS_ARRAY(coupling, Channel::Current()->set.couple.Set((InputCouple::E)i));
}


static pCHAR FuncFilter(pCHAR buffer)
{
    static pString filter[] =
    {
        " ON",
        " OFF",
        ""
    };

    SCPI_REQUEST(SCPI::SendAnswer(filter[Channel::Current()->set.modeFilter.value]));
    SCPI_PROCESS_ARRAY(filter, Channel::Current()->set.modeFilter.Set((ModeFilter::E)i));
}


static pCHAR FuncImpedance(pCHAR)
{
    return 0;
}


static pCHAR FuncDivider(pCHAR)
{
    return 0;
}

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

    SCPI_REQUEST(SCPI::SendAnswer(coupling[InputCouple::Current().value]));
    SCPI_PROCESS_ARRAY(coupling, InputCouple::Set((InputCouple::E)i));
}


static pCHAR FuncFilter(pCHAR buffer)
{
    static pString filter[] =
    {
        " ON",
        " OFF",
        ""
    };

    SCPI_REQUEST(SCPI::SendAnswer(filter[ModeFilter::Current().value]));
    SCPI_PROCESS_ARRAY(filter, ModeFilter::Set((ModeFilter::E)i));
}


static pCHAR FuncImpedance(pCHAR)
{
    return 0;
}


static pCHAR FuncDivider(pCHAR)
{
    return 0;
}

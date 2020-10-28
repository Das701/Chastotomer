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


static pString coupling[] =
{
    " AC",
    " DC",
    ""
};


static pCHAR FuncCoupling(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(coupling[InputCouple::Current()]));
    SCPI_PROCESS_ARRAY(coupling, InputCouple::Set((InputCouple::E)i));
}


static pCHAR FuncFilter(pCHAR)
{
    return 0;
}


static pCHAR FuncImpedance(pCHAR)
{
    return 0;
}


static pCHAR FuncDivider(pCHAR)
{
    return 0;
}

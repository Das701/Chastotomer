#include "defines.h"
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


//static pString coupling[] =
//{
//    " AC",
//    " DC",
//    ""
//};


static pCHAR FuncCoupling(pCHAR buffer)
{
    SCPI_PROLOG(buffer);
    
    SCPI::SendAnswer(String("COUPLIING").c_str());
    
    SCPI_EPILOG(buffer);
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

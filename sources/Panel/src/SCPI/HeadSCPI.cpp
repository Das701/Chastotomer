#include "defines.h"
#include "SCPI/SCPI.h"
#include "Utils/String.h"


static pCHAR FuncIDN(pCHAR);
static pCHAR FuncReset(pCHAR);


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",            FuncIDN),
    SCPI_LEAF("*RST",             FuncReset),
    SCPI_EMPTY()
};


static pCHAR FuncIDN(pCHAR buffer)
{
    SCPI_PROLOG(buffer)

    SCPI::SendAnswer(String("OAO MNIPI, CH3-96/2, Cherem V1.2").c_str());

    SCPI_EPILOG(buffer)
}


static pCHAR FuncReset(pCHAR buffer)
{
    SCPI_PROLOG(buffer)
        
    SCPI_EPILOG(buffer)
}



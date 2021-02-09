#include "defines.h"
#include "SCPI/SCPI.h"


static pchar FuncFrequency(pchar);
static pchar FuncPeriod(pchar);
static pchar FuncDuration(pchar);
static pchar FuncCounter(pchar);


const StructSCPI SCPI::measure[] =
{
    SCPI_LEAF(":FREQUENCY", FuncFrequency),
    SCPI_LEAF(":PERIOD",    FuncPeriod),
    SCPI_LEAF(":DURATION",  FuncDuration),
    SCPI_LEAF(":CNT",       FuncCounter),
    ""
};


static pchar FuncFrequency(pchar buffer)
{
    return nullptr;
}


static pchar FuncPeriod(pchar buffer)
{
    return nullptr;
}


static pchar FuncDuration(pchar buffer)
{
    return nullptr;
}


static pchar FuncCounter(pchar buffer)
{
    return nullptr;
}

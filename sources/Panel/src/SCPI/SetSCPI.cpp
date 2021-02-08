#include "defines.h"
#include "SCPI/SCPI.h"
#include "Utils/String.h"


static pchar FuncTimeBase(pchar);
static pchar FuncN(pchar);
static pchar FuncTimeLabels(pchar);


const StructSCPI SCPI::set[] =
{
    SCPI_LEAF(":TIMEBASE",   FuncTimeBase),
    SCPI_LEAF(":N",          FuncN),
    SCPI_LEAF(":TIMELABELS", FuncTimeLabels),
    SCPI_EMPTY()
};


static pchar FuncTimeBase(pchar buffer)
{
    return nullptr;
}


static pchar FuncN(pchar buffer)
{
    return nullptr;
}


static pchar FuncTimeLabels(pchar buffer)
{
    return nullptr;
}

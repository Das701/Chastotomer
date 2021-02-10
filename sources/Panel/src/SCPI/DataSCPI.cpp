#include "defines.h"
#include "SCPI/SCPI.h"


static pchar FuncMode(pchar);
static pchar FuncValue(pchar);
static pchar FuncArray(pchar);


const StructSCPI SCPI::values[] =
{
    SCPI_LEAF(":MODE", FuncMode),
    SCPI_LEAF(":VALUE?", FuncValue),
    SCPI_LEAF(":ARRAY",  FuncArray),
    SCPI_EMPTY()
};


static pchar FuncMode(pchar buffer)
{
    return nullptr;
}


static pchar FuncValue(pchar buffer)
{
    return nullptr;
}


static pchar FuncArray(pchar buffer)
{
    return nullptr;
}

#include "defines.h"
#include "Calculate/ValueFPGA.h"


ValueFPGA::ValueFPGA(uint, uint, uint, uint)
{

}


int ValueFPGA::NumDigitsInNumber(uint number) const
{
    if (number == 0)
    {
        return 1;
    }

    int result = 0;

    while (number > 0)
    {
        result++;
        number /= 10;
    }

    return result;
}


char *ValueFPGA::GetSuffixUnit(int order) const
{
    if (order >= 15)       { return "P"; }
    else if (order >= 12)  { return "T"; }
    else if (order >= 9)   { return "G"; }
    else if (order >= 6)   { return "M"; }
    else if (order >= 3)   { return "k"; }
    else if (order >= 0)   { return "";  }
    else if (order >= -3)  { return "m"; }
    else if (order >= -6)  { return "u"; }
    else if (order >= -9)  { return "n"; }
    else if (order >= -12) { return "p"; }
    
    return "ph";
}


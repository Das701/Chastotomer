#include "defines.h"
#include "Calculate/ValueFPGA.h"


ValueFPGA::ValueFPGA(uint, uint, uint, uint)
{

}


int ValueFPGA::NumDigitsInNumber(uint number)
{
    int result = 1;

    while (number > 0)
    {
        result++;
        number /= 10;
    }

    return result;
}

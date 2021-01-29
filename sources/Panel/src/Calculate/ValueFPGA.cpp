#include "defines.h"
#include "Calculate/ValueFPGA.h"
#include "Utils/StringUtils.h"
#include <cstdio>
#include <cstring>


ValueFPGA::ValueFPGA()
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


void ValueFPGA::SetValue(ValueSTRICT strict, uint counter, int sub)
{
    int order = 0;

    int numDigitsInCounter = NumDigitsInNumber(counter) + sub;

    if (numDigitsInCounter < 1)
    {
        numDigitsInCounter = 1;
    }

    if (strict.ToDouble() < 0.0)
    {
        while (strict.ToUnits(Order::Milli) < 1)
        {
            strict.MulUINT(1000);
            order -= 3;
        }
    }
    else
    {
        while (strict.ToDouble() >= 1000.0)
        {
            strict.DivUINT(1000);
            order += 3;
        }
    }

    if (!strict.IsZero())
    {
        while ((uint)strict.ToDouble() < 1)
        {
            strict.MulUINT(1000);
            order -= 3;
        }

        if (NumDigitsInNumber((uint)strict.ToDouble()) > numDigitsInCounter)
        {
            strict.DivUINT(1000);
            order += 3;
        }
    }

    char buffer[30];
    std::sprintf(buffer, "%10.10f", strict.ToDouble());

    if (strict.IsZero())
    {
        SU::LeaveFewDigits(buffer, 29, numDigitsInCounter);
    }
    else
    {
        SU::LeaveFewSignedDigits(buffer, 29, numDigitsInCounter);
    }

    std::strcat(buffer, " ");
    std::strcat(buffer, GetSuffixUnit(order));
    std::strcat(buffer, mainUnits.c_str());

    value.Set(TypeConversionString::None, buffer);
}


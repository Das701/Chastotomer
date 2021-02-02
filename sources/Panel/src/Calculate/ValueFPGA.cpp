#include "defines.h"
#include "Calculate/MathFPGA.h"
#include "Calculate/ValuesFPGA.h"
#include "Utils/StringUtils.h"
#include "Utils/Debug.h"
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


char *ValueFPGA::GetSuffixUnitRelated(int order)
{
    if (order >= 15) { return "E+15"; }
    else if (order >= 12) { return "E+12"; }
    else if (order >= 9) { return "E+9"; }
    else if (order >= 6) { return "E+6"; }
    else if (order >= 3) { return "E+3"; }
    else if (order >= 0) { return "E+0"; }
    else if (order >= -3) { return "E-3"; }
    else if (order >= -6) { return "E-6"; }
    else if (order >= -9) { return "E-9"; }
    else if (order >= -12) { return "E-12"; }

    return "E-15";
}


void ValueFPGA::SetValue(ValueSTRICT strict, uint counter)
{
    DEBUG_POINT_0;
    int order = 0;

    int numDigitsInCounter = NumDigitsInNumber(counter);

    if (numDigitsInCounter < 1)
    {
        numDigitsInCounter = 1;
    }

    DEBUG_POINT_0;
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

    DEBUG_POINT_0;
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
    DEBUG_POINT_0;
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
    DEBUG_POINT_0;

    String digits(buffer);

    DEBUG_POINT_0;

    MathFPGA::Data::SetDigits(digits);

    DEBUG_POINT_0;

    std::strcat(buffer, " ");

    DEBUG_POINT_0;

    std::strcat(buffer, GetSuffixUnit(order));

    DEBUG_POINT_0;

    std::strcat(buffer, mainUnits.c_str());

    DEBUG_POINT_0;

    value.Set(TypeConversionString::None, buffer);

    DEBUG_POINT_0;

    std::strcpy(buffer, GetSuffixUnit(order));

    DEBUG_POINT_0;
    std::strcat(buffer, mainUnits.c_str());
    DEBUG_POINT_0;
    MathFPGA::Data::SetUnits(String(buffer));
    DEBUG_POINT_0;
}


void ValueFPGA::SetInfiniteValue()
{
    MathFPGA::Data::SetDigits(MathFPGA::Data::UGO_DivNULL);
    MathFPGA::Data::SetUnits("");
    value.Set(TypeConversionString::None, MathFPGA::Data::UGO_DivNULL);
}


void ValueFPGA::SetIntegerValue(uint counter)
{
    value.Set(TypeConversionString::None, "%d", counter);

    MathFPGA::Data::SetDigits(String("%d", counter));
    MathFPGA::Data::SetUnits(String(""));
}

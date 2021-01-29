#include "defines.h"
#include "Calculate/ValueFrequency.h"
#include <cmath>
#include <cstdio>
#include <cstring>


ValueFrequency::ValueFrequency(uint value1, uint value2, uint value3, uint value4) :
    ValueFPGA(value1, value2, value3, value4)
{
    mainUnits.Set(TypeConversionString::None, "Hz");
}


ValueFrequency_Frequency::ValueFrequency_Frequency(uint counter) : ValueFrequency(counter)
{
    int numDigits = NumDigitsInNumber(counter);

    numDigits = numDigits;

    char buffer[100];
    std::sprintf(buffer, "%d", counter / 2);
    std::strcat(buffer, " ");
    std::strcat(buffer, mainUnits.c_str());

    value.Set(TypeConversionString::None, buffer);
}


ValueFrequency_T_1::ValueFrequency_T_1(uint counter1) : ValueFrequency(counter1)
{

}

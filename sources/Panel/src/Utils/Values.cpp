#include "defines.h"
#include "Utils/Values.h"
#include "Math.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>


const float Float::ERROR = std::numeric_limits<float>::quiet_NaN();


static char *FloatToString(float value, bool alwaysSign, int numDigits, char bufferOut[20])
{
    if (Math::IsEquals(value, Float::ERROR))
    {
        std::strcpy(bufferOut, String::_ERROR); //-V2513
        return bufferOut;
    }

    value = Math::RoundFloat(value, numDigits);

    char *pBuffer = bufferOut;

    if (value < 0)
    {
        *pBuffer++ = '-';
    }
    else if (alwaysSign) //-V2516
    {
        *pBuffer++ = '+';
    }

    char format[10] = "%4.2f\0\0";

    format[1] = static_cast<char>(numDigits) + 0x30;

    int numDigitsInInt = Math::DigitsInIntPart(value);

    format[3] = static_cast<char>((numDigits - numDigitsInInt) + 0x30);
    if (numDigits == numDigitsInInt)
    {
        format[5] = '.';
    }

    float absValue = std::fabsf(value);
    std::sprintf(pBuffer, static_cast<char *>(format), static_cast<double>(absValue));

    float val = static_cast<float>(std::atof(pBuffer)); //-V2508

    if (Math::DigitsInIntPart(val) != numDigitsInInt)
    {
        numDigitsInInt = Math::DigitsInIntPart(val);
        format[3] = static_cast<char>((numDigits - numDigitsInInt) + 0x30);
        if (numDigits == numDigitsInInt)
        {
            format[5] = '.';
        }
        std::sprintf(pBuffer, format, static_cast<double>(value));
    }

    bool signExist = alwaysSign || value < 0;
    while (std::strlen(bufferOut) < static_cast<uint>(numDigits + (signExist ? 2 : 1))) //-V2513
    {
        std::strcat(bufferOut, "0"); //-V2513
    }

#ifdef WIN32

    char *pointer = bufferOut;  //\
                                //|
    while (*pointer)             //|
    {                           //| Это нужно на PC, где вместо точки может быть запятая
        if (*pointer == ',')     //| 
        {                       //|
            *pointer = '.';     //|
        }                       //|
        pointer++;              //|
    }                           ///

#endif

    return bufferOut;
}


Float::Float(float val) : value(val)
{
}


String Float::ToString(bool alwaysSign, int numDigits) const
{
    char buffer[30];
    return String(FloatToString(value, alwaysSign, numDigits, buffer));
}

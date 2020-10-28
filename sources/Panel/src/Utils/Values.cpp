#include "defines.h"
#include "Values.h"
#include "Math.h"
#include <cstring>
#include <cmath>
#include <stdlib.h>
#include <cstdio>
#include <limits>



const float Float::ERROR = std::numeric_limits<float>::quiet_NaN();



// ����������� �������� ������� � ��������� ������
static char *FrequencyToString(float freq, char buffer[20]);
// ����������� ����� � ��������� ������
static char *TimeToString(float time, bool alwaysSign, char buffer[20]);
// ����������� �������� ���������� � ��������� ������
static char *VoltageToString(float voltts, bool alwaysSign, char buffer[20]);
// \brief ��������� ����� � ��������� ������ � ��������� ���. numDigits - ����� �������� ���������
// \attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ������������ ����.
// \retval ��������� �� ������ � ������.
static char *FloatToString(float value, bool alwaysSign, int numDigits, char bufferOut[20]);
// \brief ����������� value � ��������� ������
// \attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ����������� ����
static char *IntegerToString(int value, bool alwaysSign, int numMinFields, char bufferOut[20]);



Float::Float(float val) : value(val)
{
//    if (value == std::numeric_limits<float>::infinity())
//    {
//        LOG_ERROR("������������ �����");
//    }
}


char* Hex::ToBin(int depth, char buffer[36]) const
{
    int byte = 3;       // � ����� ����� �������� �����. �.�. � ������ ������ - ������� ����, � ����� - �������

    switch (depth)
    {
        case 8: byte = 0;  break;
        case 16: byte = 1; break;
    }

    char *pointer = buffer;

    while (byte >= 0)
    {
        BinToString8(static_cast<uint8>(value >> (byte * 8)), pointer);
        if (byte > 0)
        {
            *(pointer + 8) = ' ';
        }
        pointer += 9;
        byte--;
    }

    return buffer;
}


char* Hex::BinToString8(uint8 val, char buffer[9]) const
{
    for (int bit = 0; bit < 8; bit++)
    {
        buffer[7 - bit] = _GET_BIT(val, bit) ? '1' : '0';
    }
    buffer[8] = '\0';
    return buffer;
}


char* Hex::ToHex(int depth, char buffer[9]) const
{
    switch (depth)
    {
        case 8:     std::sprintf(buffer, "%02X", value); break;
        case 16:    std::sprintf(buffer, "%04X", value); break;
        case 32:    std::sprintf(buffer, "%08X", value); break;
        default:
            // ��� ������ �� ������
            break;
    }

    return buffer;
}


char Hex::DigitInPosition(int pos) const
{
    uint val = value;

    while (pos > 0)
    {
        val /= 10;
        pos--;
    }

    return static_cast<char>((val % 10) | 0x30);
}


static char *IntegerToString(int value, bool alwaysSign, int numMinFields, char buffer[20])
{
    const int SIZE = 20;
    char format[SIZE] = "%";
    std::snprintf(&(format[1]), SIZE, "0%d", numMinFields);
    std::strcat(format, "d"); //-V2513

    if (alwaysSign && value >= 0)
    {
        buffer[0] = '+';
        std::snprintf(buffer + 1, SIZE - 1, format, value);
    }
    else
    {
        std::snprintf(buffer, SIZE, format, value);
    }

    return buffer;
}


static char *FrequencyToString(float freq, char bufferOut[20]) //-V2506
{
    bufferOut[0] = 0;
    const char *suffix = 0;

    if (Math::IsEquals(freq, Float::ERROR))
    {
        std::strcat(bufferOut, String::_ERROR);
        return bufferOut;
    }

    if (freq >= 1e6F)
    {
        suffix = "���";
        freq /= 1e6F;
    }
    else if (freq >= 1e3F)
    {
        suffix = "���";
        freq /= 1e3F;
    }
    else
    {
        suffix = "��";
    }

    std::sprintf(bufferOut, "%s%s", Float(freq).ToString(false, 4).c_str(), suffix);
    return bufferOut;
}


char *Frequency::ToStringAccuracy(char bufferOut[20], int numDigits) const //-V2506
{
    float freq = value;

    bufferOut[0] = 0;
    const char *suffix = "��";

    if (Math::IsEquals(freq, Float::ERROR))
    {
        std::strcat(bufferOut, String::_ERROR); //-V2513
        return bufferOut;
    }

    if (freq >= 1e6F)
    {
        suffix = "���";
        freq /= 1e6F;
    }
    else if (freq >= 1e3F)
    {
        suffix = "���";
        freq /= 1e3F;
    }

    std::strcat(bufferOut, Float(freq).ToString(false, numDigits).c_str()); //-V2513
    std::strcat(bufferOut, suffix); //-V2513
    return bufferOut;
}


String Time::ToString(bool alwaysSign) const
{
    char buffer[20];
    return String(TimeToString(value, alwaysSign, buffer));
}


String Frequency::ToString() const
{
    char buffer[50];
    return String(FrequencyToString(value, buffer));
}


char *TimeToString(float time, bool alwaysSign, char buffer[20]) //-V2506
{
    if (Math::IsEquals(time, Float::ERROR))
    {
        std::strcpy(buffer, String::_ERROR); //-V2513
        return buffer;
    }

    pString suffix[4] = { "��", "���", "��", "�" };

    static const float factor[4] = { 1e9F, 1e6F, 1e3F, 1.0F };

    float absTime = std::fabsf(time);

    int num = 0;

    if (absTime + 0.5e-10F < 1e-6F)     { }
    else if (absTime + 0.5e-7F < 1e-3F) { num = 1; }
    else if (absTime + 0.5e-3F < 1.0F)  { num = 2; }
    else                                { num = 3; }

    std::strcpy(buffer, Float(time * factor[num]).ToString(alwaysSign, 4).c_str()); //-V2513
    std::strcat(buffer, suffix[num]); //-V2513
    return buffer;
}


char* Time::ToStringAccuracy(bool alwaysSign, char buffer[20], int numDigits) const //-V2506
{
    float time = value;

    buffer[0] = 0;
    const char *suffix = "�";

    float fabsTime = std::fabsf(time);

    if (Math::IsEquals(time, Float::ERROR))
    {
        std::strcat(buffer, String::_ERROR); //-V2513
        return buffer;
    }
    else if (fabsTime + 0.5e-10F < 1e-6F)
    {
        suffix = "��";
        time *= 1e9F;
    }
    else if (fabsTime + 0.5e-7F < 1e-3F)
    {
        suffix = "���";
        time *= 1e6F;
    }
    else if (fabsTime + 0.5e-3F < 1.0F)
    {
        suffix = "��";
        time *= 1e3F;
    }

    std::strcat(buffer, Float(time).ToString(alwaysSign, numDigits).c_str()); //-V2513
    std::strcat(buffer, suffix); //-V2513

    return buffer;
}


String Voltage::ToString(bool alwaysSign) const
{
    char buffer[20];
    return String(VoltageToString(value, alwaysSign, buffer));
}


static char* VoltageToString(float voltage, bool alwaysSign, char buffer[20]) //-V2506
{
    if (Math::IsEquals(voltage, Float::ERROR))
    {
        std::strcpy(buffer, String::_ERROR); //-V2513
        return buffer;
    }

    static const pString suf[4] =
    {
        "\x10���", "\x10��", "\x10�", "\x10��"
    };

    static const float factor[4] = { 1e6F, 1e3F, 1.0F, 1e-3F };

    int num = 0;
    float absValue = std::fabsf(voltage) + 0.5e-4F;

    if (absValue < 1e-3F)
    {
        num = 0;
    }
    else if (absValue < 1.0F)
    {
        num = 1;
    }
    else if (absValue < 1e3F)
    {
        num = 2;
    }
    else
    {
        num = 3;
    }

    std::strcpy(buffer, Float(voltage * factor[num]).ToString(alwaysSign, 4).c_str()); //-V2513
    std::strcat(buffer, suf[num]); //-V2513
    return buffer;
}


String Current::ToString() const
{
    char buffer[50];
    return String(ToString(buffer));
}


char *Current::ToString(char buffer[50]) const //-V2506
{
    float current = value;

    if (Math::IsEquals(current, Float::ERROR))
    {
        std::strcpy(buffer, String::_ERROR); //-V2513
        return buffer;
    }

    pString suf[4] =
    {
        "\x10���", "\x10��", "\x10�", "\x10��"
    };

    static const float factor[4] = { 1e6F, 1e3F, 1.0F, 1e-3F };

    int num = 0;

    float absValue = std::fabsf(current) + 0.5e-4F;

    if (absValue < 1e-3F)
    {
        num = 0;
    }
    else if (absValue < 1.0F)
    {
        num = 1;
    }
    else if (absValue < 1e3F)
    {
        num = 2;
    }
    else
    {
        num = 3;
    }

    std::strcpy(buffer, Float(current * factor[num]).ToString(true, 4).c_str()); //-V2513
    std::strcat(buffer, suf[num]); //-V2513
    return buffer;
}


char* Phase::ToString(char bufferOut[20]) const
{
    std::sprintf(bufferOut, "%s\xa8", Float(value).ToString(false, 4).c_str());
    return bufferOut;
}


String Float::ToString(bool alwaysSign, int numDigits) const
{
    char buffer[30];
    return String(FloatToString(value, alwaysSign, numDigits, buffer));
}


String Integer::ToString(bool alwaysSign, int numMinFields) const
{
    char buffer[30];
    return String(IntegerToString(value, alwaysSign, numMinFields, buffer));
}


int Integer::operator [](int n)
{
    int val = value;

    for (int i = 0; i < n; i++)
    {
        val /= 10;
    }

    float rest = val - static_cast<int>(val / 10.0F) * 10.0F;

    return static_cast<int>(rest);
}


static char *FloatToString(float value, bool alwaysSign, int numDigits, char bufferOut[20]) //-V2506
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
    else if (alwaysSign)
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
    
    float val = static_cast<float>(atof(pBuffer)); //-V2508
    
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
    while (std::strlen(bufferOut) < static_cast<size_t>(numDigits + (signExist ? 2 : 1)))
    {
        std::strcat(bufferOut, "0");
    }
    
#ifdef WIN32

    char *pointer = bufferOut;  //\
                                //|
    while(*pointer)             //|
    {                           //| ��� ����� �� PC, ��� ������ ����� ����� ���� �������
        if(*pointer == ',')     //| 
        {                       //|
            *pointer = '.';     //|
        }                       //|
        pointer++;              //|
    }                           ///

#endif

    return bufferOut;
}


int Hex::NumDigits() const
{
    uint val = value / 10;
    int num = 1;

    while (val > 0)
    {
        num++;
        val = val / 10;
    }
    return num;
}


int Integer::NumDigits() const
{
    int val = Math::Abs(value);
    int num = 1;
    val = val / 10;
    while (val > 0)
    {
        num++;
        val = val / 10;
    }
    return num;
}

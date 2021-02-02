#include "defines.h"
#include "Log.h"
#include "Settings.h"
#include "Calculate/MathFPGA.h"
#include "Display/Display.h"
#include "Display/Objects.h"
#include "Hardware/FPGA.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Utils/Debug.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Utils/ValueSTRICT.h"
#include <cmath>
#include <cstdio>
#include <cstring>


int    MathFPGA::NA = 0; //-V707
int    MathFPGA::NB = 0; //-V707

char MathFPGA::Data::digits[30];
char MathFPGA::Data::units[10];

bool MathFPGA::Validator::isEmpty = true;
uint MathFPGA::Validator::timeClearedFlag = 0;

uint   MathFPGA::Auto::fpgaMin = 0;
uint   MathFPGA::Auto::fpgaMid = 0;
uint   MathFPGA::Auto::fpgaMax = 0;
       
ValueFPGA   *MathFPGA::Measure::valueFPGA = nullptr;

const char *MathFPGA::Data::UGO_DivNULL = "=X/0";


String MathFPGA::Data::GiveDigits()
{
    return String(digits);
}


String MathFPGA::Data::GiveUnits()
{
    return String(units);
}


void MathFPGA::Data::SetDigits(const String &_digits)
{
    DEBUG_POINT_0;

    if (FPGA::IsOverloaded())
    {
        std::strcpy(digits, "œ≈–≈œŒÀÕ≈Õ»≈");
    }
    else
    {
        std::strcpy(digits, _digits.c_str()); //-V2513
    }

    DEBUG_POINT_0;

    Display::zoneData->Refresh();
}


void MathFPGA::Data::SetUnits(const String &_units)
{
    std::strcpy(units, _units.c_str()); //-V2513
    Display::zoneData->Refresh();
}


void MathFPGA::Measure::SetNewData(uint value1, uint value2, uint value3, uint value4, uint value5)
{ 
    DEBUG_POINT_0;
    CreateValue(value1, value2, value3, value4, value5);
    DEBUG_POINT_0;
    Validator::SetValidData();
    DEBUG_POINT_0;
    ProgressBarTimeMeasureZone::timeStart = TIME_MS;
    DEBUG_POINT_0;
}


bool MathFPGA::Measure::CreateValue(uint value1, uint value2, uint value3, uint value4, uint value5)
{
    DEBUG_POINT_0;
    TypeMeasure &type = Channel::Current()->mod.typeMeasure;
    DEBUG_POINT_0;
    if (valueFPGA != nullptr)
    {
        DEBUG_POINT_0;
        delete valueFPGA;
        valueFPGA = nullptr;
    }

    DEBUG_POINT_0;
    if (type.IsFrequency())
    {
        switch (Channel::Current()->mod.modeFrequency)
        {
        case ModeFrequency::Frequency:
            DEBUG_POINT_0;
            valueFPGA = new ValueFrequency_Frequency(value1);
            VERIFY_ON_NULL(valueFPGA);
            DEBUG_POINT_0;
            break;
        case ModeFrequency::T_1:
            valueFPGA = new ValueFrequency_T_1(value1);
            VERIFY_ON_NULL(valueFPGA);
            break;
        case ModeFrequency::Tachometer:
            valueFPGA = new ValueFrequency_Tachometer(value1);
            VERIFY_ON_NULL(valueFPGA);
            break;
        case ModeFrequency::Comparator:
            valueFPGA = new ValueFrequency_Comparator(value1, (int)value2, (int)value3, (int)value4, (int)value5);
            VERIFY_ON_NULL(valueFPGA);
            break;
        case ModeFrequency::RatioAB:
        case ModeFrequency::RatioAC:
        case ModeFrequency::RatioBA:
        case ModeFrequency::RatioBC:
        case ModeFrequency::RatioCA:
        case ModeFrequency::RatioCB:
            valueFPGA = new ValueFrequency_Ratio(value1, value2);
            VERIFY_ON_NULL(valueFPGA);
            break;
        }

        return true;
    }
    else if (type.IsPeriod())
    {
        DEBUG_POINT_0;
        switch (Channel::Current()->mod.modePeriod)
        {
        case ModePeriod::Period:
            DEBUG_POINT_0;
            valueFPGA = new ValuePeriod_Period(value1);
            VERIFY_ON_NULL(valueFPGA);
            DEBUG_POINT_0;
            break;
        case ModePeriod::F_1:
            DEBUG_POINT_0;
            valueFPGA = new ValuePeriod_F_1(value1);
            VERIFY_ON_NULL(valueFPGA);
            DEBUG_POINT_0;
            break;
        }
        DEBUG_POINT_0;

        return true;
    }
    else if (type.IsDuration())
    {
        DEBUG_POINT_0;
        switch (Channel::Current()->mod.modeDuration)
        {
        case ModeDuration::Ndt_1ns:
            valueFPGA = new ValueDuration_Ndt_1ns(value1, value2, value3);
            VERIFY_ON_NULL(valueFPGA);
            break;;
        case ModeDuration::FillFactor:
        case ModeDuration::Phase:
            valueFPGA = new ValueDuration_Phase_FillFactor(value1, value2);
            VERIFY_ON_NULL(valueFPGA);
            break;;
        case ModeDuration::Ndt:
        case ModeDuration::StartStop:
            valueFPGA = new ValueDuration_Ndt_StartStop(value1);
            VERIFY_ON_NULL(valueFPGA);
            break;
        }

        return true;
    }
    else if (type.IsCountPulse())
    {
        DEBUG_POINT_0;
        valueFPGA = new ValueCountPulse(value1);
        VERIFY_ON_NULL(valueFPGA);
        return true;
    }

    DEBUG_POINT_0;

    return false;
}


void MathFPGA::Validator::SetInvalidData()
{
    isEmpty = true;
    timeClearedFlag = TIME_MS;
    ProgressBarTimeMeasureZone::timeStart = timeClearedFlag;
    Data::SetDigits(String("----------"));
    Data::SetUnits(String(""));
}


void MathFPGA::Validator::SetValidData()
{
    isEmpty = false;
}

bool MathFPGA::Validator::DataIsValid()
{
    return !isEmpty;
}


ValueComparator operator-(const ValueComparator &first, const ValueComparator &second)
{
    ValueComparator result = first;
    result.Sub(second);
    return result;
}


ValueComparator operator-(const ValueComparator &first, int second)
{
    return first - ValueComparator(second);
}


ValueComparator operator/(const ValueComparator &first, uint second)
{
    ValueComparator result = first;
    result.Div(second);
    return result;
}


int MathFPGA::Auto::Mid()
{
    return (int)fpgaMid;
}


int MathFPGA::Auto::Min()
{
    return (int)fpgaMin;
}


int MathFPGA::Auto::Max()
{
    return (int)fpgaMax;
}


String MathFPGA::Auto::Give()
{
    if (CURRENT_CHANNEL_IS_A)
    {
        LEVEL_SYNCH_A = ((int)fpgaMid - 512) * 2;
        NA = (int)fpgaMid - 512;
    }

    if (CURRENT_CHANNEL_IS_B)
    {
        LEVEL_SYNCH_B = ((int)fpgaMid - 512) * 2;
        NB = (int)fpgaMid - 512;

    }

    return String("Ã‡ÍÒ %s ÃËÌ %s",
        SU::Int2String(((int)fpgaMax - 512) * 2).c_str(),
        SU::Int2String(((int)fpgaMin - 512) * 2).c_str());
}


void MathFPGA::DecToBin(int dec, char *bin)
{
    int x = dec;
    for (int i = 0; i < 10; i++)
    {
        if (x % 2 != 0) { bin[i] = 1; } //-V2563
        else            { bin[i] = 0; } //-V2563
        x = x / 2;
    }
}


String MathFPGA::BinToString(pString bin, int num)
{
    char buffer[20];

    for (int i = 0; i < num; i++)
    {
        if (bin[i] == 0) //-V2563
        {
            buffer[i] = '0';
        }
        else
        {
            buffer[i] = '1';
        }
    }

    buffer[num] = '\0';

    return String(buffer);
}


void MathFPGA::Auto::Refresh()
{
    for (int i = 0; i < 10; i++)
    {
        fpgaMin = 0;
        fpgaMid = 0;
        fpgaMax = 0;
    }
}

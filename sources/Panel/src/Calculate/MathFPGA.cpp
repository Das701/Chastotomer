#include "defines.h"
#include "Log.h"
#include "Settings.h"
#include "Calculate/MathFPGA.h"
#include "Display/Display.h"
#include "Display/Objects.h"
#include "Hardware/FPGA.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Utils/ValueSTRICT.h"
#include <cmath>
#include <cstdio>
#include <cstring>


int    MathFPGA::NA = 0; //-V707
int    MathFPGA::NB = 0; //-V707

bool MathFPGA::Validator::isEmpty = true;
uint MathFPGA::Validator::timeClearedFlag = 0;

uint   MathFPGA::Auto::fpgaMin = 0;
uint   MathFPGA::Auto::fpgaMid = 0;
uint   MathFPGA::Auto::fpgaMax = 0;
       
void MathFPGA::Measure::SetNewData(uint value1, uint value2, uint value3, uint value4, uint value5)
{ 
    ValueFPGA::Create(value1, value2, value3, value4, value5);

    Validator::SetValidData();
    ProgressBarTimeMeasureZone::timeStart = TIME_MS;
}


void MathFPGA::Validator::SetInvalidData()
{
    isEmpty = true;
    timeClearedFlag = TIME_MS;

    ProgressBarTimeMeasureZone::timeStart = timeClearedFlag;

    ValueFPGA::SetInvalidData();
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

    return String("Макс %s Мин %s",
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

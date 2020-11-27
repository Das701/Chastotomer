#include "defines.h"
#include "Settings.h"
#include "Display/Display.h"
#include "Display/Objects.h"
#include "Hardware/FPGA.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Utils/Value.h"
#include <cstdio>
#include <cstring>


int    MathFPGA::NA = 0; //-V707
int    MathFPGA::NB = 0; //-V707

char MathFPGA::Data::digits[30];
char MathFPGA::Data::units[10];

bool MathFPGA::Validator::isEmpty = true;
uint MathFPGA::Validator::timeClearedFlag = 0;

float  MathFPGA::Interpolator::value = 0.0F;
       
uint   MathFPGA::Auto::fpgaMin = 0;
uint   MathFPGA::Auto::fpgaMid = 0;
uint   MathFPGA::Auto::fpgaMax = 0;
       
float  MathFPGA::FillFactor::value = 0.0F;
int    MathFPGA::FillFactor::zeroes = 0;
       
int       MathFPGA::Measure::decDA = 1;
ValueNANO MathFPGA::Measure::decDataA(0);
ValueNANO MathFPGA::Measure::decDataB(0);
ValueNANO MathFPGA::Measure::decDataC(0);

ValuePICO MathFPGA::Comparator::value(0);


static bool isDivZero = false;


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
    std::strcpy(digits, _digits.c_str());
    Display::zoneData->Refresh();
}


void MathFPGA::Data::SetUnits(const String &_units)
{
    std::strcpy(units, _units.c_str());
    Display::zoneData->Refresh();
}


int MathFPGA::Measure::CalculateFrequencyEmptyZeros(int &manualZeros)
{
    int result = 0;

    const ModeMeasureFrequency &mode = ModeMeasureFrequency::Current();

    if (mode.IsT_1())
    {
        manualZeros = 10 * Channel::timeLabels.ToZeros() / 1000 * Channel::numberPeriods.ToAbs();

        decDataA.Div((uint)Channel::timeLabels.ToZeros());

        double test1 = decDataA.ToDouble();

        if (test1 == 0.0)
        {
            isDivZero = true;
        }
        else
        {
            double test2 = test1 / Channel::numberPeriods.ToAbs();

            decDataA.FromDouble((float)(4 / test2));
        }

        decDA = (int)(decDataA.ToDouble() / 2.0);

        if (decDA < 1000)           { }
        else if (decDA < 1000000)   { decDataA.Div(1000);    }
        else                        { decDataA.Div(1000000); }

        result = 1;
    }
    else if (mode.IsRatioAB() || mode.IsRatioBA())
    {
        result = Channel::numberPeriods.ToAbs();
    }
    else if (mode.IsRatioCA() || mode.IsRatioCB())
    {
        decDataA.Mul(100);
        result = Channel::numberPeriods.ToAbs();
    }
    else if (mode.IsRatioAC() || mode.IsRatioBC())
    {
        int sT = Channel::timeMeasure.ToMS();

        if (decDataB.ToDouble() == 0.0)
        {
            isDivZero = true;
        }

        decDataA.FromDouble(decDataA.ToDouble() / decDataB.ToDouble() / 32);
        manualZeros = 1000000 * sT;
        result = 1;
    }
    else
    {
        int mhz = 1000 * Channel::timeMeasure.ToMS();
        int khz = Channel::timeMeasure.ToMS();

        if (((decDataA.ToDouble() / (float)khz) / 2.0F) < 1000.0F)
        {
            result = khz;
        }
        else
        {
            result = mhz;
        }
        decDA = (int)((decDataA.ToDouble() / (double)khz) / 2.0);

        if (CURRENT_CHANNEL_IS_C)
        {
            if (decDataA.ToUINT64() < 10000)
            {
                decDataC.FromDouble(decDataA.ToDouble());
                khz = khz * 10;
                result = khz;
            }
            else
            {
                decDataC.FromDouble(decDataA.ToDouble());
                mhz = mhz * 10;
                result = mhz;
            }
        }

        if (CURRENT_CHANNEL_IS_D)
        {
            if (decDataA.ToDouble() * 64.0F / (1000.0F * (float)khz) > 19000.0F)
            {
                decDataC.FromDouble(0.0);
                result = khz;
            }
            else
            {
                decDataC.FromDouble(decDataA.ToDouble() * 64 / 1000);
                result = mhz;
            }

            decDA = (int)decDataC.ToUINT64();
        }
    }

    return result;
}


int MathFPGA::Measure::CalculatePeriodEmptyZeros()
{
    int result = 1;

    if (ModeMeasurePeriod::Current().IsF_1())
    {
        int sT = Channel::timeMeasure.ToMS();

        decDA = (int)(decDataA.ToDouble() / (2.0 * (double)sT));

        if (decDataA.ToDouble() == 0.0)
        {
            isDivZero = true;
        }
        else
        {
            decDataA.FromDouble(4 / decDataA.ToDouble());
        }

        decDataA.Mul((uint)sT);
        decDataA.Mul((uint)sT);

        if (decDA >= 1000)      { decDataA.Mul(10000000); }
        else if (decDA <= 1)    { decDataA.Mul(10);       }
        else                    { decDataA.Mul(10000);    }

        result = sT * 10;
    }
    else
    {
        if (Channel::timeLabels.IsT_7() || Channel::timeLabels.IsT_4())
        {
            result *= 10;
        }
        else if (Channel::timeLabels.IsT_8() || Channel::timeLabels.IsT_5())
        {
            result *= 100;
        }

        result *= Channel::numberPeriods.ToAbs();
    }

    return result;
}


int MathFPGA::Measure::CalculateDurationEmptyZeros()
{
    if (Channel::timeLabels.IsT_7())
    {
        return 10;
    }
    else if (Channel::timeLabels.IsT_8() || Channel::timeLabels.IsT_5())
    {
        return 100;
    }

    return 1;
}


void MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::E type, uint value1, uint value2, uint value3)
{
    if (Validator::VerySmallTime())
    {
        return;
    }

    isDivZero = false;

    switch (type)
    {
    case TypeData::MainCounters:        AppendDataMainCounters(value1, value2);             break;
    case TypeData::Interpolator:        Interpolator::Calculate(value1, value2, value3);    break;
    case TypeData::FillFactorPhase:     FillFactor::Calculate(value1, value2);              break;
    case TypeData::Comparator:          Comparator::Calculate(value1, value2, value3);      break;
    }

    CalculateNewData();

    CalculateUnits();

    Validator::SetValidData();

    ProgressBarTimeMeasureZone::timeStart = TIME_MS;
}


void MathFPGA::Validator::SetInvalidData()
{
    isEmpty = true;
    timeClearedFlag = TIME_MS;
    ProgressBarTimeMeasureZone::timeStart = TIME_MS;
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


void MathFPGA::Comparator::Calculate(uint fx, uint tizm, uint nkal)
{
    int decNkal = (int)nkal;

    if (decNkal != 0)
    {
        int decTizm = (int)tizm;

        if ((tizm & (1U << 15)) != 0)
        {
            decTizm -= 65536;
        }

        ValuePICO dx(decTizm);
        dx.Div((uint)decNkal);
        dx.Div(2 * 5000000);

        ValuePICO k(5000000);
        k.Sub(ValuePICO((int)fx));
        k.Div(5000000);
        k.Sub(dx);
        k.Mul(1000000);

        k.SetSign(1);

        value = k;

        if (MathFPGA::Comparator::values.AppendValue(k.ToDouble()))
        {
            Display::Refresh();
        }
    }
}


void MathFPGA::Measure::AppendDataMainCounters(uint counterA, uint counterB)
{
    decDataA.FromDouble((double)counterA);

    if ((ModeMeasureFrequency::Current().IsRatioAC() || ModeMeasureFrequency::Current().IsRatioBC()) && PageModesA::RelationCheck())
    {
        decDataB.FromDouble(counterB);
    }

    if (CURRENT_CHANNEL_IS_C)
    {
        decDataA.Mul(64);
        decDataA.Div(100);
    }
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

    return String("Ìàêñ %s Ìèí %s", SU::Int2String(((int)fpgaMax - 512) * 2).c_str(), SU::Int2String(((int)fpgaMin - 512) * 2).c_str());
}


void MathFPGA::FillFactor::Calculate(uint period, uint duration)
{
    value = (float)duration / (float)period;

    if (ModeMeasureDuration::Current().IsPhase())
    {
        value *= 360;

        if (value == 360.0F)
        {
            value = 0.0F;
        }
    }
    else
    {
        if (value < 0)
        {
            value *= -1;
        }

        if (value != 0.0F) //-V2550 //-V550
        {
            while (value < 1)
            {
                value *=  10;
                zeroes++;
            }
        }
    }
}


void MathFPGA::DecToBin(int dec, char *bin)
{
    int x = dec;
    for (int i = 0; i < 10; i++)
    {
        if (x % 2 != 0) { bin[i] = 1; }
        else            { bin[i] = 0; }
        x = x / 2;
    }
}


String MathFPGA::BinToString(pString bin, int num)
{
    char buffer[20];

    for (int i = 0; i < num; i++)
    {
        if (bin[i] == 0)
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


void MathFPGA::Measure::Calculate(int &emptyZeros, ValueNANO &data)
{
    int manualZeros = 1;

    if (Channel::Current().typeMeasure.IsFrequency())
    {
        emptyZeros = CalculateFrequencyEmptyZeros(manualZeros);
    }
    else if (Channel::Current().typeMeasure.IsDuration())
    {
        emptyZeros = CalculateDurationEmptyZeros();
    }
    else if (Channel::Current().typeMeasure.IsPeriod())
    {
        emptyZeros = CalculatePeriodEmptyZeros();
    }

    data = decDataA;

    if (CURRENT_CHANNEL_IS_D)
    {
        data.FromDouble(decDataC.ToDouble());
    }

    data.Div((uint)(2 * emptyZeros));

    if (manualZeros != 1) //-V1051
    {
        emptyZeros = manualZeros;
    }
}


void MathFPGA::Measure::CalculateNewData()
{
    if (FPGA::IsOverloaded())
    {
        Data::SetDigits(String("ÏÅÐÅÏÎËÍÅÍÈÅ"));
        return;
    }

    if (Channel::Current().typeMeasure.IsCountPulse())
    {
        float value = decDataA.ToFloat() / 2.0F;

        if (CURRENT_CHANNEL_IS_C)
        {
            value *= 100.0F;
        }

        if (ModeMeasureCountPulse::Current().IsFromPeriod())
        {
            value /= (float)Channel::numberPeriods.ToAbs();
        }

        Data::SetDigits(String("%10.0f", decDataA.ToFloat()));
    }
    else
    {
        if (ModeMeasureFrequency::Current().IsTachometer())
        {
            Data::SetDigits(String("%10.0f", decDataA.ToFloat() / 2.0F));
        }
        else if (ModeMeasureFrequency::Current().IsComparator())
        {
            Data::SetDigits(Comparator::value.ToString());
        }
        else if (ModeMeasureDuration::Current().IsNdt_1ns())
        {
            Data::SetDigits(String("%10.2f", MathFPGA::Interpolator::value));
        }
        else if (Channel::Current().typeMeasure.IsDuration() && (ModeMeasureDuration::Current().IsFillFactor() || ModeMeasureDuration::Current().IsPhase()))
        {
            if (ModeMeasureDuration::Current().IsPhase())
            {
                if (Math::InBound(MathFPGA::FillFactor::value, 0.0F, 360.0F))
                {
                    Data::SetDigits(String("%10.3f", MathFPGA::FillFactor::value));
                }
                else
                {
                    Data::SetDigits(String(" "));
                }
            }
            else
            {
                Data::SetDigits(String("%10.7f", MathFPGA::FillFactor::value));
            }
        }
        else
        {
            int emptyZeros = 0;
            ValueNANO data(0);

            Calculate(emptyZeros, data);

            if (isDivZero)
            {
                Data::SetDigits(String("=X/0"));
                return;
            }

            int pow = 0;

            while (emptyZeros >= 10)
            {
                pow++;
                emptyZeros /= 10;
            }

            if (pow < 10)
            {
                char format[10];
                std::strcpy(format, "%10.0f");
                format[4] = (char)(pow | 0x30);

                Data::SetDigits(String(format, data.ToFloat()));
            }
            else
            {
                char format[10];
                std::strcpy(format, "%10.10f");
                format[5] = (char)((pow - 10) | 0x30);

                Data::SetDigits(String(format, data.ToFloat()));
            }
        }
    }
}


void MathFPGA::Measure::CalculateUnits()
{
    if (ModeMeasureDuration::Current().IsNdt_1ns())
    {
        Data::SetUnits(String(" ns"));
    }
    else if(Channel::Current().typeMeasure.IsDuration() && (ModeMeasureDuration::Current().IsFillFactor() || ModeMeasureDuration::Current().IsPhase()))
    {
        if (ModeMeasureDuration::Current().IsPhase())
        {
            Data::SetUnits(String(" $"));
        }
        else
        {
            String result(" E-0");
            result[3] = (char)(FillFactor::zeroes | 0x30);

            FillFactor::zeroes = 0;

            Data::SetUnits(result);
        }
    }
    else
    {
        if (ModeMeasureFrequency::Current().IsRatio() ||
            ModeMeasureFrequency::Current().IsTachometer() ||
            Channel::Current().typeMeasure.IsCountPulse())
        {
            Data::SetUnits(String(" "));
        }
        else
        {
            if (Channel::Current().typeMeasure.IsFrequency())
            {
                if (ModeMeasureFrequency::Current().IsT_1())
                {
                    if (decDA < 1000)           { Data::SetUnits(String(" Hz"));  }
                    else if (decDA < 1000000)   { Data::SetUnits(String(" kHz")); }
                    else                        { Data::SetUnits(String(" MHz")); }
                }
                else if (PageModesA::modeMeasureFrequency.IsComparator() && CURRENT_CHANNEL_IS_A)
                {
                    Data::SetUnits(String(" E-6"));
                }
                else
                {
                    if (CURRENT_CHANNEL_IS_C)
                    {
                        if (decDataC.ToUINT64() / 2 < 10000)    { Data::SetUnits(String(" MHz")); }
                        else                                    { Data::SetUnits(String(" GHz")); }
                    }
                    else if (CURRENT_CHANNEL_IS_D)   
                    {
                        Data::SetUnits(String(" GHz"));
                    }
                    else
                    {
                        if (decDA < 1000)           { Data::SetUnits(String(" kHz")); }
                        else                        { Data::SetUnits(String(" MHz")); }
                    }
                }
            }
            else
            {
                if ((CURRENT_CHANNEL_IS_A && Channel::A.typeMeasure.IsPeriod() && PageModesA::modeMeasurePeriod.IsF_1()) ||
                    (CURRENT_CHANNEL_IS_B && Channel::B.typeMeasure.IsPeriod() && PageModesB::modeMeasurePeriod.IsF_1()))
                {
                    if (decDA >= 1000)      { Data::SetUnits(String(" ns")); }
                    else if (decDA <= 1)    { Data::SetUnits(String(" ms")); }
                    else                    { Data::SetUnits(String(" us")); }
                }
                else
                {
                    PeriodTimeLabels &current = Channel::timeLabels;

                    if (current.IsT_3() || current.IsT_4() || current.IsT_5())  { Data::SetUnits(String(" ms")); }
                    else                                                        { Data::SetUnits(String(" us")); }
                }
            }
        }
    }
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


void MathFPGA::Interpolator::Calculate(uint timer, uint cal1, uint cal2)
{
    value = (float)(100 * timer) / (float)(cal2 - cal1);
}

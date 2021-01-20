#include "defines.h"
#include "Settings.h"
#include "Display/Display.h"
#include "Display/Objects.h"
#include "Hardware/FPGA.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Utils/ValueSTRICT.h"
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
ValueSTRICT MathFPGA::Measure::decDataA(0.0);
ValueSTRICT MathFPGA::Measure::decDataB(0.0);
ValueSTRICT MathFPGA::Measure::decDataC(0.0);

ValueComparator MathFPGA::Comparator::value(0);


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
    std::strcpy(digits, _digits.c_str()); //-V2513
    Display::zoneData->Refresh();
}


void MathFPGA::Data::SetUnits(const String &_units)
{
    std::strcpy(units, _units.c_str()); //-V2513
    Display::zoneData->Refresh();
}


int MathFPGA::Measure::CalculateFrequencyEmptyZeros(int &manualZeros)
{
    int result = 0;

    const ModeFrequency &mode = ModeFrequency::Current();

    if (mode.IsT_1())
    {
        manualZeros = 10 * ModesChannel::timeLabels.ToZeros() / 1000 * ModesChannel::numberPeriods.ToAbs();

        decDataA.DivUINT((uint)ModesChannel::timeLabels.ToZeros()); //-V2533

        double test1 = decDataA.ToDouble();

        if (test1 == 0.0) //-V2550 //-V550
        {
            isDivZero = true;
        }
        else
        {
            double test2 = test1 / ModesChannel::numberPeriods.ToAbs(); //-V2564

            decDataA.FromDouble((float)(4 / test2)); //-V2533 //-V2564
        }

        decDA = (int)(decDataA.ToDouble() / 2.0); //-V2533

        if (decDA < 1000)           { }
        else if (decDA < 1000000)   { decDataA.DivUINT(1000);    }
        else                        { decDataA.DivUINT(1000000); }

        result = 1;
    }
    else if (mode.IsRatioAB() || mode.IsRatioBA())
    {
        result = ModesChannel::numberPeriods.ToAbs();
    }
    else if (mode.IsRatioCA() || mode.IsRatioCB())
    {
        decDataA.MulUINT(100);
        result = ModesChannel::numberPeriods.ToAbs();
    }
    else if (mode.IsRatioAC() || mode.IsRatioBC())
    {
        int sT = ModesChannel::timeMeasure.ToMS();

        if (decDataB.ToDouble() == 0.0) //-V2550 //-V550
        {
            isDivZero = true;
        }

        decDataA.FromDouble(decDataA.ToDouble() / decDataB.ToDouble() / 32); //-V2564
        manualZeros = 1000000 * sT;
        result = 1;
    }
    else
    {
        int mhz = 1000 * ModesChannel::timeMeasure.ToMS();
        int khz = ModesChannel::timeMeasure.ToMS();

        if (((decDataA.ToDouble() / (float)khz) / 2.0F) < 1000.0F) //-V2533
        {
            result = khz;
        }
        else
        {
            result = mhz;
        }
        decDA = (int)((decDataA.ToDouble() / (double)khz) / 2.0); //-V2533

        if (CURRENT_CHANNEL_IS_C)
        {
            if(decDataA.ToUnits(Order::Micro) < 10)
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
            if (decDataA.ToDouble() * 64.0F / (1000.0F * (float)khz) > 19000.0F) //-V2533
            {
                decDataC.FromDouble(0.0);
                result = khz;
            }
            else
            {
                decDataC.FromDouble(decDataA.ToDouble() * 64 / 1000); //-V2564
                result = mhz;
            }

            decDA = (int)decDataC.ToUnits(Order::Nano); //-V2533
        }
    }

    return result;
}


int MathFPGA::Measure::CalculatePeriodEmptyZeros()
{
    int result = 1;

    if (ModePeriod::Current().IsF_1())
    {
        int sT = ModesChannel::timeMeasure.ToMS();

        double d = decDataA.ToDouble();

        decDA = (int)(d / (2.0 * (double)sT)); //-V2533

        if (decDataA.ToDouble() == 0.0) //-V550 //-V2550
        {
            isDivZero = true;
        }
        else
        {
            ValueSTRICT value4((int64)4);

            value4 = value4;

            decDataA = 4 / decDataA;
        }

        decDataA.MulUINT((uint)sT); //-V2533
        decDataA.MulUINT((uint)sT); //-V2533

        if (decDA >= 1000)  { decDataA.MulUINT(10000000);    }
        else if(decDA > 1)  { decDataA.MulUINT(10000);     }
        else                { decDataA.MulUINT(10); }

        result = sT * 10;
    }
    else
    {
        if (ModesChannel::timeLabels.IsT_7() || ModesChannel::timeLabels.IsT_4())
        {
            result *= 10;
        }
        else if (ModesChannel::timeLabels.IsT_8() || ModesChannel::timeLabels.IsT_5()) //-V2516
        {
            result *= 100;
        }

        result *= ModesChannel::numberPeriods.ToAbs();
    }

    return result;
}


int MathFPGA::Measure::CalculateDurationEmptyZeros() //-V2506
{
    if (ModesChannel::timeLabels.IsT_7())
    {
        return 10;
    }
    else if (ModesChannel::timeLabels.IsT_8() || ModesChannel::timeLabels.IsT_5()) //-V2516
    {
        return 100;
    }

    return 1;
}


void MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::E type, uint value1, uint value2, uint value3, uint value4, uint value5) //-V2506
{
    if (Validator::VerySmallTime())
    {
        return;
    }

    isDivZero = false;

    switch (type)
    {
    case TypeData::MainCounters:        AppendDataMainCounters(value1, value2);                                             break;
    case TypeData::Interpolator:        Interpolator::Calculate(value1, value2, value3);                                    break;
    case TypeData::FillFactorPhase:     FillFactor::Calculate(value1, value2);                                              break;
    case TypeData::Comparator:          Comparator::Calculate(value1, (int)value2, (int)value3, (int)value4, (int)value5);  break;
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


void MathFPGA::Comparator::Calculate(uint counter, int interpol1, int cal1, int interpol2, int cal2)
{
    /*
    *   A = (N - counter) / N - dx / N;
    *   A = (N - conter - dx) / N
    *   dx = (interpol1 / cal1 - interpol2 / cal2) / 2
    */

    if (cal1 != 0 && cal2 != 0)
    {
        if ((interpol1 & (1U << 15)) != 0)
        {
            interpol1 -= 65536;
        }

        if ((interpol2 & (1U << 15)) != 0)
        {
            interpol2 -= 65536;
        }

        uint N = 5000000U;

        if (Channel::Current()->mod.timeComparator.Is_10s())
        {
            N *= 10;
        }

        ValueComparator k1 = ValueComparator(interpol1) / (uint)cal1;

        ValueComparator k2 = ValueComparator(interpol2) / (uint)cal2;

        ValueComparator dx = (k1 - k2) / 2;

        ValueComparator A((int)N - (int)counter);
        A.Sub(dx);
        A.Div(N);

        A.Mul(1000000);     // Это приводим к своей выводимой степени

        if (!Channel::Current()->mod.timeComparator.Is_1s())
        {
            A.Mul(10);
        }

        A.SetSign(1);

        value = A;

        if (MathFPGA::Comparator::values.AppendValue(A.ToDouble()))
        {
            Display::Refresh();
        }
    }
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


void MathFPGA::Measure::AppendDataMainCounters(uint counterA, uint counterB)
{
    decDataA.FromDouble((double)counterA); //-V2533

    if ((ModeFrequency::Current().IsRatioAC() || ModeFrequency::Current().IsRatioBC()) && Relation::IsEnabled())
    {
        decDataB.FromDouble(counterB); //-V2564
    }

    if (CURRENT_CHANNEL_IS_C)
    {
        decDataA.MulUINT(64);

        if (Channel::Current()->mod.typeMeasure.IsFrequency())
        {
            decDataA.DivUINT(100);
        }
    }
    else if (CURRENT_CHANNEL_IS_D) //-V2516
    {
    }
}


int MathFPGA::Auto::Mid()
{
    return (int)fpgaMid; //-V2533
}


int MathFPGA::Auto::Min()
{
    return (int)fpgaMin; //-V2533
}


int MathFPGA::Auto::Max()
{
    return (int)fpgaMax; //-V2533
}


String MathFPGA::Auto::Give()
{
    if (CURRENT_CHANNEL_IS_A)
    {
        LEVEL_SYNCH_A = ((int)fpgaMid - 512) * 2; //-V2533
        NA = (int)fpgaMid - 512; //-V2533
    }

    if (CURRENT_CHANNEL_IS_B)
    {
        LEVEL_SYNCH_B = ((int)fpgaMid - 512) * 2; //-V2533
        NB = (int)fpgaMid - 512; //-V2533

    }

    return String("Макс %s Мин %s", SU::Int2String(((int)fpgaMax - 512) * 2).c_str(), SU::Int2String(((int)fpgaMin - 512) * 2).c_str()); //-V2533
}


void MathFPGA::FillFactor::Calculate(uint period, uint duration)
{
    value = (float)duration / (float)period; //-V2533

    if (ModeDuration::Current().IsPhase())
    {
        value *= 360; //-V2564

        if (value == 360.0F) //-V2550 //-V550
        {
            value = 0.0F;
        }
    }
    else
    {
        if (value < 0) //-V2564
        {
            value *= -1; //-V2564
        }

        if (value != 0.0F) //-V2550 //-V550
        {
            while (value < 1) //-V2564
            {
                value *=  10; //-V2564
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


void MathFPGA::Measure::Calculate(int &emptyZeros, ValueSTRICT &data)
{
    int manualZeros = 1;

    TypeMeasure &type = Channel::Current()->mod.typeMeasure;

    if (type.IsFrequency())
    {
        emptyZeros = CalculateFrequencyEmptyZeros(manualZeros);
    }
    else if (type.IsDuration())
    {
        emptyZeros = CalculateDurationEmptyZeros();
    }
    else if (type.IsPeriod()) //-V2516
    {
        emptyZeros = CalculatePeriodEmptyZeros();
    }

    data = decDataA;

    if (CURRENT_CHANNEL_IS_D)
    {
        data.FromDouble(decDataC.ToDouble());
    }

    data.DivUINT((uint)(2 * emptyZeros)); //-V2533

    if (manualZeros != 1) //-V1051
    {
        emptyZeros = manualZeros;
    }
}


void MathFPGA::Measure::CalculateNewData() //-V2506
{
    if (FPGA::IsOverloaded())
    {
        Data::SetDigits(String("ПЕРЕПОЛНЕНИЕ"));
        return;
    }

    if (Channel::Current()->mod.typeMeasure.IsCountPulse())
    {
        double value = decDataA.ToDouble() / 2.0;

        if (ModeCountPulse::Current().IsFromPeriod())
        {
            value /= (double)ModesChannel::numberPeriods.ToAbs(); //-V2533
        }

        Data::SetDigits(String("%10.0f", value));
    }
    else
    {
        if (ModeFrequency::Current().IsTachometer())
        {
            Data::SetDigits(String("%10.0f", decDataA.ToDouble() / 2.0));
        }
        else if (ModeFrequency::Current().IsComparator())
        {
            Data::SetDigits(Comparator::value.ToString());
        }
        else if (ModeDuration::Current().IsNdt_1ns())
        {
            Data::SetDigits(String("%10.2f", MathFPGA::Interpolator::value));
        }
        else if (Channel::Current()->mod.typeMeasure.IsDuration() && (ModeDuration::Current().IsFillFactor() || ModeDuration::Current().IsPhase()))
        {
            if (ModeDuration::Current().IsPhase())
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
            ValueSTRICT data(0.0);

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
                std::strcpy(format, "%10.0f"); //-V2513
                format[4] = (char)(pow | 0x30); //-V2533

                Data::SetDigits(String(format, data.ToDouble()));
            }
            else
            {
                char format[10];
                std::strcpy(format, "%10.10f"); //-V2513
                format[5] = (char)((pow - 10) | 0x30); //-V2533

                Data::SetDigits(String(format, data.ToDouble()));
            }
        }
    }
}


void MathFPGA::Measure::CalculateUnits()
{
    TypeMeasure &type = Channel::Current()->mod.typeMeasure;

    if (ModeDuration::Current().IsNdt_1ns())
    {
        Data::SetUnits(String(" ns"));
    }
    else if(type.IsDuration() && (ModeDuration::Current().IsFillFactor() || ModeDuration::Current().IsPhase()))
    {
        if (ModeDuration::Current().IsPhase())
        {
            Data::SetUnits(String(" $"));
        }
        else
        {
            String result(" E-0");
            result[3] = (char)(FillFactor::zeroes | 0x30); //-V2533

            FillFactor::zeroes = 0;

            Data::SetUnits(result);
        }
    }
    else
    {
        if (ModeFrequency::Current().IsRatio() ||
            ModeFrequency::Current().IsTachometer() ||
            type.IsCountPulse())
        {
            Data::SetUnits(String(" "));
        }
        else
        {
            if (type.IsFrequency())
            {
                if (ModeFrequency::Current().IsT_1())
                {
                    if (decDA < 1000)           { Data::SetUnits(String(" Hz"));  }
                    else if (decDA < 1000000)   { Data::SetUnits(String(" kHz")); }
                    else                        { Data::SetUnits(String(" MHz")); }
                }
                else if (Channel::A->mod.modeFrequency.IsComparator() && CURRENT_CHANNEL_IS_A)
                {
                    if (Channel::Current()->mod.timeComparator.Is_1s())
                    {
                        Data::SetUnits(String(" E-6"));
                    }
                    else
                    {
                        Data::SetUnits(String(" E-7"));
                    }
                }
                else
                {
                    if (CURRENT_CHANNEL_IS_C)
                    {
                        if(decDataC.ToUnits(Order::Micro) / 2 < 10) { Data::SetUnits(String(" MHz")); }
                        else                                        { Data::SetUnits(String(" GHz")); }
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
                if ((CURRENT_CHANNEL_IS_A && Channel::A->mod.typeMeasure.IsPeriod() && Channel::A->mod.modePeriod.IsF_1()) ||
                    (CURRENT_CHANNEL_IS_B && Channel::B->mod.typeMeasure.IsPeriod() && Channel::B->mod.modePeriod.IsF_1()))
                {
                    if (decDA >= 1000)      { Data::SetUnits(String(" ns")); }
                    else if (decDA <= 1)    { Data::SetUnits(String(" ms")); }
                    else                    { Data::SetUnits(String(" us")); }
                }
                else
                {
                    PeriodTimeLabels &current = ModesChannel::timeLabels;

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
    value = (float)(100 * timer) / (float)(cal2 - cal1); //-V2533
}

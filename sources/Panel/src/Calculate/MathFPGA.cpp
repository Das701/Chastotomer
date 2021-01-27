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

char MathFPGA::Data::digits[30];
char MathFPGA::Data::units[10];

bool MathFPGA::Validator::isEmpty = true;
uint MathFPGA::Validator::timeClearedFlag = 0;

float  MathFPGA::Interpolator::value = 0.0F;
       
uint   MathFPGA::Auto::fpgaMin = 0;
uint   MathFPGA::Auto::fpgaMid = 0;
uint   MathFPGA::Auto::fpgaMax = 0;
       
ValueSTRICT MathFPGA::FillFactor::value((int64)0);
int         MathFPGA::FillFactor::zeroes = 0;
       
int         MathFPGA::Measure::decDA = 1;
ValueSTRICT MathFPGA::Measure::counterA((int64)0);
ValueSTRICT MathFPGA::Measure::counterB((int64)0);
ValueSTRICT MathFPGA::Measure::counterC((int64)0);
int         MathFPGA::Measure::powDataA = 0;

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


int MathFPGA::Measure::CalculateFrequencyEmptyZeros()
{
    int result = 0;

    const ModeFrequency &mode = ModeFrequency::Current();

    if (mode.IsT_1())
    {
        result = 1;

        uint v = (uint)counterA.ToDouble();

        do 
        {
            result *= 10;
            v /= 10;

        } while (v > 0);

        counterA.DivUINT((uint)ModesChannel::timeLabels.ToZeros());

        if (counterA.ToDouble() == 0.0) //-V2550 //-V550
        {
            isDivZero = true;
        }
        else
        {
            ValueSTRICT value(4.0 * ModesChannel::numberPeriods.ToAbs()); //-V2564

            value.DivDOUBLE(counterA.ToDouble()); //-V2564
            
            counterA = value;
        }

        decDA = (int)(counterA.ToDouble() / 2.0);

        if (decDA < 1000)      { }
        else if (decDA < 1000000)   { counterA.DivUINT(1000);    }
        else                        { counterA.DivUINT(1000000); }

        result = 1;
    }
    else if (mode.IsRatioAB() || mode.IsRatioBA())
    {
        result = ModesChannel::numberPeriods.ToAbs();
    }
    else if (mode.IsRatioCA() || mode.IsRatioCB())
    {
        counterA.MulUINT(100);
        result = ModesChannel::numberPeriods.ToAbs();
    }
    else if (mode.IsRatioAC() || mode.IsRatioBC())
    {
        int sT = ModesChannel::timeMeasure.ToMS();

        if (counterB.ToDouble() == 0.0) //-V2550 //-V550
        {
            isDivZero = true;
        }

        counterA.FromDouble(counterA.ToDouble() / counterB.ToDouble() / 32); //-V2564
        result = 1000000 * sT;
    }
    else
    {
        int mhz = 1000 * ModesChannel::timeMeasure.ToMS();
        int khz = ModesChannel::timeMeasure.ToMS();

        if (((counterA.ToDouble() / (float)khz) / 2.0F) < 1000.0F)
        {
            result = khz;
        }
        else
        {
            result = mhz;
        }
        decDA = (int)((counterA.ToDouble() / (double)khz) / 2.0);

        if (CURRENT_CHANNEL_IS_C)
        {
            if (counterA.ToUnits(Order::Micro) < 10)
            {
                counterC.FromDouble(counterA.ToDouble());
                khz = khz * 10;
                result = khz;
            }
            else
            {
                counterC.FromDouble(counterA.ToDouble());
                mhz = mhz * 10;
                result = mhz;
            }
        }

        if (CURRENT_CHANNEL_IS_D)
        {
            if (counterA.ToDouble() * 64.0F / (1000.0F * (float)khz) > 19000.0F)
            {
                counterC.FromDouble(0.0);
                result = khz;
            }
            else
            {
                counterC.FromDouble(counterA.ToDouble() * 64 / 1000); //-V2564
                result = mhz;
            }

            decDA = (int)counterC.ToUnits(Order::Nano);
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

        decDA = (int)(counterA.ToDouble() / (2.0 * (double)sT));

        if (counterA.ToDouble() == 0.0) //-V550 //-V2550
        {
            isDivZero = true;
        }
        else
        {
            ValueSTRICT value(40000.0 * sT);

            value.DivDOUBLE(counterA.ToDouble());

            counterA = value;
        }

        counterA.MulUINT((uint)sT);

        if (decDA >= 1000)  { counterA.MulUINT(10000); }
        else if (decDA > 1) { counterA.MulUINT(10);    }
        else                { counterA.DivUINT(1000);    }

        result = sT * 100;
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


int MathFPGA::Measure::CalculateDurationEmptyZeros()
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


void MathFPGA::Measure::SetNewData(TypeData::E type, uint value1, uint value2, uint value3, uint value4, uint value5)
{
    isDivZero = false;

    value1 = 5000000;

    if (CreateValue(type, value1, value2, value3, value4, value5))
    {
        Validator::SetValidData();

        ProgressBarTimeMeasureZone::timeStart = TIME_MS;

        return;
    }

    switch (type)
    {
    case TypeData::MainCounters:
        AppendDataMainCounters(value1, value2);
        break;

    case TypeData::Interpolator:
        Interpolator::Calculate(value1, value2, value3);
        break;

    case TypeData::FillFactorPhase:
        FillFactor::Calculate(value1, value2);
        break;

    case TypeData::Comparator:
        Comparator::Calculate(value1, (int)value2, (int)value3, (int)value4, (int)value5);
        break;
    }

    CalculateNewData();

    CalculateUnits();

    Validator::SetValidData();

    ProgressBarTimeMeasureZone::timeStart = TIME_MS;
}


bool MathFPGA::Measure::CreateValue(TypeData::E typeData,
    uint value1, uint value2, uint value3, uint value4, uint value5)
{
    TypeMeasure &type = Channel::Current()->mod.typeMeasure;

    if (typeData == TypeData::MainCounters)
    {
        if (type.IsFrequency())
        {
            if (Channel::Current()->mod.modeFrequency.IsFrequency())
            {

            }
        }
    }

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


void MathFPGA::Measure::AppendDataMainCounters(uint valueA, uint valueB)
{
    LOG_WRITE("%d", valueA);

    counterA.FromDouble((double)valueA);

    powDataA = 0;

    do
    {
        powDataA++;
        valueA /= 10;

    } while (valueA > 0);

    if ((ModeFrequency::Current().IsRatioAC() || ModeFrequency::Current().IsRatioBC()) && Relation::IsEnabled())
    {
        counterB.FromDouble(valueB);
    }

    if (CURRENT_CHANNEL_IS_C)
    {
        counterA.MulUINT(64);

        if (Channel::Current()->mod.typeMeasure.IsFrequency())
        {
            counterA.DivUINT(100);
        }
    }
    else if (CURRENT_CHANNEL_IS_D)
    {
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

    return String("Макс %s Мин %s",
        SU::Int2String(((int)fpgaMax - 512) * 2).c_str(),
        SU::Int2String(((int)fpgaMin - 512) * 2).c_str());
}


void MathFPGA::FillFactor::Calculate(uint period, uint duration)
{
    LOG_WRITE("%d %d", period, duration);

    if (period == 0 && duration == 0)
        period = 1;

    value = ValueSTRICT((int64)duration);
    value.DivUINT(period);

    Measure::powDataA = 1;

    do
    {
        Measure::powDataA++;
        period /= 10;

    } while (period > 0);

    if (ModeDuration::Current().IsPhase())
    {
        value.MulUINT(360);

        if (value.ToDouble() == 360.0)
        {
            value = ValueSTRICT((int64)0);
        }
    }
    else
    {
        if (value.Sign() < 0)
        {
            value.SetSign(1);
        }

        if (value.ToDouble() != 0.0)
        {
            while (value.ToUnits(Order::Milli) < 1000)
            {
                value.MulUINT(10);
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


void MathFPGA::Measure::Calculate(int &pow, ValueSTRICT &data)
{
    int emptyZeros = CalculateEmptyZeros();

    if (CURRENT_CHANNEL_IS_D) { data.FromDouble(counterC.ToDouble()); }
    else                      { data = counterA; }

    data.DivUINT((uint)(2 * emptyZeros));

    pow = (int)std::log10((float)emptyZeros);
}


int MathFPGA::Measure::CalculateEmptyZeros()
{
    int result = 0;

    switch (Channel::Current()->mod.typeMeasure.value)
    {
    case TypeMeasure::Frequency:    result = CalculateFrequencyEmptyZeros();  break;
    case TypeMeasure::Duration:     result = CalculateDurationEmptyZeros();   break;
    case TypeMeasure::Period:       result = CalculatePeriodEmptyZeros();     break;
    }

    return result;
}


void MathFPGA::Measure::CalculateNewData()
{
    if (FPGA::IsOverloaded())
    {
        Data::SetDigits(String("ПЕРЕПОЛНЕНИЕ"));
        return;
    }

    if (Channel::Current()->mod.typeMeasure.IsCountPulse())
    {
        double value = counterA.ToDouble() / 2.0;

        if (ModeCountPulse::Current().IsFromPeriod())
        {
            value /= (double)ModesChannel::numberPeriods.ToAbs();
        }

        Data::SetDigits(String("%10.0f", value));
    }
    else
    {
        if (ModeFrequency::Current().IsTachometer())
        {
            Data::SetDigits(String("%10.0f", counterA.ToDouble() / 2.0));
        }
        else if (ModeFrequency::Current().IsComparator())
        {
            Data::SetDigits(Comparator::value.ToString());
        }
        else if (ModeDuration::Current().IsNdt_1ns())
        {
            Data::SetDigits(String("%10.2f", MathFPGA::Interpolator::value));
        }
        else if (Channel::Current()->mod.typeMeasure.IsDuration() &&
            (ModeDuration::Current().IsFillFactor() || ModeDuration::Current().IsPhase()))
        {
            if (ModeDuration::Current().IsPhase())
            {
                if (Math::InBound((float)MathFPGA::FillFactor::value.ToDouble(), 0.0F, 360.0F))
                {
                    Data::SetDigits(String("%10.3f", MathFPGA::FillFactor::value.ToDouble()));
                }
                else
                {
                    Data::SetDigits(String(" "));
                }
            }
            else
            {
                char buffer[30];
                std::sprintf(buffer, "%10.10f", MathFPGA::FillFactor::value.ToDouble());
                SU::LeaveFewDigits(buffer, 30, powDataA - 1);
                Data::SetDigits(String(buffer));
            }
        }
        else
        {
            if (isDivZero)
            {
                Data::SetDigits(String("=X/0"));
                return;
            }

            int pow = 0;
            ValueSTRICT data((int64)0);

            Calculate(pow, data);

            char format[10];

            if (pow < 10)
            {
                std::strcpy(format, "%10.0f");
                format[4] = (char)(pow | 0x30);
            }
            else
            {
                std::strcpy(format, "%10.10f");
                format[5] = (char)((pow - 10) | 0x30);
            }

            char text[30];

            std::sprintf(text, format, data.ToDouble());

            LOG_WRITE(text);
            
                 // Если косвенное измерение частоты
            if ((Channel::Current()->mod.typeMeasure.IsFrequency() && Channel::Current()->mod.modeFrequency.IsT_1()) ||
                 // или периода
                 Channel::Current()->mod.typeMeasure.IsPeriod() && Channel::Current()->mod.modePeriod.IsF_1())
            {
                SU::LeaveFewDigits(text, 30, powDataA);
            }

            Data::SetDigits(String(text));
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
            result[3] = (char)(FillFactor::zeroes | 0x30);

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
                        if (counterC.ToUnits(Order::Micro) / 2 < 10) { Data::SetUnits(String(" MHz")); }
                        else                                         { Data::SetUnits(String(" GHz")); }
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
                if ((CURRENT_CHANNEL_IS_A &&
                    Channel::A->mod.typeMeasure.IsPeriod() &&
                    Channel::A->mod.modePeriod.IsF_1())
                    ||
                    (CURRENT_CHANNEL_IS_B &&
                    Channel::B->mod.typeMeasure.IsPeriod() &&
                    Channel::B->mod.modePeriod.IsF_1()))
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
    value = (float)(100 * timer) / (float)(cal2 - cal1);
}

#include "defines.h"
#include "Log.h"
#include "Settings.h"
#include "Calculate/MathFPGA.h"
#include "Calculate/ValueDuration.h"
#include "Calculate/ValueFrequency.h"
#include "Calculate/ValuePeriod.h"
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

uint   MathFPGA::Auto::fpgaMin = 0;
uint   MathFPGA::Auto::fpgaMid = 0;
uint   MathFPGA::Auto::fpgaMax = 0;
       
int         MathFPGA::Measure::decDA = 1;
ValueSTRICT MathFPGA::Measure::counterA((int64)0);
ValueSTRICT MathFPGA::Measure::counterB((int64)0);
ValueSTRICT MathFPGA::Measure::counterC((int64)0);
int         MathFPGA::Measure::powDataA = 0;
ValueFPGA   *MathFPGA::Measure::valueFPGA = nullptr;

const char *MathFPGA::Data::UGO_DivNULL = "=X/0";

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
    if (FPGA::IsOverloaded())
    {
        Data::SetDigits(String("œ≈–≈œŒÀÕ≈Õ»≈"));
    }
    else
    {
        std::strcpy(digits, _digits.c_str()); //-V2513
        Display::zoneData->Refresh();
    }
}


void MathFPGA::Data::SetUnits(const String &_units)
{
    std::strcpy(units, _units.c_str()); //-V2513
    Display::zoneData->Refresh();
}


int MathFPGA::Measure::CalculatePeriodEmptyZeros()
{
    int result = 1;

    if (ModesChannel::timeLabels.IsT_7() || ModesChannel::timeLabels.IsT_4())
    {
        result *= 10;
    }
    else if (ModesChannel::timeLabels.IsT_8() || ModesChannel::timeLabels.IsT_5()) //-V2516
    {
        result *= 100;
    }

    result *= ModesChannel::numberPeriods.ToAbs();

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


void MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::E type, uint value1, uint value2, uint value3, uint value4, uint value5)
{ 
    isDivZero = false;

    if (CreateValue(value1, value2, value3, value4, value5))
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

    case TypeData::FillFactorPhase:
    case TypeData::Comparator:
    case TypeData::Interpolator:
        break;
    }

    CalculateNewData();

    CalculateUnits();

    Validator::SetValidData();

    ProgressBarTimeMeasureZone::timeStart = TIME_MS;
}


bool MathFPGA::Measure::CreateValue(uint value1, uint value2, uint value3, uint value4, uint value5)
{
    TypeMeasure &type = Channel::Current()->mod.typeMeasure;

    if (valueFPGA != nullptr)
    {
        delete valueFPGA;
        valueFPGA = nullptr;
    }

    if (type.IsFrequency())
    {
        if (Channel::Current()->mod.modeFrequency.IsRatio())
        {
            valueFPGA = new ValueFrequency_Ratio(value1, value2);
            return true;
        }

        switch (Channel::Current()->mod.modeFrequency)
        {
        case ModeFrequency::Frequency:  valueFPGA = new ValueFrequency_Frequency(value1);                                                       return true;
        case ModeFrequency::T_1:        valueFPGA = new ValueFrequency_T_1(value1); ;                                                           return true;
        case ModeFrequency::Tachometer: valueFPGA = new ValueFrequency_Tachometer(value1);                                                      return true;
        case ModeFrequency::Comparator: valueFPGA = new ValueFrequency_Comparator(value1, (int)value2, (int)value3, (int)value4, (int)value5);  return true;
        }
    }
    else if (type.IsPeriod())
    {
        switch (Channel::Current()->mod.modePeriod)
        {
        case ModePeriod::Period:    valueFPGA = new ValuePeriod_Period(value1); return true;
        case ModePeriod::F_1:       valueFPGA = new ValuePeriod_F_1(value1);    return true;
        }
    }
    else if (type.IsDuration())
    {
        switch (Channel::Current()->mod.modeDuration)
        {
        case ModeDuration::Ndt_1ns:    valueFPGA = new ValueDuration_Ndt_1ns(value1, value2, value3);  return true;
        case ModeDuration::FillFactor:
        case ModeDuration::Phase:      valueFPGA = new ValueDuration_Phase_FillFactor(value1, value2); return true;
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


void MathFPGA::Measure::AppendDataMainCounters(uint valueA, uint)
{
    counterA.FromDouble((double)valueA);

    powDataA = 0;

    do
    {
        powDataA++;
        valueA /= 10;

    } while (valueA > 0);

    if (CURRENT_CHANNEL_IS_C)
    {
        counterA.MulUINT(64);

        if (Channel::Current()->mod.typeMeasure.IsFrequency())
        {
            counterA.DivUINT(100);
        }
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
    case TypeMeasure::Duration:     result = CalculateDurationEmptyZeros();   break;
    case TypeMeasure::Period:       result = CalculatePeriodEmptyZeros();     break;
    }

    return result;
}


void MathFPGA::Measure::CalculateNewData()
{
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
        if (Channel::Current()->mod.typeMeasure.IsDuration() &&
            (ModeDuration::Current().IsFillFactor() || ModeDuration::Current().IsPhase()))
        {
        }
        else
        {
            if (isDivZero)
            {
                Data::SetDigits(String(MathFPGA::Data::UGO_DivNULL));
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
            
            Data::SetDigits(String(text));
        }
    }
}


void MathFPGA::Measure::CalculateUnits()
{
    TypeMeasure &type = Channel::Current()->mod.typeMeasure;

    if(type.IsDuration() && (ModeDuration::Current().IsFillFactor() || ModeDuration::Current().IsPhase()))
    {
    }
    else
    {
        if (ModeFrequency::Current().IsRatio() ||  type.IsCountPulse())
        {
            Data::SetUnits(String(" "));
        }
        else
        {
            if (type.IsFrequency())
            {
                if (CURRENT_CHANNEL_IS_C)
                {
                    if (counterC.ToUnits(Order::Micro) / 2 < 10) { Data::SetUnits(String(" MHz")); }
                    else { Data::SetUnits(String(" GHz")); }
                }
                else if (CURRENT_CHANNEL_IS_D)
                {
                    Data::SetUnits(String(" GHz"));
                }
                else
                {
                    if (decDA < 1000) { Data::SetUnits(String(" kHz")); }
                    else { Data::SetUnits(String(" MHz")); }
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

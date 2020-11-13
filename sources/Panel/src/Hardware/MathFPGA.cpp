#include "defines.h"
#include "Settings.h"
#include "Display/Display.h"
#include "Hardware/FPGA.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Utils/StringUtils.h"
#include "Utils/Value.h"
#include <cstdio>
#include <cstring>


int    MathFPGA::NA = 0; //-V707
int    MathFPGA::NB = 0; //-V707

uint MathFPGA::curFX = 0;
uint MathFPGA::curTIZM = 0;
uint MathFPGA::curNKAL = 0;

Stack<uint> MathFPGA::sFX(50);
Stack<uint> MathFPGA::sTIZM(50);
Stack<uint> MathFPGA::sNKAL(50);
Stack<double> MathFPGA::values(50);


float  MathFPGA::Interpolator::value = 0.0F;
       
uint   MathFPGA::Auto::fpgaMin = 0;
uint   MathFPGA::Auto::fpgaMid = 0;
uint   MathFPGA::Auto::fpgaMax = 0;
       
float  MathFPGA::FillFactor::value = 0.0F;
int    MathFPGA::FillFactor::zeroes = 0;
       
int       MathFPGA::Measure::decDA = 1;
ValuePICO MathFPGA::Measure::valueComparator(0);
bool      MathFPGA::Measure::isEmpty = true;
ValueNANO MathFPGA::Measure::decDataA(0);
ValueNANO MathFPGA::Measure::decDataB(0);
ValueNANO MathFPGA::Measure::decDataC(0);



int MathFPGA::Measure::CalculateFrequency(int &manualZeros)
{
    int result = 0;

    ModeMeasureFrequency &mode = ModeMeasureFrequency::Current();

    if (mode.IsT_1())
    {
        manualZeros = 10 * PageModesA::timeLabels.ToZeros() / 1000 * PageModesA::numberPeriods.ToAbs();

        decDataA.Div((uint)PageModesA::timeLabels.ToZeros());

        double test1 = decDataA.ToDouble();

        double test2 = test1 / PageModesA::numberPeriods.ToAbs();

        decDataA.FromDouble((float)(4 / test2));

        decDA = (int)(decDataA.ToDouble() / 2.0);

        if (decDA < 1000)           { }
        else if (decDA < 1000000)   { decDataA.Div(1000);    }
        else                        { decDataA.Div(1000000); }

        result = 1;
    }
    else if (mode.IsRatioAB() || mode.IsRatioBA())
    {
        result = PageModesA::numberPeriods.ToAbs();
    }
    else if (mode.IsRatioCA() || mode.IsRatioCB())
    {
        decDataA.Mul(100);
        result = PageModesC::numberPeriods.ToAbs();
    }
    else if (mode.IsRatioAC() || mode.IsRatioBC())
    {
        int sT = PageModesA::timeMeasure.ToMS();

        decDataA.FromDouble(decDataA.ToDouble() / decDataB.ToDouble() / 32);
        manualZeros = 1000000 * sT;
        result = 1;
    }
    else
    {
        int mhz = 1000 * PageModesA::timeMeasure.ToMS();
        int khz = PageModesA::timeMeasure.ToMS();

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


int MathFPGA::Measure::CalculatePeriod()
{
    int result = 1;

    if (ModeMeasurePeriod::Current().IsF_1())
    {
        int sT = PageModesA::timeMeasure.ToMS();

        decDA = (int)(decDataA.ToDouble() / (2.0 * (double)sT));
        decDataA.FromDouble(4 / decDataA.ToDouble());

        decDataA.Mul((uint)sT);
        decDataA.Mul((uint)sT);

        if (decDA >= 1000)      { decDataA.Mul(10000000); }
        else if (decDA <= 1)    { decDataA.Mul(10);       }
        else                    { decDataA.Mul(10000);    }

        result = sT * 10;
    }
    else
    {
        if (PageModesA::timeLabels.IsT_7() || PageModesA::timeLabels.IsT_4())
        {
            result *= 10;
        }
        else if (PageModesA::timeLabels.IsT_8() || PageModesA::timeLabels.IsT_5())
        {
            result *= 100;
        }

        result *= PageModesA::numberPeriods.ToAbs();
    }

    return result;
}


int MathFPGA::Measure::CalculateDuration()
{
    if (PageModesA::timeLabels.IsT_7())
    {
        return 10;
    }
    else if (PageModesA::timeLabels.IsT_8() || PageModesA::timeLabels.IsT_5())
    {
        return 100;
    }

    return 1;
}


void MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::E type, uint value1, uint value2, uint value3)
{
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
        CalculateComparator(value1, value2, value3);
        break;
    }
}


void MathFPGA::Measure::CalculateComparator(uint fx, uint tizm, uint nkal)
{
    curFX = fx;
    curTIZM = tizm;
    curNKAL = nkal;

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

        MathFPGA::Measure::valueComparator = k;

        if (MathFPGA::Comparator::values.AppendValue(k.ToDouble()))
        {
            Display::Refresh();
        }
        else
        {
            if (MathFPGA::sFX.IsFull())
            {
                MathFPGA::sFX.Clear();
                MathFPGA::sTIZM.Clear();
                MathFPGA::sNKAL.Clear();
            }

            MathFPGA::sFX.Push(fx);
            MathFPGA::sTIZM.Push(tizm);
            MathFPGA::sNKAL.Push(nkal);
            MathFPGA::values.Push(k.ToDouble());
        }
    }

    MathFPGA::Measure::SetFlagValidData();
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

    SetFlagValidData();
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
    MathFPGA::Measure::SetFlagValidData();

    value = (float)duration / (float)period;

    if (ModeMeasureDuration::Current().Is_Phase())
    {
        value *= 360;
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


void MathFPGA::Measure::Calculate(int &emptyZeros, ValueNANO &data)
{
    int manualZeros = 1;

    if (TypeMeasure::Current().IsFrequency())
    {
        emptyZeros = CalculateFrequency(manualZeros);
    }
    else if (TypeMeasure::Current().IsDuration())
    {
        emptyZeros = CalculateDuration();
    }
    else if (TypeMeasure::Current().IsPeriod())
    {
        emptyZeros = CalculatePeriod();
    }

    data = decDataA;

    if (CURRENT_CHANNEL_IS_D)
    {
        data.FromDouble(decDataC.ToDouble() * 2.0);
    }

    data.Div((uint)(2 * emptyZeros));

    if (manualZeros != 1) //-V1051
    {
        emptyZeros = manualZeros;
    }
}


String MathFPGA::Measure::GiveData()
{
    if (isEmpty)
    {
        return String(".......................................................");
    }

    if (FPGA::IsOverloaded() && !ModeMeasureFrequency::Current().IsTachometer())
    {
        return String("ÏÅÐÅÏÎËÍÅÍÈÅ");
    }

    if (TypeMeasure::Current().IsCountPulse())
    {
        float value = decDataA.ToFloat() / 2.0F;

        if (CURRENT_CHANNEL_IS_C)
        {
            value *= 100.0F;
        }

        if (ModeMeasureCountPulse::Current().IsFromPeriod())
        {
            value /= (float)PageModesA::numberPeriods.ToAbs();
        }

        return String("%10.0f", decDataA.ToFloat());
    }
    else
    {
        if (ModeMeasureFrequency::Current().IsTachometer())
        {
            return String("%10.0f", decDataA.ToFloat() / 2.0F);
        }
        else if (ModeMeasureFrequency::Current().IsComparator())
        {
            return valueComparator.ToString();
        }
        else if (ModeMeasureDuration::Current().Is_Ndt_1ns())
        {
            return String("%10.2f", MathFPGA::Interpolator::GetValue());
        }
        else if (TypeMeasure::Current().IsDuration() && (ModeMeasureDuration::Current().Is_FillFactor() || ModeMeasureDuration::Current().Is_Phase()))
        {
            if (ModeMeasureDuration::Current().Is_Phase())
            {
                return String("%10.3f", MathFPGA::FillFactor::GetValue());
            }
            
            return String("%10.7f", MathFPGA::FillFactor::GetValue());
        }
        else
        {
            int emptyZeros = 0;
            ValueNANO data(0);

            Calculate(emptyZeros, data);

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

                return String(format, data.ToFloat());
            }
            else
            {
                char format[10];
                std::strcpy(format, "%10.10f");
                format[5] = (char)((pow - 10) | 0x30);

                return String(format, data.ToFloat());
            }
        }
    }
}


String MathFPGA::Measure::GiveUnits() //-V2008
{
    if (ModeMeasureDuration::Current().Is_Ndt_1ns())
    {
        return String(" ns");
    }
    else if(TypeMeasure::Current().IsDuration() && (ModeMeasureDuration::Current().Is_FillFactor() || ModeMeasureDuration::Current().Is_Phase()))
    {
        if (ModeMeasureDuration::Current().Is_Phase())
        {
            return String(" $");
        }
        else
        {
            String result(" E-0");
            result[3] = (char)(FillFactor::zeroes | 0x30);

            FillFactor::zeroes = 0;

            return result;
        }
    }
    else
    {
        if (ModeMeasureFrequency::Current().IsRatio() ||
            ModeMeasureFrequency::Current().IsTachometer() ||
            TypeMeasure::Current().IsCountPulse())
        {
            return String(" ");
        }
        else
        {
            if (TypeMeasure::Current().IsFrequency())
            {
                if (ModeMeasureFrequency::Current().IsT_1())
                {
                    if (decDA < 1000)           { return String(" Hz");  }
                    else if (decDA < 1000000)   { return String(" kHz"); }
                    else                        { return String(" MHz"); }
                }
                else if (PageModesA::modeMeasureFrequency.IsComparator() && CURRENT_CHANNEL_IS_A)
                {
                    return String(" E-6");
                }
                else
                {
                    if (CURRENT_CHANNEL_IS_C)
                    {
                        if (decDataC.ToUINT64() / 2 < 10000)    { return String(" MHz"); }
                        else                                    { return String(" GHz"); }
                    }
                    else if (CURRENT_CHANNEL_IS_D)   
                    {
                        return String(" GHz");
                    }
                    else
                    {
                        if (decDA < 1000)           { return String(" kHz"); }
                        else                        { return String(" MHz"); }
                    }
                }
            }
            else
            {
                if ((CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsPeriod() && PageModesA::modeMeasurePeriod.IsF_1()) ||
                    (CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsPeriod() && PageModesB::modeMeasurePeriod.IsF_1()))
                {
                    if (decDA >= 1000)      { return String(" ns"); }
                    else if (decDA <= 1)    { return String(" ms"); }
                    else                    { return String(" us"); }
                }
                else
                {
                    PeriodTimeLabels &current = PeriodTimeLabels::Current();

                    if (current.IsT_3() || current.IsT_4() || current.IsT_5())  { return String(" ms"); }
                    else                                                        { return String(" us"); }
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
    MathFPGA::Measure::SetFlagValidData();

    value = (float)(100 * timer) / (float)(cal2 - cal1);
}

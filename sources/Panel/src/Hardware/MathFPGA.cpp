#include "defines.h"
#include "Settings.h"
#include "Hardware/FPGA.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Utils/StringUtils.h"
#include "Utils/Value.h"
#include <cstdio>
#include <cstring>


static int decMinAuto = 0;
static int decMidAuto = 0;
static int decMaxAuto = 0;
char MathFPGA::Auto::dataMin[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
char MathFPGA::Auto::dataMid[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
char MathFPGA::Auto::dataMax[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

char MathFPGA::Measure::dataFrequencyA[32] = { 0 };
char MathFPGA::Measure::dataFrequencyB[32] = { 0 };
char MathFPGA::Measure::dataPeriod[32] = { 0 };
char MathFPGA::Measure::dataDuration[32] = { 0 };

ValuePICO MathFPGA::Measure::valueComparator(0);

bool MathFPGA::DutyCycle::enabled = false;

static ValueNANO decDataA(0);
static ValueNANO decDataB(0);
static ValueNANO decDataC(0);
static int decDA = 1;
static int emptyZeros = 0;
int MathFPGA::NA = 0; //-V707
int MathFPGA::NB = 0; //-V707

static float dutyCycle = 0.0F;
static int dcycleZeros = 0;

static float interpol = 0.0F;

static char minAutoData[7] = { 0 };
static char maxAutoData[7] = { 0 };

static int decPeriod = 0;
static int decDuration = 0;
static int decTimer1 = 0;
static int decCAL1 = 0;
static int decCAL2 = 0;

static char procDataInterpol[30] = { 0 };
static char procDataDcycle[30] = { 0 };


void MathFPGA::Measure::Calculate()
{
    int x = 0;
    int manualZeros = 1;

    if (TypeMeasure::Current().IsFrequency())
    {
        x = CalculateFrequency(manualZeros);
    }
    else if (TypeMeasure::Current().IsDuration())
    {
        x = CalculateDuration();
    }
    else if (TypeMeasure::Current().IsPeriod())
    {
        x = CalculatePeriod();
    }

    if (CURRENT_CHANNEL_IS_D)
    {
        decDataA.FromDouble(decDataC.ToDouble() * 2.0);
    }

    decDataA.Div((uint)(2 * x));

    emptyZeros = x;

    if (manualZeros != 1) //-V1051
    {
        emptyZeros = manualZeros;
    }
}


int MathFPGA::Measure::CalculateFrequency(int &manualZeros)
{
    int result = 0;

    ModeMeasureFrequency &mode = ModeMeasureFrequency::Current();

    if (mode.IsT_1())
    {
        manualZeros = 10 * PageModesA::periodTimeLabels.ToZeros() / 1000 * PageModesA::numberPeriods.ToAbs();

        decDataA.Div((uint)PageModesA::periodTimeLabels.ToZeros());

        double test1 = decDataA.ToDouble();

        double test2 = test1 / PageModesA::numberPeriods.ToAbs();

        decDataA.FromDouble((float)(4 / test2));

        decDA = (int)(decDataA.ToDouble() / 2.0F);

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
        decDA = (int)((decDataA.ToDouble() / (float)khz) / 2.0F);

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

        decDA = (int)(decDataA.ToDouble() / (2.0F * (float)sT));
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
        if (PageModesA::periodTimeLabels.IsT_7() || PageModesA::periodTimeLabels.IsT_4())
        {
            result *= 10;
        }
        else if (PageModesA::periodTimeLabels.IsT_8() || PageModesA::periodTimeLabels.IsT_5())
        {
            result *= 100;
        }

        result *= PageModesA::numberPeriods.ToAbs();
    }

    return result;
}


int MathFPGA::Measure::CalculateDuration()
{
    if (PageModesA::periodTimeLabels.IsT_7())
    {
        return 10;
    }
    else if (PageModesA::periodTimeLabels.IsT_8() || PageModesA::periodTimeLabels.IsT_5())
    {
        return 100;
    }

    return 1;
}


void MathFPGA::Measure::BinToDec()
{
    decDataA.FromDouble(MathFPGA::BinToDec(dataFrequencyA));

    if (ModeMeasureFrequency::Current().IsRatioAC() || ModeMeasureFrequency::Current().IsRatioBC())
    {
        decDataB.FromDouble(MathFPGA::BinToDec(dataFrequencyB));
    }

    if (CURRENT_CHANNEL_IS_C)
    {
        decDataA.Mul(64);
        decDataA.Div(100);
    }
}


float MathFPGA::BinToDec(const char bin[32])
{
    int base = 1;

    float result = 0.0F;

    for (int i = 31; i >= 0; i--)
    {
        if (bin[i] == 1)
        {
            result += (float)base;
        }

        base *= 2;
    }

    return result;
}


uint MathFPGA::BinToUint32(const char bin[32])
{
    int base = 1;

    uint result = 0;

    for (int i = 31; i >= 0; i--)
    {
        if (bin[i] == 1)
        {
            result += base;
        }

        base *= 2;
    }

    return result;
}


uint16 MathFPGA::BinToUint16(const char bin[16])
{
    int base = 1;

    uint16 result = 0;

    for (int i = 15; i >= 0; i--)
    {
        if (bin[i] == 1)
        {
            result += (uint16)base;
        }

        base *= 2;
    }

    return (uint16)result;
}


void MathFPGA::Auto::Calculate()
{
    decMinAuto = 0;
    decMidAuto = 0;
    decMaxAuto = 0;
    int base1 = 1;
    int base2 = 1;
    int base3 = 1;
    int len = 10;

    for (int i = len - 1; i >= 0; i--)
    {
        if (dataMin[i] == 1)
        {
            decMinAuto += base1;
        }
        base1 = base1 * 2;
    }

    for (int i = len - 1; i >= 0; i--)
    {
        if (dataMid[i] == 1)
        {
            decMidAuto += base2;
        }
        base2 = base2 * 2;
    }

    for (int i = len - 1; i >= 0; i--)
    {
        if (dataMax[i] == 1)
        {
            decMaxAuto += base3;
        }
        base3 = base3 * 2;
    }
}


int MathFPGA::Auto::Mid()
{
    Calculate();
    return decMidAuto;
}


int MathFPGA::Auto::Min()
{
    Calculate();
    return decMinAuto;
}


int MathFPGA::Auto::Max()
{
    Calculate();
    return decMaxAuto;
}


char *MathFPGA::Auto::Give()
{
    static char result[20] = { 0 };

    Auto::Calculate();
    SU::Int2String((decMinAuto - 512) * 2, minAutoData);
    SU::Int2String((decMaxAuto - 512) * 2, maxAutoData);
    std::strcpy(result, "Макс ");
    std::strcat(result, maxAutoData);
    std::strcat(result, " Мин ");
    std::strcat(result, minAutoData);

    if (CURRENT_CHANNEL_IS_A)
    {
        LEVEL_SYNCH_A = (decMidAuto - 512) * 2;
        NA = decMidAuto - 512;
    }

    if (CURRENT_CHANNEL_IS_B)
    {
        LEVEL_SYNCH_B = (decMidAuto - 512) * 2;
        NB = decMidAuto - 512;

    }

    return result;
}


void MathFPGA::Measure::CalculateDcycle()
{
    decPeriod = 0;
    decDuration = 0;
    int base1 = 1;
    int base2 = 1;
    int len = 32;
    for (int i = len - 1; i >= 0; i--)
    {
        if (dataPeriod[i] == 1)
        {
            decPeriod += base1;
        }
        base1 = base1 * 2;
    }
    for (int i = len - 1; i >= 0; i--)
    {
        if (dataDuration[i] == 1)
        {
            decDuration += base2;
        }
        base2 = base2 * 2;
    }
    dutyCycle = (float)decDuration / (float)decPeriod;

    if (ModeMeasureDuration::Current().Is_Phase())
    {
        dutyCycle = dutyCycle * 360;
    }
    else
    {
        if (dutyCycle < 0)
        {
            dutyCycle = dutyCycle * (-1);
        }

        if (dutyCycle != 0.0F) //-V2550 //-V550
        {
            while (dutyCycle < 1)
            {
                dutyCycle = dutyCycle * 10;
                dcycleZeros++;
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


void MathFPGA::Measure::CalculateInterpolate()
{
    decTimer1 = 0;
    decCAL1 = 0;
    decCAL2 = 0;
    int base1 = 1;
    int base2 = 1;
    int base3 = 1;
    int len = 24;

    for (int i = len - 1; i >= 0; i--)
    {
        if (FPGA::dataTimer[i] == 1)
        {
            decTimer1 += base1;
        }
        base1 = base1 * 2;
    }

    for (int i = len - 1; i >= 0; i--)
    {
        if (FPGA::dataCAL1[i] == 1)
        {
            decCAL1 += base2;
        }
        base2 = base2 * 2;
    }

    for (int i = len - 1; i >= 0; i--)
    {
        if (FPGA::dataCAL2[i] == 1)
        {
            decCAL2 += base3;
        }
        base3 = base3 * 2;
    }

    interpol = (float)(100 * decTimer1) / (float)(decCAL2 - decCAL1);
}


char *MathFPGA::Measure::GiveData()
{
    static char result[40] = { 0 };

    result[0] = 0;

    if (TypeMeasure::Current().IsCountPulse())
    {
        BinToDec();
        decDataA.Div(2);

        if (CURRENT_CHANNEL_IS_C)
        {
            decDataA.Mul(100);
        }

        if (ModeMeasureCountPulse::Current().IsFromPeriod())
        {
            decDataA.Div((uint)PageModesA::numberPeriods.ToAbs());
        }

        std::sprintf(result, "%10.0f", decDataA.ToFloat());

        return result;
    }
    else
    {
        if (ModeMeasureFrequency::Current().IsTachometer())
        {
            BinToDec();
            decDataA.Div(2);
            std::sprintf(result, "%10.0f", decDataA.ToFloat());

            return result;
        }
        else if (ModeMeasureFrequency::Current().IsComparator())
        {
            std::sprintf(result, "%s", valueComparator.ToString());

            return result;
        }
        else if (ModeMeasureDuration::Current().Is_Ndt_1ns() && PageModesA::InterpolateCheck())
        {
            CalculateInterpolate();
            std::sprintf(procDataInterpol, "%10.2f", interpol);
            return procDataInterpol;
        }
        else if ((ModeMeasureDuration::Current().Is_DutyCycle() || ModeMeasureDuration::Current().Is_Phase()) && DutyCycle::IsEnabled())
        {
            CalculateDcycle();

            if (ModeMeasureDuration::Current().Is_Phase())  { std::sprintf(procDataDcycle, "%10.3f", dutyCycle); }
            else                                            { std::sprintf(procDataDcycle, "%10.7f", dutyCycle); }

            return procDataDcycle;
        }
        else
        {
            BinToDec();
            Calculate();

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
                std::sprintf(result, format, decDataA.ToFloat());
            }
            else
            {
                char format[10];
                std::strcpy(format, "%10.10f");
                format[5] = (char)((pow - 10) | 0x30);
                std::sprintf(result, format, decDataA.ToFloat());
            }

            emptyZeros = 1;
        }

        return result;
    }
}


char *MathFPGA::GiveSpec() //-V2008
{
    static char result[10] = { 0 };

    if (PageModesA::InterpolateCheck() && ModeMeasureDuration::Current().Is_Ndt_1ns())
    {
        std::strcpy(result, " ns");
    }
    else if ((PageModesA::typeMeasure.IsDuration() && PageModesA::modeMeasureDuration.Is_DutyCycle() && CURRENT_CHANNEL_IS_A) ||
        (PageModesB::typeMeasure.IsDuration() && PageModesB::modeMeasureDuration.Is_DutyCycle() && CURRENT_CHANNEL_IS_B) ||
        (PageModesA::typeMeasure.IsDuration() && PageModesA::modeMeasureDuration.Is_Phase() && CURRENT_CHANNEL_IS_A) ||
        (PageModesB::typeMeasure.IsDuration() && PageModesB::modeMeasureDuration.Is_Phase() && CURRENT_CHANNEL_IS_B))
    {
        if (ModeMeasureDuration::Current().Is_Phase())
        {
            std::strcpy(result, " $");
        }
        else
        {
            std::strcpy(result, " E-0");
            result[3] = (char)(dcycleZeros | 0x30);

            dcycleZeros = 0;
        }
    }
    else
    {
        if (ModeMeasureFrequency::Current().IsRatio() ||
            ModeMeasureFrequency::Current().IsTachometer() ||
            TypeMeasure::Current().IsCountPulse())
        {
            std::strcpy(result, " ");
        }
        else
        {
            if (TypeMeasure::Current().IsFrequency())
            {
                if (ModeMeasureFrequency::Current().IsT_1())
                {
                    if (decDA < 1000)           { std::strcpy(result, " Hz");  }
                    else if (decDA < 1000000)   { std::strcpy(result, " kHz"); }
                    else                        { std::strcpy(result, " MHz"); }
                }
                else if (PageModesA::modeMeasureFrequency.IsComparator() && CURRENT_CHANNEL_IS_A)
                {
                    std::strcpy(result, " E-6");
                }
                else
                {
                    if (CURRENT_CHANNEL_IS_C)
                    {
                        if (decDataC.ToUINT64() / 2 < 10000)    { std::strcpy(result, " MHz"); }
                        else                                    { std::strcpy(result, " GHz"); }
                    }
                    else if (CURRENT_CHANNEL_IS_D)   
                    {
                        std::strcpy(result, " GHz");
                    }
                    else
                    {
                        if (decDA < 1000)           { std::strcpy(result, " kHz"); }
                        else                        { std::strcpy(result, " MHz"); }
                    }
                }
            }
            else
            {
                if ((CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsPeriod() && PageModesA::modeMeasurePeriod.IsF_1()) ||
                    (CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsPeriod() && PageModesB::modeMeasurePeriod.IsF_1()))
                {
                    if (decDA >= 1000)      { std::strcpy(result, " ns"); }
                    else if (decDA <= 1)    { std::strcpy(result, " ms"); }
                    else                    { std::strcpy(result, " us"); }
                }
                else
                {
                    PeriodTimeLabels &current = PeriodTimeLabels::Current();

                    if (current.IsT_3() || current.IsT_4() || current.IsT_5())  { std::strcpy(result, " ms"); }
                    else                                                        { std::strcpy(result, " us"); }
                }
            }
        }
    }
    return result;
}


char *MathFPGA::GiveIdent()
{
    static char identInfo[10] = { 0 };

    if (FPGA::dataIdent[0] == 0)        { std::strcpy(identInfo, "0"); }
    else                                { std::strcpy(identInfo, "1"); }

    for (int i = 1; i < 4; i++)
    {
        if (FPGA::dataIdent[i] == 0)    { std::strcat(identInfo, "0"); }
        else                            { std::strcat(identInfo, "1"); }
    }

    return identInfo;
}


void MathFPGA::Auto::Refresh()
{
    for (int i = 0; i < 10; i++)
    {
        dataMin[i] = 0;
        dataMid[i] = 0;
        dataMax[i] = 0;
    }
}


void MathFPGA::DutyCycle::Enable()
{
    enabled = true;
}


void MathFPGA::DutyCycle::Disable()
{
    enabled = false;
}


bool MathFPGA::DutyCycle::IsEnabled()
{
    return enabled;
}

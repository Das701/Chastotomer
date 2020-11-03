#include "defines.h"
#include "Settings.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Pages/PageModesA.h"
#include "Menu/Pages/PageModesC.h"
#include "Utils/StringUtils.h"
#include <cstring>


char MathFPGA::period[32] = { 0 };
char MathFPGA::duration[32] = { 0 };

char MathFPGA::dataA[32] = { 0 };
char MathFPGA::dataB[32] = { 0 };
float MathFPGA::decDataA = 0.0F;
float MathFPGA::decDataB = 0.0F;
float MathFPGA::decDataC = 0.0F;
int MathFPGA::decDA = 1;
int MathFPGA::emptyZeros = 0;
int MathFPGA::NA = 0; //-V707
int MathFPGA::NB = 0; //-V707


int MathFPGA::decMinAuto = 0;
int MathFPGA::decMidAuto = 0;
int MathFPGA::decMaxAuto = 0;

char MathFPGA::minAuto[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
char MathFPGA::midAuto[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
char MathFPGA::maxAuto[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static char minAutoData[7] = { 0 };
static char maxAutoData[7] = { 0 };
static char autoData[20] = { 0 };

static int decPeriod = 0;
static int decDuration = 0;
float MathFPGA::dutyCycle = 0.0F;
int MathFPGA::dcycleZeros = 0;

void MathFPGA::Calculate()
{
    int x = 0;
    int manualZeros = 1;

    if (CurrentTypeMeasure::IsFrequency())
    {
        x = CalculateFrequency(manualZeros);
    }
    else if (CurrentTypeMeasure::IsDuration())
    {
        x = CalculateDuration();
    }
    else if (CurrentTypeMeasure::IsPeriod())
    {
        x = CalculatePeriod();
    }

    if (CURRENT_CHANNEL_IS_D)
    {
        decDataA = decDataC * 2;
    }

    decDataA = (float)decDataA / (2.0F * (float)x);
    emptyZeros = x;

    if (manualZeros != 1) //-V1051
    {
        emptyZeros = manualZeros;
    }
}


int MathFPGA::CalculateFrequency(int &manualZeros)
{
    int result = 0;

    if (CurrentModeMeasureFrequency::IsT_1())
    {
        manualZeros = 10 * PageModesA::periodTimeLabels.ToZeros() / 1000 * PageModesA::numberPeriods.ToAbs();

        double test1 = (double)decDataA / PageModesA::periodTimeLabels.ToZeros();

        double test2 = test1 / PageModesA::numberPeriods.ToAbs();

        decDataA = (float)(4 / test2);

        decDA = (int)(decDataA / 2.0F);

        if (decDA < 1000)
        {
        }
        else if (decDA < 1000000)
        {
            decDataA = decDataA / 1000;
        }
        else
        {
            decDataA = decDataA / 1000000;
        }

        result = 1;
    }
    else if (CurrentModeMeasureFrequency::Is_AB_or_BA())
    {
        result = PageModesA::numberPeriods.ToAbs();
    }
    else if (CurrentModeMeasureFrequency::Is_CA_or_CB())
    {
        decDataA = decDataA * 100;
        result = PageModesC::numberPeriods.ToAbs();
    }
    else if (CurrentModeMeasureFrequency::Is_AC_or_BC())
    {
        int sT = PageModesA::timeMeasure.ToMS();

        decDataA = decDataA / decDataB / 32;
        manualZeros = 1000000 * sT;
        result = 1;
    }
    else
    {
        int mhz = 1000 * PageModesA::timeMeasure.ToMS();
        int khz = PageModesA::timeMeasure.ToMS();

        if (((decDataA / (float)khz) / 2.0F) < 1000.0F)
        {
            result = khz;
        }
        else
        {
            result = mhz;
        }
        decDA = (int)((decDataA / (float)khz) / 2.0F);

        if (CURRENT_CHANNEL_IS_C)
        {
            if (decDataA < 10000)
            {
                decDataC = decDataA;
                khz = khz * 10;
                result = khz;
            }
            else
            {
                decDataC = decDataA;
                mhz = mhz * 10;
                result = mhz;
            }
        }

        if (CURRENT_CHANNEL_IS_D)
        {
            if (decDataA * 64.0F / (1000.0F * (float)khz) > 19000.0F)
            {
                decDataC = 0;
                result = khz;
            }
            else
            {
                decDataC = (float)decDataA * 64 / 1000;
                result = mhz;
            }
            decDA = (int)decDataC;
        }
    }

    return result;
}


int MathFPGA::CalculatePeriod()
{
    int result = 1;

    if (ModeMeasurePeriod::Current().IsF_1())
    {
        int sT = PageModesA::timeMeasure.ToMS();

        decDA = (int)(decDataA / (2.0F * (float)sT));
        decDataA = 4 / decDataA;

        if (decDA >= 1000) //-V1051
        {
            decDataA = decDataA * 10000000 * (float)sT * (float)sT;
        }
        else if (decDA <= 1)
        {
            decDataA = decDataA * 10 * (float)sT * (float)sT;
        }
        else
        {
            decDataA = decDataA * 10000 * (float)sT * (float)sT;
        }

        result = sT * 10;
    }
    else
    {
        int usT = 1;

        if (PageModesA::periodTimeLabels.IsT_7() || PageModesA::periodTimeLabels.IsT_4())
        {
            usT *= 10;
        }
        else if (PageModesA::periodTimeLabels.IsT_8() || PageModesA::periodTimeLabels.IsT_5())
        {
            usT *= 100;
        }

        result = usT * PageModesA::numberPeriods.ToAbs();
    }

    return result;
}


int MathFPGA::CalculateDuration()
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


void MathFPGA::BinToDec()
{
    int len = 32;
    decDataA = 0;
    int base = 1;

    for (int i = len - 1; i >= 0; i--)
    {
        if (dataA[i] == 1)
        {
            decDataA += (float)base;
        }
        base = base * 2;
    }

    if (CurrentModeMeasureFrequency::Is_AC_or_BC())
    {
        decDataB = 0;
        int baseB = 1;

        for (int i = len - 1; i >= 0; i--)
        {
            if (dataB[i] == 1)
            {
                decDataB += (float)baseB;
            }
            baseB = baseB * 2;
        }
    }

    if (CURRENT_CHANNEL_IS_C)
    {
        decDataA = decDataA * 64 / 100;
    }
}


void MathFPGA::CalculateAuto()
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
        if (minAuto[i] == 1)
        {
            decMinAuto += base1;
        }
        base1 = base1 * 2;
    }

    for (int i = len - 1; i >= 0; i--)
    {
        if (midAuto[i] == 1)
        {
            decMidAuto += base2;
        }
        base2 = base2 * 2;
    }

    for (int i = len - 1; i >= 0; i--)
    {
        if (maxAuto[i] == 1)
        {
            decMaxAuto += base3;
        }
        base3 = base3 * 2;
    }
}


int MathFPGA::MidAuto()
{
    CalculateAuto();
    return decMidAuto;
}


int MathFPGA::MinAuto()
{
    CalculateAuto();
    return decMinAuto;
}


int MathFPGA::MaxAuto()
{
    CalculateAuto();
    return decMaxAuto;
}


void MathFPGA::RefreshAuto()
{
    for (int i = 0; i < 10; i++)
    {
        minAuto[i] = 0;
        midAuto[i] = 0;
        maxAuto[i] = 0;
    }
}


char *MathFPGA::GiveAuto()
{
    CalculateAuto();
    SU::Int2String((MathFPGA::decMinAuto - 512) * 2, minAutoData);
    SU::Int2String((MathFPGA::decMaxAuto - 512) * 2, maxAutoData);
    std::strcpy(autoData, "Макс ");
    std::strcat(autoData, maxAutoData);
    std::strcat(autoData, " Мин ");
    std::strcat(autoData, minAutoData);

    if (CURRENT_CHANNEL_IS_A)
    {
        LEVEL_SYNCH_A = (MathFPGA::decMidAuto - 512) * 2;
        NA = MathFPGA::decMidAuto - 512;
    }

    if (CURRENT_CHANNEL_IS_B)
    {
        LEVEL_SYNCH_B = (MathFPGA::decMidAuto - 512) * 2;
        NB = MathFPGA::decMidAuto - 512;

    }

    return autoData;
}


void MathFPGA::CalculateDcycle()
{
    decPeriod = 0;
    decDuration = 0;
    int base1 = 1;
    int base2 = 1;
    int len = 32;
    for (int i = len - 1; i >= 0; i--)
    {
        if (period[i] == 1)
        {
            decPeriod += base1;
        }
        base1 = base1 * 2;
    }
    for (int i = len - 1; i >= 0; i--)
    {
        if (duration[i] == 1)
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

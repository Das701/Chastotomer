#include "defines.h"
#include "Settings.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Pages/PageModesA.h"
#include "Menu/Pages/PageModesC.h"


float MathFPGA::decDataA = 0.0F;
float MathFPGA::decDataB = 0.0F;
float MathFPGA::decDataC = 0.0F;
int MathFPGA::decDA = 1;
int MathFPGA::emptyZeros = 0;


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

            x = sT * 10;
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

            x = usT * PageModesA::numberPeriods.ToAbs();
        }
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

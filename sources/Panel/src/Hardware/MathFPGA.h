#pragma once
#include "Utils/Value.h"


struct MathFPGA
{
    static char *GiveSpec();
    static char *GiveIdent();

    static void DecToBin(int dec, char *bin);

    static int NA; //-V707
    static int NB; //-V707

    struct Interpolation
    {
        static void Enable();
        static void Disable();
        static bool IsEnabled();
    private:
        static bool enabled;
    };

    struct DutyCycle
    {
    friend struct FPGA;

        static void Enable();
        static void Disable();
        static bool IsEnabled();
        static void Calculate();
    private:
        static bool enabled;
        static char dataPeriod[32];
        static char dataDuration[32];
    };

    struct Measure
    {
    friend struct FPGA;

        static char *GiveData();

        static void Calculate();

        static void BinToDec();

    private:

        static char dataFrequencyA[32];
        static char dataFrequencyB[32];

        static ValuePICO valueComparator;               // Рассчитанное значение компаратора

        static int CalculateFrequency(int &manualZeros);
        static int CalculatePeriod();
        static int CalculateDuration();

        static void CalculateInterpolate();
    };

    struct Auto
    {
    friend struct FPGA;

        static void Calculate();
        static int Mid();
        static int Min();
        static int Max();

        static char *Give();

        static void Refresh();

    private:
        static char dataMin[10];
        static char dataMid[10];
        static char dataMax[10];
    };

private:

    static float BinToDec(const char bin[32]);

public:

    static uint BinToUint32(const char bin[32]);
    static uint16 BinToUint16(const char bin[16]);
};

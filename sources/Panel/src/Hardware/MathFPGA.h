#pragma once
#include "Utils/Value.h"


struct MathFPGA
{
    static void DecToBin(int dec, char *bin);

    static int NA; //-V707
    static int NB; //-V707

    struct Interpolation
    {
        static void Enable();
        static void Disable();
        static bool IsEnabled();
        static void Calculate();
        static float value;
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
        static float value;
        static int zeroes;

    private:
        static bool enabled;
        static uint fpgaDuration;               // Считанное из FPGA значение
        static uint fpgaPeriod;                 // Считанное из FPGA значение
    };

    struct Measure
    {
    friend struct FPGA;
    friend struct MathFPGA;

        static char *GiveData();
        static char *GiveSpec();
        static char *GiveIdent();

        static void Calculate();
        static void BinToDec();

    private:

        static int decDA;
        static int emptyZeros;

        static ValuePICO valueComparator;       // Рассчитанное значение компаратора

        static uint fpgaFrequencyA;             // Считанное из FPGA значение
        static uint fpgaFrequencyB;             // Считанное из FPGA значение
        static ValueNANO decDataA;
        static ValueNANO decDataB;
        static ValueNANO decDataC;

        static int CalculateFrequency(int &manualZeros);
        static int CalculatePeriod();
        static int CalculateDuration();
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
        static uint fpgaMax;

        static int decMin;
        static int decMid;
    };

public:

    static uint BinToUint32(const char bin[32]);
    static uint16 BinToUint16(const char bin[16]);
};

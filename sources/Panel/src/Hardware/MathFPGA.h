#pragma once
#include "Utils/String.h"
#include "Utils/Value.h"


struct MathFPGA
{
    static void DecToBin(int dec, char *bin);

    static int NA; //-V707
    static int NB; //-V707

    struct Interpolation
    {
        static float Calculate();
    };

    struct DutyCycle
    {
    friend struct FPGA;

        static void Calculate();
        static float value;
        static int zeroes;

    private:
        static uint fpgaDuration;               // Считанное из FPGA значение
        static uint fpgaPeriod;                 // Считанное из FPGA значение
    };

    struct Measure
    {
    friend struct FPGA;
    friend struct MathFPGA;

        static String GiveData();
        static String GiveSpec();
        static String GiveIdent();

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

        static int Mid();
        static int Min();
        static int Max();

        static char *Give();

        static void Refresh();

    private:
        static uint fpgaMin;
        static uint fpgaMid;
        static uint fpgaMax;
    };
};

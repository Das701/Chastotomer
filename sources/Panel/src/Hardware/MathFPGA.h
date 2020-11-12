#pragma once
#include "Utils/String.h"
#include "Utils/Value.h"


struct MathFPGA
{
    static void DecToBin(int dec, char *bin);

    static int NA; //-V707
    static int NB; //-V707

    struct Interpolator
    {
        static void Calculate(uint timer, uint cal1, uint cal2);
        static float GetValue() { return value; };
    private:
        static float value;
    };

    struct Measure
    {
    friend struct FPGA;
    friend struct MathFPGA;

        static String GiveData();
        static String GiveSpec();

        static void AppendDataFrequency(uint frequencyA, uint frequencyB);

    private:

        static int decDA;
        static int emptyZeros;
        static int pow;

        static ValuePICO valueComparator;       // Рассчитанное значение компаратора

        static ValueNANO decDataA;
        static ValueNANO decDataB;
        static ValueNANO decDataC;

        static int CalculateFrequency(int &manualZeros);
        static int CalculatePeriod();
        static int CalculateDuration();

        static void Calculate();
    };

    struct FillFactor
    {
    friend struct MathFPGA::Measure;
        static void Calculate(uint period, uint duration);
        static float GetValue() { return value; }
    private:
        static float value;
        static int zeroes;
    };

    struct Auto
    {
    friend struct FPGA;

        static int Mid();
        static int Min();
        static int Max();

        static String Give();

        static void Refresh();

    private:
        static uint fpgaMin;
        static uint fpgaMid;
        static uint fpgaMax;
    };
};

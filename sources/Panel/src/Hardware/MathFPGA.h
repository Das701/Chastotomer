#pragma once


struct MathFPGA
{
    static void Calculate();
    static void BinToDec();

    static float decDataA;
    static float decDataB;
    static float decDataC;
    static int decDA;
    static int emptyZeros;
    static char dataA[32];
    static char dataB[32];

private:
    static int CalculateFrequency(int &manualZeros);
    static int CalculatePeriod();
    static int CalculateDuration();
};

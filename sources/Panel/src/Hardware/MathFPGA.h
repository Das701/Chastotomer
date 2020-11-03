#pragma once


struct MathFPGA
{
    static void Calculate();

    static float decDataA;
    static float decDataB;
    static float decDataC;
    static int decDA;
    static int emptyZeros;

private:
    static int CalculateFrequency(int &manualZeros);
    static int CalculatePeriod();
    static int CalculateDuration();
};

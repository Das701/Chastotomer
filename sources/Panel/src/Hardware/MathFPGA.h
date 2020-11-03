#pragma once


struct MathFPGA
{
    static void Calculate();
    static void BinToDec();
    static void CalculationAuto();

    static int MidAuto();
    static int MinAuto();
    static int MaxAuto();

    static void RefreshAuto();

    static float decDataA;
    static float decDataB;
    static float decDataC;
    static int decDA;
    static int emptyZeros;
    static char dataA[32];
    static char dataB[32];

    static int decMinAuto;
    static int decMidAuto;
    static int decMaxAuto;

    static char minAuto[10];
    static char midAuto[10];
    static char maxAuto[10];

private:
    static int CalculateFrequency(int &manualZeros);
    static int CalculatePeriod();
    static int CalculateDuration();
};

#pragma once


struct MathFPGA
{
    static void BinToDec();
    static void DecToBin(int dec, char *bin);

    static void Calculate();
    static void CalculateAuto();
    static void CalculateDcycle();
    static void CalculateComparator();
    static void CalculateInterpolate();

    static int MidAuto();
    static int MinAuto();
    static int MaxAuto();
    static void RefreshAuto();
    static char *GiveAuto();

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

    static int NA; //-V707
    static int NB; //-V707

    static char period[32];
    static char duration[32];

    static float dutyCycle;
    static int dcycleZeros;

    static float decFx;
    static float decTizm;
    static float decNkal;

    static char binFx[32];
    static char binTizm[16];
    static char binNkal[16];

    static char timer1[27];
    static char CAL1[24];
    static char CAL2[24];

    static float interpol;

private:
    static int CalculateFrequency(int &manualZeros);
    static int CalculatePeriod();
    static int CalculateDuration();
};

#pragma once


struct MathFPGA
{
    static char *GiveData();
    static char *GiveSpec();
    static char *GiveIdent();

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
    static char *GiveAuto();

    static int decMidAuto;
    static int decMaxAuto;

    static int NA; //-V707
    static int NB; //-V707

    static float dutyCycle;
    static int dcycleZeros;

    static float decFx;
    static float decTizm;
    static float decNkal;

    static float interpol;

private:
    static int CalculateFrequency(int &manualZeros);
    static int CalculatePeriod();
    static int CalculateDuration();
};

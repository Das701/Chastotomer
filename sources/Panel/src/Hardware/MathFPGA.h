#pragma once


struct MathFPGA
{
    static char *GiveData();
    static char *GiveSpec();
    static char *GiveIdent();

    static void BinToDec();
    static void DecToBin(int dec, char *bin);

    static void Calculate();
    static void CalculateDcycle();
    static void CalculateComparator();
    static void CalculateInterpolate();

    static int NA; //-V707
    static int NB; //-V707

    static float dutyCycle;
    static int dcycleZeros;

    static float decFx;
    static float decTizm;
    static float decNkal;

    static float interpol;

    struct Auto
    {
        static void Calculate();
        static int Mid();
        static int Min();
        static int Max();

        static char *Give();

    private:
        static int decMin;
        static int decMid;
        static int decMax;
    };

private:
    static int CalculateFrequency(int &manualZeros);
    static int CalculatePeriod();
    static int CalculateDuration();

    static float BinToDec(char bin[32]);
};

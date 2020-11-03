#pragma once


struct MathFPGA
{
    static char *GiveSpec();
    static char *GiveIdent();

    static void DecToBin(int dec, char *bin);

    static int NA; //-V707
    static int NB; //-V707

    struct Measure
    {
    friend struct FPGA;

        static char *GiveData();

        static void Calculate();

        static void BinToDec();

    private:

        static char dataA[32];
        static char dataB[32];
        static char dataPeriod[32];
        static char dataDuration[32];

        static char dataComparatorFx[32];   // |
        static char dataComparatorTizm[16]; // | Данные для компаратора
        static char dataComparatorNkal[16]; // /

        static int CalculateFrequency(int &manualZeros);
        static int CalculatePeriod();
        static int CalculateDuration();

        static void CalculateDcycle();
        static void CalculateComparator();
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
        static int decMin;
        static int decMid;
        static int decMax;

        static char dataMin[10];
        static char dataMid[10];
        static char dataMax[10];
    };

private:

    static float BinToDec(char bin[32]);
};

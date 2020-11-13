#pragma once
#include "Utils/String.h"
#include "Utils/Value.h"


struct MathFPGA
{
    static void DecToBin(int dec, char *bin);

    static int NA; //-V707
    static int NB; //-V707

    struct Measure
    {
        friend struct FPGA;
        friend struct MathFPGA;

        static String GiveData();
        static String GiveUnits();

        struct TypeData
        {
            enum E
            {
                MainCounters        // ������� ��������, ��� ����������� ���������
            };
        };

        static void SetNewData(TypeData::E type, uint value1, uint value2);

        // ��������� �������� ����, ��� ��������� ���������� � ��� ���������� ������ (��� GiveData())
        static void ClearFlagValidData() { isEmpty = true;  }
        // ��������� �������� ����, ��� �������� ���������� ������ (��� GiveData())
        static void SetFlagValidData() { isEmpty = false;  }

    private:

        static int decDA;

        static ValuePICO valueComparator;       // ������������ �������� �����������

        static ValueNANO decDataA;
        static ValueNANO decDataB;
        static ValueNANO decDataC;

        static bool isEmpty;                    // ������������� � true �������� ��������, ��� ������ ��� ����������� ���

        static void AppendDataMainCounters(uint counterA, uint counterB);

        static int CalculateFrequency(int &manualZeros);
        static int CalculatePeriod();
        static int CalculateDuration();

        static void Calculate(int &emptyZeroes, ValueNANO &data);
    };


    struct Interpolator
    {
        static void Calculate(uint timer, uint cal1, uint cal2);
        static float GetValue() { return value; };
    private:
        static float value;
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

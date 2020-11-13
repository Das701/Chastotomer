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
                MainCounters,       // Главные счётчики, для большинства измерений
                Interpolator,       // Данные интерполятора
                FillFactorPhase,    // Данные для коэффициента заполнения и фазы
                Comparator
            };
        };

        static void SetNewData(TypeData::E type, uint value1, uint value2, uint value3 = 0);

        // Установка признака того, что настройки изменились и нет корректных данных (для GiveData())
        static void ClearFlagValidData() { isEmpty = true;  }
        // Установка признака того, что получены корректные данные (для GiveData())
        static void SetFlagValidData() { isEmpty = false;  }

    private:

        static int decDA;

        static ValuePICO valueComparator;       // Рассчитанное значение компаратора

        static ValueNANO decDataA;
        static ValueNANO decDataB;
        static ValueNANO decDataC;

        static bool isEmpty;                    // Установленное в true значение означает, что данных для отображения нет

        static void AppendDataMainCounters(uint counterA, uint counterB);

        static int CalculateFrequency(int &manualZeros);
        static int CalculatePeriod();
        static int CalculateDuration();
        static void CalculateComparator(uint fx, uint tizm, uint nkal);

        static void Calculate(int &emptyZeroes, ValueNANO &data);
    };


    struct Interpolator
    {
        friend struct Measure;
        static float GetValue() { return value; };
    private:
        static void Calculate(uint timer, uint cal1, uint cal2);
        static float value;
    };


    struct FillFactor
    {
        friend struct Measure;
        static float GetValue() { return value; }
    private:
        static float value;
        static int zeroes;
        static void Calculate(uint period, uint duration);
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

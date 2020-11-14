#pragma once
#include "Utils/Stack.h"
#include "Utils/String.h"
#include "Utils/Value.h"


struct MathFPGA
{
    static void DecToBin(int dec, char *bin);

    static int NA; //-V707
    static int NB; //-V707

    // Структура отвечает за валидность данных
    struct Validator
    {
        // Установка признака того, что настройки изменились и нет корректных данных (для GiveData())
        static void SetInvalidData();

        // Установка признака того, что данным можно доверять
        static void SetValidData();

        // Возвращает true, если данные валидны
        static bool DataIsValid();

    private:

        static bool isEmpty;            // Установленное в true значение означает, что данных для отображения нет
        static uint timeClearedFlag;    // Время, когда данные были очищены
    };

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

    private:

        static int decDA;

        static ValueNANO decDataA;
        static ValueNANO decDataB;
        static ValueNANO decDataC;

        static void AppendDataMainCounters(uint counterA, uint counterB);

        static int CalculateFrequencyEmptyZeros(int &manualZeros);
        static int CalculatePeriodEmptyZeros();
        static int CalculateDurationEmptyZeros();

        static void Calculate(int &emptyZeroes, ValueNANO &data);
    };


    struct Interpolator
    {
        friend struct Measure;
    private:
        static void Calculate(uint timer, uint cal1, uint cal2);
        static float value;
    };


    struct FillFactor
    {
        friend struct Measure;
    private:
        static float value;
        static int zeroes;
        static void Calculate(uint period, uint duration);
    };


    struct Comparator
    {
        friend struct Measure;

    private:

        static void Calculate(uint fx, uint tizm, uint nkal);
        static ValuePICO value;

    public:

        class Stack : public ::Stack<double>
        {
        public:
            Stack(int size) : ::Stack<double>(size) {};
            bool AppendValue(double value);
            double GetFromEnd(int fromEnd);
        private:
        };

        static Stack values;
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

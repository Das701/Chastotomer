#pragma once
#include "Hardware/HAL/HAL.h"
#include "Utils/Stack.h"
#include "Utils/String.h"
#include "Utils/ValueSTRICT.h"


struct MathFPGA
{
    struct Data
    {
        friend struct MathFPGA;

        static String GiveDigits();
        static String GiveUnits();

    private:

        static void SetDigits(const String &digits);
        static void SetUnits(const String &units);

        static char digits[30];
        static char units[10];
    };

    static void DecToBin(int dec, char *bin);
    static String BinToString(pString bin, int num);

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

        // Возвращает true, если прошло слишком мало времени после последнего изменения настроек
        static bool VerySmallTime() { return TIME_MS - timeClearedFlag < deltaTime; };

    private:

        static bool isEmpty;                // Установленное в true значение означает, что данных для отображения нет
        static uint timeClearedFlag;        // Время, когда данные были очищены
        static const uint deltaTime = 200;  // Это время нужно выждать после того, как изменились настройки
    };

    struct Measure
    {
        friend struct FPGA;
        friend struct MathFPGA;

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

        static void SetNewData(TypeData::E type, uint value1, uint value2, uint value3 = 0, uint value4 = 0, uint value5 = 0);

    private:

        static int decDA;

        static ValueSTRICT counterA;
        static int powDataA;            // Сколько знаков в значении dataA
        static ValueSTRICT counterB;
        static ValueSTRICT dataC;

        static void AppendDataMainCounters(uint counterA, uint counterB);

        static int CalculateFrequencyEmptyZeros();
        static int CalculatePeriodEmptyZeros();
        static int CalculateDurationEmptyZeros();

        static void Calculate(int &pow, ValueSTRICT &data);
        static int CalculateEmptyZeros();

        static void CalculateNewData();
        static void CalculateUnits();
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
        static ValueSTRICT value;
        static int zeroes;
        static void Calculate(uint period, uint duration);
    };


    struct Comparator
    {
        friend struct Measure;

    private:

        static void Calculate(uint counter, int interpol1, int cal1, int interpol2, int cal2);
        static ValueComparator value;

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

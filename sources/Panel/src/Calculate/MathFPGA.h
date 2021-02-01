#pragma once
#include "Calculate/ValueFPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Stack.h"
#include "Utils/String.h"
#include "Utils/ValueSTRICT.h"


struct MathFPGA
{
    struct Data
    {
        friend struct MathFPGA;
        friend struct ValueFPGA;
        friend struct ValueDuration_Ndt_1ns;
        friend struct ValueFrequency_Tachometer;
        friend struct ValueFrequency_Comparator;
        friend struct ValueDuration_Phase_FillFactor;

        static String GiveDigits();
        static String GiveUnits();

        static const char *UGO_DivNULL;

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

    // —труктура отвечает за валидность данных
    struct Validator
    {
        // ”становка признака того, что настройки изменились и нет корректных данных (дл€ GiveData())
        static void SetInvalidData();

        // ”становка признака того, что данным можно довер€ть
        static void SetValidData();

        // ¬озвращает true, если данные валидны
        static bool DataIsValid();

        // ¬озвращает true, если прошло слишком мало времени после последнего изменени€ настроек
        static bool VerySmallTime() { return TIME_MS - timeClearedFlag < deltaTime; };

    private:

        static bool isEmpty;                // ”становленное в true значение означает, что данных дл€ отображени€ нет
        static uint timeClearedFlag;        // ¬рем€, когда данные были очищены
        static const uint deltaTime = 200;  // Ёто врем€ нужно выждать после того, как изменились настройки
    };

    struct Measure
    {
        friend struct FPGA;
        friend struct MathFPGA;

        static void SetNewData(uint value1, uint value2, uint value3 = 0, uint value4 = 0, uint value5 = 0);

        static ValueFPGA *valueFPGA;

    private:

        static bool CreateValue(uint value1, uint value2, uint value3 = 0, uint value4 = 0, uint value5 = 0);

        static int decDA;

        static ValueSTRICT counterA;
        static int powDataA;            // —колько знаков в значении dataA
        static ValueSTRICT counterB;
        static ValueSTRICT counterC;
    };


    struct Comparator
    {
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

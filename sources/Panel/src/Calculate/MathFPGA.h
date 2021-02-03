#pragma once
#include "Calculate/ValuesFPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Stack.h"
#include "Utils/String.h"
#include "Utils/ValueSTRICT.h"


struct MathFPGA
{
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
        static void SetNewData(uint value1, uint value2, uint value3 = 0, uint value4 = 0, uint value5 = 0);

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

#pragma once
#include "defines.h"
#include "Utils/Stack.h"
#include "Utils/String.h"


struct FPGA
{
friend struct MathFPGA;

    static void Init();
    static void Update();
    static void WriteCommand(const char command[4], const char argument[6]);
    static void WriteData();

    static void IncreaseN();
    static void DecreaseN();

    static void SwitchAuto();
    static bool AutoMode();

    static int CalibNumber();
    static void ReadCalibNumber();

    static String GiveIdent();

    // Возвращает true в случае переполнения
    static bool IsOverloaded();

    static uint fx;
    static uint tizm;
    static uint nkal;

    static Stack<uint> sFX;
    static Stack<uint> sTIZM;
    static Stack<uint> sNKAL;
    static Stack<double> values;

    struct Comparator
    {
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

private:

    static void ReadInterpolator();
    static void ReadAutoMode();
    static void ReadComparator();
    static void ReadFillFactorPhase();
};

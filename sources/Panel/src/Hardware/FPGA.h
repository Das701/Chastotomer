#pragma once
#include "defines.h"


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

    // Возвращает true в случае переполнения
    static bool IsOverloaded();

private:

    static uint fpgaTimer;
    static uint fpgaIdent;
    static uint fpgaCAL1;
    static char dataCAL2[24];
};

#pragma once
#include "defines.h"
#include "Utils/Stack.h"
#include "Utils/String.h"


struct Command
{
    enum E
    {
        CurrentChannel = 0x00,
        TimeMeasure    = 0x01,
        NumberPeriods  = 0x01,
        MemoryMode     = 0x03,
        LaunchSource   = 0x05,
        OneTime        = 0x05,
        StartStop      = 0x05,
        ModeFrequency  = 0x06,
        ModeCountPulse = 0x06,
        TimeLabels     = 0x07,
        Impedance      = 0x08,
        RefGenerator   = 0x09,
        DisplayTime    = 0x0D,
        Calibration    = 0x0F,
        Auto           = 0x0F,
        Test           = 0x0F
    };

    uint value;

    Command(E v) : value(v) {}
    void SetBit(int i) { _SET_BIT(value, i); };
    char GetBit(int i) const { return (char)_GET_BIT(value, i); }
};


struct FPGA
{
friend struct MathFPGA;

    static void Init();
    static void Update();
    static void WriteCommand(const char command[4], const char argument[6]);
    static void WriteCommand(const Command &command);
    static void WriteDataGovernor();

    static void IncreaseN();
    static void DecreaseN();

    static void SwitchAuto();
    static bool AutoMode();

    static int CalibNumber();
    static void ReadCalibNumber();

    static String GiveIdent();

    // Возвращает true в случае переполнения
    static bool IsOverloaded();

private:

    static void ReadInterpolator();
    static void ReadAutoMode();
    static void ReadComparator();
    static void ReadFillFactorPhase();

    static void CalculateData();
};

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
        ModeFront      = 0x02,
        MemoryMode     = 0x03,
        Divider        = 0x04,
        LaunchSource   = 0x05,
        OneTime        = 0x05,
        StartStop      = 0x05,
        ModeFrequency  = 0x06,
        ModeCountPulse = 0x06,
        ModePeriod     = 0x06,
        ModeDuration   = 0x06,
        Impedance      = 0x08,
        RefGenerator   = 0x09,
        ModeFilter     = 0x0A,
        TypeSynch      = 0x0B,
        Couple         = 0x0C,
        DisplayTime    = 0x0D,
        TimeLabels     = 0x0E,
        Calibration    = 0x0F,
        Auto           = 0x0F,
        Test           = 0x0F,
        Reset          = 0x0F
    };

    uint value;

    Command(E v) : value(v) {}
    void SetBit(int i) { _SET_BIT(value, i); };
    char GetBit(int i) const { return (char)_GET_BIT(value, i); }
};


struct FPGA
{
    static void Init();
    static void Update();
    static void WriteCommand(const Command &command);

    static void SwitchAuto();
    static bool AutoMode();

    static void ReadValueCalibrator();

    // Возвращает true в случае переполнения
    static bool IsOverloaded();

private:

    static void ReadInterpolator();
    static void ReadAutoMode();
    static void ReadComparator();
    static void ReadFillFactorPhase();

    static void CycleRead(int numBits, uint &value, bool verifyOnOverload);
    static void CycleWrite(uint value, int numBits);
    static void WriteBit(uint bit);

    static void DecToBin(int dec, char *bin);
    static String BinToString(pString bin, int num);

    // Установка признака того, что настройки изменились и нет корректных данных
    static void SetInvalidData();

    static uint timeChangeSettings;        // Время, когда данные были очищены

public:

    struct GovernorData
    {
        static void IncreaseN();
        static void DecreaseN();
        static void Reset();
        static int ValueCalibrator();
        static void Write();
        static void SetValueCalibrator(uint value);
    private:
        static void Calculate();

        static uint kCalib;         // Это значение считывается непосредственно из FPGA
        static int NAC;             // Поправка для калибровочного коэффициента
    };
};

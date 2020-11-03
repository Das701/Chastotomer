#pragma once

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

private:

    static char dataTimer[27];
    static char dataIdent[4];
    static char dataCAL1[24];
    static char dataCAL2[24];
    static char dataPeriod[32];
    static char dataDuration[32];
    static char dataBinFx[32];
    static char dataBinTizm[16];
    static char dataBinNkal[16];
};

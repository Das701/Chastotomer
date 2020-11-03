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
    static void RefreshAuto();

private:
    static char dataA[32];
    static char dataB[32];
    static char timer1[27];
    static char ident[4];
    static char minAuto[10];
    static char midAuto[10];
    static char maxAuto[10];
    static char CAL1[24];
    static char CAL2[24];
    static char period[32];
    static char duration[32];
    static char binFx[32];
    static char binTizm[16];
    static char binNkal[16];
};

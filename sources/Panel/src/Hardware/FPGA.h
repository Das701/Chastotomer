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

    static char timer1[27];
    static char ident[4];
    static char CAL1[24];
    static char CAL2[24];
    static char period[32];
    static char duration[32];
    static char binFx[32];
    static char binTizm[16];
    static char binNkal[16];
};

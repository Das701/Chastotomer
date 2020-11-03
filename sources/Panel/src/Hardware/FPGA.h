#pragma once

struct FPGA
{
    static void Init();
    static void Update();
    static void WriteCommand(const char *command, const char *argument);
    static void WriteData();
    static void IncreaseN();
    static void DecreaseN();
    static void SwitchAuto();
    static bool AutoMode();
    static int CalibNumber();
    static void ReadCalibNumber();
    static void RefreshAuto();

    static char dataA[32];
    static char dataB[32];
    static char timer1[27];
    static char ident[4];
    static char minAuto[10];
    static char midAuto[10];
    static char maxAuto[10];
    static char CAL1[24];
    static char CAL2[24];
};

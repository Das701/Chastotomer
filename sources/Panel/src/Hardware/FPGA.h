#pragma once

struct FPGA
{
    static void Init();
    static void Update();
    static char *GiveData();
    static char *GiveSpec();
    static char *GiveAuto();
    static void WriteCommand(const char *command, const char *argument);
    static void WriteData();
    static void IncreaseN();
    static void DecreaseN();
    static void SwitchAuto();
    static bool AutoMode();
    static int CalibNumber();
    static void ReadCalibNumber();
    static char *GiveIdent();
};

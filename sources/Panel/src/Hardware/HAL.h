#pragma once
#include "defines.h"


void ERROR_HANDLER();

struct HAL
{
    static void Init();
    // Используется для записи в дисплей
    //struct SPI1_
    //{
        //static void Init();
        //static void Send(uint8 *data, uint16 num);
    //};
};


struct HAL_FSMC
{
    static void Init();
    static void Reset();
    static void WriteCommand(uint16 command);
    static void WriteData(uint data);
    static uint16 ReadData();

    static void SendBuffer(uint8 *buffer);
    
    static uint16 GetData(uint16 address);

public:
    struct Test
    {
        static void Init();
        static void Run();
    };
};

struct PLIS
{
    static void Init();
    static void Update();
    static char* GiveData(); 
    static char* GiveSpec(); 
    static void WriteCommand(char* command, char* argument);
    static void WriteData();
    static void IncreaseN();
    static void DecreaseN();
    static void SwitchAuto();
    static bool AutoMode();
    static char* GiveAuto();
    static int MidAuto();
    static int MinAuto();
    static int MaxAuto();
    static void RefreshAuto();
    static int CalibNumber();
    static void ReadCalibNumber();
    static char* GiveIdent(); 
};

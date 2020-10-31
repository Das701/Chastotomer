#pragma once
#include "defines.h"


void ERROR_HANDLER();


struct HAL
{
    static void Init();
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


#pragma once
#include "defines.h"


#define TIME_MS   HAL_TIM::TimeMS()


struct HAL
{
    static void Init();
    static void ERROR_HANDLER();
};


struct HAL_FSMC
{
    static void Init();
    static void Reset();
    static void WriteCommand(uint16 command);
    static void WriteCommand(uint16 command, uint data);
    static void WriteCommand(uint16 command, uint data1, uint data2, uint data3);
    static void WriteData(uint data);
    static uint16 ReadData();

    static void SendBuffer(uint8 *buffer, int top, int left, int right, int bottom);
    
    static uint16 GetData(uint16 address);
};


struct HAL_PCD
{
    // PCD_HandleTypeDef
    static void *handle;
};


struct HAL_TIM
{
    static void Init();

    static uint TimeMS();

    static void DelayMS(uint timeMS);

    static void DelayUS(uint timeUS);
};


struct HAL_USBD
{
    // USBD_HandleTypeDef
    static void *handle;

private:
    friend class VCP;

    static void Init();

    static bool PrevSendingComplete();

    static void SetBufferTX(uint8 *buffer, uint size);

    static void Flush(uint8 *buffer, int size);

    static void SendDataSynch(int sizeBuffer, uint sizeSend, uint8 *buffSend, char *buffer);
};

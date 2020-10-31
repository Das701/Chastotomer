#pragma once
#include "defines.h"


void ERROR_HANDLER();

#define TIME_MS   HAL_TIM::TimeMS()


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
};


struct HAL_PCD
{
    // PCD_HandleTypeDef
    static void *handle;
};


struct HAL_TIM
{
    static uint TimeMS();

    static void Delay(uint timeMS);
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

    //static void SendDataSynch(const void *buffer, uint size);
    static void SendDataSynch(int sizeBuffer, uint sizeSend, uint8 *buffSend, char *buffer);
};

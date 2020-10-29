#pragma once
#include "defines.h"
#include <usbd_def.h>


#define VCP_FLUSH()                       VCP::Flush()
#define VCP_SEND_DATA_SYNCH(buffer, size) VCP::SendDataSynch(buffer, size)
#define CONNECTED_TO_USB                  VCP::connectedToUSB
#define CABLE_USB_IS_CONNECTED            VCP::cableUSBisConnected

class VCP
{
public:
    static const int DEVICE_FS = 0;

    // Инициализация
    static void Init();

    static void SendDataAsynch(const uint8 *data, uint size);

    static void SendDataSynch(const void *data, uint size = 0);
    // Передаётся строка без завершающего нуля
    static void SendStringAsynch(const char *data);
    // Передаётся строка без завершающего нуля
    static void SendStringSynch(char *data);
    // Эта строка передаётся с завершающими символами \\r\\n
    static void SendFormatStringAsynch(char *format, ...);
    // Эта строка передаётся с завершающими символами \\r\\n
    static void SendFormatStringSynch(char *format, ...);

    static void SendByte(uint8 data);

    static void Flush();

    static USBD_HandleTypeDef handleUSBD;

    static PCD_HandleTypeDef handlePCD;

    static bool connectedToUSB;

    static bool cableUSBisConnected;
    
private:
    static bool PrevSendingComplete();
};

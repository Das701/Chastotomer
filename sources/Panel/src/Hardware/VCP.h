#pragma once
#include "defines.h"
#include <usbd_def.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup VCP
 *  @brief Virtual Com Port
 *  @{
 */
 
#define VCP_FLUSH()                       VCP::Flush()
#define VCP_SEND_DATA_SYNCH(buffer, size) VCP::SendDataSynch(buffer, size)
#define CONNECTED_TO_USB                  VCP::connectedToUSB
#define CABLE_USB_IS_CONNECTED            VCP::cableUSBisConnected

class VCP
{
public:
    static const int DEVICE_FS = 0;

    /// �������������
    static void Init();

    static void SendDataAsynch(const uint8 *data, uint size);

    static void SendDataSynch(const void *data, uint size = 0);
    /// ��������� ������ ��� ������������ ����
    static void SendStringAsynch(char *data);
    /// ��������� ������ ��� ������������ ����
    static void SendStringSynch(char *data);
    /// ��� ������ ��������� � ������������ ��������� \\r\\n
    static void SendFormatStringAsynch(char *format, ...);
    /// ��� ������ ��������� � ������������ ��������� \\r\\n
    static void SendFormatStringSynch(char *format, ...);

    static void SendByte(uint8 data);

    static void Flush();

    static USBD_HandleTypeDef handleUSBD;

    static PCD_HandleTypeDef handlePCD;

    static bool connectedToUSB;

    static bool cableUSBisConnected;
    
    static uint8 * BuffRx();
    

private:
    static bool PrevSendingComplete();
};



/** @}
 */

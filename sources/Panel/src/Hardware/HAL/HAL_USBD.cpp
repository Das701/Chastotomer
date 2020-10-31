#include "defines.h"
#include "Hardware/VCP.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/USBD/usbd_cdc_interface.h"
#include "Hardware/USBD/usbd_conf.h"
#include "Hardware/USBD/usbd_desc.h"
#include "Utils/Math.h"
#include <cstring>


static USBD_HandleTypeDef hUSBD;
void *HAL_USBD::handle = &hUSBD;


void HAL_USBD::Init()
{
    USBD_Init(&hUSBD, &VCP_Desc, 0);
    USBD_RegisterClass(&hUSBD, &USBD_CDC);
    USBD_CDC_RegisterInterface(&hUSBD, &USBD_CDC_fops);
    USBD_Start(&hUSBD);
}


bool HAL_USBD::PrevSendingComplete()
{
    USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)hUSBD.pClassData;
    return pCDC->TxState == 0;
}


void HAL_USBD::SetBufferTX(uint8 *buffer, uint size)
{
    USBD_CDC_SetTxBuffer(&hUSBD, buffer, (uint16)size);
    USBD_CDC_TransmitPacket(&hUSBD);
}


void HAL_USBD::Flush(uint8 *buffer, int size)
{
    volatile USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)hUSBD.pClassData;

    while (pCDC->TxState == 1)
    {
    }; //-V712

    USBD_CDC_SetTxBuffer(&hUSBD, buffer, (uint16)size);
    USBD_CDC_TransmitPacket(&hUSBD);

    while (pCDC->TxState == 1)
    {
    }; //-V654 //-V712
}


void HAL_USBD::SendDataSynch(int sizeBuffer, uint sizeSend, uint8 *buffSend, char *buffer)
{
    volatile USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)hUSBD.pClassData;

    do
    {
        if (sizeBuffer + sizeSend > SIZE_BUFFER_VCP)
        {
            int reqBytes = SIZE_BUFFER_VCP - sizeBuffer;
            LIMITATION(reqBytes, 0, (int)sizeSend); //-V2516

            while (pCDC->TxState == 1) {
            }; //-V712

            std::memcpy(buffSend + sizeBuffer, (void *)buffer, (uint)reqBytes);
            USBD_CDC_SetTxBuffer(&hUSBD, buffSend, SIZE_BUFFER_VCP);
            USBD_CDC_TransmitPacket(&hUSBD);
            sizeSend -= reqBytes;
            buffer += reqBytes;
            sizeBuffer = 0;
        }
        else
        {
            std::memcpy(buffSend + sizeBuffer, (void *)buffer, (uint)sizeSend);
            sizeBuffer += sizeSend;
            sizeSend = 0;
        }
    } while (sizeSend);
}

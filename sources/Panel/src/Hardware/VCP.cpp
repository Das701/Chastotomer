#include "defines.h"
#include "Hardware/VCP.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>


bool VCP::cableUSBisConnected = false;
bool VCP::connectedToUSB = false;


void VCP::Init()
{
    HAL_USBD::Init();
} 


void VCP::Send64BytesOrLess(const uint8 *buffer, uint size)
{
#define SIZE_BUFFER 64U
    static uint8 trBuf[SIZE_BUFFER];

    size = Math::Min(size, SIZE_BUFFER);
    while (!HAL_USBD::PrevSendingComplete()) {};
    std::memcpy(trBuf, buffer, (uint)size);

    HAL_USBD::SetBufferTX(trBuf, size);
}


void VCP::SendDataAsynch(const uint8 *buffer, uint size)
{
    while (size != 0)
    {
        if (size < 64)
        {
            Send64BytesOrLess(buffer, size);
            size = 0;
        }
        else
        {
            Send64BytesOrLess(buffer, 64);
            size -= 64;
            buffer += 64;
        }
    }
}


static uint8 buffSend[SIZE_BUFFER_VCP];
static int sizeBuffer = 0;


void VCP::Flush()
{
    if (sizeBuffer)
    {
        HAL_USBD::Flush(buffSend, sizeBuffer);
    }

    sizeBuffer = 0;
}


void VCP::SendDataSynch(const void *_buffer, uint size)
{
    if (CONNECTED_TO_USB)
    {
        char *buffer = (char *)_buffer;
        if (size == 0)
        {
            size = std::strlen(buffer);
        }

        HAL_USBD::SendDataSynch(sizeBuffer, size, buffSend, buffer);
    }
}


void VCP::SendStringAsynch(const char *data)
{
    SendDataAsynch((uint8 *)data, std::strlen(data));
}


void VCP::SendStringSynch(char *data)
{
    SendDataSynch((uint8 *)data, std::strlen(data));
}


void VCP::SendFormatStringAsynch(char *format, ...)
{
    if (CONNECTED_TO_USB)
    {
        static char buffer[200];
        std::va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);
        std::strcat(buffer, "\r\n");
        SendDataAsynch((uint8 *)buffer, std::strlen(buffer));
    }
}


void VCP::SendFormatStringSynch(char *format, ...)
{
    char buffer[200];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(buffer, format, args);
    va_end(args);
    std::strcat(buffer, "\r\n");
    SendDataSynch((uint8 *)buffer, std::strlen(buffer));
}


void VCP::SendByte(uint8 byte)
{
    SendDataSynch(&byte, 1);
}

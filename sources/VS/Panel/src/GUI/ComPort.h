#pragma once


struct ComPort
{
    static bool Open();
    static void Close();
    static bool IsOpened();
    static void Send(pchar);
    static int Receive(char *buffer, int size);
};

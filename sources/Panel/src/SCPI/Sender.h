#pragma once


namespace SCPI
{
    struct Sender
    {
        // После вычисления очередного измеренного значения ValueFPGA должно вызывать эту функцию
        static void SendValue(pchar data);
    private:
        // Если true, то данные нуждаются в засылке
        static bool needSend;
    };
}

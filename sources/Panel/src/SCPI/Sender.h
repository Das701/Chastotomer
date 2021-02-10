#pragma once


namespace SCPI
{
    struct Sender
    {
        struct Mode
        {
            enum E
            {
                Hand,
                Auto,
                Count
            };
        };

        // После вычисления очередного измеренного значения ValueFPGA должно вызывать эту функцию
        static void SendValue(pchar data);

        // Установка ручного или автоматического режима засылки данных
        static void SetMode(Mode::E m) { mode = m; };

        // Приказывает заслать в порт n последних значений
        static void SendArray(int n)   { valuesForSend = n; };

    private:

        static int valuesForSend;

        static Mode::E mode;
    };
}

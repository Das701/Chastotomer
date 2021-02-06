#pragma once


namespace Tests
{
    void Run();

    void ValuesFPGA();

    void ValuesSTRICT();

    void FailExit();

    namespace Utils
    {
        // Возвращает случайное число в максимальном диапазоне
        double RandDouble();

        bool EqualsDouble(double d1, double d2);
    }
}


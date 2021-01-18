#pragma once
#include "Utils/String.h"


struct Order
{
    enum E
    {
        Mega,
        Kilo,
        One,
        Milli,
        Micro,
        Nano,
        Count
    };

    // Возвращает степень, соответсвующую порядку
    static int GetPow10(Order::E order);

    // Сохранить order во внутренней памяти
    static void Store(Order::E order);

    // Восстановить значение, ранее сохранённое функцией Store()
    static Order::E Restore();
};


struct ValueATTO //-V690
{
    explicit ValueATTO(int v);
    ValueATTO(const ValueATTO &v);

    void FromUNITS(int units, uint mUnits, uint uUnits, uint nUnits, uint pUnits, int sign);
    void FromINT(int v);

    void FromDouble(double v);

    void Div(uint div);
    void Mul(uint mul);

    void Add(ValueATTO &value);
    void Sub(const ValueATTO &value);

    int Sign() const;
    void SetSign(int sign);

    // Возращает строку значения
    String ToString() const;

    double ToDouble() const;

    uint64 Abs() const;

    int Integer() const;

    uint64 FractPico() const;

    Order::E GetOrder() const;

    uint64 ToUINT64() const;

private:

    uint64 value;       // Значение параметра в единицах измерения "нано". Установленный в "1" старший бит означает, что число отрицательное
};


ValueATTO operator/(const ValueATTO &first, uint second);
ValueATTO operator-(const ValueATTO &first, const ValueATTO &second);
ValueATTO operator-(const ValueATTO &first, int second);

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


struct ValueNANO
{
    explicit ValueNANO(double v);
    explicit ValueNANO(int v);

    // Берёт значение из строкового представления. При этом первым символом может идти знак ("+" или "-"), дробная часть отделяется от целой точкой ("."),
    // а order указыват, на сколько нужно умножжить итоговое число (3 - умножить на 1000, -3 - разделить на 1000)
    explicit ValueNANO(const char *const buffer, Order::E order);

    void FromUnits(int units, uint mUnits, uint uUnits, uint nUnits, int sign);
    void FromDouble(double v);
    void FromUINT64(uint64 v) { value = v; }
    void FromINT(int v);
    void FromString(const char *const buffer, int pow10);
    bool FromString(const char *buffer, char **end, int numDigitsAfterComma);

    double ToDouble() const;
    float ToFloat() const;
    uint64 ToUINT64() const { return value; }

    void Div(uint div);
    void Mul(uint mul);
    void Add(ValueNANO value);
    void Sub(ValueNANO value);
    // Умножить на 10 в степени pow
    void MulPow10(int pow);

    void SetSign(int sign);

    // Возвращает знак
    int Sign() const;

    // Возвращает целую часть
    int Integer() const;

    // Возвращает количество наночастиц в дробной части
    int FractNano() const;

    uint64 Abs() const;

    // Возвращает:
    // Order::Mega  - ValueNANO::Integer() >= 1e6
    // Order::Kilo  - ValueNANO::Integer() >= 1e3
    // Order::One   - ValueNANO::Integer() >= 0
    // Order::Milli - ValueNANO::Integer() >= 1e-3
    // Order::Micro - ValueNANO::Integer() >= 1e-6
    Order::E GetOrder() const;

    // Возращает строку значения
    String ToString(bool sign, Order::E order = Order::Count) const;

    bool operator<(const ValueNANO &);
    bool operator>(const ValueNANO &);
    bool operator<=(const ValueNANO &);
    bool operator>=(const ValueNANO &);
    bool operator==(const ValueNANO &);
    bool operator!=(const ValueNANO &);

private:

    uint64 value;       // Значение параметра в единицах измерения "нано". Установленный в "1" старший бит означает, что число отрицательное
};


struct ValuePICO //-V690
{
    explicit ValuePICO(int v);
    explicit ValuePICO(const ValuePICO &v);

    void FromUNITS(int units, uint mUnits, uint uUnits, uint nUnits, uint pUnits, int sign);
    void FromINT(int v);

    void Div(uint div);
    void Mul(uint mul);

    void Add(ValuePICO &value);
    void Sub(const ValuePICO &value);

    int Sign() const;
    void SetSign(int sign);

    // Возращает строку значения
    String ToString() const;

    double ToDouble() const;

    uint64 Abs() const;

    int Integer() const;

    uint64 FractPico() const;

private:

    uint64 value;       // Значение параметра в единицах измерения "нано". Установленный в "1" старший бит означает, что число отрицательное
};

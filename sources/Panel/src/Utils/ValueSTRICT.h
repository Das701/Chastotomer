#pragma once
#include "Utils/String.h"


struct ValueSTRICT
{
    explicit ValueSTRICT(double v);
    explicit ValueSTRICT(int64 v);

    void FromDouble(double v);

    double ToDouble() const;
    uint64 ToUINT64() const { return units; }

    void DivUINT(uint div);
    void DivDOUBLE(double div);
    void MulUINT(uint mul);

    void SetSign(int sign);

    // Возвращает знак
    int Sign() const;

private:

    int sign;           // Если sign < 0 - значение ниже нуля.
    uint powUnit;       // Размерность юнита члена units. 1 - 1e-1, 3 - милли, 6 - микро, 7 - 1е-7
    uint64 units;       // Значение параметра в юнитах. "Вес" юнита определяется параметром powUnit

    // Возвращает количество юнитов в единице (например, при pow == 3 result == 1000)
    uint64 UnitsInOne(uint pow) const;
};


ValueSTRICT operator/(int64 first, const ValueSTRICT &second);


struct ValueComparator //-V690
{
    explicit ValueComparator(int v);

    void FromUNITS(int units, uint mUnits, uint uUnits, uint nUnits, uint pUnits, int sign);
    void FromINT(int v);

    void Div(uint div);
    void Mul(uint mul);

    void Add(ValueComparator &value);
    void Sub(const ValueComparator &value);

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


ValueComparator operator/(const ValueComparator &first, uint second);
ValueComparator operator-(const ValueComparator &first, const ValueComparator &second);
ValueComparator operator-(const ValueComparator &first, int second);

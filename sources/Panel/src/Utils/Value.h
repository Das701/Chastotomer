#pragma once
#include "Utils/String.h"


struct ValueNANO
{
    explicit ValueNANO(double v);
    explicit ValueNANO(int v);

    void FromUnits(int units, uint mUnits, uint uUnits, uint nUnits, int sign);
    void FromDouble(double v);
    void FromUINT64(uint64 v) { value = v; }
    void FromINT(int v);
    void FromString(const char *const buffer, int pow10);

    double ToDouble() const;
    uint64 ToUINT64() const { return value; }

    void Div(uint div);
    void Mul(uint mul);
    void Add(ValueNANO value);
    void Sub(ValueNANO value);
    // ”множить на 10 в степени pow
    void MulPow10(int pow);

    void SetSign(int sign);

    // ¬озвращает знак
    int Sign() const;

    // ¬озвращает целую часть
    int Integer() const;

    // ¬озвращает количество наночастиц в дробной части
    int FractNano() const;

    uint64 Abs() const;

    bool operator<(const ValueNANO &);
    bool operator>(const ValueNANO &);
    bool operator<=(const ValueNANO &);
    bool operator>=(const ValueNANO &);
    bool operator==(const ValueNANO &);
    bool operator!=(const ValueNANO &);

private:

    uint64 value;       // «начение параметра в единицах измерени€ "нано". ”становленный в "1" старший бит означает, что число отрицательное
};


struct ValuePICO //-V690
{
    explicit ValuePICO(int v);
    ValuePICO(const ValuePICO &v);

    void FromUNITS(int units, uint mUnits, uint uUnits, uint nUnits, uint pUnits, int sign);
    void FromINT(int v);

    void Div(uint div);
    void Mul(uint mul);

    void Add(ValuePICO &value);
    void Sub(const ValuePICO &value);

    int Sign() const;
    void SetSign(int sign);

    // ¬озращает строку значени€
    String ToString() const;

    double ToDouble() const;

    uint64 Abs() const;

    int Integer() const;

    uint64 FractPico() const;

private:

    uint64 value;       // «начение параметра в единицах измерени€ "нано". ”становленный в "1" старший бит означает, что число отрицательное
};


ValuePICO operator/(const ValuePICO &first, uint second);
ValuePICO operator-(const ValuePICO &first, const ValuePICO &second);
ValuePICO operator-(const ValuePICO &first, int second);

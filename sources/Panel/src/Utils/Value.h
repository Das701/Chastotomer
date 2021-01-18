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

    // ���������� �������, �������������� �������
    static int GetPow10(Order::E order);

    // ��������� order �� ���������� ������
    static void Store(Order::E order);

    // ������������ ��������, ����� ���������� �������� Store()
    static Order::E Restore();
};


struct ValuePICO //-V690
{
    explicit ValuePICO(int v);
    ValuePICO(const ValuePICO &v);

    void FromUNITS(int units, uint mUnits, uint uUnits, uint nUnits, uint pUnits, int sign);
    void FromINT(int v);

    void FromDouble(double v);

    void Div(uint div);
    void Mul(uint mul);

    void Add(ValuePICO &value);
    void Sub(const ValuePICO &value);

    int Sign() const;
    void SetSign(int sign);

    // ��������� ������ ��������
    String ToString() const;

    double ToDouble() const;

    uint64 Abs() const;

    int Integer() const;

    uint64 FractPico() const;

    Order::E GetOrder() const;

    uint64 ToUINT64() const;

private:

    uint64 value;       // �������� ��������� � �������� ��������� "����". ������������� � "1" ������� ��� ��������, ��� ����� �������������
};


ValuePICO operator/(const ValuePICO &first, uint second);
ValuePICO operator-(const ValuePICO &first, const ValuePICO &second);
ValuePICO operator-(const ValuePICO &first, int second);

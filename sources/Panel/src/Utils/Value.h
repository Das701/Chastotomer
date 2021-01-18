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


ValueATTO operator/(const ValueATTO &first, uint second);
ValueATTO operator-(const ValueATTO &first, const ValueATTO &second);
ValueATTO operator-(const ValueATTO &first, int second);

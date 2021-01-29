#pragma once
#include "Utils/String.h"


struct Order
{
    enum E
    {
        Milli,  // 1e-3
        Micro,  // 1e-6
        Nano,   // 1e-9
        Pico,   // 1e-12
        Phemto, // 1e-15
        Atto,   // 1e-18
        Count
    };

    Order(E v) : value(v) {}

    // ���������� ���������� ������ � ����� �������. ��������, ��� Milli ����� 
    // 1000
    uint64 UnitsInOne() const;

    bool Increase();

    bool Decrease();

    E value;
};


Order::E operator++(Order::E &, int);
Order::E operator--(Order::E &, int);
Order::E operator-(Order::E, int);


struct ValueSTRICT
{
    explicit ValueSTRICT(double v);
    explicit ValueSTRICT(uint v);
    explicit ValueSTRICT(int64 v);

    void FromDouble(double v);
    void FromINT64(int64);

    double ToDouble() const;

    bool IsZero() const { return units == 0; }

    // ���������� ���������� �������� � ���� ���������� ������ �����������
    // dimension
    uint64 ToUnits(Order order) const;

    void DivUINT(uint div);
    void DivINT(int div);
    void DivDOUBLE(double div);
    void MulUINT(uint mul);

    void SetSign(int sign);

    // ���������� ����
    int Sign() const;

private:

    int sign;       // ���� sign < 0 - �������� ���� ����.
    Order order;    // ����������� ����� ����� units. 1 - 1e-1, 3 - �����,
                    // 6 - �����, 7 - 1�-7
    uint64 units;   // �������� ��������� � ������. "���" ����� ������������
                    // ���������� powUnit

    void Normalize();
};


ValueSTRICT operator/(int64 first, const ValueSTRICT &second);


struct ValueComparator //-V690
{
    explicit ValueComparator(int v);

    void FromUNITS(int units, uint mUnits, uint uUnits, uint nUnits,
        uint pUnits, int sign);
    void FromINT(int v);

    void Div(uint div);
    void Mul(uint mul);

    void Add(ValueComparator &value);
    void Sub(const ValueComparator &value);

    int Sign() const;
    void SetSign(int sign);

    // ��������� ������ ��������
    String ToString() const;

    double ToDouble() const;

    uint64 Abs() const;

    int Integer() const;

    uint64 FractPico() const;

private:

    uint64 value;   // �������� ��������� � �������� ��������� "����".
                    // ������������� � "1" ������� ��� ��������, ��� �����
                    // �������������
};


ValueComparator operator/(const ValueComparator &first, uint second);
ValueComparator operator-(const ValueComparator &first,
    const ValueComparator &second);
ValueComparator operator-(const ValueComparator &first, int second);

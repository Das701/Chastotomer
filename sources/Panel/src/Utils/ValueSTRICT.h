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
    explicit ValueSTRICT(int v);

    void FromDouble(double v);
    void FromINT64(int64);

    double ToDouble() const;

    bool IsZero() const { return units == 0; }

    // ���������� ���������� �������� � ���� ���������� ������ �����������
    // dimension
    uint64 ToUnits(Order order) const;

    void Div(uint div);
    void Div(int div);
    void Div(double div);

    void Mul(uint mul);
    void Mul(int mul);

    void Sub(const ValueSTRICT &sub);

    void SetSign(int sign);

    // ���������� ����
    int Sign() const;

    // ������������� units � ����������� �������. ���� ������� Milli, �������� ����������, � ���������� false.
    bool IncreaseOrder();

    // ������������� units � ���������� �������. ���� ��������� ������� ����������, ���������� false.
    // !!! �������� !!! ������ �������� ����� �������� ��������
    bool DecreaseOrder();

    void Abs() { SetSign(1); }

    bool operator!=(const ValueSTRICT &second) const;

private:

    int    sign;    // ���� sign < 0 - �������� ���� ����.
    Order  order;
    uint64 units;   // �������� ��������� � ������. "���" ����� ������������ ���������� powUnit

    void Normalize();

    // ����������� units � ����������� newOrder
    bool LeadTo(Order::E newOrder);

    // ��������� ������� ����� �������� � value
    void AlignOrders(ValueSTRICT &value);

    // �������� u ������ �������� �������. ���� � �������� ���������� ��������� ������������, ������������� �����������
    // ��������� �������
    void AddUnits(uint64 u);
};


ValueSTRICT operator*(const ValueSTRICT &first, int second);

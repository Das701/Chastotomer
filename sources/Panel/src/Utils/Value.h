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


struct ValueNANO
{
    explicit ValueNANO(double v);
    explicit ValueNANO(int v);

    // ���� �������� �� ���������� �������������. ��� ���� ������ �������� ����� ���� ���� ("+" ��� "-"), ������� ����� ���������� �� ����� ������ ("."),
    // � order ��������, �� ������� ����� ��������� �������� ����� (3 - �������� �� 1000, -3 - ��������� �� 1000)
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
    // �������� �� 10 � ������� pow
    void MulPow10(int pow);

    void SetSign(int sign);

    // ���������� ����
    int Sign() const;

    // ���������� ����� �����
    int Integer() const;

    // ���������� ���������� ���������� � ������� �����
    int FractNano() const;

    uint64 Abs() const;

    // ����������:
    // Order::Mega  - ValueNANO::Integer() >= 1e6
    // Order::Kilo  - ValueNANO::Integer() >= 1e3
    // Order::One   - ValueNANO::Integer() >= 0
    // Order::Milli - ValueNANO::Integer() >= 1e-3
    // Order::Micro - ValueNANO::Integer() >= 1e-6
    Order::E GetOrder() const;

    // ��������� ������ ��������
    String ToString(bool sign, Order::E order = Order::Count) const;

    bool operator<(const ValueNANO &);
    bool operator>(const ValueNANO &);
    bool operator<=(const ValueNANO &);
    bool operator>=(const ValueNANO &);
    bool operator==(const ValueNANO &);
    bool operator!=(const ValueNANO &);

private:

    uint64 value;       // �������� ��������� � �������� ��������� "����". ������������� � "1" ������� ��� ��������, ��� ����� �������������
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

    // ��������� ������ ��������
    String ToString() const;

    double ToDouble() const;

    uint64 Abs() const;

    int Integer() const;

    uint64 FractPico() const;

private:

    uint64 value;       // �������� ��������� � �������� ��������� "����". ������������� � "1" ������� ��� ��������, ��� ����� �������������
};

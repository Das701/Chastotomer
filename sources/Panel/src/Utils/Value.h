#pragma once


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

    static pString Suffix(Order::E order);

    // ���������� �������, �������������� �������
    static int GetPow10(Order::E order);

    // ��������� order �� ���������� ������
    static void Store(Order::E order);

    // ������������ ��������, ����� ���������� �������� Store()
    static Order::E Restore();
};


struct Value
{
    explicit Value(double v);
    explicit Value(int v);

    // ���� �������� �� ���������� �������������. ��� ���� ������ �������� ����� ���� ���� ("+" ��� "-"), ������� ����� ���������� �� ����� ������ ("."),
    // � order ��������, �� ������� ����� ��������� �������� ����� (3 - �������� �� 1000, -3 - ��������� �� 1000)
    explicit Value(const char *const buffer, Order::E order);

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
    void Add(Value value);
    void Sub(Value value);
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
    // Order::Mega  - Value::Integer() >= 1e6
    // Order::Kilo  - Value::Integer() >= 1e3
    // Order::One   - Value::Integer() >= 0
    // Order::Milli - Value::Integer() >= 1e-3
    // Order::Micro - Value::Integer() >= 1e-6
    Order::E GetOrder() const;

    // ��������� ������ ��������
    pString ToString(bool sign, Order::E order = Order::Count) const;

    bool operator<(const Value &);
    bool operator>(const Value &);
    bool operator<=(const Value &);
    bool operator>=(const Value &);
    bool operator==(const Value &);
    bool operator!=(const Value &);

private:

    uint64 value;       // �������� ��������� � �������� ��������� "����". ������������� � "1" ������� ��� ��������, ��� ����� �������������
};

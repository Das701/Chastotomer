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

    // Возвращает количество юнитов в одной единице. Например, для Milli вернёт 
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

    // Возвращает абсолютное значение в виде количества юнитов размерности
    // dimension
    uint64 ToUnits(Order order) const;

    void Div(uint div);
    void Div(int div);
    void Div(double div);

    void Mul(uint mul);
    void Mul(int mul);

    void Sub(const ValueSTRICT &sub);

    void SetSign(int sign);

    // Возвращает знак
    int Sign() const;

    // Пересчитывает units к предыдущему порядку. Если порядок Milli, пересчёт невозможен, и возвращает false.
    bool IncreaseOrder();

    // Пересчитывает units к следующему порядку. Если изменение порядка невозможно, возвращает false.
    // !!! ВНИМАНИЕ !!! Данная операция может понизить точность
    bool DecreaseOrder();

    void Abs() { SetSign(1); }

    bool operator!=(const ValueSTRICT &second) const;

private:

    int    sign;    // Если sign < 0 - значение ниже нуля.
    Order  order;
    uint64 units;   // Значение параметра в юнитах. "Вес" юнита определяется параметром powUnit

    void Normalize();

    // Пересчитать units к размерности newOrder
    bool LeadTo(Order::E newOrder);

    // Выровнять порядки этого значения и value
    void AlignOrders(ValueSTRICT &value);

    // Добавить u юнитов текущего порядка. Если в процессе добавления возникает переполнение, автоматически выполняется
    // изменение порядка
    void AddUnits(uint64 u);
};


ValueSTRICT operator*(const ValueSTRICT &first, int second);

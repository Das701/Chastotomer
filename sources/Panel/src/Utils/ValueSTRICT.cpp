#include "defines.h"
#include "Utils/Math.h"
#include "Utils/Stack.h"
#include "Utils/ValueSTRICT.h"
#include <cmath>
#include <cstring>


#define THOUSAND   ((uint64)1000U)
#define MAX_UINT64 ((uint64)0xFFFFFFFFFFFFFFFF)


ValueSTRICT::ValueSTRICT(double v) : sign(1), order(Order::Nano)
{
    FromDouble(v);
}


ValueSTRICT::ValueSTRICT(int64 v) : sign(v < 0 ? -1 : 1), order(Order::Nano), units(0)
{
    FromINT64(v);
}


void ValueSTRICT::FromINT64(int64 v)
{
    sign = (v < 0) ? -1 : 1;

    order.value = Order::Nano;

    v = Math::Abs(v);

    units = v * order.UnitsInOne();

    while (units < (MAX_UINT64 / THOUSAND))
    {
        if (order.Increase())
            units *= THOUSAND;
        else
            break;
    }
}


ValueSTRICT::ValueSTRICT(uint v) : sign(1), order(Order::Nano), units(0)
{
    FromINT64((int64)v);
}


ValueSTRICT::ValueSTRICT(int v) : sign(1), order(Order::Nano), units(0)
{
    FromINT64(v);
}


void ValueSTRICT::FromDouble(double v)
{
    order.value = Order::Nano;

    units = static_cast<uint64>(std::fabs(v) * 1.E9);

    SetSign(v < 0.0 ? -1 : 1);
}


uint64 ValueSTRICT::ToUnits(Order o) const
{
    uint64 result = units;

    while (o.value > order.value)
    {
        o.Decrease();
        result *= THOUSAND;
    }

    while (o.value < order.value)
    {
        o.Increase();
        result /= THOUSAND;
    }

    return result;
}


uint64 Order::UnitsInOne() const
{
    static const uint64 units[Order::Count] =
    {
/* 1e-3  */ THOUSAND,
/* 1e-6  */ THOUSAND * THOUSAND,
/* 1e-9  */ THOUSAND * THOUSAND * THOUSAND,
/* 1e-12 */ THOUSAND * THOUSAND * THOUSAND * THOUSAND,
/* 1e-15 */ THOUSAND * THOUSAND * THOUSAND * THOUSAND * THOUSAND,
/* 1e-18 */ THOUSAND * THOUSAND * THOUSAND * THOUSAND * THOUSAND * THOUSAND
    };

    return units[value];
}


bool Order::Increase()
{
    if (value == Count - 1)
    {
        return false;
    }

    value++;

    return true;
}


bool Order::Decrease()
{
    if (value == 0)
    {
        return false;
    }

    value--;

    return true;
}


Order::E operator++(Order::E &param, int)
{
    if (param != Order::Count - 1)
    {
        param = (Order::E)(param + 1);
    }

    return param;
}


Order::E operator--(Order::E &param, int)
{
    if (param != 0)
    {
        param = (Order::E)(param - 1);
    }

    return param;
}


Order::E operator-(Order::E first, int second)
{
    int result = (int)first - second;

    return (Order::E)result;
}


double ValueSTRICT::ToDouble() const
{
    return static_cast<double>(units) / (double)order.UnitsInOne() * static_cast<double>(Sign());
}


int ValueSTRICT::Sign() const
{
    return sign;
}


void ValueSTRICT::DivUINT(uint div)
{
    Normalize();

    if (div == 0)
    {
        units = 0;
    }
    else
    {
        units /= div;
    }

    Normalize();
}


void ValueSTRICT::DivINT(int div)
{
    SetSign(Sign() * Math::Sign(div));

    ValueSTRICT::DivUINT((uint)Math::Abs(div));
}


void ValueSTRICT::MulINT(int mul)
{
    SetSign(Sign() * Math::Sign(mul));

    ValueSTRICT::MulUINT((uint)Math::Abs(mul));
}


void ValueSTRICT::DivDOUBLE(double div)
{
    Normalize();

    if (sign > 0)
    {
        if (div > 0) { sign = 1;  }
        else         { sign = -1; }
    }
    else
    {
        if (div < 0) { sign = 1;  }
        else         { sign = -1; }
    }

    units = (uint64)((double)units / std::abs(div));

    Normalize();
}


void ValueSTRICT::MulUINT(uint mul)
{
    while (((double)units * (double)mul) >= (double)MAX_UINT64)
    {
        order.Decrease();
        units /= THOUSAND;
    }

    units *= mul;
}


void ValueSTRICT::SetSign(int s)
{
    sign = s;
}


void ValueSTRICT::Normalize()
{
    while ((double)units * 1000.0 < (double)MAX_UINT64)
    {
        if (!order.Increase())
        {
            break;
        }

        units *= THOUSAND;
    }
}


int ValueComparator::Integer() const
{
    return (int)(Abs() / 1000 / 1000 / 1000 / 1000) * Sign();
}


ValueComparator::ValueComparator(int v)
{
    FromINT(v);
}


void ValueComparator::FromINT(int v)
{
    FromUNITS(v < 0 ? -v : v, 0, 0, 0, 0, v < 0 ? -1 : 1);
}


void ValueComparator::FromUNITS(int units, uint mUnits, uint uUnits,
    uint nUnits, uint pUnits, int sign)
{
    value = (uint64)units;

    value = value * 1000 * 1000 * 1000 * 1000;

    value += (uint64)pUnits +
        (uint64)nUnits * 1000 + 
        (uint64)uUnits * 1000 * 1000 + 
        (uint64)mUnits * 1000 * 1000 * 1000;

    if (sign < 0)
    {
        SetSign(sign);
    }
}


void ValueComparator::Div(uint div)
{
    int sign = Sign();

    SetSign(1);

    value /= div;

    SetSign(sign);
}


void ValueComparator::Mul(uint mul)
{
    int sign = Sign();

    SetSign(1);

    value *= mul;

    SetSign(sign);
}


void ValueComparator::Add(ValueComparator &add)
{
    int sign = Sign();
    int signAdd = add.Sign();

    SetSign(1);
    add.SetSign(1);

    if (sign > 0 && signAdd > 0)
    {
        value += add.value;
    }
    else if (sign < 0 && signAdd < 0)
    {
        value += add.value;
        SetSign(-1);
    }
    else if (sign > 0 && signAdd < 0)
    {
        if (value >= add.value)
        {
            value -= add.value;
        }
        else
        {
            value = add.value - value;
            SetSign(-1);
        }
    }
    else
    {
        if (add.value >= value)
        {
            value = add.value - value;
        }
        else
        {
            value -= add.value;
            SetSign(-1);
        }
    }
}


void ValueComparator::Sub(const ValueComparator &val)
{
    ValueComparator sub(val);

    sub.SetSign(-val.Sign());

    Add(sub);
}


int ValueComparator::Sign() const
{
    //                fedcba9876543210
    return (value & 0x8000000000000000U) ? -1 : 1;
}


void ValueComparator::SetSign(int sign)
{
    if (sign >= 0)
    {
        //         fedcba9876543210
        value &= 0x7FFFFFFFFFFFFFFFU;   // —брасываем старший бит -
                                        // признак положительного числа
    }
    else
    {
        //         fedcba9876543210
        value |= 0x8000000000000000U;   // ”станавливаем старший бит - 
                                        // признак отрицательного числа
    }
}


String ValueComparator::ToString() const
{
    char buffer[50];

    char symbol[2] = { 0, 0 };

    buffer[0] = 0;

    int _sign = Sign();

    int intPart = Integer();

    if (_sign < 0)
    {
        intPart *= -1;
    }

    Stack<char> stack(100);

    while (intPart != 0)
    {
        stack.Push(intPart % 10);
        intPart /= 10;
    }

    if (_sign == -1)
    {
        symbol[0] = '-';
        std::strcat(buffer, symbol);
    }
    
    if(stack.Empty())
    {
        symbol[0] = '0';
        std::strcat(buffer, symbol); //-V2513
    }

    while (!stack.Empty())                     // ѕереводим в строку целую часть
    {
        symbol[0] = stack.Pop() | 0x30;

        std::strcat(buffer, symbol); //-V2513
    }

    symbol[0] = '.';

    std::strcat(buffer, symbol); //-V2513

    ValueComparator val(*this);

    val.SetSign(1);

    val.Sub(ValueComparator(Integer() * (Sign() > 1 ? 1 : -1))); 
                                   // “еперь в val осталась только дробна€ часть

    int count = 0;

    while (count < 4)
    {
        val.Mul(10);
        
        int integer = val.Integer();
        
        symbol[0] = (char)(integer | 0x30);

        std::strcat(buffer, symbol); //-V2513

        count++;

        val.Sub(ValueComparator(integer));
    }

    return String(buffer);
}


double ValueComparator::ToDouble() const
{
    return (double)Abs() / 1E12 * (double)Sign();
}


uint64 ValueComparator::Abs() const
{   //                fedcba9876543210
    return (value & 0x7fffffffffffffff);
}


uint64 ValueComparator::FractPico() const
{
    ValueComparator val(*this);

    val.SetSign(1);

    int whole = val.Integer();

    return (val.value - (uint64)whole * 1000 * 1000 * 1000 * 1000);
}


ValueSTRICT operator/(int64 first, const ValueSTRICT &second)
{
    ValueSTRICT result(first);

    result.DivDOUBLE(second.ToDouble());

    return result;
}


ValueComparator operator-(const ValueComparator &first, const ValueComparator &second)
{
    ValueComparator result = first;
    result.Sub(second);
    return result;
}


ValueComparator operator-(const ValueComparator &first, int second)
{
    return first - ValueComparator(second);
}


ValueComparator operator/(const ValueComparator &first, uint second)
{
    ValueComparator result = first;
    result.Div(second);
    return result;
}

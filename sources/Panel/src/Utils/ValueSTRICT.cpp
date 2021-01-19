#include "defines.h"
#include "Utils/Math.h"
#include "Utils/Stack.h"
#include "Utils/ValueSTRICT.h"
#include <cmath>
#include <cstring>


ValueSTRICT::ValueSTRICT(double v)
{
    FromDouble(v);
}


ValueSTRICT::ValueSTRICT(int64 v)
{
    FromDouble(v);
}


void ValueSTRICT::FromDouble(double v)
{
    int s = (v < 0.0) ? -1 : 1;

    units = static_cast<uint64>(std::fabs(v) * 1.E9);

    if (s < 0)
    {
        SetSign(-1);
    }
}


double ValueSTRICT::ToDouble() const
{
    return static_cast<double>(units) / 1E9 * static_cast<double>(Sign());
}


int ValueSTRICT::Sign() const
{
    return sign;
}


void ValueSTRICT::DivUINT(uint div)
{
    units /= div;
}


void ValueSTRICT::DivDOUBLE(double div)
{
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
}


void ValueSTRICT::MulUINT(uint mul)
{
    units *= mul;
}


void ValueSTRICT::SetSign(int s)
{
    sign = s;
}


int ValueComparator::Integer() const
{
    return (int)(Abs() / 1000 / 1000 / 1000 / 1000) * Sign(); //-V2533
}


ValueComparator::ValueComparator(int v)
{
    FromINT(v);
}


void ValueComparator::FromINT(int v)
{
    FromUNITS(v < 0 ? -v : v, 0, 0, 0, 0, v < 0 ? -1 : 1);
}


void ValueComparator::FromUNITS(int units, uint mUnits, uint uUnits, uint nUnits, uint pUnits, int sign)
{
    value = (uint64)units; //-V2533

    value = value * 1000 * 1000 * 1000 * 1000;

    value += (uint64)pUnits + (uint64)nUnits * 1000 + (uint64)uUnits * 1000 * 1000 + (uint64)mUnits * 1000 * 1000 * 1000; //-V2533

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
        value &= 0x7FFFFFFFFFFFFFFFU;   // —брасываем старший бит - признак положительного числа
    }
    else
    {
        //         fedcba9876543210
        value |= 0x8000000000000000U;   // ”станавливаем старший бит - признак отрицательного числа
    }
}


String ValueComparator::ToString() const
{
    char buffer[50];

    char symbol[2] = { 0, 0 };

    buffer[0] = 0;

    int intPart = Integer();

    Stack<char> stack(100);

    while (intPart != 0)
    {
        stack.Push(intPart % 10);
        intPart /= 10;
    }
    
    if(stack.Empty())
    {
        symbol[0] = '0';
        std::strcat(buffer, symbol); //-V2513
    }

    while (!stack.Empty())                          // ѕереводим в строку целую часть
    {
        symbol[0] = stack.Pop() | 0x30;

        std::strcat(buffer, symbol); //-V2513
    }

    symbol[0] = '.';

    std::strcat(buffer, symbol); //-V2513

    ValueComparator val(*this);

    val.Sub(ValueComparator(Integer()));                  // “еперь в val осталась только дробна€ часть

    int count = 0;

    while (count < 4)
    {
        val.Mul(10);
        
        int integer = val.Integer();
        
        symbol[0] = (char)(integer | 0x30); //-V2533

        std::strcat(buffer, symbol); //-V2513

        count++;

        val.Sub(ValueComparator(integer));
    }

    return String(buffer);
}


double ValueComparator::ToDouble() const
{
    return (double)Abs() / 1E12 * (double)Sign(); //-V2533
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

    return (val.value - whole * 1000 * 1000 * 1000 * 1000);
}


ValueSTRICT operator/(int64 first, const ValueSTRICT &second)
{
    ValueSTRICT result(first);

    result.DivDOUBLE(second.ToDouble());

    return result;
}

#include "defines.h"
#include "Utils/Math.h"
#include "Utils/Stack.h"
#include "Utils/Value.h"
#include <cmath>
#include <cstring>


ValueNANO::ValueNANO(double v)
{
    FromDouble(v);
}


void ValueNANO::FromDouble(double v)
{
    int sign = (v < 0.0) ? -1 : 1;

    value = static_cast<uint64>(std::fabs(v) * 1.E9);

    if (sign < 0)
    {
        SetSign(-1);
    }
}


double ValueNANO::ToDouble() const
{
    return static_cast<double>(Abs()) / 1E9 * static_cast<double>(Sign());
}


int ValueNANO::Sign() const
{
    //                fedcba9876543210
    return (value & 0x8000000000000000U) ? -1 : 1;
}


uint64 ValueNANO::Abs() const
{   //                fedcba9876543210
    return (value & 0x7fffffffffffffff);
}


void ValueNANO::Div(uint div)
{
    int sign = Sign();

    SetSign(1);

    value /= div;

    SetSign(sign);
}


void ValueNANO::Mul(uint mul)
{
    int sign = Sign();

    SetSign(1);

    value *= mul;

    SetSign(sign);
}


void ValueNANO::SetSign(int sign)
{
    if (sign >= 0)
    {
        //         fedcba9876543210
        value &= 0x7FFFFFFFFFFFFFFFU;       // \todo как это может работать?
    }
    else
    {
        //         fedcba9876543210
        value |= 0x8000000000000000U;           // ”станавливаем признак отрицательного числа
    }
}


int ValueNANO::Integer() const
{
    uint64 val = Abs();

    return static_cast<int>(val / (1000 * 1000 * 1000)) * Sign();
}


int ValuePICO::Integer() const
{
    return (int)(Abs() / 1000 / 1000 / 1000 / 1000) * Sign(); //-V2533
}


int ValueNANO::FractNano() const
{
    ValueNANO val = *this;
    val.SetSign(1);

    int whole = val.Integer();

    return static_cast<int>(val.value - static_cast<int>(whole * 1000 * 1000 * 1000));
}


uint64 ValuePICO::FractPico() const
{
    ValuePICO val(*this);

    val.SetSign(1);

    int whole = val.Integer();

    return (val.value - whole * 1000 * 1000 * 1000 * 1000);
}


void ValueNANO::Add(ValueNANO add)
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


void ValueNANO::Sub(ValueNANO val)
{
    val.SetSign(-val.Sign());

    Add(val);
}


void ValueNANO::MulPow10(int pow)
{
    while (pow > 0)
    {
        Mul(10);
        pow--;
    }

    while (pow < 0)
    {
        Div(10);
        pow++;
    }
}


bool ValueNANO::operator<(const ValueNANO &rhs)
{
    return ToDouble() < rhs.ToDouble();
}


bool ValueNANO::operator<=(const ValueNANO &rhs)
{
    return ToDouble() <= rhs.ToDouble();
}


bool ValueNANO::operator>(const ValueNANO &rhs)
{
    return ToDouble() > rhs.ToDouble();
}


bool ValueNANO::operator>=(const ValueNANO &rhs)
{
    return ToDouble() >= rhs.ToDouble();
}


bool ValueNANO::operator==(const ValueNANO &rhs)
{
    return (value == rhs.value);
}


bool ValueNANO::operator!=(const ValueNANO &rhs)
{
    return (value != rhs.value);
}


ValuePICO::ValuePICO(int v)
{
    FromINT(v);
}


ValuePICO::ValuePICO(const ValuePICO &v) : value(v.value)
{
}


void ValuePICO::FromINT(int v)
{
    FromUNITS(v < 0 ? -v : v, 0, 0, 0, 0, v < 0 ? -1 : 1);
}


void ValuePICO::FromUNITS(int units, uint mUnits, uint uUnits, uint nUnits, uint pUnits, int sign)
{
    value = (uint64)units; //-V2533

    value = value * 1000 * 1000 * 1000 * 1000;

    value += (uint64)pUnits + (uint64)nUnits * 1000 + (uint64)uUnits * 1000 * 1000 + (uint64)mUnits * 1000 * 1000 * 1000; //-V2533

    if (sign < 0)
    {
        SetSign(sign);
    }
}


void ValuePICO::Div(uint div)
{
    int sign = Sign();

    SetSign(1);

    value /= div;

    SetSign(sign);
}


void ValuePICO::Mul(uint mul)
{
    int sign = Sign();

    SetSign(1);

    value *= mul;

    SetSign(sign);
}


void ValuePICO::Add(ValuePICO &add)
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


void ValuePICO::Sub(const ValuePICO &val)
{
    ValuePICO sub(val);

    sub.SetSign(-val.Sign());

    Add(sub);
}


int ValuePICO::Sign() const
{
    //                fedcba9876543210
    return (value & 0x8000000000000000U) ? -1 : 1;
}


void ValuePICO::SetSign(int sign)
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


String ValuePICO::ToString() const
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

    ValuePICO val(*this);

    val.Sub(ValuePICO(Integer()));                  // “еперь в val осталась только дробна€ часть

    int count = 0;

    while (count < 4)
    {
        val.Mul(10);
        
        int integer = val.Integer();
        
        symbol[0] = (char)(integer | 0x30); //-V2533

        std::strcat(buffer, symbol); //-V2513

        count++;

        val.Sub(ValuePICO(integer));
    }

    return String(buffer);
}


double ValuePICO::ToDouble() const
{
    return (double)Abs() / 1E12 * (double)Sign(); //-V2533
}


uint64 ValuePICO::Abs() const
{   //                fedcba9876543210
    return (value & 0x7fffffffffffffff);
}

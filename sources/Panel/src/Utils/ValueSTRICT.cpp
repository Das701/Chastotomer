#include "defines.h"
#include "Utils/Math.h"
#include "Utils/Stack.h"
#include "Utils/ValueSTRICT.h"
#include <cmath>
#include <cstring>


#define THOUSAND   ((uint64)1000U)

// \todo Разобраться, почему не работает встроенная арифметика с 64 битами
#define MAX_UINT64 ((uint64)0xFFFFFFFFFFFFFFFF)     // Максимально возможное число уменьшено в 256 раз, потому что
                                                    // при сложении 61-битных uint64 баг


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


void ValueSTRICT::Sub(const ValueSTRICT &value)
{
    ValueSTRICT sub = value;

    double dTHIS = ToDouble();
    double dSUB = sub.ToDouble();

    AlignOrders(sub);

    dTHIS = ToDouble();
    dSUB = sub.ToDouble();

    if (Sign() > 0)                                         // lhs > 0
    {
        if (sub.Sign() > 0)                                 // lhs > 0, rhs > 0
        {
            if (units > sub.units)                          // abs(lhs) > abs(rhs)
            {
                units -= sub.units;
            }
            else                                            // abs(lhs) < abs(rhs)
            {
                units = sub.units - units;
                SetSign(-1);
            }
        }
        else                                                // lhs > 0, rhs < 0
        {
            AddUnits(sub.units);
        }
    }
    else                                                    // lhs < 0
    {
        if (sub.Sign() > 0)                                 // lhs < 0, rhs > 0
        {
            AddUnits(sub.units);
        }
        else                                                // lhs < 0, rhs < 0
        {
            if (units > sub.units)                          // abs(lhs) > abs(rhs)
            {
                units -= sub.units;
            }
            else                                            // abs(lhs) < abs(rhs)
            {
                units = sub.units - units;
                SetSign(1);
            }
        }
    }

    dTHIS = ToDouble();
    dSUB = sub.ToDouble();
}


void ValueSTRICT::AddUnits(uint64 u)
{
    if (((double)units + u) > (double)MAX_UINT64)
    {
        DecreaseOrder();
        u /= THOUSAND;
    }

    units += u;
}


void ValueSTRICT::AlignOrders(ValueSTRICT &v)
{
    if (v.order.value > order.value)
    {
        while (!LeadTo(v.order.value))
        {
            v.DecreaseOrder();
        }
    }
    else if (v.order.value < order.value)
    {
        while (!v.LeadTo(order.value))
        {
            DecreaseOrder();
        }
    }
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
    while ((double)(units * THOUSAND) < (double)MAX_UINT64)
    {
        if (!IncreaseOrder())
        {
            break;
        }
    }
}


bool ValueSTRICT::IncreaseOrder()
{
    if ((double)(units) * 1000.0 > (double)MAX_UINT64)
    {
        return false;
    }

    if (!order.Increase())
    {
        return false;
    }

    units *= THOUSAND;

    return true;
}


bool ValueSTRICT::DecreaseOrder()
{
    if (!order.Decrease())
    {
        return false;
    }

    units /= THOUSAND;

    return true;
}


bool ValueSTRICT::LeadTo(Order::E newOrder)
{
    if (newOrder > order.value)
    {
        while (newOrder != order.value)
        {
            if (!IncreaseOrder())
            {
                return false;
            }
        }
    }
    else if (newOrder < order.value)
    {
        while (newOrder != order.value)
        {
            if (!DecreaseOrder())
            {
                return false;
            }
        }
    }

    return true;
}


ValueSTRICT operator*(const ValueSTRICT &first, int second)
{
    ValueSTRICT result = first;
    result.MulINT(second);
    return result;
}


bool ValueSTRICT::operator!=(const ValueSTRICT &second) const
{
    return (units != second.units) ||
           (sign != second.sign) ||
           (order.value != second.order.value);
}
